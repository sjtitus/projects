
/*_____________________________________________________________________________
 * 
 * LocalSocketServer
 *
 * Server that asynchronously handles communications for a specified Unix
 * domain socket. The server establishes a LocalSocketSession for each
 * incoming connection, then passes the session to a specified 
 * LocalSocketSessionHandler for handling of asynch IO.
 *
 * Uses boost::asio  
 *_____________________________________________________________________________
*/

#include <boost/bind.hpp> 
#include "LocalSocketServer.hpp"
#include "SimpleException.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;


using boost::asio::local::stream_protocol;

namespace com { namespace dimension3designs {

// Static logger
log4cxx::LoggerPtr LocalSocketServer::_logger(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketServer"));

    
//__________________________________________________________________________________
// Constructor
// io_service: REFERENCE to the boost asio io_service object used for asynch io.
// socketFile: file on local filesystem used as Unix domain socket
// pSessionHandler: the app-specific LocalSocketSessionHandler that will handle
//                  reading/writing on the session. 
LocalSocketServer::LocalSocketServer(boost::asio::io_service& io_service, 
            const std::string& socketFile,
            std::unique_ptr<LocalSocketSessionHandler>& pSessionHandler)
    : _io_service(io_service),
      _acceptor(io_service, stream_protocol::endpoint(socketFile)),
      _socketFile(socketFile),
      _pSessionHandler(std::move(pSessionHandler))
{
    LOG4CXX_TRACE(_logger,"LocalSocketServer::Construct: socket file "<< _socketFile);
    LOG4CXX_TRACE(_logger,"LocalSocketServer::Construct: session handler "<< _pSessionHandler.get());
}


//_________________________________________________________________________
// Start: start accepting connections on the local socket and handing
// them off to the session handler.
void LocalSocketServer::Start()
{
    LOG4CXX_TRACE(_logger,"LocalSocketServer::Start");
    LOG4CXX_TRACE(_logger,"LocalSocketServer::Start: socket file "<<_socketFile); 
    try
    {
        // create a session 
        LOG4CXX_DEBUG(_logger,"LocalSocketServer::Start: creating new session"); 
        LocalSocketSession::Ptr new_session(new LocalSocketSession(_io_service));
        LOG4CXX_DEBUG(_logger,"LocalSocketServer::Start: new session "<< new_session.get()); 

        // asynch accept of connection for the new session
        Accept(new_session);

        LOG4CXX_TRACE(_logger,"LocalSocketServer::Start: ok");
    }
    catch(std::exception &e) 
    {
        LOG4CXX_ERROR(_logger,"LocalSocketServer::Start: exception: "<< e.what()); 
        RAISE_EXCEPTION(std::string("error starting LocalSocketServer: ")+ e.what());
    }
}



    
//_________________________________________________________________________
// Stop: stop accepting connections
void LocalSocketServer::Stop()
{
    LOG4CXX_TRACE(_logger,"LocalSocketServer::Stop"); 
    // TODO: implement
    LOG4CXX_TRACE(_logger,"LocalSocketServer::Stop: ok"); 
}


//_________________________________________________________________________
// AcceptHandler: async callback handling new incoming sessions and
// dispatching them to the session handler for I/O. 
void LocalSocketServer::AcceptHandler(LocalSocketSession::Ptr &new_session,
        const boost::system::error_code& error)
{
    LOG4CXX_TRACE(_logger,"LocalSocketServer::AcceptHandler"); 
    if (error)
    {
        LOG4CXX_ERROR(_logger,"LocalSocketServer::AcceptHandler: error handling incoming connection: " << error.message()); 
        RAISE_EXCEPTION(std::string("error handling incoming connection: ") + error.message());
    }

    try
    {
        LOG4CXX_TRACE(_logger,"LocalSocketServer::AcceptHandler: connected session " << new_session.get());
     
        // Invoke the session handler (which will stash a copy of the session) 
        _pSessionHandler->HandleSession(new_session);

        // reset the session for the next incoming connection.
        LOG4CXX_DEBUG(_logger,"LocalSocketServer::AcceptHandler: resetting for next connection"); 
        new_session.reset(new LocalSocketSession(_io_service));
        LOG4CXX_DEBUG(_logger,"LocalSocketServer::AcceptHandler: new session "<< new_session.get()); 
 
        // Accept a new connection 
        Accept(new_session); 
    }
    catch(std::exception &e) 
    {
        LOG4CXX_ERROR(_logger,"LocalSocketServer::AcceptHandler: exception: "<<e.what()); 
        RAISE_EXCEPTION(std::string("error resetting for next connection: ")+e.what());
    }
}


//_________________________________________________________________________
// Accept: async accept of connection on a new session, using
// async callback AcceptHandler 
void LocalSocketServer::Accept(LocalSocketSession::Ptr &new_session)
{
    // asynch accept of a connection using the new session's
    // socket (callback = AcceptHandler method)
    LOG4CXX_DEBUG(_logger,"LocalSocketServer::Start: bind/asynch accept"); 
    _acceptor.async_accept(
        new_session->Socket(),
        boost::bind(
            &LocalSocketServer::AcceptHandler, 
            this, 
            new_session,
            boost::asio::placeholders::error
        )
    );
}
    
}}  // namespace
