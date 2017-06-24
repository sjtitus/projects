/*_____________________________________________________________________________
 *
 *  LocalSocketSession
 *  Session for asynchronous send/receive of string messages over a Unix  
 *  domain (local) socket. Invokes a MessageHandler for application-specific 
 *  message handling.
 *  
 *  Uses boost::asio for asynch network handling.  
 *
 *  LocalSocketSessionHandler
 *  Abstract class used for application specific handling of 
 *  LocalSocketSession messages.
 * ____________________________________________________________________________
*/

#ifndef _LOCALSOCKETSESSION_H_
#define _LOCALSOCKETSESSION_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <type_traits>
#include <boost/asio.hpp>
#include <string>
#include <deque>
#include <memory>

#include "MessageHandler.hpp"
#include "Logging.hpp"

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

// use local (unix domain) sockets
using boost::asio::local::stream_protocol;

namespace com { namespace dimension3designs {

//______________________________________________________________________________
// LocalSocketSession
// A bidirectional asynchronous socket connection used to send and receive
// strings over a local (Unix domain) socket.
// End-of-message delimiter is "\n"

class LocalSocketSession : public boost::enable_shared_from_this<LocalSocketSession>
{
  public:
    
    //__________________________________________________________________________
    // Types 
    typedef boost::shared_ptr<LocalSocketSession> Ptr;

    //__________________________________________________________________________
    // Constructors / Destructor
    LocalSocketSession( boost::asio::io_service& io_service );
    ~LocalSocketSession();
    
    //__________________________________________________________________________
    // SetMessageHandler: Set the handler that provides async read/write
    // callbacks. 
    void SetMessageHandler( MessageHandler::Ptr &pMessageHandler )
    {
        pMessageHandler_ = pMessageHandler;
    }

    //__________________________________________________________________________
    // Socket: access to the underlying socket
    stream_protocol::socket& Socket() { return socket_; }
   
    //__________________________________________________________________________
    // Connect: Synchronous client connect to the socket file 
    void Connect( const std::string &localFile );

    //__________________________________________________________________________
    // ReadMesssage: Async message read 
    void ReadMessage();
 
    //__________________________________________________________________________
    // WriteMesssage: Async message write 
    void WriteMessage( const std::string &message );
    
    //__________________________________________________________________________
    // Close: Close the session 
    void Close();

  private:

    // message delimiter
    static constexpr const char *DELIMITER = "\n";

    // ReadCallback: internal async read callback 
    void ReadCallback( const boost::system::error_code& error, size_t bytes_transferred );

    // WriteCallback: internal async write callback 
    void WriteCallback( const boost::system::error_code& error, size_t bytes_transferred );
    
    // Local unix-domain socket
    stream_protocol::socket socket_;

    // Data buffers 
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
 
    // Message handler providing application-level callbacks
    MessageHandler::Ptr pMessageHandler_; 

    public:    
        // logging
        static log4cxx::LoggerPtr  logger_;
};



//__________________________________________________________________________
// LocalSocketSessionHandler: socket session handling.
// Wires up the session to an application-specific MessageHandler via
// templating.
// 
template<typename T>
class LocalSocketSessionHandler
{
    public:
        LocalSocketSessionHandler()
        {
            BOOST_STATIC_ASSERT((std::is_base_of<MessageHandler, T>::value));
            LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler: construct");
        }

        void HandleSession( LocalSocketSession::Ptr &pSession )
        {
            LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler: handle session " << pSession.get())
            // Create shared_ptr codependency: MessageHandler holds Session and
            // Session holds MessaageHandler. The way the duo is released from mem
            // is when a MessageHandler does a session pointer reset, which triggers session
            // deletion, and subsequent MessageHandler deletion.
            MessageHandler::Ptr pHandler(new T(pSession));
            pSession->SetMessageHandler(pHandler);
            pHandler->Start();
        }

       
    public: 
        // logging
        static log4cxx::LoggerPtr  logger_;
};

template<typename T>
log4cxx::LoggerPtr LocalSocketSessionHandler<T>::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketSession"));


#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

}}  // namespace

#endif
