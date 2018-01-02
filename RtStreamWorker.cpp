#include "RtStreamWorker.h"

namespace RtAudioJs
{

NAN_METHOD(Factor)
{
    Callback *progress = new Callback(info[1].As<v8::Function>());
    Callback *callback = new Callback(info[2].As<v8::Function>());
    AsyncQueueWorker(new Factorizer(callback, progress, To<uint32_t>(info[0]).FromJust()));
}
}