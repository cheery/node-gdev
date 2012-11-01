/*
Copyright (c) 2012 Cheery <cheery@boxbase.org>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <stdio.h>
#include <stdint.h>
#include <unistd.h>

#include "GLES2/gl2.h"

#include "include/video.h"
#include "include/display.h"

int main() {
    DISPLAY_T dpy = displayOpen(0);

    uint32_t nativewindow[NATIVEWINDOW_SIZE];
    uint32_t width, height;

    displayGetSize(dpy, &width, &height);
    printf("display size: %ix%i\n", width, height);

    displayGetHandle(dpy, nativewindow);

    printf("window size: %x %ix%i\n", nativewindow[0], nativewindow[1], nativewindow[2]);

    SURFACE_T surface = videoCreateNative(nativewindow);

    videoMakeCurrent(surface);

    glViewport ( 0, 0, width, height);

    // Set background color and clear buffers
    glClearColor(0.15f, 0.25f, 0.35f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    videoSwapBuffers(surface);
    sleep(1);

    glClearColor(0.85f, 0.25f, 0.35f, 1.0f);
    glClear( GL_COLOR_BUFFER_BIT );
    videoSwapBuffers(surface);
    sleep(1);

}
