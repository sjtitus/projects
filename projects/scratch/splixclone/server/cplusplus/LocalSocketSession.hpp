/*_____________________________________________________________________________
 *
 *  LocalSocketSession
 *  Session for asynchronously passing string messages over a Unix  
 *  domain (local) socket.
 *
 *  LocalSocketSessionHandler
 *  Encapsulates a LocalSocketSession and  provides a base class
 *  used to implement application-specific message handling 
 *  functionality. 
 * ______________________________________________________________________________
*/

#ifndef _LOCALSOCKETSESSION_H_
#define _LOCALSOCKETSESSION_H_

#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>
#include <string>
#include <deque>
#include <memory>

#if defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

// use local (unix domain) sockets
using boost::asio::local::stream_protocol;

//______________________________________________________________________________
// LocalSocketSession
// A bidirectional asynchronous socket connection used to send and receive
// strings. Delimiter is '\n'.
class LocalSocketSession : public boost::enable_shared_from_this<LocalSocketSession>
{
  public:
    
    //__________________________________________________________________________
    // Types 
    typedef boost::shared_ptr<LocalSocketSession> Ptr;

    //__________________________________________________________________________
    // MessageHandler (abstract base class)
    // Each LocalSocketSession uses a message handler which provides 
    // application-specific read/write callbacks for async operation. 
    class MessageHandler
    {
        public:
            typedef boost::shared_ptr<MessageHandler> Ptr;
            virtual void HandleRead( std::unique_ptr<std::string> pMessage ) = 0;
            virtual void HandleWrite( size_t bytesWritten ) = 0;
            virtual void HandleReadError( const boost::system::error_code& error ) = 0;
            virtual void HandleWriteError( const boost::system::error_code& error ) = 0;
            virtual void Start()=0;
    };

    //__________________________________________________________________________
    // Constructors / Destructor
    LocalSocketSession( boost::asio::io_service& io_service );
    
    ~LocalSocketSession();
    
    //__________________________________________________________________________
    // SetMessageHandler: Set the handler that provides async read/write
    // callbacks. 
    void SetMessageHandler( LocalSocketSession::MessageHandler::Ptr &pMessageHandler );

    //__________________________________________________________________________
    // Socket: access to the underlying socket
    stream_protocol::socket& Socket();
   
    //__________________________________________________________________________
    // Connect: Synchronous client connect to a server
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
    void ReadCallback( const boost::system::error_code& error, 
            size_t bytes_transferred );

    // WriteCallback: internal async write callback 
    void WriteCallback( const boost::system::error_code& error, 
            size_t bytes_transferred );
    
    // Local unix-domain socket
    stream_protocol::socket socket_;

    // Data buffers 
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
 
    // Message handler providing application-level callbacks
    LocalSocketSession::MessageHandler::Ptr pMessageHandler_; 
};



//__________________________________________________________________________
// LocalSocketSessionHandler: abstract class for application-specific
// session handling.
class LocalSocketSessionHandler
{
    public:
        virtual void HandleSession( LocalSocketSession::Ptr &pSession ) = 0;
};



/*
//__________________________________________________________________________
// LocalSocketSessionHandler
// Encapsulates a LocalSocketSession and  provides a base class used to 
// implement application-specific message handling functionality. 
class LocalSocketSessionHandler: public LocalSocketSession::MessageHandler
{
    public:
        long UseCount() { return pSession_.use_count(); }
        virtual void HandleSession( LocalSocketSession::Ptr &pSession ); 
        virtual void Start(); 
        
    public:
        virtual void HandleRead( std::unique_ptr<std::string> pMessage );
        virtual void HandleWrite( size_t bytesWritten );
        virtual void HandleReadError( const boost::system::error_code& error );
        virtual void HandleWriteError( const boost::system::error_code& error );

    protected:
        LocalSocketSession::Ptr pSession_;
};
*/




#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

#endif
