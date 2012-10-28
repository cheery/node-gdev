#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "GLES2/gl2.h"
#include "object.h"

namespace webgl {
    using namespace v8;

    Persistent<Function> GLObject::constructor;

    void GLObject::Init(Handle<Object> target) {
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        t->SetClassName(String::NewSymbol("WebGLObject"));
        t->InstanceTemplate()->SetInternalFieldCount(1);
        constructor = Persistent<Function>::New(t->GetFunction());
    }

    GLuint GLObject::GetId(Handle<Object> object) {
        GLObject* obj = ObjectWrap::Unwrap<GLObject>(object);
        return obj->id;
    }

    Handle<Value> GLObject::New(GLuint id) {
        HandleScope scope;
        const unsigned argc = 0;
        Handle<Value> argv[argc] = { };
        Local<Object> instance = constructor->NewInstance(argc, argv);
        GLObject* obj = ObjectWrap::Unwrap<GLObject>(instance);
        obj->id = id;
        return scope.Close(instance);
    }

    Handle<Value> GLObject::New(const Arguments& args) {
        HandleScope scope;
        GLObject* obj = new GLObject();
        obj->id = 0;
        obj->Wrap(args.This());
        return args.This();
    }
}
