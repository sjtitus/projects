/*_____________________________________________________________________________
 *  CommandSessionHandler 
 * ____________________________________________________________________________
*/

#ifndef _COMMANDSESSIONHANDLER_H_
#define _COMMANDSESSIONHANDLER_H_

#include "LocalSocketSession.hpp"
#include "LocalSocketSessionHandler.hpp"
#include "Logging.hpp"


namespace com { namespace dimension3designs {


class CommandSessionHandler: public LocalSocketSessionHandler
{
    public:

        CommandSessionHandler();
        virtual void HandleSession( LocalSocketSession::Ptr &pSession );

};

}}  // namespace

#endif
