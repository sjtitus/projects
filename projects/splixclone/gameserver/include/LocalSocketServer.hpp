#ifndef _LOCALSOCKETSERVER_H_
#define _LOCALSOCKETSERVER_H_
/*_____________________________________________________________________________
 * 
 * LocalSocketServer
 *
 * Server that asynchronously handles communications for a specified Unix
 * domain socket. The server establishes a LocalSocketSession for each
 * incoming connection, then passes the session to a specified 
 * LocalSocketSessionHandler for handling of asynch IO.
 *_____________________________________________________________________________
*/

#include <boost/asio.hpp>   

#include "LocalSocketSession.hpp"
#include "LocalSocketSessionHandler.hpp"


using boost::asio::local::stream_protocol;

namespace com { namespace dimension3designs {

    
class LocalSocketServer
{
  public:
    
    //__________________________________________________________________________________
    // Constructor
    // io_service: REFERENCE to the boost asio io_service object used for asynch io.
    // socketFile: file on local filesystem used as Unix domain socket
    // pSessionHandler: the app-specific LocalSocketSessionHandler that will handle
    //                  reading/writing on the session. 
    LocalSocketServer(boost::asio::io_service& io_service, 
            const std::string& socketFile,
            std::unique_ptr<LocalSocketSessionHandler>& pSessionHandler);

    //_________________________________________________________________________
    // Start: start accepting connections on the local socket and handing
    // them off to the session handler.
    void Start();
    
    //_________________________________________________________________________
    // Stop: stop accepting connections
    void Stop();


  private:
    
    //_________________________________________________________________________
    // Accept: async accept of connection on a new session, using
    // async callback AcceptHandler 
    void Accept(LocalSocketSession::Ptr &new_session);

    //_________________________________________________________________________
    // AcceptHandler: async callback handling new incoming sessions and
    // dispatching them to the session handler for I/O. 
    void AcceptHandler(LocalSocketSession::Ptr &new_session,
            const boost::system::error_code& error);

    boost::asio::io_service&                        _io_service;        // handles asynch IO (note: reference) 
    stream_protocol::acceptor                       _acceptor;          // used to accept incoming local socket connections
    std::string                                     _socketFile;        // file on local file system for unix socket
    std::unique_ptr<LocalSocketSessionHandler>      _pSessionHandler;   // app-specific session handler (owned by the server)  
    static log4cxx::LoggerPtr                       _logger;            // logging
};


}}   // namespace

#endif

