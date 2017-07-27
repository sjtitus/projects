#ifndef _COMMANDTHREAD_H_
#define _COMMANDTHREAD_H_
/*______________________________________________________________________________
 * CommandThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"

namespace com { namespace dimension3designs {

class CommandThread: public WorkerThread 
{
    public:

    CommandThread( const std::string &name );
    
    void DoWork() override; 
};


}}

#endif
