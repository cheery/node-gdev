/* this one is a slight wrapper over EGL, which provides most of the features. */
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "GLES2/gl2.h"
/*  */
#include <assert.h>

//for error handling.
#include <stdio.h>

#include "video.h"

static void check(int line, const char* function) {
    EGLint e = eglGetError();
    if (e != EGL_SUCCESS) {
        printf("libvideo.c:%i: EGL error at %s: 0x%4x\n", line, function, e);
    }
}
#define CHECK() check(__LINE__, __FUNCTION__);


static void __attribute__ ((constructor)) libraryInit(void);
static void __attribute__ ((destructor)) libraryScrap(void);

static EGLDisplay display;

static void libraryInit() {
    EGLBoolean result;
    display = eglGetDisplay(EGL_DEFAULT_DISPLAY);
    assert(display!=EGL_NO_DISPLAY);
    result = eglInitialize(display, NULL, NULL);
    CHECK();
    assert(EGL_FALSE != result);
}

static void libraryScrap() {
    eglTerminate(display);
}

struct surface {
    EGLSurface surface;
    EGLContext context;
    int type;
    int current;
    EGL_DISPMANX_WINDOW_T window; // this will be union
};

SURFACE_T videoCreateNative(uint32_t* nativewindow) {
    EGLBoolean result;
    SURFACE_T p = malloc(sizeof(*p));

    EGL_DISPMANX_WINDOW_T* window = &p->window;
    window->element = nativewindow[0];
    window->width   = nativewindow[1];
    window->height  = nativewindow[2];

    // does "webgl" only, no specials.
    static const EGLint attribute_list[] =
    {
        EGL_RED_SIZE, 8,
        EGL_GREEN_SIZE, 8,
        EGL_BLUE_SIZE, 8,
        EGL_ALPHA_SIZE, 8,
        EGL_SURFACE_TYPE, EGL_WINDOW_BIT,
        EGL_NONE
    };

    static const EGLint context_attributes[] = 
    {
        EGL_CONTEXT_CLIENT_VERSION, 2,
        EGL_NONE
    };

    EGLConfig config;
    EGLint num_config;
    result = eglChooseConfig(display, attribute_list, &config, 1, &num_config);
    CHECK();
    assert(EGL_FALSE != result);
    result = eglBindAPI(EGL_OPENGL_ES_API);
    CHECK();
    assert(EGL_FALSE != result);

    p->context = eglCreateContext(display, config, EGL_NO_CONTEXT, context_attributes);
    CHECK();
    assert(p->context!=EGL_NO_CONTEXT);

    p->surface = eglCreateWindowSurface( display, config, window, NULL );
    CHECK();
    assert(p->surface != EGL_NO_SURFACE);

    p->current = 0;
    p->type = 1;

    return p;
}

void videoDestroySurface(SURFACE_T surface) {
    if (surface->current) videoMakeCurrent(NULL);
    eglDestroySurface( display, surface->surface );
    eglDestroyContext( display, surface->context );
    free(surface);
}

void videoMakeCurrent(SURFACE_T p) {
    EGLBoolean result;
    if (p == NULL) {
        result = eglMakeCurrent(display, EGL_NO_SURFACE, EGL_NO_SURFACE, EGL_NO_CONTEXT);
    } else {
        result = eglMakeCurrent(display, p->surface, p->surface, p->context);
        p->current = 1;
    }
    CHECK();
    assert(EGL_FALSE != result);
}

int videoIsCurrent(SURFACE_T p) {
    EGLContext context = eglGetCurrentContext();
    return p->context == context;
}

void videoSwapBuffers(SURFACE_T p) {
    // eglFlushBRCM() if things don't work out
    eglSwapBuffers(display, p->surface);
    CHECK();
}

// this method is retarded.
/*
SURFACE_T videoCreateComposite(uint32_t width, uint32_t height, int* global_image) {
    EGLint pixel_format = EGL_PIXEL_FORMAT_ARGB_8888_BRCM;
    EGLint rt;
    eglGetConfigAttrib(
        display->display,
        display->config,
        EGL_RENDERABLE_TYPE,
        &rt
    );

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
    int* global_image = handle;
    global_image[0] = 0;
    global_image[1] = 0;
    global_image[2] = width;
    global_image[3] = height;
    global_image[4] = pixel_format;

    eglCreateGlobalImageBRCM(
        width,
        height,
        global_image[4],
        0,
        width*4,
        global_image
    );

    EGLint attrs[] = {
        EGL_VG_COLORSPACE, EGL_VG_COLORSPACE_sRGB,
        EGL_VG_ALPHA_FORMAT, pixel_format & EGL_PIXEL_FORMAT_ARGB_8888_PRE_BRCM ? EGL_VG_ALPHA_FORMAT_PRE : EGL_VG_ALPHA_FORMAT_NONPRE,
        EGL_NONE
    };

    EGLSurface surface = eglCreatePixmapSurface(
        display->display,
        display->config,
        (EGLNativePixmapType)global_image,
        attrs
    );
    assert(surface != EGL_NO_SURFACE);
}

void videoCompositeTexture2D(GLEnum target, int* global_image) {
    int* global_image = handle;
    assert (eglQueryGlobalImageBRCM(global_image, global_image+2));

    EGLImageKHR image = (EGLImageKHR)eglCreateImageKHR(
        display->display,
        EGL_NO_CONTEXT,
        EGL_NATIVE_PIXMAP_KHR,
        (EGLClientBuffer)global_image,
        NULL
    );
    assert(image != EGL_NO_IMAGE_KHR);

    GlobalImage g = malloc(sizeof(*g));
    g->display = display;
    g->image = image;
    g->width = global_image[2];
    g->height = global_image[3];
    return g;

    //// destroy global image handle, can be done after assigned to texture.
    //
    eglDestroyImageKHR(image->display->display, image->image);

    //// assign global image handle to texture.
    //
    glEGLImageTargetTexture2DOES(target, image->image);
}
*/
