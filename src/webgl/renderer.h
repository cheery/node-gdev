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
            ////Whole Framebuffer Operations
            //
            static Handle<Value> ClearColor(const Arguments& args);
            static Handle<Value> Clear(const Arguments& args);
            ////Buffer Objects
            //
            static Handle<Value> CreateBuffer(const Arguments& args);
            static Handle<Value> DeleteBuffer(const Arguments& args);
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
            static Handle<Value> CreateTexture(const Arguments& args);
            static Handle<Value> DeleteTexture(const Arguments& args);
            ////Special Functions
            //
            ////Renderbuffer Objects
            //
            static Handle<Value> CreateRenderbuffer(const Arguments& args);
            static Handle<Value> DeleteRenderbuffer(const Arguments& args);
            ////Writing to the Draw Buffer
            //
            static Handle<Value> DrawArrays(const Arguments& args);
            static Handle<Value> DrawElements(const Arguments& args);
            ////Read Back Pixels
            //
            ////Framebuffer Objects
            //
    };
}
