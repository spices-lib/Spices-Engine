/**
* @file EPollPoller_test.h.
* @brief The EPollPoller_test Definitions.
* @author Spices.
*/

#pragma once
#include <gmock/gmock.h>
#include <Network/Poller/EPollPoller.h>
#include <Network/EventLoop.h>
#include <Network/Channel.h>
#include "Instrumentor.h"

namespace SpicesTest {

    int gEventFd = -1;
    int gEventFd2 = -1;
    bool fRunning = true;

    void ThreadFunc()
    {
        HANDLE epollFd = epoll_create1(0);

        epoll_event evEvent;
        evEvent.events = EPOLLIN;
        evEvent.data.fd = gEventFd;
        //evEvent.data.hnd = static_cast<void*>(gEventFd);
        evEvent.data.sock = gEventFd;
        evEvent.data.u32 = gEventFd;
        evEvent.data.u64 = gEventFd;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, gEventFd, &evEvent);

        epoll_event evEvent2;
        evEvent2.events = EPOLLIN;
        evEvent2.data.fd = gEventFd2;

        evEvent.data.sock = gEventFd;
        evEvent.data.u32 = gEventFd;
        evEvent.data.u64 = gEventFd;
        epoll_ctl(epollFd, EPOLL_CTL_ADD, gEventFd2, &evEvent2);

        epoll_event events[2];

        char exp = 0;
        int result = 0;
        while (fRunning)
        {
            int nfd = epoll_wait(epollFd, events, 2, -1);
            if (nfd > 0)
            {
                for (int i = 0; i < nfd; ++i)
                {
                    exp = 0;
                    result = 0;

                    if (events[i].data.fd == gEventFd)
                    {
                        result = recv(gEventFd, &exp, sizeof(char), 0);
                        std::cout << "gEventFd recv : " << result;
                    }
                    else if (events[i].data.fd == gEventFd2)
                    {
                        result = recv(gEventFd2, &exp, sizeof(char), 0);
                        std::cout << "gEventFd2 recv : " << result;
                    }
                }
            }
        }
    };

    void SendEventFd()
    {
        std::this_thread::sleep_for(std::chrono::seconds(1));
        
        char u = 100;
        send(gEventFd, &u, sizeof(char), 0);
        std::cout << WSAGetLastError() << std::endl;

        send(gEventFd2, &u, sizeof(char), 0);
    }

    /**
    * @brief Testing Spices::Net::EPollPoller.
    */
    TEST(EPollPoller_test, EPollPoller) {

        SPICESTEST_PROFILE_FUNCTION();

        Spices::Net::Socket socket;
        socket.Create();
        gEventFd = socket.Fd();

        Spices::Net::Socket socket2;
        socket2.Create();
        gEventFd2 = socket2.Fd();

        std::thread t1(ThreadFunc);
        std::thread t2(SendEventFd);

        t1.join();
        t2.join();
    }

}