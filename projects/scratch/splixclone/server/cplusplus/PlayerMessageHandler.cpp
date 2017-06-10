#include "PlayerMessageHandler.hpp"
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
PlayerMessageHandler::PlayerMessageHandler(LocalSocketSession::Ptr &pSession)
    :pSession_(pSession)
{
    L_INFO << "PlayerMessageHandler: construct";
}
        
PlayerMessageHandler::~PlayerMessageHandler()
{
    L_INFO << "PlayerMessageHandler: destruct";
}

        
//_____________________________________________________________________
// Start
// Start handling commands (reading from socket session) 
void PlayerMessageHandler::Start()
{
    L_INFO << "PlayerMessageHandler: start";
    pSession_->ReadMessage();
}
    
//_____________________________________________________________________
// Stop 
// Stop handling commands (close the socket session and reset the
// session pointer).
// NOTE: Using the SessionHandler/MessageHandler pattern below,
// invoking this function will result in deletion of the
// MessageHandler.  
void PlayerMessageHandler::Stop()
{
    L_INFO << "PlayerMessageHandler: stop";
    pSession_->Close();
    pSession_.reset();
}

//_____________________________________________________________________
// HandleRead 
// Handle an incoming message by executing the appropriate
// command and writing a response.  
void PlayerMessageHandler::HandleRead( std::unique_ptr<std::string> pMessage )
{
    boost::thread::id tid = boost::this_thread::get_id();
    L_INFO << "PlayerMessageHandler [" << tid << "]: handle read";
    pSession_->WriteMessage("10-4 from player handler!"); 
}

//_____________________________________________________________________
// HandleWrite
// Invoked after an async write is complete.
void PlayerMessageHandler::HandleWrite( size_t bytesWritten )
{
    L_INFO << "PlayerMessageHandler: handlewrite: " << bytesWritten << " bytes"; 
    pSession_->ReadMessage();
}

//_____________________________________________________________________
// HandleReadError/HandleWriteError 
// Handle errors occurring during read/write. 
void PlayerMessageHandler::HandleReadError( const boost::system::error_code& error )
{
    L_INFO << "PlayerMessageHandler: handlereaderror: " << error.message(); 
    Stop(); 
}

void PlayerMessageHandler::HandleWriteError( const boost::system::error_code& error )
{
    L_INFO << "PlayerMessageHandler: handlewriteerror: " << error.message(); 
    Stop(); 
}


//_____________________________________________________________________________
// PlayerSessionHandler
// Handle an incoming socket sessions by handing it off to newly-created
// PlayerMessageHandler and then starting socket communication activity. 
void PlayerSessionHandler::HandleSession( LocalSocketSession::Ptr &pSession )
{
    //
    // IMPORTANT
    // 
    // Note the shared_ptr circular dependency: MessageHandler holds
    // the session uses the MessaageHandler. The dependency is
    // cleaned up when the MessageHandler does a session pointer reset 
    // via its Stop() method -- this triggers session deletion, and 
    // subsequently MessageHandler deletion.
    //
    LocalSocketSession::MessageHandler::Ptr pHandler(new PlayerMessageHandler(pSession));
    pSession->SetMessageHandler(pHandler);  
    pHandler->Start(); 
}

