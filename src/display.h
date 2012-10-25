#ifndef DISPLAY_H
#define DISPLAY_H

#include <node.h>

class Display : public node::ObjectWrap {
    public:
    static void Init(v8::Handle<v8::Object> target);
//    static v8::Handle<v8::Value> NewInstance(const v8::Arguments& args);

    private:
    Display();
    ~Display();

//    static v8::Persistent<v8::Function> constructor;
    static v8::Handle<v8::Value> New(const v8::Arguments& args);
    static v8::Handle<v8::Value> Close(const v8::Arguments& args);

    void OpenImpl(uint32_t screen);
    void CloseImpl();

    bool closed;

    DISPMANX_DISPLAY_HANDLE_T   display;
    DISPMANX_MODEINFO_T         info;
    void                       *image;
    DISPMANX_UPDATE_HANDLE_T    update;
    DISPMANX_RESOURCE_HANDLE_T  resource;
    DISPMANX_ELEMENT_HANDLE_T   element;
    uint32_t                    vc_image_ptr;
};

#endif
