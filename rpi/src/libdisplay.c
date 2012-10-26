/* this library relies on BCM_HOST */
#include "bcm_host.h"
#include "EGL/egl.h"
/*  */
#include <stdlib.h>
#include <assert.h>

#include "display.h"

#define TRUE 1
#define FALSE 0

static void __attribute__ ((constructor)) libraryInit(void);
static void __attribute__ ((destructor)) libraryScrap(void);

static void libraryInit() {
    bcm_host_init();
}

static void libraryScrap() {
}

struct display {
    DISPMANX_DISPLAY_HANDLE_T   display;
    DISPMANX_MODEINFO_T         info;
    DISPMANX_ELEMENT_HANDLE_T   element;
};

DISPLAY_T displayOpen(uint32_t id) {
    int ret;
    DISPLAY_T p = malloc(sizeof(*p));

    p->display = vc_dispmanx_display_open( id );
    ret = vc_dispmanx_display_get_info( p->display, &p->info );
    assert( ret == 0 );

    VC_RECT_T                   dst_rect;
    VC_RECT_T                   src_rect;
    DISPMANX_UPDATE_HANDLE_T    update;

    dst_rect.x = 0;
    dst_rect.y = 0;
    dst_rect.width = p->info.width;
    dst_rect.height = p->info.height;

    src_rect.x = 0;
    src_rect.y = 0;
    src_rect.width = p->info.width << 16;
    src_rect.height = p->info.height << 16;

    update = vc_dispmanx_update_start( 0 );
    p->element = vc_dispmanx_element_add ( update, p->display,
                                           0/*layer*/, &dst_rect, 0/*src*/,
                                           &src_rect, DISPMANX_PROTECTION_NONE,
                                           0 /*alpha*/, 0/*clamp*/, 0/*transform*/);
    vc_dispmanx_update_submit_sync( update );
    
    return p;
}

void displayClose(DISPLAY_T p) {
    int ret;
    DISPMANX_UPDATE_HANDLE_T    update;
    update = vc_dispmanx_update_start( 10 );
    assert( update );
    ret = vc_dispmanx_element_remove( update, p->element );
    assert( ret == 0 );
    ret = vc_dispmanx_update_submit_sync( update );
    assert( ret == 0 );
    ret = vc_dispmanx_display_close( p->display );
    assert( ret == 0 );
    free(p);
}

void displayGetSize(DISPLAY_T p, uint32_t* width, uint32_t* height) {
    *width = p->info.width;
    *height = p->info.height;
}

void displayGetHandle(DISPLAY_T p, uint32_t* nativewindow) {
    nativewindow[0] = p->element;
    nativewindow[1] = p->info.width;
    nativewindow[2] = p->info.height;
}
