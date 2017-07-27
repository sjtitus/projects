/*______________________________________________________________________________
 * PlayerMoveThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "PlayerMoveThread.hpp"

namespace com { namespace dimension3designs {


PlayerMoveThread::PlayerMoveThread( const std::string &name )
    :WorkerThread(name)
{
    LOG4CXX_TRACE(_logger,"PlayerMoveThread::PlayerMoveThread: construct thread " << name_); 
}

void PlayerMoveThread::DoWork()
{
    LOG4CXX_TRACE(_logger,"PlayerMoveThread::DoWork: starting work for thread " << name_); 
}


}}

