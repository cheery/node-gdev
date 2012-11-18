#include "gdev_internal.h"

static const EGLint gles2_context_attributes[] = 
{
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
};

int gdevCreateContext(GDevContext* ctx, GDevConfig cfg, int api)
{
    EGLBoolean e;
    const EGLint* attr;
    switch (api) {
        case GDEV_OPENGL_ES_2: {
            e = eglBindAPI(EGL_OPENGL_ES_API);
            attr = gles2_context_attributes;
            break;
        }
        case GDEV_OPENVG: {
            e = eglBindAPI(EGL_OPENVG_API);
            attr = NULL;
            break;
        }
        default:
            return GDEV_UNKNOWN_API;
    }
    returnErrorIf(GDEV_API_BIND_FAIL, e == EGL_FALSE);
    returnOnEglError;
    EGLContext context = eglCreateContext(display, toEGLConfig(cfg), EGL_NO_CONTEXT, attr);
    returnErrorIf(GDEV_NO_CONTEXT, context == EGL_NO_CONTEXT);
    returnOnEglError;

    *ctx = fromEGLContext(context);
    returnSuccess;
}

int gdevMakeCurrent(GDevSurface draw, GDevSurface read, GDevContext ctx)
{
    EGLBoolean e = eglMakeCurrent(display, toEGLSurface(draw), toEGLSurface(read), toEGLContext(ctx));
    returnErrorIf(GDEV_INVALID_ARGUMENTS, EGL_FALSE == e);
    returnOnEglError;
    returnSuccess;
}

int gdevGetCurrent(GDevContext* ctx)
{
    EGLContext context = eglGetCurrentContext();
    if (context == EGL_NO_CONTEXT) {
        *ctx = NULL;
    } else {
        *ctx = fromEGLContext(context);
    }
    returnOnEglError;
    returnSuccess;
}

int gdevDestroyContext(GDevContext ctx)
{
    eglDestroyContext(display, toEGLContext(ctx));
    returnSuccess;
}
