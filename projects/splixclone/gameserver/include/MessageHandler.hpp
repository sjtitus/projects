#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

/*_____________________________________________________________________________
 * 
 *  MessageHandler 
 *  
 *  Abstract class for handling reading/writing of messages over a specified
 *  LocalSocketSession. 
 *_____________________________________________________________________________
 */ 

#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <memory>
#include "Logging.hpp"


namespace com { namespace dimension3designs {

class LocalSocketSession;

class MessageHandler
{
    public:

        MessageHandler(LocalSocketSession *pSession):_pSession(pSession) {}
        
        virtual ~MessageHandler()
        {
            LOG4CXX_TRACE(_logger,"MessageHandler::destructor: NULLing (not freeing) session " << _pSession);
            _pSession = NULL;
        }

        //_______________________________________________________________
        // Abstract interface for handling session read/writes.
        // The 'Handle*' functions are invoked as asynch callbacks by
        // the session when reads/writes occur. Start is used to initiate the 
        // asynch IO chain; Close to end communications.
        virtual void HandleRead( std::unique_ptr<std::string> pMessage ) = 0;
        virtual void HandleWrite( size_t bytesWritten ) = 0;
        virtual void HandleReadError( const boost::system::error_code& error ) = 0;
        virtual void HandleWriteError( const boost::system::error_code& error ) = 0;
        virtual void Start() = 0;
        virtual void Close() = 0; 
    
    protected:
        // the session used for I/O
        // Note: raw pointer is used b/c the session owns the handler instance
        LocalSocketSession *_pSession;
    
    public: 
        // logging
        static log4cxx::LoggerPtr  _logger;
};

}}


#endif
