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

#include "LocalSocketSession.hpp"
#include "Logging.hpp"
#include "SimpleException.hpp"


// use local (unix domain) sockets
using boost::asio::local::stream_protocol;


namespace com { namespace dimension3designs {

    //__________________________________________________________________________
    // Initialize logger
    log4cxx::LoggerPtr LocalSocketSession::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketSession"));

    //__________________________________________________________________________
    // Constructor
    LocalSocketSession::LocalSocketSession( boost::asio::io_service& io_service )
        :socket_(io_service), 
         pMessageHandler_(std::unique_ptr<MessageHandler>(nullptr)) 
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession: Construct: " << this);
    } 
    
    //__________________________________________________________________________
    // Destructor 
    LocalSocketSession::~LocalSocketSession()
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession: Destruct: " << this);
    }
    
    //__________________________________________________________________________
    // Connect: Synchronous client connect to the socket file 
    void LocalSocketSession::Connect( const std::string &localFile )
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession::Connect: file=" << localFile);
        try 
        {
            socket_.connect(stream_protocol::endpoint(localFile));
        } 
        catch(std::exception &e)
        {
            LOG4CXX_ERROR(logger_,"LocalSocketSession::Connect: file="+localFile+", exception: " << e.what());
            RAISE_EXCEPTION(std::string("error connecting to file ")+localFile+": "+ e.what());
        }
    }


    //__________________________________________________________________________
    // ReadMesssage: Async message read 
    void LocalSocketSession::ReadMessage()
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
            LOG4CXX_ERROR(logger_,"LocalSocketSession::ReadMessage: exception: " << e.what());
            RAISE_EXCEPTION(std::string("error reading message: ")+e.what());
        }
    }

 
    //__________________________________________________________________________
    // WriteMesssage: Async message write 
    void LocalSocketSession::WriteMessage( const std::string &message )
    {
        BOOST_ASSERT_MSG(pMessageHandler_,"LocalSocketSession::WriteMessage: MessageHandler must be defined");
        LOG4CXX_TRACE(logger_,"LocalSocketSession::WriteMessage, len="<< message.size());
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
            LOG4CXX_ERROR(logger_,"LocalSocketSession::WriteMessage: exception: " << e.what());
            RAISE_EXCEPTION(std::string("error writing message: ")+e.what());
        }
    }
    
    //__________________________________________________________________________
    // Close: Close the session 
    void LocalSocketSession::Close()
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession::Close: shutting down then closing socket");
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_.close(); 
    }



    //__________________________________________________________________________
    //  HELPERS


    //__________________________________________________________________________
    // ReadCallback: internal async read callback 
    void LocalSocketSession::ReadCallback( const boost::system::error_code& error, 
            size_t bytes_transferred )
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession::ReadCallback: receiving message");
        if (error)
        {
            LOG4CXX_ERROR(logger_,"LocalSocketSession::ReadCallback: error reading messaage: " << error.message());
            if (pMessageHandler_)
            { 
                LOG4CXX_TRACE(logger_,"LocalSocketSession::ReadCallback: invoking error handler");
                pMessageHandler_->HandleReadError(error);
            }
        }
        else if (bytes_transferred > 0) 
        {
            LOG4CXX_DEBUG(logger_,"LocalSocketSession::ReadCallback: " << bytes_transferred << " bytes");
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
                    LOG4CXX_TRACE(logger_,"LocalSocketSession::ReadCallback: invoking incoming message handler");
                    pMessageHandler_->HandleRead(std::move(pMessage));
                }
            }
            catch(std::exception &e) 
            {
                LOG4CXX_DEBUG(logger_,"LocalSocketSession::ReadCallback: caught exception: " << e.what());
            }
        }
        else
        {
            LOG4CXX_ERROR(logger_,"LocalSocketSession::ReadCallback: 0 bytes transferred (socket closed?)");
            RAISE_EXCEPTION(std::string("error reading: 0 bytes transferred (socket closed?)"));
        }
    }



    //__________________________________________________________________________
    // WriteCallback: internal async write callback 
    void LocalSocketSession::WriteCallback( const boost::system::error_code& error, 
            size_t bytes_transferred )
    {
        LOG4CXX_TRACE(logger_,"LocalSocketSession::WriteCallback");
        if (error)
        {
            LOG4CXX_ERROR(logger_,"LocalSocketSession::WriteCallback: error during write: " << error.message());
            if (pMessageHandler_)
            {
                LOG4CXX_TRACE(logger_,"LocalSocketSession::WriteCallback: invoking error callback");
                pMessageHandler_->HandleWriteError(error);
            }
        }
        else if (bytes_transferred > 0)
        { 
            LOG4CXX_DEBUG(logger_,"LocalSocketSession::WriteCallback: written message size: " << bytes_transferred);
            // empty the write buffer? Seems like it's not needed..? 
            // request_.consume(request_.size()); 
            if (pMessageHandler_)
            { 
                LOG4CXX_TRACE(logger_,"LocalSocketSession::WriteCallback: invoking write callback");
                pMessageHandler_->HandleWrite(bytes_transferred);
            }
        }
        else
        {
            LOG4CXX_ERROR(logger_,"LocalSocketSession::WriteCallback: 0 bytes transferred (socket closed?)");
            RAISE_EXCEPTION(std::string("error writing: 0 bytes transferred (socket closed?)"));
        }
    }
    

}}  // namespace
