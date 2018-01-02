#ifndef RTQUEUE_H
#define RTQUEUE_H
#include <iostream>
#include <string>
#include <algorithm>
#include <iterator>
#include <thread>
#include <deque>
#include <mutex>
#include <chrono>
#include <condition_variable>
#include <nan.h>

namespace RtAudioJs {

using namespace Nan;
using namespace std;

/**
 * Thread safe prod/con queue to synchronize communication 
 * from RtAudio Streams to NodeJs Event Loop 
 * Based on Scott Frees PCQueue Implementation 
 * see https://github.com/freezer333/nodecpp-demo/blob/master/streaming/dist/streaming-worker.h
 */ 

template<typename Data>
class RtQueue
{
public:
    void write(Data data) {
        while(true) {
            std::unique_lock<std::mutex> locker(mu_); //Create a lock for the mutex (unique_lock allows unlocking)
            buffer_.push_back(data); // write new data onto the buffer
            locker.unlock(); //Unlock the mutex
            cond_.notify_all();
            return;
        }
    }

    //Read next data packet from the queue
    Data read() {
        while(true)
        {
            std::unique_lock<std::mutex> locker(mu_);
            // NOTE(liam): This abomination seems to be C++ lambdas 
            // Wait until the buffer has data and then lock the mutex
            cond_.wait(locker, [this](){ return buffer_.size() > 0; });
            //Get the next packet from the buffer
            Data back = buffer_.front();
            //Remove the packet from the queue.
            buffer_.pop_front();
            locker.unlock(); //unlock the mutex
            cond_.notify_all(); // Notify consumers
            return back;

        }
    }

    // Read all packets and flush queue
    void readAll(std::deque<Data> &target) {
        std::unique_lock<std::mutex> locker(mu_);
        //Flush the buffer: Copy all data from the queue into target
        std::copy(buffer_.begin(), buffer_.end(), std::back_inserter(target));
        buffer_.clear();// Drain the buffer
        locker.unlock();// Unlock the mutex
    }
    RtQueue() {}
private:
    std::mutex mu_;
    std::condition_variable cond_;
    std::deque<Data> buffer_;
};

}
#endif