
#include "MessageHandler.hpp"

class CommandMessageHandler: public MessageHandler
{
    public:
        typedef boost::shared_ptr<CommandMessageHandler> Ptr;

        virtual void HandleRead( std::unique_ptr<std::string> pMessage )
        {
        }
        
        virtual void HandleWrite( size_t bytesWritten )
        {
        }
        
        virtual void HandleReadError( const boost::system::error_code& error )
        {
        }
        
        virtual void HandleWriteError( const boost::system::error_code& error )
        {
        }
        
        virtual void Start()
        {
        }
};
