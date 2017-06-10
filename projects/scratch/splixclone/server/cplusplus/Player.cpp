#include "Player.hpp" 

    // Constructor 
    Player::Player( std::size_t pid, LocalSocketSession::Ptr pSession )
      : pid_(pid), 
        pSession_(pSession)
    {
        L_DEBUG << "Player " << pid_ << "(session " << pSession_.get() << "): create"; 
    }
   
    // Desructor 
    Player::~Player()
    {
        L_DEBUG << "Player " << pid_ << "(session " << pSession_.get() << "): delete (use count=" << pSession_.use_count() << ")"; 
    }
 
    // HandleRead (MessageHandler override)
    // Callback invoked on incoming player messages 
    void Player::HandleRead( Message::MessagePtr pMessage )
    {
        L_DEBUG << "Player " << pid_ << "(session " << pSession_.get() << "): message received";
        pMessage->Print();

        // chain the next read 
        pSession_->ReadMessage();
    }
    
    // HandleWrite (MessageHandler override)
    // Callback invoked when outgoing message written to socket 
    void Player::HandleWrite( size_t bytesWritten ) 
    {
        L_INFO << "Player " << pid_ << ": " << bytesWritten << " bytes written to session " << pSession_.get();
    }

    // HandleReadError (MessageHandler override)
    // Callback invoked when an error occurs on incoming read from socket 
    void Player::HandleReadError( const boost::system::error_code& error )
    {
        L_ERROR << "Player " << pid_ << ": got a read error: " << error.message(); 
    }
    
    // HandleWriteError (MessageHandler override)
    // Callback invoked when an error occurs on outgoing write to socket 
    void Player::HandleWriteError( const boost::system::error_code& error )
    {
        L_ERROR << "Player " << pid_ << ": got a write error: " << error.message(); 
    }

    // Start
    // Start player communications by reading the socket for the player's first action 
    void Player::Start()
    {
        pSession_->ReadMessage();
    }
