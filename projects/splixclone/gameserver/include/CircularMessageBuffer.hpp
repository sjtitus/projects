#ifndef _CIRCULARBUFFER_H_
#define _CIRCULARBUFFER_H_

//_____________________________________________________________________________
// Circular Message Buffer
// A circular buffer of Messages that can be shared between writer/reader 
// threads (singleton).  
//_____________________________________________________________________________

#include <cstdio>
#include <iostream>
#include <boost/shared_ptr.hpp>
#include <boost/circular_buffer.hpp>
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp> 

#include "Logging.hpp"

namespace com { namespace dimension3designs {


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
    // Push a message onto the tail
    bool PushBack(const std::string &msg);

    //_____________________________________________________________________________
    // Pop and return a message from the head 
    std::unique_ptr<std::string> PopFront();
    
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
    boost::circular_buffer<std::unique_ptr<std::string>> _message_buffer; 
        
    static log4cxx::LoggerPtr _logger;        // logging
};

}}
 
#endif
