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
        NODE_SET_PROTOTYPE_METHOD(t, "blendColor", BlendColor);
        NODE_SET_PROTOTYPE_METHOD(t, "blendEquation", BlendEquation);
        NODE_SET_PROTOTYPE_METHOD(t, "blendEquationSeparate", BlendEquationSeparate);
        SET_PROTOTYPE_CONSTANT(t, "FUNC_ADD", GL_FUNC_ADD);
        SET_PROTOTYPE_CONSTANT(t, "FUNC_SUBTRACT", GL_FUNC_SUBTRACT);
        SET_PROTOTYPE_CONSTANT(t, "FUNC_REVERSE_SUBTRACT", GL_FUNC_REVERSE_SUBTRACT);
        NODE_SET_PROTOTYPE_METHOD(t, "blendFunc", BlendFunc);
        NODE_SET_PROTOTYPE_METHOD(t, "blendFuncSeparate", BlendFuncSeparate);
        SET_PROTOTYPE_CONSTANT(t, "SRC_ALPHA_SATURATE", GL_SRC_ALPHA_SATURATE);
        SET_PROTOTYPE_CONSTANT(t, "ZERO", GL_ZERO);
        SET_PROTOTYPE_CONSTANT(t, "ONE", GL_ONE);
        SET_PROTOTYPE_CONSTANT(t, "SRC_COLOR", GL_SRC_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "ONE_MINUS_SRC_COLOR", GL_ONE_MINUS_SRC_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "DST_COLOR", GL_DST_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "ONE_MINUS_DST_COLOR", GL_ONE_MINUS_DST_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "SRC_ALPHA", GL_SRC_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "SRC_ONE_MINUS_ALPHA", GL_ONE_MINUS_SRC_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "DST_ALPHA", GL_DST_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "DST_ONE_MINUS_ALPHA", GL_ONE_MINUS_DST_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "CONSTANT_COLOR", GL_CONSTANT_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "CONSTANT_ONE_MINUS_COLOR", GL_ONE_MINUS_CONSTANT_COLOR);
        SET_PROTOTYPE_CONSTANT(t, "CONSTANT_ALPHA", GL_CONSTANT_ALPHA);
        SET_PROTOTYPE_CONSTANT(t, "ONE_MINUS_CONSTANT_ALPHA", GL_ONE_MINUS_CONSTANT_ALPHA);
        NODE_SET_PROTOTYPE_METHOD(t, "depthFunc", DepthFunc);
        NODE_SET_PROTOTYPE_METHOD(t, "sampleCoverage", SampleCoverage);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilFunc", StencilFunc);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilFuncSeparate", StencilFuncSeparate);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilOp", StencilOp);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilOpSeparate", StencilOpSeparate);
        SET_PROTOTYPE_CONSTANT(t, "NEVER", GL_NEVER);
        SET_PROTOTYPE_CONSTANT(t, "ALWAYS", GL_ALWAYS);
        SET_PROTOTYPE_CONSTANT(t, "LESS", GL_LESS);
        SET_PROTOTYPE_CONSTANT(t, "EQUAL", GL_EQUAL);
        SET_PROTOTYPE_CONSTANT(t, "LEQUAL", GL_LEQUAL);
        SET_PROTOTYPE_CONSTANT(t, "GREATER", GL_GREATER);
        SET_PROTOTYPE_CONSTANT(t, "GEQUAL", GL_GEQUAL);
        SET_PROTOTYPE_CONSTANT(t, "NOTEQUAL", GL_NOTEQUAL);
        SET_PROTOTYPE_CONSTANT(t, "FRONT", GL_FRONT);
        SET_PROTOTYPE_CONSTANT(t, "BACK", GL_BACK);
        SET_PROTOTYPE_CONSTANT(t, "FRONT_AND_BACK", GL_FRONT_AND_BACK);
        SET_PROTOTYPE_CONSTANT(t, "KEEP", GL_KEEP);
        SET_PROTOTYPE_CONSTANT(t, "ZERO", GL_ZERO);
        SET_PROTOTYPE_CONSTANT(t, "REPLACE", GL_REPLACE);
        SET_PROTOTYPE_CONSTANT(t, "INCR", GL_INCR);
        SET_PROTOTYPE_CONSTANT(t, "DECR", GL_DECR);
        SET_PROTOTYPE_CONSTANT(t, "INVERT", GL_INVERT);
        SET_PROTOTYPE_CONSTANT(t, "INCR_WRAP", GL_INCR_WRAP);
        SET_PROTOTYPE_CONSTANT(t, "DECR_WRAP", GL_DECR_WRAP);
        ////Whole Framebuffer Operations
        //
        NODE_SET_PROTOTYPE_METHOD(t, "clear", Clear);
        NODE_SET_PROTOTYPE_METHOD(t, "clearColor", ClearColor);
        NODE_SET_PROTOTYPE_METHOD(t, "ClearDepth", ClearDepth);
        NODE_SET_PROTOTYPE_METHOD(t, "ClearStencil", ClearStencil);
        NODE_SET_PROTOTYPE_METHOD(t, "ColorMask", ColorMask);
        NODE_SET_PROTOTYPE_METHOD(t, "DepthMask", DepthMask);
        NODE_SET_PROTOTYPE_METHOD(t, "StencilMask", StencilMask);
        NODE_SET_PROTOTYPE_METHOD(t, "StencilMaskSeparate", StencilMaskSeparate);
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
    //
    Handle<Value> Renderer::BlendColor(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        double r = args[0]->NumberValue();
        double g = args[1]->NumberValue();
        double b = args[2]->NumberValue();
        double a = args[3]->NumberValue();
        glBlendColor(r,g,b,a);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendEquation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendEquation(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendEquationSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendEquationSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendFunc(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendFunc(
            args[0]->IntegerValue(),
            args[1]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BlendFuncSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBlendFuncSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::DepthFunc(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDepthFunc(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::SampleCoverage(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glSampleCoverage(
            args[0]->NumberValue(),
            args[1]->BooleanValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilFunc(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilFunc(
            args[0]->IntegerValue(),
            args[1]->Int32Value(),
            args[2]->Uint32Value()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilFuncSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilFuncSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->Int32Value(),
            args[3]->Uint32Value()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilOp(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilOp(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::StencilOpSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilOpSeparate(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    ////Whole Framebuffer Operations
    //

    Handle<Value> Renderer::Clear(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        uint32_t flags = args[0]->IntegerValue();
        glClear(flags);
        return scope.Close(Undefined());
    }

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

    Handle<Value> Renderer::ClearDepth(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glClearDepthf(
            args[0]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::ClearStencil(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glClearStencil(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::ColorMask(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glColorMask(
            args[0]->BooleanValue(),
            args[1]->BooleanValue(),
            args[2]->BooleanValue(),
            args[3]->BooleanValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::DepthMask(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDepthMask(
            args[0]->BooleanValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::StencilMask(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilMask(
            args[0]->Uint32Value()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::StencilMaskSeparate(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glStencilMaskSeparate(
            args[0]->IntegerValue(),
            args[1]->Uint32Value()
        );
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
