/*
 * _____________________________________________________________________________
 *
 * PlayerMessageHandler
 * Handle game server player messages (e.g. create player, etc).
 * The handler owns a socket session and provides the callbacks
 * associated with async session communications.
 *
 * PlayerSessionHandler
 * Handle game server player sessions that are created by a 
 * LocalSocketServer. The PlayerSessionHandler spawns a PlayerMessageHandler
 * to oversee the session.
 * _____________________________________________________________________________
 *
*/
#ifndef _PLAYERMESSAGEHANDLER_H_
#define _PLAYERMESSAGEHANDLER_H_

#include "LocalSocketSession.hpp"
#include "LocalSocketServer.hpp"
#include <boost/system/system_error.hpp> 

class PlayerMessageHandler: public LocalSocketSession::MessageHandler
{
    public:
        //_____________________________________________________________________
        // Constructor / Destructor 
        PlayerMessageHandler(LocalSocketSession::Ptr &pSession);
        ~PlayerMessageHandler();

        //_____________________________________________________________________
        // Start
        void Start();
    
        //_____________________________________________________________________
        // Stop 
        void Stop();
        
        //_____________________________________________________________________
        // HandleRead 
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
// PlayerSessionHandler
// Handle an incoming socket sessions by handing it off to newly-created
// PlayerMessageHandler and then starting socket communication activity. 
class PlayerSessionHandler: public LocalSocketSessionHandler
{
    public:
        void HandleSession( LocalSocketSession::Ptr &pSession );
};

#endif
