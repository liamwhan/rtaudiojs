#include <vector>
#include <uv.h>
#include <string>
#include <nan.h>

namespace RtAudioJs {

using namespace v8;
using namespace std;
    // This structure binds the async handler and all the required datq including all the events and event handlers.
    struct AsyncWork {
        uv_async_t async;
        std::vector<string> msgArr;
        std::vector<Persistent<Function, CopyablePersistentTraits<Function>>> handlers;
        uv_rwlock_t lock;
    };

    static AsyncWork* work = new AsyncWork();


    // This function is called when recieving messages from C++ (e.g. RtAudio)
    void onMessage(const std::string& msg) {

        //Lock when adding new message to the buffer.
        uv_rwlock_wrlock(&work->lock);
        //Add msg to the buffer (here we would setup some kind of payload to Javascript, maybe a Buffer to write to and some metadata about size of buffer and type of audio?)
        work->msgArr.push_back(msg);
        //Unlock the buffer
        uv_rwlock_wrunlock(&work->lock);
        
        uv_async_send(&work->async);
    }

}