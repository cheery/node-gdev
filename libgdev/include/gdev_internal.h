#include <gdev.h>
#include "bcm_host.h"
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglext_brcm.h>

struct gdev_surface {
    EGLSurface surface;
    GDevLayer layer;
};

extern EGLDisplay display;

static inline GDevDisplay fromDMDisplay(DISPMANX_DISPLAY_HANDLE_T dpy) { return (GDevDisplay)dpy; }
static inline DISPMANX_DISPLAY_HANDLE_T toDMDisplay(GDevDisplay dpy) { return (DISPMANX_DISPLAY_HANDLE_T)dpy; }

static inline GDevConfig fromEGLConfig(EGLConfig cfg) { return (GDevConfig)cfg; }
static inline EGLConfig toEGLConfig(GDevConfig cfg) { return (EGLConfig)cfg; }

static inline GDevImage fromEGLImage(EGLImageKHR img) { return (GDevImage)img; }
static inline EGLImageKHR toEGLImage(GDevImage img) { return (EGLImageKHR)img; }

static inline EGLSurface toEGLSurface(GDevSurface f) { return f->surface; }

static inline EGLContext toEGLContext(GDevContext ctx) { return (EGLContext)ctx; }
static inline GDevContext fromEGLContext(EGLContext ctx) { return (GDevContext)ctx; }

int toGDevError(EGLint e);

#define returnErrorIf(value, cond) {if(cond) return (value);}
#define returnOnEglError {EGLint e = eglGetError(); if (e != EGL_SUCCESS) return toGDevError(e);}
#define returnSuccess return GDEV_SUCCESS


