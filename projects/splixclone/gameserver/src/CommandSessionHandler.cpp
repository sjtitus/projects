/*_____________________________________________________________________________
 *  CommandSessionHandler 
 * ____________________________________________________________________________
*/

#include "CommandSessionHandler.hpp"
#include "CommandMessageHandler.hpp"
#include "Logging.hpp"


namespace com { namespace dimension3designs {


CommandSessionHandler::CommandSessionHandler()
{
    LOG4CXX_TRACE(logger_,"CommandSessionHandler: Construct");
}

void CommandSessionHandler::HandleSession( LocalSocketSession::Ptr &pSession )
{
    LOG4CXX_TRACE(logger_,"CommandSessionHandler::HandleSession session " << pSession.get());
    std::unique_ptr<MessageHandler> pHandler(new CommandMessageHandler(pSession.get()));
    LOG4CXX_TRACE(logger_,"CommandSessionHandler::HandleSessions: binding new command message handler " << pHandler.get());
    pSession->SetMessageHandler(pHandler);
}


}}  // namespace

