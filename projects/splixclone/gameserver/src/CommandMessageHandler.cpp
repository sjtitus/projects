
#include "LocalSocketSession.hpp"
#include "CommandMessageHandler.hpp"
#include "Game.hpp"

namespace com { namespace dimension3designs {
       
// Initialize static
Game*   CommandMessageHandler::_c_pGame = NULL;  
        
CommandMessageHandler::CommandMessageHandler(LocalSocketSession *pSession)
    :MessageHandler(pSession)
{
    LOG4CXX_TRACE(logger_,"CommandMessageHandler: construct: session " << pSession_);
    LOG4CXX_TRACE(logger_,"CommandMessageHandler: construct: game " << _c_pGame);
}


//______________________________________________________________________________
// HandleRead
// Read an incoming command message and push it onto the game's command buffer
// for processing. 
void CommandMessageHandler::HandleRead( std::unique_ptr<std::string> pMessage )
{
    LOG4CXX_TRACE(logger_,"CommandMessageHandler::HandleRead: session " << pSession_);
    LOG4CXX_TRACE(logger_,"CommandMessageHandler::HandleRead: message: " << *pMessage); 
    LOG4CXX_TRACE(logger_,"CommandMessageHandler::HandleRead: pushing to command buffer"); 
    //bool pushed = _c_pGame->CommandBuffer().PushBack(*pMessage);
    _c_pGame->CommandBuffer().PushBack(std::move(pMessage));
    //LOG4CXX_TRACE(logger_,"CommandMessageHandler::HandleRead: message push failed: " << pushed); 
}

void CommandMessageHandler::HandleWrite( size_t bytesWritten )
{
}

void CommandMessageHandler::HandleReadError( const boost::system::error_code& error )
{
}

void CommandMessageHandler::HandleWriteError( const boost::system::error_code& error )
{
}

void CommandMessageHandler::Start()
{
    LOG4CXX_TRACE(logger_,"CommandMessageHandler::Start: session " << pSession_);
    pSession_->ReadMessage();
}
        
void CommandMessageHandler::Close()
{ 
    pSession_->Close(); 
}

}}
