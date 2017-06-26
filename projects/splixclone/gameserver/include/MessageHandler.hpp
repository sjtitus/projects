#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <memory>
#include "Logging.hpp"

/*_____________________________________________________________________________
 * MessageHandler 
 * Abstract class for handling reading/writing of messages over a specified
 * LocalSocketSession. 
 *_____________________________________________________________________________
 */ 


namespace com { namespace dimension3designs {

class LocalSocketSession;

class MessageHandler
{
    public:

        MessageHandler(boost::shared_ptr<LocalSocketSession> &pSession)
        :pSession_(pSession) 
        {
        }
        
        ~MessageHandler()
        {
            LOG4CXX_TRACE(logger_,"MessageHandler::destructor: session " << pSession_.get());
        }

        typedef boost::shared_ptr<MessageHandler> Ptr;
        
        virtual void HandleRead( std::unique_ptr<std::string> pMessage ) = 0;
        virtual void HandleWrite( size_t bytesWritten ) = 0;
        virtual void HandleReadError( const boost::system::error_code& error ) = 0;
        virtual void HandleWriteError( const boost::system::error_code& error ) = 0;
        virtual void Start() = 0;
        virtual void Close() = 0; 
    
    protected:
        // the session being used for I/O 
        boost::shared_ptr<LocalSocketSession> pSession_;
    
    public: 
        // logging
        static log4cxx::LoggerPtr  logger_;
};

}}



#endif
