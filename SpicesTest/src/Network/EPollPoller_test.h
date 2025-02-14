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
#include <WinSock2.h>

namespace SpicesTest {

    /**
    * @brief Testing Spices::Net::EPOLLPOLL.
    */
    TEST(EPollPoller_test, EPOLLPOLL) {

        SPICESTEST_PROFILE_FUNCTION();
        
        using namespace Spices::Net;

        int r;

        InetAddress addr(8000);

        Socket listen_sock;
        listen_sock.Create();
        listen_sock.BindAddress(addr);
        listen_sock.Listen();

        Socket client_sock;
        client_sock.Create();

        EPollPoller epoller(EventLoopThreadWrapper::GetInst());
        HANDLE ephnd = epoller.GetHandle();

        // add listen_sock read event to epoll
        {
            epoll_event ev;
            ev.events = EPOLLIN | EPOLLPRI;
            ev.data.sock = listen_sock.Fd();

            r = epoll_ctl(ephnd, EPOLL_CTL_ADD, listen_sock.Fd(), &ev);
        }

        // add listen_sock write event to epoll
        {
            epoll_event ev;
            ev.events = EPOLLOUT;
            ev.data.sock = client_sock.Fd();

            r = epoll_ctl(ephnd, EPOLL_CTL_ADD, client_sock.Fd(), &ev);
        }

        client_sock.Connect(&addr);

        // shoule be 2 event.
        epoll_event evs[8];
        r = epoll_wait(ephnd, evs, 8, -1);

        InetAddress peerAddress;
        Socket server_sock(listen_sock.Accept(&peerAddress));

        for (int round = 0; round < 5; round++)
        {
            static char buf[1 << 20];
            epoll_event evs[8];
            int bytes_received, bytes_sent;

            r = epoll_wait(ephnd, evs, 8, -1);

            memset(buf, round, sizeof(buf));

            bytes_sent = 0;
            do {
                client_sock.Send(std::string(buf, sizeof(buf)));
                bytes_sent += sizeof(buf);

                r = epoll_wait(ephnd, evs, 8, 0);
            } while (r > 0);

            bytes_received = 0;
            do {
                auto s = server_sock.Receive();
                bytes_received += s.size();
            } while (bytes_received < bytes_sent);
        }

        r = epoll_ctl(ephnd, EPOLL_CTL_DEL, client_sock.Fd(), NULL);
    }

}