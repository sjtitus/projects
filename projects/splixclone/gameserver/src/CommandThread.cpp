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
    try
    {
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: starting work for thread " << name_);
        while (true)
        {
            LOG4CXX_TRACE(_logger,"CommandThread::DoWork: sleeping for 2 seconds");
            boost::this_thread::sleep_for(boost::chrono::seconds(2)); 
            boost::this_thread::interruption_point();
        }
    }
    catch (const boost::thread_interrupted& e)
    {
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: stopping");
    }
}


}}

