#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace v8;

extern "C" {
#include "video.h"
//#include <assert.h>
}

#include "context.cc"

Handle<Value> GetVideoContext(const Arguments& args) {
    HandleScope scope;
    return scope.Close(VideoContext::NewInstance(args));
}

#include "GLES2/gl2.h"

Handle<Value> Demonstrate(const Arguments& args) {
    HandleScope scope;
    double r = args[0]->NumberValue();
    double g = args[1]->NumberValue();
    double b = args[2]->NumberValue();
    double a = args[3]->NumberValue();
    glClearColor(r,g,b,a);
    glClear(GL_COLOR_BUFFER_BIT);
    return Undefined();
}

static void Init(Handle<Object> target) {
    printf("Init()\n");
    VideoContext::Init(target);
    printf("Init().36\n");
    printf("Init().38\n");
    target->Set(String::NewSymbol("getContext"),
        FunctionTemplate::New(GetVideoContext)->GetFunction());
    printf("Init().39\n");
    target->Set(String::NewSymbol("demonstrate"),
        FunctionTemplate::New(Demonstrate)->GetFunction());
    printf("Init().42\n");
}
NODE_MODULE(video, Init)
