#include "gdev_internal.h"
#include <string.h>

int gdevGetSurface(GDevSurface* p, GDevLayer* hwd, GDevConfig cfg)
{
    GDevSurface f = malloc(sizeof(*f));
    memcpy(&f->layer, hwd, sizeof(GDevLayer));

    EGLSurface surface;
    switch (hwd->type) {
        case GDEV_GLOBAL_LAYER: {
            int pixel_format = hwd->d[4];
            EGLint attrs[] = {
                EGL_VG_COLORSPACE, EGL_VG_COLORSPACE_sRGB,
                EGL_VG_ALPHA_FORMAT, pixel_format & EGL_PIXEL_FORMAT_ARGB_8888_PRE_BRCM ? EGL_VG_ALPHA_FORMAT_PRE : EGL_VG_ALPHA_FORMAT_NONPRE,
                EGL_NONE
            };
            f->surface = eglCreatePixmapSurface(display, toEGLConfig(cfg), (EGLNativePixmapType)f->layer.b, attrs);
            break;
        }
        case GDEV_NATIVE_LAYER: {
            f->surface = eglCreateWindowSurface(display, toEGLConfig(cfg), (EGL_DISPMANX_WINDOW_T*)f->layer.b, NULL);
            break;
        }
        default:
            free(f);
            return GDEV_UNKNOWN_LAYER;
    }
    EGLint e = eglGetError();
    if (e != EGL_SUCCESS) {
        free(f);
        return toGDevError(e);
    }
    if (f->surface == EGL_NO_SURFACE) {
        free(f);
        return GDEV_NO_SURFACE;
    }
    *p = f;
    returnSuccess;
}

int gdevDiscardSurface(GDevSurface f)
{
    eglDestroySurface(display, toEGLSurface(f));
    free(f);
    returnSuccess;
}

int gdevSwapBuffers(GDevSurface f)
{
    // eglFlushBRCM() if things don't work out with globalimages
    eglSwapBuffers(display, toEGLSurface(f));
    returnOnEglError;
    returnSuccess;
}
