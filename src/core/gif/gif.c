/*
 * This file is vibe-coded because i dont know how to use Xorg!
 * i feel... dirty
 *
 */

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

#include "gif.h"

static unsigned long
scale_channel(unsigned char value, unsigned long mask) // converts color(0-255) to bit format, bit format is what X11 wants
{
    if (mask == 0)
    {
        return 0;
    }

    unsigned long m = mask;
    int shift = 0;
    while ((m & 1UL) == 0) { // bit offset of channel
        m >>= 1;
        shift++;
    }

    int bits = 0;
    while ((m & 1UL) == 1UL) { // how mnany bits the channel uses
        bits++;
        m >>= 1;
    }
    
    // scale the chanel range
    unsigned long max = (bits >= (int)(sizeof(unsigned long) * 8)) ? ~0UL : ((1UL << bits) - 1UL);
    unsigned long scaled = ((unsigned long)value * max) / 255UL;
    return scaled << shift;
}

static unsigned long
rgb_to_pixel(Visual* visual, unsigned char r, unsigned char g, unsigned char b) // without this X11 cries
{
    unsigned long pixel = 0;
    pixel |= scale_channel(r, visual->red_mask);
    pixel |= scale_channel(g, visual->green_mask);
    pixel |= scale_channel(b, visual->blue_mask);
    return pixel;
}

static unsigned char
blend_chan(unsigned char src, unsigned char alpha, unsigned char bg) // like alpha but isnt alpha. Mixes bg with a color using alpha chad ohio rizz?
{
    return (unsigned char)((src * alpha + bg * (255 - alpha)) / 255);
}

int
load_frames(const char* folder, Frame* frames, int count) // loads frames from a dir on RAM
{
    for (int i = 0; i < count; i++) {
        char path[256];
        snprintf(path, sizeof(path), "%s/frame_%03d.png", folder, i);

        frames[i].data = stbi_load(path, &frames[i].w, &frames[i].h, NULL, 4);
        if (!frames[i].data) {
            fprintf(stderr, "Error loading your images %s\n", path);
            return 0;
        }
    }

    return 1;
}

void
free_frames(Frame* frames, int count) // bye from ram, frames
{
    for (int i = 0; i < count; i++) {
        if (frames[i].data) {
            stbi_image_free(frames[i].data);
            frames[i].data = NULL;
        }
    }
}

void
draw_frame(Display* d, Window win, GC gc, Frame* frame) // renders the final PNG on a X11 window
{
    int screen = DefaultScreen(d);
    Visual* visual = DefaultVisual(d, screen);
    int depth = DefaultDepth(d, screen);

    XImage* img = XCreateImage( // compatible con the visual we use
        d,
        visual,
        depth,
        ZPixmap,
        0,
        NULL,
        frame->w,
        frame->h,
        32,
        0
    );

    if (!img)
        return;
    
    // buffer for all pixels
    unsigned char* buffer = calloc(1, img->bytes_per_line * frame->h);
    if (!buffer) {
        img->data = NULL;
        XDestroyImage(img);
        return;
    }

    img->data = (char*)buffer;
    
    // render for all  pixels
    for (int y = 0; y < frame->h; y++) {
        for (int x = 0; x < frame->w; x++) {
            int i = (y * frame->w + x) * 4;

            unsigned char r = frame->data[i + 0];
            unsigned char g = frame->data[i + 1];
            unsigned char b = frame->data[i + 2];
            unsigned char a = frame->data[i + 3];

            // white-bg
            unsigned char rr = blend_chan(r, a, 255);
            unsigned char gg = blend_chan(g, a, 255);
            unsigned char bb = blend_chan(b, a, 255);

            unsigned long pixel = rgb_to_pixel(visual, rr, gg, bb);
            XPutPixel(img, x, y, pixel);
        }
    }

    XPutImage(d, win, gc, img, 0, 0, 0, 0, frame->w, frame->h);
    XFlush(d);
    
    // This is for making the program "safer" by cleaning
    img->data = NULL;
    XDestroyImage(img);
    free(buffer);
}
