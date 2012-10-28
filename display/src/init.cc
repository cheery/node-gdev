#include <v8.h>
#include <node.h>
#include <node_buffer.h>

using namespace v8;

#include "display.cc"

Handle<Value> OpenDisplay(const Arguments& args) {
    HandleScope scope;
    return scope.Close(Display::NewInstance(args));
}

static void Init(Handle<Object> target) {
    Display::Init(target);
    target->Set(String::NewSymbol("open"),
        FunctionTemplate::New(OpenDisplay)->GetFunction());
}
NODE_MODULE(display, Init)
