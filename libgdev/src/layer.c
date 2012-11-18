#include "gdev_internal.h"

int gdevCreateLayer(GDevLayer* hwd, int width, int height, GDevConfig cfg)
{
    EGLint pixel_format = EGL_PIXEL_FORMAT_ARGB_8888_BRCM;
    EGLint rt;
    eglGetConfigAttrib(display, toEGLConfig(cfg), EGL_RENDERABLE_TYPE, &rt);
    returnOnEglError;

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

    hwd->type = GDEV_GLOBAL_LAYER;
    hwd->d[0] = 0;
    hwd->d[1] = 0;
    hwd->d[2] = width;
    hwd->d[3] = height;
    hwd->d[4] = pixel_format;

    eglCreateGlobalImageBRCM( width, height, hwd->d[4], 0, width*4, hwd->d);
    returnOnEglError;

    returnSuccess;
}

int gdevDestroyLayer(GDevLayer* hwd)
{
    switch (hwd->type) {
        case GDEV_GLOBAL_LAYER:
            eglDestroyGlobalImageBRCM(hwd->d);
            break;
        case GDEV_NATIVE_LAYER: {
            int ret;
            EGL_DISPMANX_WINDOW_T* w = (EGL_DISPMANX_WINDOW_T*)hwd;
            DISPMANX_UPDATE_HANDLE_T update;
            update = vc_dispmanx_update_start( 10 );
            if (!update) return GDEV_UPDATE_LAYERS_FAIL;
            ret = vc_dispmanx_element_remove( update, w->element );
            if (ret != 0) return GDEV_UPDATE_LAYERS_FAIL;
            ret = vc_dispmanx_update_submit_sync( update );
            if (ret != 0) return GDEV_UPDATE_LAYERS_FAIL;
            break;
        }
        default:
            return GDEV_UNKNOWN_LAYER;
    }
    hwd->type = 0;
    returnSuccess;
}
