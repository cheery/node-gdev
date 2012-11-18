#include "gdev_internal.h"

int gdevConfigure(GDevConfig* cfg, int* kw)
{
    const int PREDEFINED = 1;
    int ac;
    for (ac = 0; kw[2*ac] != 0; ac++);
    EGLint attrlist[2*ac + 2*PREDEFINED + 1];

    int i;
    int j = 0;
    for (i = 0; kw[2*i] != 0; i++) {
        int value = kw[2*i+1];
        switch(kw[2*i]) {
            case GDEV_RED_SIZE:
                attrlist[j++] = EGL_RED_SIZE;
                attrlist[j++] = value;
                break;
            case GDEV_GREEN_SIZE:
                attrlist[j++] = EGL_GREEN_SIZE;
                attrlist[j++] = value;
                break;
            case GDEV_BLUE_SIZE:
                attrlist[j++] = EGL_BLUE_SIZE;
                attrlist[j++] = value;
                break;
            case GDEV_ALPHA_SIZE:
                attrlist[j++] = EGL_ALPHA_SIZE;
                attrlist[j++] = value;
                break;
        }
    }
    attrlist[j++] = EGL_SURFACE_TYPE;
    attrlist[j++] = EGL_WINDOW_BIT | EGL_PBUFFER_BIT;
    attrlist[j++] = EGL_NONE;

    EGLBoolean e;
    EGLConfig config;
    EGLint nconfig;
    e = eglChooseConfig(display, attrlist, &config, 1, &nconfig);
    returnOnEglError;
    returnErrorIf(GDEV_NO_CONFIG, EGL_FALSE == e);
    returnErrorIf(GDEV_NO_CONFIG, nconfig == 0);

    *cfg = fromEGLConfig(config);
    returnSuccess;
}
