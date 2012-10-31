namespace webgl {
    using namespace v8;

    class Renderer : public node::ObjectWrap {
        public:
            static void Init(Handle<Object> target);
            static Handle<Value> Synthesize(IVideoContext* context);

        private:
            IVideoContext* context;
            inline void MakeCurrentLazy() {
                if (!context->IsCurrent()) {
                    context->MakeCurrent();
                }
            }

            static Persistent<Function> constructor;
            static Handle<Value> New(const Arguments& args);
            static Handle<Value> Close(const Arguments& args);
            static Handle<Value> SwapBuffers(const Arguments& args);

            ////Per-Fragment Operations
            //
            static Handle<Value> BlendColor(const Arguments& args);
            static Handle<Value> BlendEquation(const Arguments& args);
            static Handle<Value> BlendEquationSeparate(const Arguments& args);
            static Handle<Value> BlendFunc(const Arguments& args);
            static Handle<Value> BlendFuncSeparate(const Arguments& args);
            static Handle<Value> DepthFunc(const Arguments& args);
            static Handle<Value> SampleCoverage(const Arguments& args);
            static Handle<Value> StencilFunc(const Arguments& args);
            static Handle<Value> StencilFuncSeparate(const Arguments& args);
            static Handle<Value> StencilOp(const Arguments& args);
            static Handle<Value> StencilOpSeparate(const Arguments& args);
            ////Whole Framebuffer Operations
            //
            static Handle<Value> Clear(const Arguments& args);
            static Handle<Value> ClearColor(const Arguments& args);
            static Handle<Value> ClearDepth(const Arguments& args);
            static Handle<Value> ClearStencil(const Arguments& args);
            static Handle<Value> ColorMask(const Arguments& args);
            static Handle<Value> DepthMask(const Arguments& args);
            static Handle<Value> StencilMask(const Arguments& args);
            static Handle<Value> StencilMaskSeparate(const Arguments& args);
            ////Buffer Objects
            //
            static Handle<Value> BindBuffer(const Arguments& args);
            static Handle<Value> BufferData(const Arguments& args); // 2
            static Handle<Value> BufferSubData(const Arguments& args);
            static Handle<Value> CreateBuffer(const Arguments& args);
            static Handle<Value> DeleteBuffer(const Arguments& args);
            static Handle<Value> GetBufferParameter(const Arguments& args);
            static Handle<Value> IsBuffer(const Arguments& args);
            ////View and Clip
            //
            static Handle<Value> DepthRange(const Arguments& args);
            static Handle<Value> Scissor(const Arguments& args);
            static Handle<Value> Viewport(const Arguments& args);
            ////Rasterization
            //
            static Handle<Value> CullFace(const Arguments& args);
            static Handle<Value> FrontFace(const Arguments& args);
            static Handle<Value> LineWidth(const Arguments& args);
            static Handle<Value> PolygonOffset(const Arguments& args);
            ////Detect and Enable Extensions
            //
            ////Detect context lost events
            //
            ////Programs and Shaders
            //
            static Handle<Value> AttachShader(const Arguments& args);
            static Handle<Value> BindAttribLocation(const Arguments& args);
            static Handle<Value> CompileShader(const Arguments& args);
            static Handle<Value> CreateProgram(const Arguments& args);
            static Handle<Value> CreateShader(const Arguments& args);
            static Handle<Value> DeleteProgram(const Arguments& args);
            static Handle<Value> DeleteShader(const Arguments& args);
            static Handle<Value> DetachShader(const Arguments& args);
            static Handle<Value> GetAttachedShaders(const Arguments& args);
            static Handle<Value> GetProgramParameter(const Arguments& args);
            static Handle<Value> GetProgramInfoLog(const Arguments& args);
            static Handle<Value> GetShaderParameter(const Arguments& args);
            static Handle<Value> GetShaderInfoLog(const Arguments& args);
            static Handle<Value> GetShaderSource(const Arguments& args);
            static Handle<Value> IsProgram(const Arguments& args);
            static Handle<Value> IsShader(const Arguments& args);
            static Handle<Value> LinkProgram(const Arguments& args);
            static Handle<Value> ShaderSource(const Arguments& args);
            static Handle<Value> UseProgram(const Arguments& args);
            static Handle<Value> ValidateProgram(const Arguments& args);
            ////Uniforms and Attributes
            //
            static Handle<Value> DisableVertexAttribArray(const Arguments& args);
            static Handle<Value> EnableVertexAttribArray(const Arguments& args);
            static Handle<Value> GetActiveAttrib(const Arguments& args);
            static Handle<Value> GetActiveUniform(const Arguments& args);
            static Handle<Value> GetAttribLocation(const Arguments& args);
            static Handle<Value> GetUniform(const Arguments& args);
            static Handle<Value> GetUniformLocation(const Arguments& args);
            static Handle<Value> GetVertexAttrib(const Arguments& args);
            static Handle<Value> GetVertexAttribOffset(const Arguments& args);
            static Handle<Value> Uniform1f(const Arguments& args);
            static Handle<Value> Uniform2f(const Arguments& args);
            static Handle<Value> Uniform3f(const Arguments& args);
            static Handle<Value> Uniform4f(const Arguments& args);
            static Handle<Value> Uniform1i(const Arguments& args);
            static Handle<Value> Uniform2i(const Arguments& args);
            static Handle<Value> Uniform3i(const Arguments& args);
            static Handle<Value> Uniform4i(const Arguments& args);

            static Handle<Value> Uniform1fv(const Arguments& args);
            static Handle<Value> Uniform2fv(const Arguments& args);
            static Handle<Value> Uniform3fv(const Arguments& args);
            static Handle<Value> Uniform4fv(const Arguments& args);
            static Handle<Value> Uniform1iv(const Arguments& args);
            static Handle<Value> Uniform2iv(const Arguments& args);
            static Handle<Value> Uniform3iv(const Arguments& args);
            static Handle<Value> Uniform4iv(const Arguments& args);

            static Handle<Value> UniformMatrix2fv(const Arguments& args);
            static Handle<Value> UniformMatrix3fv(const Arguments& args);
            static Handle<Value> UniformMatrix4fv(const Arguments& args);
            static Handle<Value> VertexAttrib1f(const Arguments& args);
            static Handle<Value> VertexAttrib2f(const Arguments& args);
            static Handle<Value> VertexAttrib3f(const Arguments& args);
            static Handle<Value> VertexAttrib4f(const Arguments& args);
            static Handle<Value> VertexAttrib1fv(const Arguments& args);
            static Handle<Value> VertexAttrib2fv(const Arguments& args);
            static Handle<Value> VertexAttrib3fv(const Arguments& args);
            static Handle<Value> VertexAttrib4fv(const Arguments& args);

            // this is missing some, because I'm not sure about buffers.
            static Handle<Value> VertexAttribPointer(const Arguments& args);
            ////Texture Objects
            //
            static Handle<Value> ActiveTexture(const Arguments& args);
            static Handle<Value> BindTexture(const Arguments& args);
            static Handle<Value> CopyTexImage2D(const Arguments& args);
            static Handle<Value> CopyTexSubImage2D(const Arguments& args);
            static Handle<Value> CreateTexture(const Arguments& args);
            static Handle<Value> DeleteTexture(const Arguments& args);
            static Handle<Value> GenerateMipmap(const Arguments& args);
            static Handle<Value> GetTexParameter(const Arguments& args);
            static Handle<Value> IsTexture(const Arguments& args);
            static Handle<Value> TexImage2D(const Arguments& args);
            static Handle<Value> TexParameterf(const Arguments& args);
            static Handle<Value> TexParameteri(const Arguments& args);
            static Handle<Value> TexSubImage2D(const Arguments& args);
            ////Special Functions
            //
            static Handle<Value> Disable(const Arguments& args);
            static Handle<Value> Enable(const Arguments& args);
            static Handle<Value> Finish(const Arguments& args);
            static Handle<Value> Flush(const Arguments& args);
            static Handle<Value> GetError(const Arguments& args);
            static Handle<Value> GetParameter(const Arguments& args);
            static Handle<Value> Hint(const Arguments& args);
            static Handle<Value> IsEnabled(const Arguments& args);
            static Handle<Value> PixelStorei(const Arguments& args);
            ////Renderbuffer Objects
            //
            static Handle<Value> BindRenderbuffer(const Arguments& args);
            static Handle<Value> CreateRenderbuffer(const Arguments& args);
            static Handle<Value> DeleteRenderbuffer(const Arguments& args);
            static Handle<Value> GetRenderbufferParameter(const Arguments& args);
            static Handle<Value> IsRenderbuffer(const Arguments& args);
            static Handle<Value> RenderbufferStorage(const Arguments& args);
            ////Writing to the Draw Buffer
            //
            static Handle<Value> DrawArrays(const Arguments& args);
            static Handle<Value> DrawElements(const Arguments& args);
            ////Read Back Pixels
            //
            static Handle<Value> ReadPixels(const Arguments& args);
            ////Framebuffer Objects
            //
            static Handle<Value> BindFramebuffer(const Arguments& args);
            static Handle<Value> CheckFramebufferStatus(const Arguments& args);
            static Handle<Value> CreateFramebuffer(const Arguments& args);
            static Handle<Value> DeleteFramebuffer(const Arguments& args);
            static Handle<Value> FramebufferRenderbuffer(const Arguments& args);
            static Handle<Value> IsFramebuffer(const Arguments& args);
            static Handle<Value> FramebufferTexture2D(const Arguments& args);
            static Handle<Value> GetFramebufferAttachmentParameter(const Arguments& args);

            static inline void CatchError();

            static size_t Sizeof(Local<Object> object);
    };
}
