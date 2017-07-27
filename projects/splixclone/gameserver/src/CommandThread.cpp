/*______________________________________________________________________________
 * CommandThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "CommandThread.hpp"

namespace com { namespace dimension3designs {


CommandThread::CommandThread( const std::string &name )
    :WorkerThread(name)
{
    LOG4CXX_TRACE(_logger,"CommandThread::CommandThread: construct thread " << name_); 
}

void CommandThread::DoWork()
{
    LOG4CXX_TRACE(_logger,"CommandThread::DoWork: starting work for thread " << name_); 
}


}}

