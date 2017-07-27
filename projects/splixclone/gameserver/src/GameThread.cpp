/*______________________________________________________________________________
 * GameThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "GameThread.hpp"

namespace com { namespace dimension3designs {


GameThread::GameThread( const std::string &name )
    :WorkerThread(name)
{
    LOG4CXX_TRACE(_logger,"GameThread::GameThread: construct thread " << name_); 
}

void GameThread::DoWork()
{
    LOG4CXX_TRACE(_logger,"GameThread::DoWork: starting work for thread " << name_); 
}


}}

