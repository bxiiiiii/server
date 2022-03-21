#include "Thread.h"

namespace server
{
    namespace CurrentThread
    {
        thread_local int t_cachedTid = 0;
        thread_local std::string t_tidString;       
    }

    namespace detail
    {
    
}

        Thread::Thread(ThreadFunc func) 
            : started_(false),
                tid_(0),
                func_(std::move(func))
        {
            
        }

        Thread::~Thread()
        {
            
        }

        void Thread::start()
        {
            started_ = true;
            thread_
        }
    }