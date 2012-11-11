#include <gdev.h>
#include "bcm_host.h"
#include "EGL/egl.h"
#include <assert.h>

struct gdevdisplay {
    DISPMANX_DISPLAY_HANDLE_T   display;
    DISPMANX_MODEINFO_T         info;
};

GDEVDisplay gdevDisplayOpen(unsigned int id) {
    int ret;
    GDEVDisplay dpy = malloc(sizeof(*dpy));

    dpy->display = vc_dispmanx_display_open( id );
    ret = vc_dispmanx_display_get_info( dpy->display, &dpy->info );
    assert( ret == 0 );

    return dpy;
}

void gdevDisplayGetSize(GDEVDisplay dpy, int* width, int* height) {
    *width = dpy->info.width;
    *height = dpy->info.height;
}

void gdevDisplayClose(GDEVDisplay dpy) {
    int ret;
    ret = vc_dispmanx_display_close( dpy->display );
    assert( ret == 0 );
    free(dpy);
}

void gdevDisplayAddLayer(GDEVDisplay dpy, int x, int y, int width, int height, int layer, char handle[32]) {
    EGL_DISPMANX_WINDOW_T* w = (EGL_DISPMANX_WINDOW_T*)handle;

    VC_RECT_T                   dst_rect;
    VC_RECT_T                   src_rect;
    DISPMANX_ELEMENT_HANDLE_T   element;
    DISPMANX_UPDATE_HANDLE_T    update;

    dst_rect.x = x;
    dst_rect.y = y;
    dst_rect.width = width;
    dst_rect.height = height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = width << 16;
    src_rect.height = height << 16;

    update = vc_dispmanx_update_start( 0 );
    assert( update );
    element = vc_dispmanx_element_add ( update, dpy->display,
                                           layer/*layer*/, &dst_rect, 0/*src*/,
                                           &src_rect, DISPMANX_PROTECTION_NONE, // FIXME: we'll probably want protection. ehm. :-D
                                           0 /*alpha*/, 0/*clamp*/, 0/*transform*/);
    vc_dispmanx_update_submit_sync( update );
    w->element = element;
    w->width = width;
    w->height = height;
}

void gdevDisplayRemoveLayer(GDEVDisplay dpy, char handle[32]) {
    int ret;
    EGL_DISPMANX_WINDOW_T* w = (EGL_DISPMANX_WINDOW_T*)handle;
    DISPMANX_UPDATE_HANDLE_T    update;
    update = vc_dispmanx_update_start( 10 );
    assert( update );
    ret = vc_dispmanx_element_remove( update, w->element );
    assert( ret == 0 );
    ret = vc_dispmanx_update_submit_sync( update );
    assert( ret == 0 );
}
