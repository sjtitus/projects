/*_____________________________________________________________________________
 * 
 * LocalSocketServer
 * Server that asynchronously handles connections to a specified Unix
 * domain socket endpoint (file). When a connection is made, the server
 * creates a LocalSocketSession with a specified handler for incoming
 * messages that is invoked asynchronously as messages arrive. 
 *_____________________________________________________________________________
*/

#include "Logger.hpp"
#include "Exception.hpp"
#include "LocalSocketSession.hpp"
#include "LocalSocketServer.hpp"
#include "Message.hpp"

#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   

//_________________________________________________________________________
// Constructor
LocalSocketServer::LocalSocketServer(
            boost::asio::io_service& io_service, 
            const std::string& file, 
            LocalSocketSessionHandler &sessionHandler)
    : io_service_(io_service),
      acceptor_(io_service, stream_protocol::endpoint(file)),
      sessionHandler_(sessionHandler),
      file_(file)
{
    L_DEBUG << "construct (file " << file_ << ")";
}


//_________________________________________________________________________
// Stop: stop accepting connections and gracefully shut down? 
void LocalSocketServer::Stop()
{
    // TODO: implement
}


//_________________________________________________________________________
// Start: start accepting connections 
void LocalSocketServer::Start()
{
    L_INFO << "starting (accepting connections on " << file_ << ")";
    try
    {
        // create a session 
        LocalSocketSession::Ptr new_session(
            new LocalSocketSession(io_service_)
        );

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
        RETHROW_SYSTEM_EXCEPTION(e); 
    }
}


//_________________________________________________________________________
// AcceptHandler: handle an incoming session resulting from an
// accepted client connection. 
void LocalSocketServer::AcceptHandler(LocalSocketSession::Ptr &new_session,
        const boost::system::error_code& error)
{
    L_DEBUG << "handling incoming connection";
    if (error)
    {
        L_ERROR << "handling incoming connection: " << error.message();
        THROW_SYSTEM_EXCEPTION("ERROR handling incoming connection: " + error.message());
    }

    try
    {
        // Hand the newly-connected session off to the handler 
        sessionHandler_.HandleSession(new_session);
       
        // Setup for the next connection: reset the smart pointer with
        // a new session
        L_DEBUG << "resetting for next connection";
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
        RETHROW_SYSTEM_EXCEPTION(e); 
    }
}




#ifdef _UNIT_TEST_LOCALSOCKETSERVER_
 
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp> 
#include "CircularMessageBuffer.hpp"


class CommandMessageHandler: public LocalSocketSession::MessageHandler
{
    public:

        CommandMessageHandler(LocalSocketSession::Ptr &pSession)
            :pSession_(pSession)
        {
            L_INFO << "CommandMessageHandler: construct with session " << pSession_.get();
        }
        
        ~CommandMessageHandler()
        {
            L_INFO << "CommandMessageHandler: destruct " << this; 
        }

        long UseCount() { return pSession_.use_count(); }
        
        void Start()
        {
            L_INFO << "CommandMessageHandler: start: session use count=" << pSession_.use_count() << ", unique=" << pSession_.unique();
            pSession_->ReadMessage();
        } 
        
        void HandleRead( std::unique_ptr<std::string> pMessage )
        {
            L_INFO << "CommandMessageHandler: handleread: message='" << *pMessage << "'";
            L_INFO << "HandleRead: Session pointer use count: " << pSession_.use_count() << " for " << pSession_.get();
            pSession_->WriteMessage("10-4 good buddy!"); 
            L_INFO << "HandleRead: closing socket";
            pSession_->Close();
            pSession_.reset();
        }
        
        void HandleWrite( size_t bytesWritten )
        {
            L_INFO << "CommandMessageHandler: handlewrite: " << bytesWritten << " bytes"; 
        }
        
        void HandleReadError( const boost::system::error_code& error )
        {
            L_INFO << "CommandMessageHandler: handlereaderror: " << error.message(); 
        }
        
        void HandleWriteError( const boost::system::error_code& error )
        {
            L_INFO << "CommandMessageHandler: handlewriteerror: " << error.message(); 
        }

    protected:
        LocalSocketSession::Ptr pSession_;
};



class CommandSessionHandler: public LocalSocketSessionHandler
{
    public:

        void HandleSession( LocalSocketSession::Ptr &pSession )
        {
            // Create shared_ptr codependency: MessageHandler holds Session and
            // Session holds MessaageHandler. The way the duo is released from mem
            // is when a MessageHandler does a session pointer reset, which triggers session 
            // deletion, and subsequent MessageHandler deletion. 
            LocalSocketSession::MessageHandler::Ptr pHandler(new CommandMessageHandler(pSession));
            pSession->SetMessageHandler(pHandler);  
            pHandler->Start(); 
        }
};




int main(int argc, const char **argv)
{

  const char *COMMAND_ENDPOINT = "/tmp/foo";
  boost::asio::io_service io_service;
  
  unlink(COMMAND_ENDPOINT);

  try
  {
    boost::thread::id id = boost::this_thread::get_id();
    L_INFO << "Server: Main thread (" << id << ") started";
    
    L_INFO << "Main thread (" << id << ") starting command server";
    CommandSessionHandler commandSessionHandler;
    const std::string ce(COMMAND_ENDPOINT);
    
    LocalSocketServer CommandServer(
        io_service, 
        ce, 
        commandSessionHandler
    );
 
    CommandServer.Start();
   
    L_INFO << "calling io_service::run";
    try 
    {
        io_service.run();
    }
    catch (SystemException& e)
    {
        L_FATAL <<  "IO SERVICE: " << e.Info() << "\n";
    }
    L_INFO << "exiting";

  }
  catch (SystemException& e)
  {
    L_FATAL <<  e.Info() << "\n";
  }
  catch (DomainException& e)
  {
    L_FATAL <<  e.Info() << "\n";
  }

  L_INFO << "exiting with status 0";
  return 0;
}


#endif
