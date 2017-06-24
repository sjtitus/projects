#ifndef _MESSAGE_HANDLER_H_
#define _MESSAGE_HANDLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/system/error_code.hpp>
#include <string>
#include <memory>

/*_____________________________________________________________________________
 * MessageHandler 
 * Abstract class for handling mesaage reading/writing.
 *
 * SEE ALSO: LocalSocketSession, which uses MessageHandler for asynch
 * message handling.
 *_____________________________________________________________________________
 */ 


namespace com { namespace dimension3designs {

class LocalSocketSession;

class MessageHandler
{
    public:

        MessageHandler(boost::shared_ptr<LocalSocketSession> pSession)
        :pSession_(pSession) {}

        typedef boost::shared_ptr<MessageHandler> Ptr;
        
        virtual void HandleRead( std::unique_ptr<std::string> pMessage ) = 0;
        virtual void HandleWrite( size_t bytesWritten ) = 0;
        virtual void HandleReadError( const boost::system::error_code& error ) = 0;
        virtual void HandleWriteError( const boost::system::error_code& error ) = 0;
        virtual void Start()=0;
    
    private:
        boost::shared_ptr<LocalSocketSession> pSession_;
};

}}



#endif
