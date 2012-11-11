#include <gdev.h>
#include <assert.h>
#include <string.h>
#include <error.h>
#include <EGL/egl.h>
#include <EGL/eglext.h>
#include <EGL/eglext_brcm.h>

#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"

struct gdevcontext {
    EGLSurface surface;
    EGLContext context;
    char handle[32];
};

static EGLDisplay display = 0;

static void checkError(int line, const char* function) {
    EGLint e = eglGetError();
    if (e != EGL_SUCCESS) {
        error(1, 0, "%s at libgdev:%i: eglGetError() == 0x%4x", function, line, e);
    }
}
#define ERRORCHECK {checkError(__LINE__, __FUNCTION__);}

static void __attribute__ ((constructor)) gdevInit(void);
static void __attribute__ ((destructor)) gdevScrap(void);

void gdevInit() {
    EGLBoolean e;
    bcm_host_init();
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display!=EGL_NO_DISPLAY);
    e = eglInitialize(display, NULL, NULL); ERRORCHECK
    assert(EGL_FALSE != e);
}

void gdevScrap() {
    eglTerminate(display);
    bcm_host_deinit();
}

void gdevDestroy(GDEVContext ctx) {
    eglDestroySurface( display, ctx->surface );
    eglDestroyContext( display, ctx->context );
    free(ctx);
}

void gdevMakeCurrent(GDEVContext ctx) {
    EGLBoolean e;
    e = eglMakeCurrent(display, ctx->surface, ctx->surface, ctx->context); ERRORCHECK
    assert(EGL_FALSE != e);
}

void gdevSwapBuffers(GDEVContext ctx) {
    // eglFlushBRCM() if things don't work out with globalimages
    eglSwapBuffers(display, ctx->surface); ERRORCHECK
}

int gdevIsCurrent(GDEVContext ctx) {
    EGLContext context = eglGetCurrentContext(); ERRORCHECK
    return ctx->context == context;
}

static inline unsigned int readconfig(unsigned int *config, unsigned int field, unsigned int defaultvalue) {
    if (field < config[GDEV_CONFIG_LENGTH]) {
        return config[field];
    } else {
        return defaultvalue;
    }
}

static const EGLint attribute_list[] = {
    EGL_RED_SIZE, 8,
    EGL_GREEN_SIZE, 8,
    EGL_BLUE_SIZE, 8,
    EGL_ALPHA_SIZE, 8,
    EGL_SURFACE_TYPE, EGL_WINDOW_BIT & EGL_SWAP_BEHAVIOR_PRESERVED_BIT,
    EGL_NONE
};

static const EGLint gles2_context_attributes[] = 
{
    EGL_CONTEXT_CLIENT_VERSION, 2,
    EGL_NONE
};

static int mkcontext(GDEVContext ctx, EGLConfig config, unsigned int *cfg) {
    EGLBoolean e;
    switch (readconfig(cfg, GDEV_CONFIG_API, 0)) {
        case GDEV_API_GLESv2:
            e = eglBindAPI(EGL_OPENGL_ES_API); ERRORCHECK
            assert(EGL_FALSE != e);
            ctx->context = eglCreateContext(display, config, EGL_NO_CONTEXT, NULL); ERRORCHECK
            assert(ctx->context!=EGL_NO_CONTEXT);
            return 1;
        case GDEV_API_OPENVG:
            e = eglBindAPI(EGL_OPENVG_API); ERRORCHECK
            assert(EGL_FALSE != e);
            ctx->context = eglCreateContext(display, config, EGL_NO_CONTEXT, gles2_context_attributes); ERRORCHECK
            assert(ctx->context!=EGL_NO_CONTEXT);
            return 1;
        default:
            return 0;
    }
}

