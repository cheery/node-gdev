#include <v8.h>
#include <node.h>
#include <node_buffer.h>

#include "GLES2/gl2.h"
#include "../videocontext.h"
#include "object.h"
#include "renderer.h"

#define SET_PROTOTYPE_CONSTANT(tpl, name, value) \
    tpl->PrototypeTemplate()->Set(String::NewSymbol(name), \
        Integer::New(value));

namespace webgl {

    Persistent<Function> Renderer::constructor;

    void Renderer::Init(Handle<Object> target) {
        Local<FunctionTemplate> t = FunctionTemplate::New(New);
        t->SetClassName(String::NewSymbol("WebGLRenderer"));
        t->InstanceTemplate()->SetInternalFieldCount(1);
        NODE_SET_PROTOTYPE_METHOD(t, "close", Close);
        NODE_SET_PROTOTYPE_METHOD(t, "swapBuffers", SwapBuffers);
        ////Per-Fragment Operations
        //
        ////Whole Framebuffer Operations
        //
        NODE_SET_PROTOTYPE_METHOD(t, "clearColor", ClearColor);
        NODE_SET_PROTOTYPE_METHOD(t, "clear", Clear);
        SET_PROTOTYPE_CONSTANT(t, "COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_BUFFER_BIT", GL_DEPTH_BUFFER_BIT);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BUFFER_BIT", GL_STENCIL_BUFFER_BIT);
        ////Buffer Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "createBuffer", CreateBuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteBuffer", DeleteBuffer);
        ////View and Clip
        //
        ////Rasterization
        //
        ////Detect and Enable Extensions
        //
        ////Detect context lost events
        //
        ////Programs and Shaders
        //
        ////Uniforms and Attributes
        //
        ////Texture Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "createTexture", CreateTexture);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteTexture", DeleteTexture);
        ////Special Functions
        //
        ////Renderbuffer Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "createRenderbuffer", CreateRenderbuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteRenderbuffer", DeleteRenderbuffer);
        ////Writing to the Draw Buffer
        //
        NODE_SET_PROTOTYPE_METHOD(t, "drawArrays", DrawArrays);
        NODE_SET_PROTOTYPE_METHOD(t, "drawElements", DrawElements);
        SET_PROTOTYPE_CONSTANT(t, "POINTS", GL_POINTS);
        SET_PROTOTYPE_CONSTANT(t, "LINE_STRIP", GL_LINE_STRIP);
        SET_PROTOTYPE_CONSTANT(t, "LINE_LOOP", GL_LINE_LOOP);
        SET_PROTOTYPE_CONSTANT(t, "LINES", GL_LINES);
        SET_PROTOTYPE_CONSTANT(t, "TRIANGLE_STRIP", GL_TRIANGLE_STRIP);
        SET_PROTOTYPE_CONSTANT(t, "TRIANGLE_FAN", GL_TRIANGLE_FAN);
        SET_PROTOTYPE_CONSTANT(t, "TRIANGLES", GL_TRIANGLES);
        ////Read Back Pixels
        //
        ////Framebuffer Objects
        //
        ////Datatype constants
        //
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_BYTE", GL_UNSIGNED_BYTE);
        SET_PROTOTYPE_CONSTANT(t, "UNSIGNED_SHORT", GL_UNSIGNED_SHORT);
        SET_PROTOTYPE_CONSTANT(t, "BYTE", GL_BYTE);
        SET_PROTOTYPE_CONSTANT(t, "SHORT", GL_SHORT);
        SET_PROTOTYPE_CONSTANT(t, "FIXED", GL_FIXED);
        SET_PROTOTYPE_CONSTANT(t, "FLOAT", GL_FLOAT);
        //
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
    ////Per-Fragment Operations
    //
    ////Whole Framebuffer Operations
    //
    Handle<Value> Renderer::ClearColor(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        double r = args[0]->NumberValue();
        double g = args[1]->NumberValue();
        double b = args[2]->NumberValue();
        double a = args[3]->NumberValue();
        glClearColor(r,g,b,a);
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Clear(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        uint32_t flags = args[0]->IntegerValue();
        glClear(flags);
        return scope.Close(Undefined());
    }
    ////Buffer Objects
    //
    Handle<Value> Renderer::CreateBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id;
        glGenBuffers(1, &id);
        return scope.Close(GLObject::New(id));
    }

    Handle<Value> Renderer::DeleteBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteBuffers(1, &id);
        return scope.Close(Undefined());
    }

    ////View and Clip
    //
    ////Rasterization
    //
    ////Detect and Enable Extensions
    //
    ////Detect context lost events
    //
    ////Programs and Shaders
    //
    ////Uniforms and Attributes
    //
    ////Texture Objects
    //
    Handle<Value> Renderer::CreateTexture(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id;
        glGenTextures(1, &id);
        return scope.Close(GLObject::New(id));
    }

    Handle<Value> Renderer::DeleteTexture(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteTextures(1, &id);
        return scope.Close(Undefined());
    }
    ////Special Functions
    //
    ////Renderbuffer Objects
    //
    Handle<Value> Renderer::CreateRenderbuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id;
        glGenRenderbuffers(1, &id);
        return scope.Close(GLObject::New(id));
    }

    Handle<Value> Renderer::DeleteRenderbuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glDeleteRenderbuffers(1, &id);
        return scope.Close(Undefined());
    }
    ////Writing to the Draw Buffer
    //
    Handle<Value> Renderer::DrawArrays(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDrawArrays(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::DrawElements(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDrawElements(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            (void*)args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    ////Read Back Pixels
    //
    ////Framebuffer Objects
    //
    /*
    Handle<Value> Renderer::Template(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();

        return scope.Close(Undefined());
    }
    */
}
