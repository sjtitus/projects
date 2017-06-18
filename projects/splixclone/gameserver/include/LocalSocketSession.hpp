/*_____________________________________________________________________________
 *
 *  LocalSocketSession
 *  Session for asynchronous send/receive of string messages over a Unix  
 *  domain (local) socket. Invokes a LocalSocketSessionHandler (see below)
 *  for application-specific message handling.
 *
 *  LocalSocketSessionHandler
 *  Abstract class used for application specific handling of 
 *  LocalSocketSession messages.
 *
 *  Uses boost::asio for asynch network handling.  
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

template<typename T>  
class LocalSocketSession : public boost::enable_shared_from_this<LocalSocketSession<T>>
{
  public:
    
    //__________________________________________________________________________
    // Types 
    typedef boost::shared_ptr<LocalSocketSession> Ptr;

    //__________________________________________________________________________
    // Constructors / Destructor
    // template<class a_type> void a_class<a_type>::a_function(){...}
    LocalSocketSession( boost::asio::io_service& io_service )
    {
        // make sure that class T is a subclass of MessageHandler
        BOOST_STATIC_ASSERT((std::is_base_of<MessageHandler, T>::value));
        LOG4CXX_TRACE(logger_,"LocalSocketSession: construct");
    } 
    
    ~LocalSocketSession()
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession: destruct");
    }
    
    //__________________________________________________________________________
    // SetMessageHandler: Set the handler that provides async read/write
    // callbacks. 
    //void SetMessageHandler( LocalSocketSession::MessageHandler::Ptr &pMessageHandler );

    //__________________________________________________________________________
    // Socket: access to the underlying socket
    stream_protocol::socket& Socket() { return socket_; }
   
    //__________________________________________________________________________
    // Connect: Synchronous client connect to the socket file 
    void Connect( const std::string &localFile )
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession:sync connect to " << localFile);
        try 
        {
            socket_.connect(stream_protocol::endpoint(localFile));
        } 
        catch(std::exception &e) 
        {
        }
    }

    //__________________________________________________________________________
    // ReadMesssage: Async message read 
    void ReadMessage()
    {
        BOOST_ASSERT_MSG(pMessageHandler_,"LocalSocketSession::ReadMessage: MessageHandler must be defined");
        LOG4CXX_TRACE(logger_,"LocalSocketSession:asynch read message");
        try
        {
            boost::asio::async_read_until(
                socket_, 
                response_, 
                LocalSocketSession::DELIMITER,
                boost::bind(
                    &LocalSocketSession::ReadCallback, 
                    this->shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                )
            );
        }
        catch(std::exception &e) 
        {
        }
    }

 
    //__________________________________________________________________________
    // WriteMesssage: Async message write 
    void WriteMessage( const std::string &message )
    {
        BOOST_ASSERT_MSG(pMessageHandler_,"*** MessageHandler must be defined ***");
        LOG4CXX_TRACE(logger_,"LocalSocketSession::WriteMessage:: asynch write message, message='" << message << "'");
        try
        {
            // TODO: make sure message does not contain delimeter
            std::ostream request_stream(&request_);
            request_stream << message << LocalSocketSession::DELIMITER; 
            LOG4CXX_DEBUG(logger_,"LocalSocketSession::WriteMessage: queued message size: " << request_.size()); 

            boost::asio::async_write(
                socket_, 
                request_,
                boost::bind(
                    &LocalSocketSession::WriteCallback,
                    this->shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                )
            );
        }
        catch(std::exception &e) 
        {
            LOG4CXX_DEBUG(logger_,"LocalSocketSession::WriteMessage: caught exception: " << e);
        }
    }
    
    //__________________________________________________________________________
    // Close: Close the session 
    void Close()
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession::Close: shutting down and closing socket");
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_.close(); 
    }

  private:

    // message delimiter
    static constexpr const char *DELIMITER = "\n";




    // ReadCallback: internal async read callback 
    void ReadCallback( const boost::system::error_code& error, 
            size_t bytes_transferred )
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession::ReadCallback: receiving message");
        if (error)
        {
            LOG4CXX_ERROR(logger_,"LocalSocketSession::ReadCallback: error reading messaage: " << error.message());
            if (pMessageHandler_)
            { 
                LOG4CXX_DEBUG(logger_,"LocalSocketSession::ReadCallback: invoking error handler");
                pMessageHandler_->HandleReadError(error);
            }
        }
        else if (bytes_transferred > 0) 
        {
            LOG4CXX_TRACE(logger_,"LocalSocketSession::ReadCallback: " << bytes_transferred << " bytes");
            try 
            { 
                // read entire socket stream into a new message (string) 
                std::istream response_stream(&response_);
                std::unique_ptr<std::string> pMessage = std::unique_ptr<std::string>(
                    new std::string(std::istreambuf_iterator<char>(response_stream), {})
                );
                if (response_stream.fail())
                {
                    LOG4CXX_ERROR(logger_,"LocalSocketSession::ReadCallback: getline stream fail");
                } 
                if (response_stream.eof())
                {
                    LOG4CXX_ERROR(logger_,"LocalSocketSession::ReadCallback: getline stream eof");
                } 
            
                // invoke the message handler, passing message ownership
                LOG4CXX_DEBUG(logger_,"LocalSocketSession::ReadCallback: incoming message: '" <<  *pMessage << "'");
                if (pMessageHandler_)
                { 
                    LOG4CXX_DEBUG(logger_,"LocalSocketSession::ReadCallback: invoking incoming message handler");
                    pMessageHandler_->HandleRead(std::move(pMessage));
                }
            }
            catch(std::exception &e) 
            {
                LOG4CXX_DEBUG(logger_,"LocalSocketSession::ReadCallback: caught exception: " << e);
            }
        }
        else
        {
            LOG4CXX_DEBUG(logger_,"LocalSocketSession::ReadCallback: 0 bytes transferred (socket closed?)");
        }
    }



    // WriteCallback: internal async write callback 
    void WriteCallback( const boost::system::error_code& error, 
            size_t bytes_transferred )
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession::WriteCallback");
        if (error)
        {
            LOG4CXX_ERROR(logger_,"LocalSocketSession::WriteCallback: error during write: " << error.message());
            if (pMessageHandler_)
            {
                LOG4CXX_DEBUG(logger_,"LocalSocketSession::WriteCallback: handling error");
                pMessageHandler_->HandleWriteError(error);
            }
            //THROW_SYSTEM_EXCEPTION("ERROR writing message: " + error.message());
        }
        else if (bytes_transferred > 0)
        { 
            LOG4CXX_DEBUG(logger_,"LocalSocketSession::WriteCallback: written message size: " << bytes_transferred);
            // empty the write buffer? Seems like it's not needed..? 
            // request_.consume(request_.size()); 
            if (pMessageHandler_)
            { 
                LOG4CXX_DEBUG(logger_,"LocalSocketSession::WriteCallback: invoking write callback");
                pMessageHandler_->HandleWrite(bytes_transferred);
            }
        }
        else
        {
            LOG4CXX_DEBUG(logger_,"LocalSocketSession::WriteCallback: 0 bytes transferred (socket closed?)");
        }
    }
    
    // Local unix-domain socket
    stream_protocol::socket socket_;

    // Data buffers 
    boost::asio::streambuf request_;
    boost::asio::streambuf response_;
 
    // Message handler providing application-level callbacks
    MessageHandler::Ptr pMessageHandler_; 
    
    // logging
    static log4cxx::LoggerPtr  logger_;
};

template<typename T>
log4cxx::LoggerPtr LocalSocketSession<T>::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketSession"));




/*
 * Maybe we don't need this?
 *
//__________________________________________________________________________
// LocalSocketSessionHandler: abstract class for application-specific
// session handling.
class LocalSocketSessionHandler
{
    public:
        virtual void HandleSession( LocalSocketSession::Ptr &pSession ) = 0;
};
*/


#else // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)
# error Local sockets not available on this platform.
#endif // defined(BOOST_ASIO_HAS_LOCAL_SOCKETS)

}}  // namespace

#endif