GDEVContext gdevCreateNative(const char handle[32], unsigned int *cfg) {
    EGLBoolean e;
    GDEVContext ctx = malloc(sizeof(*ctx));
    memcpy(ctx->handle, handle, 32);

    EGLConfig config;
    EGLint nconfig;
    e = eglChooseConfig(display, attribute_list, &config, 1, &nconfig); ERRORCHECK
    assert(EGL_FALSE != e);

    if (mkcontext(ctx, config, cfg) == 0) { free(ctx); return NULL; }

    ctx->surface = eglCreateWindowSurface(display, config, (EGL_DISPMANX_WINDOW_T*)ctx->handle, NULL); ERRORCHECK
    assert(ctx->surface!=EGL_NO_SURFACE);

    eglSurfaceAttrib(display, ctx->surface, EGL_SWAP_BEHAVIOR, EGL_BUFFER_PRESERVED);

    return ctx;
}

GDEVContext gdevCreateGlobal(char handle[32], int width, int height, unsigned int *cfg) {
    EGLBoolean e;
    GDEVContext ctx = malloc(sizeof(*ctx));

    EGLConfig config;
    EGLint nconfig;
    e = eglChooseConfig(display, attribute_list, &config, 1, &nconfig); ERRORCHECK
    assert(EGL_FALSE != e);

    if (mkcontext(ctx, config, cfg) == 0) { free(ctx); return NULL; }

    EGLint pixel_format = EGL_PIXEL_FORMAT_ARGB_8888_BRCM;
    EGLint rt;
    eglGetConfigAttrib( display, config, EGL_RENDERABLE_TYPE, &rt); ERRORCHECK

    if (rt & EGL_OPENGL_ES_BIT) {
        pixel_format |= EGL_PIXEL_FORMAT_RENDER_GLES_BRCM;
        pixel_format |= EGL_PIXEL_FORMAT_GLES_TEXTURE_BRCM;
    }
    if (rt & EGL_OPENGL_ES2_BIT) {
        pixel_format |= EGL_PIXEL_FORMAT_RENDER_GLES2_BRCM;
        pixel_format |= EGL_PIXEL_FORMAT_GLES2_TEXTURE_BRCM;
    }
    if (rt & EGL_OPENVG_BIT) {
        pixel_format |= EGL_PIXEL_FORMAT_RENDER_VG_BRCM;
        pixel_format |= EGL_PIXEL_FORMAT_VG_IMAGE_BRCM;
    }
    if (rt & EGL_OPENGL_BIT) {
        pixel_format |= EGL_PIXEL_FORMAT_RENDER_GL_BRCM;
    }

    int* global_image = (int*)ctx->handle;
    global_image[0] = 0;
    global_image[1] = 0;
    global_image[2] = width;
    global_image[3] = height;
    global_image[4] = pixel_format;

    eglCreateGlobalImageBRCM( width, height, global_image[4], 0, width*4, global_image); ERRORCHECK

    EGLint attrs[] = {
        EGL_VG_COLORSPACE, EGL_VG_COLORSPACE_sRGB,
        EGL_VG_ALPHA_FORMAT, pixel_format & EGL_PIXEL_FORMAT_ARGB_8888_PRE_BRCM ? EGL_VG_ALPHA_FORMAT_PRE : EGL_VG_ALPHA_FORMAT_NONPRE,
        EGL_NONE
    };

    ctx->surface = eglCreatePixmapSurface(display, config, (EGLNativePixmapType)global_image, attrs); ERRORCHECK
    assert(ctx->surface != EGL_NO_SURFACE);

    memcpy(handle, ctx->handle, 32);
    return ctx;
}

void glGDEVGlobalTargetTexture2D(GLenum target, char handle[32]) {
    int* global_image = (int*)handle;
    assert (eglQueryGlobalImageBRCM(global_image, global_image+2)); ERRORCHECK

    EGLImageKHR image = (EGLImageKHR)eglCreateImageKHR(display, EGL_NO_CONTEXT, EGL_NATIVE_PIXMAP_KHR, (EGLClientBuffer)global_image, NULL); ERRORCHECK
    assert(image != EGL_NO_IMAGE_KHR);

    // assign global image handle to texture.
    glEGLImageTargetTexture2DOES(target, image);

    // destroy global image handle, can be done after assigned to texture.
    eglDestroyImageKHR(display, image); ERRORCHECK
}
