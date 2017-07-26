/*_____________________________________________________________________________
 *  LocalSocketSessionHandler
 *  Wires a LocalSocketSession to an application-specific MessageHandler via
 *  templating.
 * ____________________________________________________________________________
*/

#ifndef _LOCALSOCKETSESSIONHANDLER_H_
#define _LOCALSOCKETSESSIONHANDLER_H_

#include <boost/shared_ptr.hpp>
#include <boost/bind.hpp>
#include <type_traits>
#include <boost/asio.hpp>
#include <string>
#include <memory>

#include "MessageHandler.hpp"
#include "Logging.hpp"


// use local (unix domain) sockets
using boost::asio::local::stream_protocol;

namespace com { namespace dimension3designs {

//__________________________________________________________________________
// LocalSocketSessionHandler: socket session handling.
// Wires up the session to an application-specific MessageHandler via
// templating.
// 
template<typename T>
class LocalSocketSessionHandler
{
    public:
        LocalSocketSessionHandler()
        {
            BOOST_STATIC_ASSERT((std::is_base_of<MessageHandler, T>::value));
            LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler: Construct");
        }

        void HandleSession( LocalSocketSession::Ptr &pSession )
        {
            LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler::HandleSession session " << pSession.get());
            
            // ________________________________________________________________________________________
            // IMPORTANT
            // Create shared_ptr codependency: MessageHandler holds Session and
            // Session holds MessaageHandler. The way the duo is released from mem
            // is when a MessageHandler does a session pointer reset, which triggers session
            // deletion, and subsequent MessageHandler deletion.
            // ________________________________________________________________________________________
           
            // create a new message handler that will be only reference to the session 
            MessageHandler::Ptr pHandler(new T(pSession));
            // set the session's message handler
            pSession->SetMessageHandler(pHandler);
            LOG4CXX_DEBUG(logger_,"LocalSocketSessionHandler::HandleSession session " << pSession.get() << ": starting message handler");
            pHandler->Start();
        }

        // logging
        static log4cxx::LoggerPtr  logger_;
};

template<typename T>
log4cxx::LoggerPtr LocalSocketSessionHandler<T>::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketSession"));

}}  // namespace

#endif
