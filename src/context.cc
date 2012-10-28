class VideoContext : public node::ObjectWrap {
    public:
    static void Init(Handle<Object> target);
    static Handle<Value> NewInstance(const Arguments& args);

    private:
    VideoContext();
    ~VideoContext();

    static Handle<Value> New(const Arguments& args);
    static Handle<Value> Close(const Arguments& args);
    static Handle<Value> MakeCurrent(const Arguments& args);
    static Handle<Value> SwapBuffers(const Arguments& args);
//    static Handle<Value> WidthGetter(Local<String> property, const AccessorInfo& info);
//    static Handle<Value> HeightGetter(Local<String> property, const AccessorInfo& info);
//    static Handle<Value> HandleGetter(Local<String> property, const AccessorInfo& info);

    static Persistent<Function> constructor;
    SURFACE_T surface;
};

VideoContext::VideoContext() { }
VideoContext::~VideoContext() {
    if (surface != NULL){
        videoDestroySurface(surface);
        surface = NULL;
    }
}

Persistent<Function> VideoContext::constructor;

void VideoContext::Init(Handle<Object> target) {
    Local<FunctionTemplate> t = FunctionTemplate::New(New);
    t->SetClassName(String::NewSymbol("VideoContext"));
    t->InstanceTemplate()->SetInternalFieldCount(1);
    t->PrototypeTemplate()->Set(String::NewSymbol("close"),
        FunctionTemplate::New(Close)->GetFunction());
    t->PrototypeTemplate()->Set(String::NewSymbol("makeCurrent"),
        FunctionTemplate::New(MakeCurrent)->GetFunction());
    t->PrototypeTemplate()->Set(String::NewSymbol("swapBuffers"),
        FunctionTemplate::New(SwapBuffers)->GetFunction());
//    t->PrototypeTemplate()->SetAccessor(String::NewSymbol("width"), WidthGetter);
//    t->PrototypeTemplate()->SetAccessor(String::NewSymbol("height"), HeightGetter);
//    t->PrototypeTemplate()->SetAccessor(String::NewSymbol("handle"), HandleGetter);
        
    constructor = Persistent<Function>::New(t->GetFunction());
}

Handle<Value> VideoContext::NewInstance(const Arguments& args) {
    HandleScope scope;

    const unsigned argc = 1;
    Handle<Value> argv[argc] = { args[0] };
    Local<Object> instance = constructor->NewInstance(argc, argv);

    return scope.Close(instance);
}

Handle<Value> VideoContext::New(const Arguments& args) {
    HandleScope scope;
    Local<Object> buffer_obj = args[0]->ToObject();
    char *buffer_data = node::Buffer::Data(buffer_obj);
    size_t buffer_length = node::Buffer::Length(buffer_obj);
    uint32_t* nativewindow = (uint32_t*)buffer_data;
    assert (buffer_length == sizeof(uint32_t)*NATIVEWINDOW_SIZE);
    VideoContext* obj = new VideoContext();
    obj->surface = videoCreateNative(nativewindow);
    obj->Wrap(args.This());
    return args.This();
}

Handle<Value> VideoContext::Close(const Arguments& args) {
    HandleScope scope;
    VideoContext* obj = ObjectWrap::Unwrap<VideoContext>(args.This());
    if (obj->surface != NULL){
        videoDestroySurface(obj->surface);
        obj->surface = NULL;
    }
    return Undefined();
}

Handle<Value> VideoContext::MakeCurrent(const Arguments& args) {
    HandleScope scope;
    VideoContext* obj = ObjectWrap::Unwrap<VideoContext>(args.This());
    videoMakeCurrent(obj->surface);
    return Undefined();
}

Handle<Value> VideoContext::SwapBuffers(const Arguments& args) {
    HandleScope scope;
    VideoContext* obj = ObjectWrap::Unwrap<VideoContext>(args.This());
    videoSwapBuffers(obj->surface);
    return Undefined();
}
