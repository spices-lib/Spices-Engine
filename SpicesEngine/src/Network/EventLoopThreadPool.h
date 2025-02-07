/**
* @file EventLoopThreadPool.h.
* @brief The EventLoopThreadPool Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "Core/Thread/ThreadPoolBasic.h"
#include "EventLoop.h"

namespace Spices {

namespace Net {

    class EventLoopThreadPool : public ThreadPool
    {
    public:

        using ThreadInitCallback = std::function<void(EventLoop*)>;

    public:

        /**
        * @brief Constructor Function.
        * Create Specific ThreadPool.
        * @param[in] name ThreadPool Name.
        */
        EventLoopThreadPool(const std::string& name)
            : ThreadPool_Basic(name)
            , m_Next(0)
            , m_ThreadInitCallback(nullptr)
        {}

        /**
        * @brief Destructor Function.
        */
        virtual ~EventLoopThreadPool() override = default;

        EventLoop* GetNextLoop();
        std::vector<EventLoop*> GetAllLoops();

        const std::string& Name() const { return m_PoolName; }
    
        /**
        * @brief Thread Function.
        * @param[in] thread Thread Entity.
        */
        void ThreadFunc(Thread<>* thread);

        /**
        * @brief Start Run this thread pool.
        * @param[in] initThreadSize Thread Size.
        * @param[in] cb ThreadInitCallback.
        */
        void Start(int initThreadSize = 0.5 * std::thread::hardware_concurrency(), ThreadInitCallback cb = nullptr);

    private:

        int m_Next;
        std::vector<EventLoop*> m_Loops;
        ThreadInitCallback m_ThreadInitCallback;

    };

}

}