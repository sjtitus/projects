#ifndef _LOCALSOCKETSESSION_H_
#define _LOCALSOCKETSESSION_H_

/*_____________________________________________________________________________
 *
 *  LocalSocketSession
 *  Session for asynchronous read/write of string messages over a Unix  
 *  domain (local) socket. Invokes a MessageHandler 
 *  for application-specific message handling.
 *
 *  Uses boost::asio for asynch network handling.  
 * ____________________________________________________________________________
*/

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
    void SetMessageHandler( std::unique_ptr<MessageHandler> &pMessageHandler )
    {
        pMessageHandler_ = std::move(pMessageHandler);
    }

    //__________________________________________________________________________
    // Start: start asynch IO on the session by invoking the app-specific
    // handler's start function (which will read or write to the socket). 
    void Start()
    {
        pMessageHandler_->Start();
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
    std::unique_ptr<MessageHandler> pMessageHandler_;

    private:    
        // logging
        static log4cxx::LoggerPtr  logger_;
};


#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

}}  // namespace

#endif
