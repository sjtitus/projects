
#include "CommandMessageHandler.hpp"

namespace com { namespace dimension3designs {


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

}}
