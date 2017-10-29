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

        MessageHandler(LocalSocketSession *pSession)
        :_pSession(pSession) 
        {
        }
        
        virtual ~MessageHandler()
        {
            // Note: the session always owns the message handler, and the handler
            // will not be destroyed until the session is. Since session is
            // destroyed elsewhere, we need not free it here.            
            LOG4CXX_TRACE(_logger,"MessageHandler::destructor: NULLing (not freeing) session " << _pSession);
            _pSession = NULL;
        }

        virtual void HandleRead( std::unique_ptr<std::string> pMessage ) = 0;
        virtual void HandleWrite( size_t bytesWritten ) = 0;
        virtual void HandleReadError( const boost::system::error_code& error ) = 0;
        virtual void HandleWriteError( const boost::system::error_code& error ) = 0;
        virtual void Start() = 0;
        virtual void Close() = 0; 
    
    protected:
        // the session being used for I/O 
        LocalSocketSession *_pSession;
    
    public: 
        // logging
        static log4cxx::LoggerPtr  _logger;
};

}}


#endif
