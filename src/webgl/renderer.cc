#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "../videocontext.h"
#include "renderer.h"
#include "GLES2/gl2.h"

namespace webgl {

    Persistent<Function> Renderer::constructor;

    void Renderer::Init(Handle<Object> target) {
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        t->SetClassName(String::NewSymbol("WebGLRenderer"));
        t->InstanceTemplate()->SetInternalFieldCount(1);
        NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
        NODE_SET_PROTOTYPE_METHOD(t, "swapBuffers", SwapBuffers);
        NODE_SET_PROTOTYPE_METHOD(t, "clearColor", ClearColor);
        NODE_SET_PROTOTYPE_METHOD(t, "clear", Clear);
        t->PrototypeTemplate()->Set(String::NewSymbol("COLOR_BUFFER_BIT"),
            Integer::New(GL_COLOR_BUFFER_BIT));
        //SET_PROTOTYPE_CONSTANT("COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT);
        constructor = Persistent<Function>::New(t->GetFunction());
    }

    Handle<Value> Renderer::Synthesize(IVideoContext* context) {
        HandleScope scope;
        const unsigned argc = 0;
        Handle<Value> argv[argc] = { };
        Local<Object> instance = constructor->NewInstance(argc, argv);
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(instance);
        obj->context = context;
        return scope.Close(instance);
    }

    Handle<Value> Renderer::New(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = new Renderer();
        obj->context = NULL; // I need to set (IVideoContext) to this variable.
        obj->Wrap(args.This());
        return args.This();
    }

    Handle<Value> Renderer::Close(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->context->Close();
        obj->context = NULL;
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::SwapBuffers(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->context->SwapBuffers();
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::ClearColor(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        double r = args[0]->NumberValue();
        double g = args[1]->NumberValue();
        double b = args[2]->NumberValue();
        double a = args[3]->NumberValue();
        obj->MakeCurrentLazy();
        glClearColor(r,g,b,a);
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Clear(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        uint32_t flags = args[0]->IntegerValue();
        obj->MakeCurrentLazy();
        glClear(flags);
        return scope.Close(Undefined());
    }
}
