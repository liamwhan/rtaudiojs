

#ifndef RTSTREAMPARAMS_H
#define RTSTREAMPARAMS_H
#include <nan.h>

typedef unsigned int uint;

namespace RtAudioJs
{

using namespace Nan;

static v8::Persistent<v8::FunctionTemplate> constructor;
class RtStreamParams : public Nan::ObjectWrap
{
public:
  static NAN_MODULE_INIT(Init);

private:
  explicit RtStreamParams(uint deviceId = 0, uint nChannels = 2, uint firstChannel = 0, uint sampleRate = 48000)
      : deviceId_(deviceId), nChannels_(nChannels), firstChannel_(firstChannel), sampleRate_(sampleRate){};

  ~RtStreamParams() {}

  static NAN_METHOD(New);
  static NAN_GETTER(GetProperty);
  static NAN_SETTER(SetProperty);

  uint deviceId_;
  uint nChannels_;
  uint firstChannel_;
  uint sampleRate_;
};
}
#endif