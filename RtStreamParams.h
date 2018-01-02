#include <node.h>
#include <node_object_wrap.h>
#include <v8.h>

typedef unsigned int uint;

namespace RtAudioJs
{

using namespace v8;
using namespace std;

class RtStreamParams : public node::ObjectWrap
{
public:
  static void Init(Local<Object> exports);

private:
  explicit RtStreamParams(uint deviceId = 0, uint nChannels = 2, uint firstChannel = 0, uint sampleRate = 48000)
      : deviceId_(deviceId), nChannels_(nChannels), firstChannel_(firstChannel), sampleRate_(sampleRate){};
  ~RtStreamParams() {}

  static void New(const FunctionCallbackInfo<Value> &args);
  static void GetProperty(Local<String> property, const PropertyCallbackInfo<Value> &info);
  static void SetProperty(Local<String> property, Local<Value> value, const PropertyCallbackInfo<void> &info);
  static bool IsUndefinedOrNotNumber(Local<Value> val); 

  static Persistent<Function> constructor;
  uint deviceId_;
  uint nChannels_;
  uint firstChannel_;
  uint sampleRate_;
};
}