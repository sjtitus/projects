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
        CommandMessageHandler(LocalSocketSession *pSession);
        ~CommandMessageHandler();
        void HandleRead( std::unique_ptr<std::string> pMessage ) override;
        void HandleWrite( size_t bytesWritten ) override;
        void HandleReadError( const boost::system::error_code& error ) override;
        void HandleWriteError( const boost::system::error_code& error ) override;
        void Start() override;
        void Close() override;
};

}}

#endif

