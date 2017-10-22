#ifndef _LOCALSOCKETSERVER_H_
#define _LOCALSOCKETSERVER_H_
/*_____________________________________________________________________________
 * 
 * LocalSocketServer
 * Server that asynchronously handles communications for a specified Unix
 * domain socket. The server establishes a LocalSocketSession for each
 * incoming connection.
 *
 * Uses boost::asio  
 *_____________________________________________________________________________
*/

#include <cstdio>
#include <unordered_map>
#include <iostream>
#include <boost/asio.hpp>   

#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
//#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   

#include "Logging.hpp"
#include "LocalSocketSession.hpp"
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
            LOG4CXX_DEBUG(logger_,"LocalSocketServer::Start: Asynchronously accepting connections"); 
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
            LOG4CXX_TRACE(logger_,"LocalSocketServer::AcceptHandler assigning handler and starting session=" << new_session.get());
           
            // Create a new message handler for this session: it will use the raw session pointer for reading/writing
            LocalSocketSession *pSession = new_session.get(); 
            std::unique_ptr<MessageHandler> pMessageHandler(new T(pSession));
            
            // Wire it up to the session: session owns the handler
            new_session->SetMessageHandler(pMessageHandler);         
 
            // Save the session in the hash of active sessions: the server owns the sessions
            const void *address = static_cast<const void*>(new_session.get());
            std::stringstream ss;
            ss << address;  
            std::string id = ss.str();
            LOG4CXX_TRACE(logger_,"LocalSocketServer::AcceptHandler saving active session " << id << " to active connection hash"); 
            auto iresult = activeConnectionHash_.insert(std::make_pair(id,new_session));
            if (!iresult.second)
            {
                // non-unique id: abort
                RAISE_EXCEPTION("LocalSocketServer::AcceptHandler: failed saving active session "+id+" (non-unique id?)");
            }

            // Start asynch IO on the session
            new_session->Start();

            // Now set up for the next connection: reset the smart pointer with a new session 
            // for the next incoming connection.
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
   
    // Active connections 
    std::unordered_map<std::string, boost::shared_ptr<LocalSocketSession>>  activeConnectionHash_;
    std::string file_;
    
  private:    
    // logging
    static log4cxx::LoggerPtr  logger_;
};

template<typename T>
log4cxx::LoggerPtr LocalSocketServer<T>::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketServer"));


}}

#endif

