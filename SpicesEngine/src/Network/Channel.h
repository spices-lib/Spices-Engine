/**
* @file Channel.h.
* @brief The Channel Class Definitions.
* @author Spices & Muduo.
*/

#pragma once
#include "InetAddress.h"
#include "Socket.h"

#include <wepoll.h>
#include <optional>

namespace Spices {
    
namespace Net {
    
    /**
    * @brief Forward Declare.
    */
    class EventLoop;

    /**
    * @brief Wrapper of SOCKET'S Event.
    */
    class Channel
    {
    public:

        using EventCallback = std::function<void()>;

        enum EventBits
        {
            None  = 0,
            Read  = EPOLLIN | EPOLLPRI,
            Write = EPOLLOUT,
        };

        using EventFlags = uint32_t;

        enum class PollState
        {
            New     = 0,
            Added   = 1,
            Deleted = 2
        };

    public:

        /**
        * @brief Constructor Function.
        * @param[in] fd socket fd.
        * @param[in] loop EventLoop.
        */
        Channel(SOCKET fd, EventLoop* loop);

        /**
        * @brief Destructor Function.
        */
        ~Channel() = default;
        
        /**
        * @brief Copy Constructor Function.
        * @note This Class not allowed copy behaves.
        */
        Channel(const Channel&) = delete;

        /**
        * @brief Copy Assignment Operation.
        * @note This Class not allowed copy behaves.
        */
        Channel& operator=(const Channel&) = delete;

        /**
        * @brief Handle happened events on fd.
        */
        void HandleEvent();

        /**
        * @brief Set Read Event Callback.
        * @param[in] cb Read Event Callback.
        */
        void SetReadCallback(EventCallback cb) 
        { 
            m_ReadCallback = std::move(cb); 
        }

        /**
        * @brief Set Write Event Callback.
        * @param[in] cb Write Event Callback.
        */
        void SetWriteCallback(EventCallback cb) 
        { 
            m_WriteCallback = std::move(cb); 
        }

        /**
        * @brief Set Close Event Callback.
        * @param[in] cb Close Event Callback.
        */
        void SetCloseCallback(EventCallback cb) 
        { 
            m_CloseCallback = std::move(cb); 
        }

        /**
        * @brief Set Error Event Callback.
        * @param[in] cb Error Event Callback.
        */
        void SetErrorCallback(EventCallback cb) 
        { 
            m_ErrorCallback = std::move(cb); 
        }

        void Tie(const std::shared_ptr<void>& obj);

        /**
        * @brief Get this SOCKET.
        * @return Returns this SOCKET
        */
        SOCKET Fd() const { return m_Fd; }

        /**
        * @brief Get this Events type.
        * @return Returns this Events type.
        */
        int Events() const { return m_Events; }

        /**
        * @brief Set this REvents type.
        * @param[in] revt Events type.
        */
        void SetRevents(int revt) { m_Revents = revt; }

        /**
        * @brief Enable Read event.
        */
        void EnableReading() 
        { 
            m_Events |= EventBits::Read; 
            Update(); 
        }

        /**
        * @brief Disable Read event.
        */
        void DisableReading() 
        { 
            m_Events &= ~EventBits::Read; 
            Update(); 
        }

        /**
        * @brief Enable Write event.
        */
        void EnableWriting() 
        { 
            m_Events |= EventBits::Write; 
            Update(); 
        }

        /**
        * @brief Disable Write event.
        */
        void DisableWriting() 
        { 
            m_Events &= ~EventBits::Write; 
            Update(); 
        }

        /**
        * @brief Disable All event.
        */
        void DisableAll() 
        { 
            m_Events = EventBits::None; 
            Update(); 
        }

        /**
        * @brief Determine whether the event is a None Event.
        * @return Returns true if the event is a None Event, otherwise false.
        */
        bool IsNoneEvent() const { return m_Events == EventBits::None; }

        /**
        * @brief Determine whether the event is a Write Event.
        * @return Returns true if the event is a Write Event, otherwise false.
        */
        bool IsWriting() const { return m_Events & EventBits::Write; }

        /**
        * @brief Determine whether the event is a Read Event.
        * @return Returns true if the event is a Read Event, otherwise false.
        */
        bool IsReading() const { return m_Events & EventBits::Read; }

        /**
        * @brief Get Poll State.
        * @return Returns PollState.
        */
        PollState GetPollState() { return m_PollState; }

        /**
        * @brief Set Poll State.
        * @param[in] state PollState.
        */
        void SetPollState(PollState state) { m_PollState = state; }

        void Remove();

    private:

        void Update();

        /**
        * @brief Internal handle happened events on fd.
        */
        void HandleEventsWithGuard() const;

    private:

        /**
        * @brief This channel interested SOCKET.
        */
        SOCKET m_Fd;

        /**
        * @brief This channel interested EventLoop.
        */
        EventLoop* m_Loop;

        /**
        * @brief This SOCKET interested Events type.
        */
        EventFlags m_Events;

        /**
        * @brief Current happened event type.
        */
        int m_Revents;

        /**
        * @brief This channel PollState.
        */
        PollState m_PollState;

        std::optional<std::weak_ptr<void>> m_Tie;

        /**
        * @brief Read Event Callback.
        */
        EventCallback m_ReadCallback;

        /**
        * @brief Write Event Callback.
        */
        EventCallback m_WriteCallback;

        /**
        * @brief Close Event Callback.
        */
        EventCallback m_CloseCallback;

        /**
        * @brief Error Event Callback.
        */
        EventCallback m_ErrorCallback;
    };
    
}
    
}