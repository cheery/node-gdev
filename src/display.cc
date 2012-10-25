#define BUILDING_NODE_EXTENSION
#include <node.h>

#include "bcm_host.h"

#include "display.h"

using namespace v8;

#define ALIGN_UP(x,y)  ((x + (y)-1) & ~((y)-1))

Display::Display() {};
Display::~Display() {
    CloseImpl();
};


void Display::Init(Handle<Object> target) {
    bcm_host_init();
    Local<FunctionTemplate> tpl = FunctionTemplate::New(New);
    tpl->SetClassName(String::NewSymbol("Display"));
    tpl->InstanceTemplate()->SetInternalFieldCount(1);
    tpl->PrototypeTemplate()->Set(
        String::NewSymbol("close"),
        FunctionTemplate::New(Close)->GetFunction()
    );
    Persistent<Function> constructor = Persistent<Function>::New(tpl->GetFunction());
    target->Set(String::NewSymbol("Display"), constructor);
}

/*
Handle<Value> Display::NewInstance(const Arguments& args) {
  HandleScope scope;

  const unsigned argc = 1;
  Handle<Value> argv[argc] = { args[0] };
  Local<Object> instance = constructor->NewInstance(argc, argv);

  return scope.Close(instance);
}
*/

Handle<Value> Display::New(const Arguments& args) {
    HandleScope scope;
    Display* obj = new Display();
    obj->OpenImpl(args[0]->Uint32Value());
    Handle<Object> This = args.This();
    This->Set(String::New("width"), Integer::New(obj->info.width));
    This->Set(String::New("height"), Integer::New(obj->info.height));
    obj->Wrap(args.This());
    return args.This();
}

Handle<Value> Display::Close(const Arguments& args) {
    HandleScope scope;
    Display* obj = ObjectWrap::Unwrap<Display>(args.This());
    obj->CloseImpl();
    return Undefined();
}

static void FillRect( VC_IMAGE_TYPE_T type, void *image, int pitch, int aligned_height, int x, int y, int w, int h, int val )
{
    int         row;
    int         col;

    uint16_t *line = (uint16_t *)image + y * (pitch>>1) + x;

    for ( row = 0; row < h; row++ )
    {
        for ( col = 0; col < w; col++ )
        {
            line[col] = val;
        }
        line += (pitch>>1);
    }
}

void Display::OpenImpl(uint32_t screen) {
    int ret;

    display = vc_dispmanx_display_open( screen );
    closed = false;

    ret = vc_dispmanx_display_get_info( display, &info);
    assert(ret == 0);

    VC_RECT_T       src_rect;
    VC_RECT_T       dst_rect;
    VC_IMAGE_TYPE_T type = VC_IMAGE_RGB565;
    int width = info.width;
    int height = info.height;
    int pitch = ALIGN_UP(width*2, 32);
    int aligned_height = ALIGN_UP(height, 16);
    VC_DISPMANX_ALPHA_T alpha = { (DISPMANX_FLAGS_ALPHA_T)(DISPMANX_FLAGS_ALPHA_FROM_SOURCE | DISPMANX_FLAGS_ALPHA_FIXED_ALL_PIXELS), 120, /*alpha 0->255*/ 0 };

    image = calloc( 1, pitch * height );
    assert(image);

    FillRect( type, image, pitch, aligned_height,  0,  0, width,      height,      0xFFFF );
    FillRect( type, image, pitch, aligned_height,  0,  0, width,      height,      0xF800 );
    FillRect( type, image, pitch, aligned_height, 20, 20, width - 40, height - 40, 0x07E0 );
    FillRect( type, image, pitch, aligned_height, 40, 40, width - 80, height - 80, 0x001F );

        resource = vc_dispmanx_resource_create( type,
                                                      width,
                                                      height,
                                                      &vc_image_ptr );
        assert( resource );
        vc_dispmanx_rect_set( &dst_rect, 0, 0, width, height);
        ret = vc_dispmanx_resource_write_data(  resource,
                                                type,
                                                pitch,
                                                image,
                                                &dst_rect );
        assert( ret == 0 );
        update = vc_dispmanx_update_start( 10 );
        assert( update );
    
        vc_dispmanx_rect_set( &src_rect, 0, 0, width << 16, height << 16 );
    
        vc_dispmanx_rect_set( &dst_rect, 0, 0, width, height );

        element = vc_dispmanx_element_add(    update,
                                                    display,
                                                    2000,               // layer
                                                    &dst_rect,
                                                    resource,
                                                    &src_rect,
                                                    DISPMANX_PROTECTION_NONE,
                                                    &alpha,
                                                    NULL,             // clamp
                                                    (DISPMANX_TRANSFORM_T)VC_IMAGE_ROT0 );

        ret = vc_dispmanx_update_submit_sync( update );
        assert( ret == 0 );
}

void Display::CloseImpl() {
    int ret;

    if (!closed) {
        update = vc_dispmanx_update_start( 10 );
        assert( update );
        ret = vc_dispmanx_element_remove( update, element );
        assert( ret == 0 );
        ret = vc_dispmanx_update_submit_sync( update );
        assert( ret == 0 );
        ret = vc_dispmanx_resource_delete( resource );
        assert( ret == 0 );
        ret = vc_dispmanx_display_close( display );
        assert( ret == 0 );
        closed = true;
    }
};
