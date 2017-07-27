#ifndef _PLAYERMOVETHREAD_H_
#define _PLAYERMOVETHREAD_H_
/*______________________________________________________________________________
 * PlayerMoveThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"

namespace com { namespace dimension3designs {

class PlayerMoveThread: public WorkerThread 
{
    public:

    PlayerMoveThread( const std::string &name );

    void DoWork() override; 
};


}}

#endif
