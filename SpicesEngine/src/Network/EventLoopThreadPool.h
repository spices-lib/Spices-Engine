#pragma once
#include "Core/Thread/ThreadPoolBasic.h"
#include "EventLoop.h"

namespace Spices {

namespace Net {

    class EventLoopThreadPool : public ThreadPool
    {
    public:

        /**
        * @brief Constructor Function.
        * Create Specific ThreadPool.
        * @param[in] name ThreadPool Name.
        */
        EventLoopThreadPool(const std::string& name)
            : ThreadPool_Basic(name)
        {}

        /**
        * @brief Destructor Function.
        */
        virtual ~EventLoopThreadPool() override = default;

    
    private:

        
        
    };

}

}