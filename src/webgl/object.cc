/*
Copyright (c) 2012 Cheery <cheery@boxbase.org>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
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
