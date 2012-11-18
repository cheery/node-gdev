#include "gdev_internal.h"

int gdevGetImage(GDevImage* img, GDevLayer* hwd)
{
    int e;
    e = eglQueryGlobalImageBRCM(hwd->d, hwd->d+2);
    returnErrorIf(GDEV_BAD_IMAGE, !e);
    returnOnEglError;
    EGLImageKHR egi = (EGLImageKHR)eglCreateImageKHR(display, EGL_NO_CONTEXT, EGL_NATIVE_PIXMAP_KHR, (EGLClientBuffer)hwd->d, NULL);
    returnErrorIf(GDEV_NO_IMAGE, egi == EGL_NO_IMAGE_KHR);
    returnOnEglError;
    *img = fromEGLImage(egi);
    returnSuccess;
}

int gdevDiscardImage(GDevImage img)
{
    eglDestroyImageKHR(display, toEGLImage(img));
    returnOnEglError;
    returnSuccess;
}
