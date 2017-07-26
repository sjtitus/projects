#ifndef _LOCALSOCKETSERVER_H_
#define _LOCALSOCKETSERVER_H_
/*_____________________________________________________________________________
 * 
 * LocalSocketServer
 * Server that asynchronously handles communications for a specified Unix
 * domain socket. The server establishes a LocalSocketSession for each
 * incoming connection, and then passes ownership of the connected
 * session to a specified LocalSocketSessionHandler, which handles
 * message traffic for the session.
 *
 * Uses boost::asio  
 *_____________________________________________________________________________
*/

#include <cstdio>
#include <iostream>
#include <boost/asio.hpp>   

#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   

#include "Logging.hpp"
#include "LocalSocketSession.hpp"
#include "LocalSocketSessionHandler.hpp"
#include "SimpleException.hpp"


using boost::asio::local::stream_protocol;

namespace com { namespace dimension3designs {
    
//_____________________________________________________________________________
// LocalSocketServer
template<typename T>
class LocalSocketServer
{
  public:
    
    //_________________________________________________________________________
    // Constructor
    LocalSocketServer(boost::asio::io_service& io_service, 
            const std::string& file)
    : io_service_(io_service),
      acceptor_(io_service, stream_protocol::endpoint(file)),
      file_(file)
    {
        LOG4CXX_TRACE(logger_,"LocalSocketServer: Construct: file="<< file_);
    }

    //_________________________________________________________________________
    // Start: start accepting connections 
    void Start()
    {
        LOG4CXX_TRACE(logger_,"LocalSocketServer::Start: file="<<file_); 
        try
        {
            LOG4CXX_DEBUG(logger_,"LocalSocketServer::Start: Creating/binding session"); 
            
            // create a session 
            LocalSocketSession::Ptr new_session(new LocalSocketSession(io_service_));

            // asynch accept of a connection using the new session's
            // socket (callback = AcceptHandler method)
            acceptor_.async_accept(
                new_session->Socket(),
                boost::bind(
                    &LocalSocketServer::AcceptHandler, 
                    this, 
                    new_session,
                    boost::asio::placeholders::error
                )
            );
        }
        catch(std::exception &e) 
        {
            LOG4CXX_ERROR(logger_,"LocalSocketServer::Start: exception: "<<e.what()); 
            RAISE_EXCEPTION(std::string("error starting LocalSocketServer: ")+e.what());
        }
    }


    
    //_________________________________________________________________________
    // Stop: stop accepting connections
    void Stop()
    {
        LOG4CXX_TRACE(logger_,"LocalSocketServer::Stop"); 
        // TODO: implement
    }


  private:
    //_________________________________________________________________________
    // AcceptHandler: async callback handling new incoming sessions. 
    void AcceptHandler(LocalSocketSession::Ptr &new_session,
            const boost::system::error_code& error)
    {
        LOG4CXX_TRACE(logger_,"LocalSocketServer::AcceptHandler"); 
        if (error)
        {
            LOG4CXX_ERROR(logger_,"LocalSocketServer::AcceptHandler: error handling incoming connection: " << error.message()); 
            RAISE_EXCEPTION(std::string("error handling incoming connection: ") + error.message());
        }

        try
        {
            LOG4CXX_TRACE(logger_,"LocalSocketServer::AcceptHandler calling handler for session=" << new_session.get());
            // Hand the newly-connected session off to the handler 
            sessionHandler_.HandleSession(new_session);
           
            // Setup for the next connection: reset the smart pointer with a new session for next
            // incoming connection.
            LOG4CXX_DEBUG(logger_,"LocalSocketServer::AcceptHandler: resetting for next connection"); 
            new_session.reset(new LocalSocketSession(io_service_));
     
            // Accept a new connection 
            acceptor_.async_accept(
                new_session->Socket(),
                boost::bind(
                    &LocalSocketServer::AcceptHandler, 
                    this, 
                    new_session,
                    boost::asio::placeholders::error
                )
            );
        }
        catch(std::exception &e) 
        {
            LOG4CXX_ERROR(logger_,"LocalSocketServer::AcceptHandler: exception: "<<e.what()); 
            RAISE_EXCEPTION(std::string("error resetting for next connection: ")+e.what());
        }
    }


  private:
    boost::asio::io_service& io_service_;
    stream_protocol::acceptor acceptor_;
    LocalSocketSessionHandler<T> sessionHandler_;
    std::string file_;
    
  private:    
    // logging
    static log4cxx::LoggerPtr  logger_;
};

template<typename T>
log4cxx::LoggerPtr LocalSocketServer<T>::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketServer"));


}}

#endif

