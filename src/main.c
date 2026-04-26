#include <X11/Xlib.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

#include "core/gpu/gpu.h"
#include "core/gif/gif.h"

#define FRAME_COUNT 8
#define CAT_WIDTH  191
#define CAT_HEIGHT 143

int main(void)
{
    Frame frames[FRAME_COUNT] = {0};

    if (!load_frames("/usr/share/catrunner", frames, FRAME_COUNT)) { // this is where the files are supossed to be installed
        return 1;
    }

    Display* d = XOpenDisplay(NULL);
    if (!d) {
        fprintf(stderr, "No display found\n");
        free_frames(frames, FRAME_COUNT);
        return 1;
    }

    int screen = DefaultScreen(d);

    Window win = XCreateSimpleWindow( // window with the size of the cat, borderless and black BG
        d,
        RootWindow(d, screen),
        100, 100,
        CAT_WIDTH, CAT_HEIGHT,
        0,
        BlackPixel(d, screen),
        BlackPixel(d, screen)
    );

    XStoreName(d, win, "CatRunner"); // the name
    XSelectInput(d, win, ExposureMask | KeyPressMask | StructureNotifyMask);
    XMapWindow(d, win); // make it visible

    GC gc = XCreateGC(d, win, 0, NULL); // Graphic Context

    int frame = 0;

    while (1) {
        float usage = get_gpu_usage();
        const char* vendor = detect_gpu_vendor();

        fprintf(stderr, "GPU: %s | Usage: %.2f%%\r", vendor, usage); // debug on console the GPU things
        fflush(stderr);
    
        // DELAY MAGIC : MORE GPU : THE CAT IS FASTER!
        int delay = 350 - (int)(usage * 3.0f);
        if (delay < 20) delay = 20;

        draw_frame(d, win, gc, &frames[frame]); // draws the frame

        usleep(delay * 1000); // Sleep for the FPS
        frame = (frame + 1) % FRAME_COUNT; // next frame
    }
    
    // time to clear
    XFreeGC(d, gc);
    XDestroyWindow(d, win);
    XCloseDisplay(d);

    free_frames(frames, FRAME_COUNT);
    return 0;
}
