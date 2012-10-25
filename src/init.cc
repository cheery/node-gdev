#include <v8.h>
#include <node.h>

#include "bcm_host.h"

#include "display.h"

using namespace v8;

extern "C" {
//    some setting seems to break this feature.
//    Handle<Value> CreateDisplay(const Arguments& args) {
//        HandleScope scope;
//        return scope.Close(Display::NewInstance(args));
//    }

    static void InitAlt(Handle<Object> target) {
        Display::Init(target);
//        target->Set(String::New("createDisplay"),
//            FunctionTemplate::New(CreateDisplay)->GetFunction());
    }
    NODE_MODULE(video, InitAlt)
}
