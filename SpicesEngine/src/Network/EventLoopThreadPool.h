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

    /**
    * @brief ThreadPool of Multiple threading EventLoop.
    */
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

        /**
        * @brief Get next thread EventLoop.
        * @return Returns Next Thread EventLoop.
        */
        EventLoop* GetNextLoop();

        /**
        * @brief Get all thread EventLoop.
        * @return Returns all thread EventLoop.
        */
        std::vector<EventLoop*> GetAllLoops();

        /**
        * @brief Get ThreadPool Name.
        * @return Returns ThreadPool Name.
        */
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

        /**
        * @brief Next EventLoop index.
        */
        int m_Next;

        /**
        * @brief All threads EventLoop collection.
        */
        std::vector<EventLoop*> m_Loops;

        /**
        * @brief ThreadInitCallback.
        */
        ThreadInitCallback m_ThreadInitCallback;

    };

}

}