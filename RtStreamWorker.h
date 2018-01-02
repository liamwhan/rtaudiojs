#ifndef RTSTREAMWORKER_H
#define RTSTREAMWORKER_H

#include <nan.h>
#include "RtQueue.h"

namespace RtAudioJs
{

typedef unsigned int uint;
typedef unsigned long ulng;
using namespace Nan;

class Factorizer : public AsyncProgressWorker
{

  public:
    Factorizer(Callback *progress, Callback *callback, uint n) : AsyncProgressWorker(callback), progress(progress), n(n) {}
    ~Factorizer() {}

    // Executes in the new worker thread (background)
    void Execute(
        const AsyncProgressWorker::ExecutionProgress &progress)
    {
        uint32_t factor = 2;
        while (n % 2 == 0)
        {
            writeToNode(progress, factor);
            n = n / 2;
        }
        for (uint32_t i = 3; i <= n; i = i + 2)
        {
            while (n % i == 0)
            {
                writeToNode(progress, i);
                n = n / i;
            }
        }
    }

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

  protected:
    Callback *progress;
    uint n;
    void writeToNode(
        const AsyncProgressWorker::ExecutionProgress &progress, uint32_t factor)
    {
        toNode.write(factor);
        progress.Send(
            reinterpret_cast<const char *>(&toNode), sizeof(toNode));
    }

  private:
    RtQueue<uint> toNode;
    void drainQueue()
    {
        HandleScope scope;
        // drain the queue - since we might only get
        // called once for many writes
        std::deque<uint> contents;
        toNode.readAll(contents);
        for (uint32_t &item : contents)
        {
            v8::Local<v8::Value> argv[] = {
                New<v8::Integer>(*reinterpret_cast<int *>(&item))};
            progress->Call(1, argv);
        }
    }
};
}

#endif