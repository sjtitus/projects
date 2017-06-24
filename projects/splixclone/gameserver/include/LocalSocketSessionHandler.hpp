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

template<typename T>
class LocalSocketSessionHandler
{
    public:
        LocalSocketSessionHandler(LocalSocketSession::Ptr &pSession)
            :pSession_(pSession)
        {
            BOOST_STATIC_ASSERT((std::is_base_of<MessageHandler, T>::value));
            L_INFO << "LocalSocketSessionHandler: construct with session " << pSession_.get();
        }

        void HandleSession( LocalSocketSession::Ptr &pSession )
        {
            // Create shared_ptr codependency: MessageHandler holds Session and
            // Session holds MessaageHandler. The way the duo is released from mem
            // is when a MessageHandler does a session pointer reset, which triggers session
            // deletion, and subsequent MessageHandler deletion.
            LocalSocketSession::MessageHandler::Ptr pHandler(new T(pSession));
            pSession->SetMessageHandler(pHandler);
            pHandler->Start();
        }

    private:
        LocalSocketSession::Ptr pSession_;
};

}}  // namespace

#endif
