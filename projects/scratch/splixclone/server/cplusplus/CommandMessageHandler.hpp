/*
 * _____________________________________________________________________________
 *
 * CommandMessageHandler
 * Handle game server command messages (e.g. create player, etc).
 * The handler owns a socket session and manages message traffic 
 * for that session. 
 *
 * CommandSessionHandler
 * Handle game server command sessions that are created by a 
 * LocalSocketServer. The CommandSessionHandler spawns a CommandMessageHandler
 * to oversee the session.
 * _____________________________________________________________________________
 *
*/
#ifndef _COMMANDMESSAGEHANDLER_H_
#define _COMMANDMESSAGEHANDLER_H_

#include "LocalSocketSession.hpp"
#include "LocalSocketServer.hpp"
#include <boost/system/system_error.hpp> 

class CommandMessageHandler: public LocalSocketSession::MessageHandler
{
    public:

        //_____________________________________________________________________
        // Constructor / Destructor 
        CommandMessageHandler(LocalSocketSession::Ptr &pSession);
        ~CommandMessageHandler();

        //_____________________________________________________________________
        // Start
        // Start handling commands (reading from socket session) 
        void Start();
    
        //_____________________________________________________________________
        // Stop 
        // Stop handling commands (close the socket session and reset the
        // session pointer).
        // NOTE: Using the SessionHandler/MessageHandler pattern below,
        // invoking this function will result in deletion of the
        // MessageHandler.  
        void Stop();
        
        //_____________________________________________________________________
        // HandleRead 
        // Handle an incoming message by executing the appropriate
        // command and writing a response.  
        void HandleRead( std::unique_ptr<std::string> pMessage );
        
        //_____________________________________________________________________
        // HandleWrite
        // Invoked after an async write is complete.
        void HandleWrite( size_t bytesWritten );
        
        //_____________________________________________________________________
        // HandleReadError/HandleWriteError 
        // Handle errors occurring during read/write. 
        void HandleReadError( const boost::system::error_code& error );
        void HandleWriteError( const boost::system::error_code& error );

    protected:
        LocalSocketSession::Ptr pSession_;
};



//_____________________________________________________________________________
// CommandSessionHandler
// Handle an incoming socket session by handing it off to newly-created
// CommandMessageHandler and then starting socket communication activity. 
class CommandSessionHandler: public LocalSocketSessionHandler
{
    public:
        void HandleSession( LocalSocketSession::Ptr &pSession );
};

#endif
