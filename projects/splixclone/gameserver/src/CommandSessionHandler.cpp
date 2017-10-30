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
    //long uc = pSession.use_count();
    LOG4CXX_TRACE(logger_,"CommandSessionHandler::HandleSession: session " << pSession.get());
    
    LOG4CXX_TRACE(logger_,"CommandSessionHandler::HandleSession: creating/binding command message handler");
    std::unique_ptr<MessageHandler> pHandler(new CommandMessageHandler(pSession.get()));
    pSession->SetMessageHandler(pHandler);
    LOG4CXX_TRACE(logger_,"CommandSessionHandler::HandleSession: command message handler " << pHandler.get());
    
    LOG4CXX_TRACE(logger_,"CommandSessionHandler::HandleSession: saving session " << pSession.get());
    SaveSession(pSession);
    
    LOG4CXX_TRACE(logger_,"CommandSessionHandler::HandleSession: starting session " << pSession.get());
    pSession->Start();
}


}}  // namespace

