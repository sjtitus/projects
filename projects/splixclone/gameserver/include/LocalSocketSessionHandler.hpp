/*_____________________________________________________________________________
 *  LocalSocketSessionHandler
 *  Wires a LocalSocketSession to an application-specific MessageHandler via
 *  templating.
 * ____________________________________________________________________________
*/

#ifndef _LOCALSOCKETSESSIONHANDLER_H_
#define _LOCALSOCKETSESSIONHANDLER_H_

#include <unordered_map>

#include "LocalSocketSession.hpp"
#include "Logging.hpp"


// use local (unix domain) sockets
using boost::asio::local::stream_protocol;

namespace com { namespace dimension3designs {

//__________________________________________________________________________
// LocalSocketSessionHandler: app specific socket session handling.
// 
class LocalSocketSessionHandler
{
    public:
        LocalSocketSessionHandler();
        virtual ~LocalSocketSessionHandler() {};
       
        // Abstract class 
        virtual void HandleSession( LocalSocketSession::Ptr &pSession ) = 0;
       
        // Session management 
        void SaveSession(LocalSocketSession::Ptr& session);
        LocalSocketSession::Ptr GetSession(const std::string &sessionId);
        void DeleteSession(const std::string &sessionId);

    protected:
        // Hash
        std::unordered_map<std::string, LocalSocketSession::Ptr>    _sessionHash;

        static log4cxx::LoggerPtr  logger_;     // logging
};

}}  // namespace

#endif
