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
    log4cxx::LoggerPtr LocalSocketSession::_logger(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketSession"));

    //__________________________________________________________________________
    // Constructor
    LocalSocketSession::LocalSocketSession( boost::asio::io_service& io_service )
        :_socket(io_service), 
         _pMessageHandler(std::unique_ptr<MessageHandler>(nullptr)) 
    {
        LOG4CXX_TRACE(_logger,"LocalSocketSession: Construct: " << this);
    } 
    
    //__________________________________________________________________________
    // Destructor 
    LocalSocketSession::~LocalSocketSession()
    {
        LOG4CXX_TRACE(_logger,"LocalSocketSession: Destruct: " << this);
    }
    
    //__________________________________________________________________________
    // Connect: Synchronous client connect to the socket file 
    void LocalSocketSession::Connect( const std::string &localFile )
    {
        LOG4CXX_TRACE(_logger,"LocalSocketSession::Connect: file=" << localFile);
        try 
        {
            _socket.connect(stream_protocol::endpoint(localFile));
        } 
        catch(std::exception &e)
        {
            LOG4CXX_ERROR(_logger,"LocalSocketSession::Connect: file="+localFile+", exception: " << e.what());
            RAISE_EXCEPTION(std::string("error connecting to file ")+localFile+": "+ e.what());
        }
    }


    //__________________________________________________________________________
    // ReadMesssage: Async message read 
    void LocalSocketSession::ReadMessage()
    {
        BOOST_ASSERT_MSG(_pMessageHandler,"LocalSocketSession::ReadMessage: MessageHandler must be defined");
        LOG4CXX_TRACE(_logger,"LocalSocketSession:asynch read message");
        try
        {
            boost::asio::async_read_until(
                _socket, 
                _response, 
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
            LOG4CXX_ERROR(_logger,"LocalSocketSession::ReadMessage: exception: " << e.what());
            RAISE_EXCEPTION(std::string("error reading message: ")+e.what());
        }
    }

 
    //__________________________________________________________________________
    // WriteMesssage: Async message write 
    void LocalSocketSession::WriteMessage( const std::string &message )
    {
        BOOST_ASSERT_MSG(_pMessageHandler,"LocalSocketSession::WriteMessage: MessageHandler must be defined");
        LOG4CXX_TRACE(_logger,"LocalSocketSession::WriteMessage, len="<< message.size());
        try
        {
            // TODO: make sure message does not contain delimeter
            std::ostream _requeststream(&_request);
            _requeststream << message << LocalSocketSession::DELIMITER; 
            LOG4CXX_DEBUG(_logger,"LocalSocketSession::WriteMessage: queued message size: " << _request.size()); 

            boost::asio::async_write(
                _socket, 
                _request,
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
            LOG4CXX_ERROR(_logger,"LocalSocketSession::WriteMessage: exception: " << e.what());
            RAISE_EXCEPTION(std::string("error writing message: ")+e.what());
        }
    }
    
    //__________________________________________________________________________
    // Close: Close the session 
    void LocalSocketSession::Close()
    {
        LOG4CXX_TRACE(_logger,"LocalSocketSession::Close: shutting down then closing socket");
        _socket.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        _socket.close(); 
    }



    //__________________________________________________________________________
    //  HELPERS


    //__________________________________________________________________________
    // ReadCallback: internal async read callback 
    void LocalSocketSession::ReadCallback( const boost::system::error_code& error, 
            size_t bytes_transferred )
    {
        LOG4CXX_TRACE(_logger,"LocalSocketSession::ReadCallback: receiving message");
        if (error)
        {
            LOG4CXX_ERROR(_logger,"LocalSocketSession::ReadCallback: error reading messaage: " << error.message());
            if (_pMessageHandler)
            { 
                LOG4CXX_TRACE(_logger,"LocalSocketSession::ReadCallback: invoking error handler");
                _pMessageHandler->HandleReadError(error);
            }
        }
        else if (bytes_transferred > 0) 
        {
            LOG4CXX_DEBUG(_logger,"LocalSocketSession::ReadCallback: " << bytes_transferred << " bytes");
            try 
            { 
                // read entire socket stream into a new message (string) 
                std::istream responsestream(&_response);
                std::unique_ptr<std::string> pMessage = std::unique_ptr<std::string>(
                    new std::string(std::istreambuf_iterator<char>(responsestream), {})
                );
                if (responsestream.fail())
                {
                    LOG4CXX_ERROR(_logger,"LocalSocketSession::ReadCallback: getline stream fail");
                } 
                if (responsestream.eof())
                {
                    LOG4CXX_ERROR(_logger,"LocalSocketSession::ReadCallback: getline stream eof");
                } 
            
                // invoke the message handler, passing message ownership
                LOG4CXX_DEBUG(_logger,"LocalSocketSession::ReadCallback: incoming message: '" <<  *pMessage << "'");
                if (_pMessageHandler)
                { 
                    LOG4CXX_TRACE(_logger,"LocalSocketSession::ReadCallback: invoking incoming message handler");
                    _pMessageHandler->HandleRead(std::move(pMessage));
                }
            }
            catch(std::exception &e) 
            {
                LOG4CXX_DEBUG(_logger,"LocalSocketSession::ReadCallback: caught exception: " << e.what());
            }
        }
        else
        {
            LOG4CXX_ERROR(_logger,"LocalSocketSession::ReadCallback: 0 bytes transferred (socket closed?)");
            RAISE_EXCEPTION(std::string("error reading: 0 bytes transferred (socket closed?)"));
        }
    }



    //__________________________________________________________________________
    // WriteCallback: internal async write callback 
    void LocalSocketSession::WriteCallback( const boost::system::error_code& error, 
            size_t bytes_transferred )
    {
        LOG4CXX_TRACE(_logger,"LocalSocketSession::WriteCallback");
        if (error)
        {
            LOG4CXX_ERROR(_logger,"LocalSocketSession::WriteCallback: error during write: " << error.message());
            if (_pMessageHandler)
            {
                LOG4CXX_TRACE(_logger,"LocalSocketSession::WriteCallback: invoking error callback");
                _pMessageHandler->HandleWriteError(error);
            }
        }
        else if (bytes_transferred > 0)
        { 
            LOG4CXX_DEBUG(_logger,"LocalSocketSession::WriteCallback: written message size: " << bytes_transferred);
            // empty the write buffer? Seems like it's not needed..? 
            // _request.consume(_request.size()); 
            if (_pMessageHandler)
            { 
                LOG4CXX_TRACE(_logger,"LocalSocketSession::WriteCallback: invoking write callback");
                _pMessageHandler->HandleWrite(bytes_transferred);
            }
        }
        else
        {
            LOG4CXX_ERROR(_logger,"LocalSocketSession::WriteCallback: 0 bytes transferred (socket closed?)");
            RAISE_EXCEPTION(std::string("error writing: 0 bytes transferred (socket closed?)"));
        }
    }
    

}}  // namespace
