//_____________________________________________________________________________
// Circular Message Buffer
// A circular buffer of Messages that can be shared between writer/reader 
// threads (singleton).  
//_____________________________________________________________________________
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
class CircularMessageBuffer 
{
  public:
    typedef boost::shared_ptr<CircularMessageBuffer> Ptr; 
    
    //_____________________________________________________________________________
    // Constructor 
    CircularMessageBuffer(std::size_t size);
    
    //_____________________________________________________________________________
    // GetInstance: get singleton instance
    static CircularMessageBuffer::Ptr GetInstance();

    //_____________________________________________________________________________
    // Push a message onto the tail
    bool PushBack(const Message::MessagePtr pMsg);

    //_____________________________________________________________________________
    // Pop and return a message from the head 
    bool PopFront(Message::MessagePtr &pMsg);
    
    //_____________________________________________________________________________
    // Full: is buffer full? 
    bool Full();
    
    //_____________________________________________________________________________
    // Empty: is buffer empty? 
    bool Empty();
    
    //_____________________________________________________________________________
    // Size: num elements currently in buffer 
    std::size_t Size();
    
    //_____________________________________________________________________________
    // Capacity: num elements buffer can hold 
    std::size_t Capacity();


  private:
    boost::mutex io_mutex_;    // mutex for thread safety
   
    // underlying circular message buffer 
    boost::circular_buffer<Message::MessagePtr> message_buffer_; 
};
 
