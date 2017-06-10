
#ifndef _PLAYER_H_
#define _PLAYER_H_

#include "Logger.hpp"
#include "Exception.hpp"
#include "LocalSocketSession.hpp"
#include "LocalSocketServer.hpp"
#include "Message.hpp"

#include <cstdio>
#include <iostream>
#include <boost/shared_ptr.hpp>

//_____________________________________________________________________________
// Player
// Encapsulate a game player. Managers all player network communication via
// a LocalSocketSession. 
class Player: public LocalSocketSession::MessageHandler
{

  public:
    
    typedef boost::shared_ptr<Player> Ptr;
   
    // Constructor 
    Player( std::size_t pid, LocalSocketSession::Ptr pSession );
   
    // Desructor 
    virtual ~Player();
 
    // HandleRead (MessageHandler override)
    // Callback invoked on incoming player messages 
    void HandleRead( Message::MessagePtr pMessage );
    
    // HandleWrite (MessageHandler override)
    // Callback invoked when outgoing message written to socket 
    void HandleWrite( size_t bytesWritten );

    // HandleReadError (MessageHandler override)
    // Callback invoked when an error occurs on incoming read from socket 
    void HandleReadError( const boost::system::error_code& error );
    
    // HandleWriteError (MessageHandler override)
    // Callback invoked when an error occurs on outgoing write to socket 
    void HandleWriteError( const boost::system::error_code& error );

    // Start
    // Start player communications by reading the socket for the player's first action 
    void Start();

  private:
    std::size_t pid_;                   // player ID
    LocalSocketSession::Ptr pSession_;  // socket session for comms
};

#endif

