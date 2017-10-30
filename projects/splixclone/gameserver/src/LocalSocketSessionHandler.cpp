/*_____________________________________________________________________________
 *  LocalSocketSessionHandler
 *  Wires a LocalSocketSession to an application-specific MessageHandler via
 *  templating.
 * ____________________________________________________________________________
*/

#include "LocalSocketSessionHandler.hpp"
#include "Logging.hpp"
#include "SimpleException.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;

namespace com { namespace dimension3designs {

// Logging
log4cxx::LoggerPtr LocalSocketSessionHandler::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketSession"));

//_____________________________________________________________________________ 
// Constructor 
LocalSocketSessionHandler::LocalSocketSessionHandler()
{
    LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler: Construct");
}
       

//_____________________________________________________________________________
// SaveSession
// Save a session in the session hash 
void LocalSocketSessionHandler::SaveSession(LocalSocketSession::Ptr& session)
{
    std::string session_id = session->Id();
    LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler::SaveSession: saving session " << session_id); 
    auto iresult = _sessionHash.insert(std::make_pair(session_id,session));
    if (!iresult.second)
    {
        RAISE_EXCEPTION("LocalSocketSessionHandler::SaveSession: failed to save session "+session_id);
    }
}
        
//_____________________________________________________________________________
// GetSession
// Get a session from the session hash 
LocalSocketSession::Ptr LocalSocketSessionHandler::GetSession(const std::string &sessionId)
{
    LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler::GetSession: getting " << sessionId); 
    auto elem = _sessionHash.find(sessionId);
    if (elem != _sessionHash.end()) {
        LOG4CXX_DEBUG(logger_,"LocalSocketSessionHandler::GetSession: found " << elem->second->Id());
        return elem->second;
    }
    else {
        RAISE_EXCEPTION("LocalSocketSessionHandler::GetSession: failed getting session " + sessionId);
        return LocalSocketSession::Ptr(NULL);
    }
}


//_____________________________________________________________________________
// DeleteSession
// Delete a session from the session hash 
void LocalSocketSessionHandler::DeleteSession(const std::string &sessionId)
{
    LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler::DeleteSession " << sessionId);
    std::size_t n = _sessionHash.erase(sessionId);
    LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler::DeleteSession: items deleted: " << n);
}


}}  // namespace

