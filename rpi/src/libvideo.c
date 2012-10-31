/*
Copyright (c) 2012 Cheery <cheery@boxbase.org>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
/* this one is a slight wrapper over EGL, which provides most of the features. */
#include "EGL/egl.h"
#include "EGL/eglext.h"
#include "EGL/eglext_brcm.h"
#include "GLES2/gl2.h"
#include "GLES2/gl2ext.h"
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
    EGL_DISPMANX_WINDOW_T window; // this will be union
};

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

SURFACE_T videoCreateNative(uint32_t* nativewindow) {
    EGLBoolean result;
    SURFACE_T p = malloc(sizeof(*p));

    EGL_DISPMANX_WINDOW_T* window = &p->window;
    window->element = nativewindow[0];
    window->width   = nativewindow[1];
    window->height  = nativewindow[2];

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

    p->type = 1;

    return p;
}

void videoDestroySurface(SURFACE_T surface) {
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
SURFACE_T videoCreateComposite(uint32_t width, uint32_t height, int* global_image) {
    EGLBoolean result;
    SURFACE_T p = malloc(sizeof(*p));
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

    EGLint pixel_format = EGL_PIXEL_FORMAT_ARGB_8888_BRCM;
    EGLint rt;
    eglGetConfigAttrib(
        display,
        config,
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

    p->surface = eglCreatePixmapSurface(
        display,
        config,
        (EGLNativePixmapType)global_image,
        attrs
    );
    assert(p->surface != EGL_NO_SURFACE);
    return p;
}

void videoCompositeTexture2D(GLenum target, int* global_image) {
    assert (eglQueryGlobalImageBRCM(global_image, global_image+2));

    EGLImageKHR image = (EGLImageKHR)eglCreateImageKHR(
        display,
        EGL_NO_CONTEXT,
        EGL_NATIVE_PIXMAP_KHR,
        (EGLClientBuffer)global_image,
        NULL
    );
    assert(image != EGL_NO_IMAGE_KHR);

    // assign global image handle to texture.
    glEGLImageTargetTexture2DOES(target, image);

    // destroy global image handle, can be done after assigned to texture.
    eglDestroyImageKHR(display, image);
}
