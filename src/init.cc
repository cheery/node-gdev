/*
Copyright (c) 2012 Cheery <cheery@boxbase.org>

Permission is hereby granted, free of charge, to any person obtaining a copy of this software and associated documentation files (the "Software"), to deal in the Software without restriction, including without limitation the rights to use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of the Software, and to permit persons to whom the Software is furnished to do so, subject to the following conditions:

The above copyright notice and this permission notice shall be included in all copies or substantial portions of the Software.

THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
*/
#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace v8;

#include "GLES2/gl2.h"
#include "videocontext.h"
#include "webgl/object.h"
#include "webgl/renderer.h"

extern "C" {
#include "video.h"
}

class LinuxVideoContext : public IVideoContext {
    private:
        SURFACE_T surface;
    public:
        LinuxVideoContext(uint32_t width, uint32_t height, int* global_image) {
            surface = videoCreateComposite(width, height, global_image);
        };
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

        void GlobalImageTexture2D(GLenum target, void* global_image) {
            videoCompositeTexture2D(target, (int*)global_image);
        };
};

Handle<Value> GetVideoContext(const Arguments& args) {
    HandleScope scope;
    Local<Object> options = args[0]->ToObject();
    Local<Value> handle = options->Get(String::NewSymbol("handle"));
    if (handle->IsUndefined()) {
        uint32_t width = options->Get(String::NewSymbol("width"))->IntegerValue();
        uint32_t height = options->Get(String::NewSymbol("height"))->IntegerValue();
        int global_image[5];
        Handle<Value> renderer = webgl::Renderer::Synthesize(
            (IVideoContext*)(new LinuxVideoContext(width, height, global_image))
        );
        node::Buffer* buffer = node::Buffer::New((char*)global_image, sizeof(global_image));
        renderer->ToObject()->Set(String::NewSymbol("handle"), buffer->handle_);
        return scope.Close(renderer);

    } else {
        Local<Object> buffer_obj = handle->ToObject();
        char *buffer_data = node::Buffer::Data(buffer_obj);
        size_t buffer_length = node::Buffer::Length(buffer_obj);
        return scope.Close(webgl::Renderer::Synthesize(
            (IVideoContext*)(new LinuxVideoContext(buffer_data, buffer_length))
        ));
    }
}

static void Init(Handle<Object> target) {
    webgl::GLObject::Init(target);
    webgl::Renderer::Init(target);
    target->Set(String::NewSymbol("getContext"),
        FunctionTemplate::New(GetVideoContext)->GetFunction());
}
NODE_MODULE(video, Init)
