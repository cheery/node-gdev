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
        NODE_SET_PROTOTYPE_METHOD(t, "clearDepth", ClearDepth);
        NODE_SET_PROTOTYPE_METHOD(t, "clearStencil", ClearStencil);
        NODE_SET_PROTOTYPE_METHOD(t, "colorMask", ColorMask);
        NODE_SET_PROTOTYPE_METHOD(t, "depthMask", DepthMask);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilMask", StencilMask);
        NODE_SET_PROTOTYPE_METHOD(t, "stencilMaskSeparate", StencilMaskSeparate);
        SET_PROTOTYPE_CONSTANT(t, "COLOR_BUFFER_BIT", GL_COLOR_BUFFER_BIT);
        SET_PROTOTYPE_CONSTANT(t, "DEPTH_BUFFER_BIT", GL_DEPTH_BUFFER_BIT);
        SET_PROTOTYPE_CONSTANT(t, "STENCIL_BUFFER_BIT", GL_STENCIL_BUFFER_BIT);

        ////Buffer Objects
        //
        NODE_SET_PROTOTYPE_METHOD(t, "bindBuffer", BindBuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "bufferData", BufferData);
        NODE_SET_PROTOTYPE_METHOD(t, "bufferSubData", BufferSubData);
        NODE_SET_PROTOTYPE_METHOD(t, "createBuffer", CreateBuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteBuffer", DeleteBuffer);
        NODE_SET_PROTOTYPE_METHOD(t, "getBufferParameter", GetBufferParameter);
        NODE_SET_PROTOTYPE_METHOD(t, "isBuffer", IsBuffer);
        SET_PROTOTYPE_CONSTANT(t, "ARRAY_BUFFER", GL_ARRAY_BUFFER);
        SET_PROTOTYPE_CONSTANT(t, "ELEMENT_ARRAY_BUFFER", GL_ELEMENT_ARRAY_BUFFER);
        SET_PROTOTYPE_CONSTANT(t, "STATIC_DRAW", GL_STATIC_DRAW);
        SET_PROTOTYPE_CONSTANT(t, "STREAM_DRAW", GL_STREAM_DRAW);
        SET_PROTOTYPE_CONSTANT(t, "DYNAMIC_DRAW", GL_DYNAMIC_DRAW);
        SET_PROTOTYPE_CONSTANT(t, "BUFFER_SIZE", GL_BUFFER_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "BUFFER_USAGE", GL_BUFFER_USAGE);
        ////View and Clip
        //
        NODE_SET_PROTOTYPE_METHOD(t, "depthRange", DepthRange);
        NODE_SET_PROTOTYPE_METHOD(t, "scissor", Scissor);
        NODE_SET_PROTOTYPE_METHOD(t, "viewport", Viewport);
        ////Rasterization                           
        //                                          
        NODE_SET_PROTOTYPE_METHOD(t, "cullFace", CullFace);
        NODE_SET_PROTOTYPE_METHOD(t, "frontFace", FrontFace);
        NODE_SET_PROTOTYPE_METHOD(t, "lineWidth", LineWidth);
        NODE_SET_PROTOTYPE_METHOD(t, "polygonOffset", PolygonOffset);
        SET_PROTOTYPE_CONSTANT(t, "CCW", GL_CCW);
        SET_PROTOTYPE_CONSTANT(t, "CW", GL_CW);
        ////Detect and Enable Extensions
        //
        ////Detect context lost events
        //
        ////Programs and Shaders
        //
        NODE_SET_PROTOTYPE_METHOD(t, "attachShader", AttachShader);
        NODE_SET_PROTOTYPE_METHOD(t, "bindAttribLocation", BindAttribLocation);
        NODE_SET_PROTOTYPE_METHOD(t, "compileShader", CompileShader);
        NODE_SET_PROTOTYPE_METHOD(t, "createProgram", CreateProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "createShader", CreateShader);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_SHADER", GL_VERTEX_SHADER);
        SET_PROTOTYPE_CONSTANT(t, "FRAGMENT_SHADER", GL_FRAGMENT_SHADER);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteProgram", DeleteProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "deleteShader", DeleteShader);
        NODE_SET_PROTOTYPE_METHOD(t, "detachShader", DetachShader);
        NODE_SET_PROTOTYPE_METHOD(t, "getAttachedShaders", GetAttachedShaders);
        NODE_SET_PROTOTYPE_METHOD(t, "getProgramParameter", GetProgramParameter);
        SET_PROTOTYPE_CONSTANT(t, "DELETE_STATUS", GL_DELETE_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "LINK_STATUS", GL_LINK_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "VALIDATE_STATUS", GL_VALIDATE_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "ATTACHED_SHADERS", GL_ATTACHED_SHADERS);
        SET_PROTOTYPE_CONSTANT(t, "ACTIVE_ATTRIBUTES", GL_ACTIVE_ATTRIBUTES);
        SET_PROTOTYPE_CONSTANT(t, "ACTIVE_UNIFORMS", GL_ACTIVE_UNIFORMS);
        NODE_SET_PROTOTYPE_METHOD(t, "getProgramInfoLog", GetProgramInfoLog);
        NODE_SET_PROTOTYPE_METHOD(t, "getShaderParameter", GetShaderParameter);
        SET_PROTOTYPE_CONSTANT(t, "SHADER_TYPE", GL_SHADER_TYPE);
        SET_PROTOTYPE_CONSTANT(t, "DELETE_STATUS", GL_DELETE_STATUS);
        SET_PROTOTYPE_CONSTANT(t, "COMPILE_STATUS", GL_COMPILE_STATUS);
        NODE_SET_PROTOTYPE_METHOD(t, "getShaderInfoLog", GetShaderInfoLog);
        NODE_SET_PROTOTYPE_METHOD(t, "getShaderSource", GetShaderSource);
        NODE_SET_PROTOTYPE_METHOD(t, "isProgram", IsProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "isShader", IsShader);
        NODE_SET_PROTOTYPE_METHOD(t, "linkProgram", LinkProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "shaderSource", ShaderSource);
        NODE_SET_PROTOTYPE_METHOD(t, "useProgram", UseProgram);
        NODE_SET_PROTOTYPE_METHOD(t, "validateProgram", ValidateProgram);
        ////Uniforms and Attributes
        //
        NODE_SET_PROTOTYPE_METHOD(t, "disableVertexAttribArray", DisableVertexAttribArray);
        NODE_SET_PROTOTYPE_METHOD(t, "enableVertexAttribArray", EnableVertexAttribArray);
        NODE_SET_PROTOTYPE_METHOD(t, "getActiveAttrib", GetActiveAttrib);
        NODE_SET_PROTOTYPE_METHOD(t, "getActiveUniform", GetActiveUniform);
        NODE_SET_PROTOTYPE_METHOD(t, "getAttribLocation", GetAttribLocation);
        NODE_SET_PROTOTYPE_METHOD(t, "getUniform", GetUniform);
        NODE_SET_PROTOTYPE_METHOD(t, "getUniformLocation", GetUniformLocation);
        NODE_SET_PROTOTYPE_METHOD(t, "getVertexAttrib", GetVertexAttrib);
        NODE_SET_PROTOTYPE_METHOD(t, "getVertexAttribOffset", GetVertexAttribOffset);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform1f", Uniform1f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform2f", Uniform2f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform3f", Uniform3f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform4f", Uniform4f);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform1i", Uniform1i);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform2i", Uniform2i);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform3i", Uniform3i);
        NODE_SET_PROTOTYPE_METHOD(t, "uniform4i", Uniform4i);
        NODE_SET_PROTOTYPE_METHOD(t, "vertexAttribPointer", VertexAttribPointer);
        SET_PROTOTYPE_CONSTANT(t, "MAX_VERTEX_ATTRIBS", GL_MAX_VERTEX_ATTRIBS);
        SET_PROTOTYPE_CONSTANT(t, "CURRENT_VERTEX_ATTRIB", GL_CURRENT_VERTEX_ATTRIB);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_BUFFER_BINDING", GL_VERTEX_ATTRIB_ARRAY_BUFFER_BINDING);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_ENABLED", GL_VERTEX_ATTRIB_ARRAY_ENABLED);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_SIZE", GL_VERTEX_ATTRIB_ARRAY_SIZE);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_STRIDE", GL_VERTEX_ATTRIB_ARRAY_STRIDE);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_TYPE", GL_VERTEX_ATTRIB_ARRAY_TYPE);
        SET_PROTOTYPE_CONSTANT(t, "VERTEX_ATTRIB_ARRAY_NORMALIZED", GL_VERTEX_ATTRIB_ARRAY_NORMALIZED);
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
    Handle<Value> Renderer::BindBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBindBuffer(
            args[0]->IntegerValue(),
            GLObject::GetId(args[1]->ToObject())
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::BufferData(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLsizeiptr size;
        GLvoid* data = NULL;
        if (args[1]->IsObject()) {
            Local<Object> buffer_obj = args[1]->ToObject();
            data = node::Buffer::Data(buffer_obj);
            size = node::Buffer::Length(buffer_obj);
        } else {
            size = args[1]->IntegerValue();
        }
        glBufferData(
            args[0]->IntegerValue(),
            size,
            data,
            args[2]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::BufferSubData(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        Local<Object> buffer_obj = args[2]->ToObject();
        GLvoid* data = node::Buffer::Data(buffer_obj);
        GLsizeiptr size = node::Buffer::Length(buffer_obj);
        glBufferSubData(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            size,
            data
        );
        return scope.Close(Undefined());
    }

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

    Handle<Value> Renderer::GetBufferParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLenum target = args[0]->IntegerValue();
        GLenum pname = args[1]->IntegerValue();
        switch (pname) {
            case GL_BUFFER_SIZE:
            case GL_BUFFER_USAGE:
            {
                GLint data;
                glGetBufferParameteriv(target, pname, &data);
                return scope.Close(Integer::New(data));
            }
        }
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::IsBuffer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(glIsBuffer(id)));
    }
    ////View and Clip
    //
    Handle<Value> Renderer::DepthRange(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDepthRangef(
            args[0]->NumberValue(),
            args[1]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Scissor(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glScissor(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::Viewport(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glViewport(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    ////Rasterization
    //
    Handle<Value> Renderer::CullFace(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glCullFace(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::FrontFace(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glFrontFace(
            args[0]->IntegerValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::LineWidth(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glLineWidth(
            args[0]->NumberValue()
        );
        return scope.Close(Undefined());
    }

    Handle<Value> Renderer::PolygonOffset(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glPolygonOffset(
            args[0]->NumberValue(),
            args[1]->NumberValue()
        );
        return scope.Close(Undefined());
    }
    ////Detect and Enable Extensions
    //
    ////Detect context lost events
    //
    ////Programs and Shaders
    //
    Handle<Value> Renderer::AttachShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLuint shader_id = GLObject::GetId(args[1]->ToObject());
        glAttachShader(id, shader_id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::BindAttribLocation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glBindAttribLocation(
            GLObject::GetId(args[0]->ToObject()),
            args[1]->IntegerValue(),
            *(String::AsciiValue)(args[2]->ToString())
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::CompileShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glCompileShader(GLObject::GetId(args[0]->ToObject()));
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::CreateProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        return scope.Close(GLObject::New(glCreateProgram()));
    }
    Handle<Value> Renderer::CreateShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        return scope.Close(GLObject::New(glCreateShader(
            args[0]->IntegerValue()
        )));
    }
    Handle<Value> Renderer::DeleteProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject()),
        glDeleteProgram(id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::DeleteShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject()),
        glDeleteShader(id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::DetachShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLuint shader_id = GLObject::GetId(args[1]->ToObject());
        glDetachShader(id, shader_id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetAttachedShaders(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert (false);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetProgramParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert (false);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetProgramInfoLog(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLsizei sz;
        glGetProgramInfoLog(id, 0, &sz, NULL);
        char log[sz];
        glGetProgramInfoLog(id, sz, NULL, log);
        return scope.Close(String::New(log));
    }
    Handle<Value> Renderer::GetShaderParameter(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert (false);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetShaderInfoLog(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLsizei sz;
        glGetShaderInfoLog(id, 0, &sz, NULL);
        char log[sz];
        glGetShaderInfoLog(id, sz, NULL, log);
        return scope.Close(String::New(log));
    }
    Handle<Value> Renderer::GetShaderSource(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        GLsizei sz;
        glGetShaderSource(id, 0, &sz, NULL);
        char log[sz];
        glGetShaderSource(id, sz, NULL, log);
        return scope.Close(String::New(log));
    }
    Handle<Value> Renderer::IsProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(
            glIsProgram(id)
        ));
    }
    Handle<Value> Renderer::IsShader(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        return scope.Close(Boolean::New(
            glIsShader(id)
        ));
    }
    Handle<Value> Renderer::LinkProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        glLinkProgram(id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::ShaderSource(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        String::AsciiValue ascii(args[1]->ToString());
        const GLint len = ascii.length();
        const GLchar* src = *ascii;
        glShaderSource(id, 1, &src, &len);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::UseProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject()),
        glUseProgram(id);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::ValidateProgram(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject()),
        glValidateProgram(id);
        return scope.Close(Undefined());
    }
    ////Uniforms and Attributes
    //
    Handle<Value> Renderer::DisableVertexAttribArray(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glDisableVertexAttribArray(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::EnableVertexAttribArray(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glEnableVertexAttribArray(args[0]->IntegerValue());
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetActiveAttrib(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false); // I'm not sure how to implement this yet.
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetActiveUniform(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false); // I'm not sure how to implement this yet.
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetAttribLocation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        String::AsciiValue ascii(args[1]->ToString());
        GLuint loc = glGetAttribLocation(id, *ascii);
        return scope.Close(Integer::New(loc));
    }
    Handle<Value> Renderer::GetUniform(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(false); // I'm not sure how to implement this yet.
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetUniformLocation(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        GLuint id = GLObject::GetId(args[0]->ToObject());
        String::AsciiValue ascii(args[1]->ToString());
        GLuint loc = glGetUniformLocation(id, *ascii);
        return scope.Close(Integer::New(loc));
    }
    Handle<Value> Renderer::GetVertexAttrib(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        assert(0);
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::GetVertexAttribOffset(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        void* offset;
        glGetVertexAttribPointerv(
            args[0]->Uint32Value(),
            args[1]->IntegerValue(),
            &offset
        );
        return scope.Close(Integer::New((long)offset));
    }
    Handle<Value> Renderer::Uniform1f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform1f(
            args[0]->IntegerValue(),
            args[1]->NumberValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform2f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform2f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform3f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform3f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue(),
            args[3]->NumberValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform4f(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform4f(
            args[0]->IntegerValue(),
            args[1]->NumberValue(),
            args[2]->NumberValue(),
            args[3]->NumberValue(),
            args[4]->NumberValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform1i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform1i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform2i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform2i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform3i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform3i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::Uniform4i(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glUniform4i(
            args[0]->IntegerValue(),
            args[1]->IntegerValue(),
            args[2]->IntegerValue(),
            args[3]->IntegerValue(),
            args[4]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
    Handle<Value> Renderer::VertexAttribPointer(const Arguments& args) {
        HandleScope scope;
        Renderer* obj = ObjectWrap::Unwrap<Renderer>(args.This());
        obj->MakeCurrentLazy();
        glVertexAttribPointer(
            args[0]->Uint32Value(),
            args[1]->Int32Value(),
            args[2]->IntegerValue(),
            args[3]->BooleanValue(),
            args[4]->IntegerValue(),
            (GLvoid*)args[5]->IntegerValue()
        );
        return scope.Close(Undefined());
    }
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
