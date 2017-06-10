#include "CommandMessageHandler.hpp"
#include "Logger.hpp"
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp> 


//_____________________________________________________________________
// Constructor / Destructor 
CommandMessageHandler::CommandMessageHandler(LocalSocketSession::Ptr &pSession)
    :pSession_(pSession)
{
    L_INFO << "CommandMessageHandler: construct";
}
        
CommandMessageHandler::~CommandMessageHandler()
{
    L_INFO << "CommandMessageHandler: destruct";
}

        
//_____________________________________________________________________
// Start
// Start handling commands (reading from socket session) 
void CommandMessageHandler::Start()
{
    L_INFO << "CommandMessageHandler: start";
    pSession_->ReadMessage();
}
    
//_____________________________________________________________________
// Stop 
// Stop handling commands (close the socket session and reset the
// session pointer).
// NOTE: Using the SessionHandler/MessageHandler pattern below,
// invoking this function will result in deletion of the
// MessageHandler.  
void CommandMessageHandler::Stop()
{
    L_INFO << "CommandMessageHandler: stop";
    pSession_->Close();
    pSession_.reset();
}

//_____________________________________________________________________
// HandleRead 
// Handle an incoming message by executing the appropriate
// command and writing a response.  
void CommandMessageHandler::HandleRead( std::unique_ptr<std::string> pMessage )
{
    boost::thread::id tid = boost::this_thread::get_id();
    L_INFO << "CommandMessageHandler [" << tid << "]: handle read";
    pSession_->WriteMessage("10-4 from command handler!"); 
    //
    // Parse the message into a server command object to validate that
    // it is well-formed.  If it's bad, send an error message back 
    // to the client. If it's ok, the queue the server command on
    // the circular queue for the game server thread to process. 
    //
    //pSession_->WriteMessage("10-4 good buddy!"); 
}

//_____________________________________________________________________
// HandleWrite
// Invoked after an async write is complete.
void CommandMessageHandler::HandleWrite( size_t bytesWritten )
{
    L_INFO << "CommandMessageHandler: handlewrite: " << bytesWritten << " bytes";
    // Read another message from the socket session 
    pSession_->ReadMessage();
}

//_____________________________________________________________________
// HandleReadError/HandleWriteError 
// Handle errors occurring during read/write. 
void CommandMessageHandler::HandleReadError( const boost::system::error_code& error )
{
    L_INFO << "CommandMessageHandler: handlereaderror: " << error.message();
    Stop(); 
}

void CommandMessageHandler::HandleWriteError( const boost::system::error_code& error )
{
    L_INFO << "CommandMessageHandler: handlewriteerror: " << error.message(); 
    Stop(); 
}




//_____________________________________________________________________________
// CommandSessionHandler
// Handle an incoming socket session by handing it off to newly-created
// CommandMessageHandler and then starting socket communication activity. 
void CommandSessionHandler::HandleSession( LocalSocketSession::Ptr &pSession )
{
    //
    // IMPORTANT
    // 
    // Note the shared_ptr circular dependency: MessageHandler holds
    // the session, and writes/reads to/from it. The session holds a reference to
    // the MessageHandler. The dependency is
    // cleaned up when the MessageHandler does a session pointer reset 
    // via its Stop() method -- this triggers session deletion, and 
    // subsequently MessageHandler deletion; i.e. the message handler kills
    // the session and thereby kills itself. 
    //
    LocalSocketSession::MessageHandler::Ptr pHandler(new CommandMessageHandler(pSession));
    pSession->SetMessageHandler(pHandler);  
    pHandler->Start(); 
}

