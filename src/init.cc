#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace v8;

extern "C" {
#include "video.h"
}

#include "videocontext.h"
#include "webgl/renderer.h"

class LinuxVideoContext : public IVideoContext {
    private:
        SURFACE_T surface;
    public:
        LinuxVideoContext(char* nativewindow, size_t length) {
            assert (length == sizeof(uint32_t)*NATIVEWINDOW_SIZE);
            surface = videoCreateNative((uint32_t*)nativewindow);
        };
        ~LinuxVideoContext() {
            videoDestroySurface(surface);
        };

        bool IsCurrent() {
            return videoIsCurrent(surface);
        };

        void MakeCurrent() {
            videoMakeCurrent(surface);
        };

        void SwapBuffers() {
            videoSwapBuffers(surface);
        };

        void Close() {
            delete this;
        };
};

Handle<Value> GetVideoContext(const Arguments& args) {
    HandleScope scope;
    Local<Object> buffer_obj = args[0]->ToObject();
    char *buffer_data = node::Buffer::Data(buffer_obj);
    size_t buffer_length = node::Buffer::Length(buffer_obj);
    return scope.Close(webgl::Renderer::Synthesize(
        (IVideoContext*)(new LinuxVideoContext(buffer_data, buffer_length))
    ));
}

static void Init(Handle<Object> target) {
    webgl::Renderer::Init(target);
    target->Set(String::NewSymbol("getContext"),
        FunctionTemplate::New(GetVideoContext)->GetFunction());
}
NODE_MODULE(video, Init)
