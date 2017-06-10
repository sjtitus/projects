//_____________________________________________________________________________
// Circular Message Buffer
// A circular buffer of Messages that can be shared between writer/reader 
// threads (singleton).  
//_____________________________________________________________________________
#include "CircularMessageBuffer.hpp"
#include "Logger.hpp"
#include "Exception.hpp"
#include "Message.hpp"

#include <cstdio>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp> 


//_____________________________________________________________________________
// Circular Message Buffer

    //_____________________________________________________________________________
    // Constructor (private for singleton) 
    CircularMessageBuffer::CircularMessageBuffer(std::size_t size)
        :message_buffer_(size)
    {
        L_DEBUG << "construct";
    }

    //_____________________________________________________________________________
    // Push a message onto the tail
    bool CircularMessageBuffer::PushBack(const Message::MessagePtr pMsg)
    {
        boost::mutex::scoped_lock lock(io_mutex_);
        if (message_buffer_.full())
        {
            L_ERROR << "*** DATA LOSS ***: push to full message buffer";
            return false; 
        }
        L_DEBUG << "pushing a message";
        message_buffer_.push_back(pMsg);
        return true;
    }


    //_____________________________________________________________________________
    // Pop and return a message from the head 
    bool CircularMessageBuffer::PopFront(Message::MessagePtr &pMsg)
    {
        boost::mutex::scoped_lock lock(io_mutex_);
        if (message_buffer_.empty())
        {
            L_ERROR << "pop from empty message buffer";
            return false; 
        }
        L_DEBUG << "popping a message";
        pMsg = message_buffer_.front();
        message_buffer_.pop_front();
        return true;
    }
 
    //_____________________________________________________________________________
    // Full: is buffer full? 
    bool CircularMessageBuffer::Full()
    {
        boost::mutex::scoped_lock lock(io_mutex_);
        return message_buffer_.full();
    } 
    
    //_____________________________________________________________________________
    // Empty: is buffer empty? 
    bool CircularMessageBuffer::Empty()
    {
        boost::mutex::scoped_lock lock(io_mutex_);
        return message_buffer_.empty();
    }
    
    //_____________________________________________________________________________
    // Size: num elements currently in buffer 
    std::size_t CircularMessageBuffer::Size()
    {
        boost::mutex::scoped_lock lock(io_mutex_);
        return message_buffer_.size();
    }
    
    //_____________________________________________________________________________
    // Capacity: num elements buffer can hold 
    std::size_t CircularMessageBuffer::Capacity()
    {
        boost::mutex::scoped_lock lock(io_mutex_);
        return message_buffer_.capacity();
    }

#ifdef _UNIT_TEST_CIRCULARMESSAGEBUFFER_H_

int main(int argc, const char **argv)
{
    return 0;
}
#endif
