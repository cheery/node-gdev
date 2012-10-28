namespace webgl {
    using namespace v8;

    class GLObject : public node::ObjectWrap {
        public:
            static void Init(Handle<Object> target);
            static Handle<Value> New(GLuint id);
            static GLuint GetId(Handle<Object> object);
        private:
            GLuint id;
            static Persistent<Function> constructor;
            static Handle<Value> New(const Arguments& args);
    };
}
