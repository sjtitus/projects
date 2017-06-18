#ifndef _COMMANDMESSAGEHANDLER_H_
#define _COMMANDMESSAGEHANDLER_H_

#include <memory>
#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include "MessageHandler.hpp"

namespace com { namespace dimension3designs {

class CommandMessageHandler: public MessageHandler
{
    public:
        typedef boost::shared_ptr<CommandMessageHandler> Ptr;
        virtual void HandleRead( std::unique_ptr<std::string> pMessage );
        virtual void HandleWrite( size_t bytesWritten );
        virtual void HandleReadError( const boost::system::error_code& error );
        virtual void HandleWriteError( const boost::system::error_code& error );
        virtual void Start();
};

}}

#endif

