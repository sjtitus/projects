//_____________________________________________________________________________
// Circular Message Buffer
// A circular buffer of Messages that can be shared between writer/reader 
// threads (singleton).  
//_____________________________________________________________________________


#include "CircularMessageBuffer.hpp"
#include "SimpleException.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;

namespace com { namespace dimension3designs {

LoggerPtr CircularMessageBuffer::_logger(Logger::getLogger("com.dimension3designs.CircularMessageBuffer"));


//_____________________________________________________________________________
// Constructor (private for singleton) 
CircularMessageBuffer::CircularMessageBuffer(std::size_t size)
    :_message_buffer(size)
{
    LOG4CXX_TRACE(_logger,"CircularMessageBuffer::CircularMessageBuffer: size="<<size);
}


//_____________________________________________________________________________
// Push a message onto the tail
bool CircularMessageBuffer::PushBack(std::unique_ptr<std::string> &pMessage)
{
    boost::mutex::scoped_lock lock(io_mutex_);
    if (_message_buffer.full())
    {
        LOG4CXX_ERROR(_logger,"*** DATA LOSS ***: push to full message buffer");
        return false; 
    }
    LOG4CXX_DEBUG(_logger,"pushing a message");
    _message_buffer.push_back(std::move(pMessage));
    return true;
}


//_____________________________________________________________________________
// Pop and return a message from the head 
std::unique_ptr<std::string> CircularMessageBuffer::PopFront()
{
    boost::mutex::scoped_lock lock(io_mutex_);
    if (_message_buffer.empty())
    {
        LOG4CXX_ERROR(_logger,"pop from empty message buffer");
        return std::unique_ptr<std::string>(nullptr); 
    }
    LOG4CXX_DEBUG(_logger,"popping a message");
    std::unique_ptr<std::string> pMsg = std::move(_message_buffer.front());
    _message_buffer.pop_front();    // removes a now-null ptr from container
    return pMsg;
}

 
//_____________________________________________________________________________
// Full: is buffer full? 
bool CircularMessageBuffer::Full()
{
    boost::mutex::scoped_lock lock(io_mutex_);
    return _message_buffer.full();
} 
    
//_____________________________________________________________________________
// Empty: is buffer empty? 
bool CircularMessageBuffer::Empty()
{
    boost::mutex::scoped_lock lock(io_mutex_);
    return _message_buffer.empty();
}

//_____________________________________________________________________________
// Size: num elements currently in buffer 
std::size_t CircularMessageBuffer::Size()
{
    boost::mutex::scoped_lock lock(io_mutex_);
    return _message_buffer.size();
}

//_____________________________________________________________________________
// Capacity: num elements buffer can hold 
std::size_t CircularMessageBuffer::Capacity()
{
    boost::mutex::scoped_lock lock(io_mutex_);
    return _message_buffer.capacity();
}

}}
