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
