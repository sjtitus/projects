#ifndef _LOCALSOCKETSESSION_H_
#define _LOCALSOCKETSESSION_H_

/*_____________________________________________________________________________
 *
 *  LocalSocketSession
 * 
 *  Session for asynchronous read/write of string messages over a Unix  
 *  domain (local) socket. Uses a MessageHandler for application-specific 
 *  message handling (async callbacks).
 *
 * ____________________________________________________________________________
*/

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <string>
#include <memory>

#include "MessageHandler.hpp"
#include "Logging.hpp"


#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

// use local (unix domain) sockets
using boost::asio::local::stream_protocol;

namespace com { namespace dimension3designs {

//______________________________________
// LocalSocketSession

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
        _pMessageHandler = std::move(pMessageHandler);
    }
    
    //__________________________________________________________________________
    // Start: start asynch IO on the session by invoking the app-specific
    // handler's start function.
    void Start()
    {
        _pMessageHandler->Start();
    }

    //__________________________________________________________________________
    // Socket: access to the underlying socket
    stream_protocol::socket& Socket() { return _socket; }
   
    //__________________________________________________________________________
    // Connect: Synchronous client connect to a socket file endpoint
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
    
    //__________________________________________________________________________
    // Id: Return the unique ID for the session 
    const std::string& Id() { return _id; }


  private:

    // message delimiter
    static constexpr const char *DELIMITER = "\n";

    // ReadCallback: internal async read callback 
    void ReadCallback( const boost::system::error_code& error, size_t bytes_transferred );

    // WriteCallback: internal async write callback 
    void WriteCallback( const boost::system::error_code& error, size_t bytes_transferred );
    
    // Local unix-domain socket
    stream_protocol::socket _socket;

    // Data buffers 
    boost::asio::streambuf _request;
    boost::asio::streambuf _response;
 
    // Message handler providing application-level asynch callbacks
    std::unique_ptr<MessageHandler> _pMessageHandler;

    // Unique ID for the session 
    std::string _id;

  private:    
    // logging
    static log4cxx::LoggerPtr  _logger;
};


#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

}}  // namespace

#endif
