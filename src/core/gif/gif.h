#ifndef GIF_H
#define GIF_H

#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

typedef struct { // prototypes
    unsigned char* data;
    int w, h;
} Frame;

int load_frames(const char* folder, Frame* frames, int count);
void free_frames(Frame* frames, int count);
void draw_frame(Display* d, Window win, GC gc, Frame* frame);

#endif
