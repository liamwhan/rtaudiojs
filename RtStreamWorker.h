#ifndef RTSTREAMWORKER_H
#define RTSTREAMWORKER_H

#include <nan.h>
#include "RtQueue.h"

namespace RtAudioJs
{

typedef unsigned int uint;
typedef unsigned long ulng;
using namespace Nan;

class Message
{
  public:
    string name;
    string data;
    Message(string name, string data) : name(name), data(data) {}
};

class RtStreamWorker : public AsyncProgressWorker
{

  public:
    RtStreamWorker(Callback *progress, Callback *callback, Callback *error_callback) : AsyncProgressWorker(callback), progress(progress), error_callback(error_callback) {}
    ~RtStreamWorker() {}

    void HandleOKCallback()
    {
        drainQueue();
        callback->Call(0, NULL);
    }

    //Executes in event-loop thread
    void HandleProgressCallback(const char *data, size_t size)
    {
        drainQueue();
    }

    void HandleErrorCallback()
    {
        HandleScope scope;
        v8::Local<v8::Value> argv[] = {
            v8::Exception::Error(New<v8::String>(ErrorMessage()).ToLocalChecked())};
        error_callback->Call(1, argv);
    }

  protected:
    uint n;
    Callback *progress;
    Callback *error_callback;
    RtQueue<Message> toNode;

   

    void writeToNode(
        const AsyncProgressWorker::ExecutionProgress &progress, Message &msg)
    {
        toNode.write(msg);
        progress.Send(
            reinterpret_cast<const char *>(&toNode), sizeof(toNode));
    }
    void drainQueue()
    {
        HandleScope scope;
        // drain the queue - since we might only get
        // called once for many writes
        std::deque<Message> contents;
        toNode.readAll(contents);
        for (Message &msg : contents)
        {
            v8::Local<v8::Value> argv[] = {
                New<v8::String>(msg.name.c_str()).ToLocalChecked(),
                New<v8::String>(msg.data.c_str()).ToLocalChecked()
            };
            progress->Call(2, argv);
        }
    }
};

class Factorizer : public RtStreamWorker
{
  public:
    Factorizer(Callback *data, Callback *complete, Callback *error_callback, v8::Local<v8::Object> &options) 
    : RtStreamWorker(data, complete, error_callback)
    {
        N = -1;
        if (options->IsObject())
        {
            v8::Local<v8::Value> n_ =
                options->Get(New<v8::String>("n").ToLocalChecked());
            if (n_->IsNumber())
            {
                N = n_->NumberValue();
            }
        }
        if (N < 0)
        {
            SetErrorMessage("Cannot compute prime factorization " + "of negative numbers (overflowed long long?)!");
        }
    }
    ~Factorizer() {}

// Executes in the new worker thread (background)
    void Execute(
        const AsyncProgressWorker::ExecutionProgress &progress)
    {
        long long n = N;
        while (n % 2 == 0)
        {
            send_factor(progress, 2);
            n = n / 2;
        }
        for (uint32_t i = 3; i <= n; i = i + 2)
        {
            while (n % i == 0)
            {
                send_factor(progress, i);
                n = n / i;
            }
        }
    }

     void send_factor(const AsyncProgressWorker::ExecutionProgress &progress, long long factor)
    {
        Message tosend("factor", std::to_string(factor));
        writeToNode(progress, tosend);
    }
    private:
    long long N;
};
}

#endif