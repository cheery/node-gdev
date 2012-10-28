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

            static Handle<Value> New(const Arguments& args);
            static Handle<Value> Close(const Arguments& args);
            static Handle<Value> SwapBuffers(const Arguments& args);
            static Handle<Value> ClearColor(const Arguments& args);
            static Handle<Value> Clear(const Arguments& args);

            static Persistent<Function> constructor;
    };
}
