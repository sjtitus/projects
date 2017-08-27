
#include "LocalSocketSession.hpp"
#include "CommandMessageHandler.hpp"

namespace com { namespace dimension3designs {
        
CommandMessageHandler::CommandMessageHandler(boost::shared_ptr<LocalSocketSession> &pSession)
    :MessageHandler(pSession)
{
    LOG4CXX_TRACE(logger_,"CommandMessageHandler: construct: session " << pSession_.get());
}

void CommandMessageHandler::HandleRead( std::unique_ptr<std::string> pMessage )
{
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
}
        
void CommandMessageHandler::Close()
{ 
    pSession_->Close(); 
    pSession_.reset(); 
}

}}
