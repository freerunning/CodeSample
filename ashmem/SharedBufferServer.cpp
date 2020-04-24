//
// Created by illusion on 18-1-12.
//

#define LOG_TAG "SharedBufferServer"

#include <utils/Log.h>
#include <binder/MemoryBase.h>
#include <binder/MemoryHeapBase.h>
#include <binder/IServiceManager.h>
#include <binder/IPCThreadState.h>

#include "ISharedBuffer.h"

class SharedBufferService : public BnSharedBuffer
{
public:
    SharedBufferService()
    {
        sp<MemoryHeapBase> heap = new MemoryHeapBase(SHARED_BUFFER_SIZE, 0, "SharedBuffer");
        if(heap != NULL)
        {
            mMemory = new MemoryBase(heap, 0, SHARED_BUFFER_SIZE);

            int32_t* data = (int32_t*)mMemory->pointer();
            if(data != NULL)
            {
                *data = 0;
            }
        }
    }

    virtual ~SharedBufferService()
    {
        mMemory = NULL;
    }

public:
    static void instantiate()
    {
        defaultServiceManager()->addService(String16(SHARED_BUFFER_SERVICE), new SharedBufferService());
    }

    virtual sp<IMemory> getBuffer()
    {
        return mMemory;
    }

private:
    sp<IMemory> mMemory;
};

int main(int argc, char** argv)
{
    SharedBufferService::instantiate();

    ProcessState::self()->startThreadPool();
    IPCThreadState::self()->joinThreadPool();

    return 0;
}