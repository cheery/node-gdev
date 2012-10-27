extern "C" {
#include "display.h"
}

class Display : public node::ObjectWrap {
    public:
    static void Init(Handle<Object> target);
    static Handle<Value> NewInstance(const Arguments& args);

    private:
    Display();
    ~Display();

    static Handle<Value> New(const Arguments& args);
    static Handle<Value> Close(const Arguments& args);
    static Handle<Value> WidthGetter(Local<String> property, const AccessorInfo& info);
    static Handle<Value> HeightGetter(Local<String> property, const AccessorInfo& info);
    static Handle<Value> HandleGetter(Local<String> property, const AccessorInfo& info);

    static Persistent<Function> constructor;
    DISPLAY_T display;
};

Display::Display() { }
Display::~Display() {
    if (display != NULL){
        displayClose(display);
        display = NULL;
    }
}

Persistent<Function> Display::constructor;

void Display::Init(Handle<Object> target) {
    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    t->SetClassName(String::NewSymbol("Display"));
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->PrototypeTemplate()->Set(String::NewSymbol("close"),
        FunctionTemplate::New(Close)->GetFunction());
    t->PrototypeTemplate()->SetAccessor(String::NewSymbol("width"), WidthGetter);
    t->PrototypeTemplate()->SetAccessor(String::NewSymbol("height"), HeightGetter);
    t->PrototypeTemplate()->SetAccessor(String::NewSymbol("handle"), HandleGetter);
        
    constructor = Persistent<Function>::New(t->GetFunction());
}

Handle<Value> Display::NewInstance(const Arguments& args) {
    HandleScope scope;

    const unsigned argc = 1;
    Handle<Value> argv[argc] = { args[0] };
    Local<Object> instance = constructor->NewInstance(argc, argv);

    return scope.Close(instance);
}

Handle<Value> Display::New(const Arguments& args) {
    HandleScope scope;
    Display* obj = new Display();
    obj->display = displayOpen(args[0]->Uint32Value()); 
    obj->Wrap(args.This());
    return args.This();
}

Handle<Value> Display::Close(const Arguments& args) {
    HandleScope scope;
    Display* obj = ObjectWrap::Unwrap<Display>(args.This());
    if (obj->display != NULL){
        displayClose(obj->display);
        obj->display = NULL;
    }
    return Undefined();
}

Handle<Value> Display::WidthGetter(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    Display* obj = ObjectWrap::Unwrap<Display>(info.This());
    uint32_t width, height;
    displayGetSize(obj->display, &width, &height);
    return scope.Close(Integer::New(width));
}

Handle<Value> Display::HeightGetter(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    Display* obj = ObjectWrap::Unwrap<Display>(info.This());
    uint32_t width, height;
    displayGetSize(obj->display, &width, &height);
    return scope.Close(Integer::New(height));
}

Handle<Value> Display::HandleGetter(Local<String> property, const AccessorInfo& info) {
    HandleScope scope;
    Display* obj = ObjectWrap::Unwrap<Display>(info.This());
    uint32_t nativewindow[NATIVEWINDOW_SIZE];
    displayGetHandle(obj->display, nativewindow);
    return node::Encode(nativewindow, sizeof(nativewindow), node::BINARY);
}
