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

extern "C" {
#include "video.h"
}

#include "GLES2/gl2.h"
#include "videocontext.h"
#include "webgl/object.h"
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
    webgl::GLObject::Init(target);
    webgl::Renderer::Init(target);
    target->Set(String::NewSymbol("getContext"),
        FunctionTemplate::New(GetVideoContext)->GetFunction());
}
NODE_MODULE(video, Init)
