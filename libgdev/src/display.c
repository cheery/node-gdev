#include <gdev_internal.h>

int gdevDisplayOpen(GDevDisplay* dpy, unsigned int id)
{
    DISPMANX_DISPLAY_HANDLE_T handle = vc_dispmanx_display_open( id );
    if (handle == 0) return GDEV_NO_DISPLAY;
    *dpy = fromDMDisplay(handle);
    returnSuccess;
}

int gdevDisplayGetSize(GDevDisplay dpy, int* width, int* height)
{
    int ret;
    DISPMANX_MODEINFO_T info;
    ret = vc_dispmanx_display_get_info( toDMDisplay(dpy), &info );
    if (ret != 0) return GDEV_NO_DISPLAY;
    *width = info.width;
    *height = info.height;
    returnSuccess;
}

int gdevDisplayClose(GDevDisplay dpy)
{
    int ret;
    ret = vc_dispmanx_display_close( toDMDisplay(dpy) );
    if (ret != 0) return GDEV_NO_DISPLAY;
    returnSuccess;
}

int gdevAddLayer(GDevLayer* hwd, GDevDisplay dpy, int x, int y, int width, int height, int layer)
{
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
    if (!update) return GDEV_UPDATE_LAYERS_FAIL;
    element = vc_dispmanx_element_add ( update, toDMDisplay(dpy),
                                           layer/*layer*/, &dst_rect, 0/*src*/,
                                           &src_rect, DISPMANX_PROTECTION_NONE, // FIXME: we'll probably want protection. ehm. :-D
                                           0 /*alpha*/, 0/*clamp*/, 0/*transform*/);
    vc_dispmanx_update_submit_sync( update );

    hwd->type = GDEV_NATIVE_LAYER;
    EGL_DISPMANX_WINDOW_T* w = (EGL_DISPMANX_WINDOW_T*)hwd->b;
    w->element = element;
    w->width = width;
    w->height = height;
    returnSuccess;
}
