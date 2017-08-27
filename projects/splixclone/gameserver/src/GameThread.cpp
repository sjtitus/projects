/*______________________________________________________________________________
 * GameThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "GameThread.hpp"

namespace com { namespace dimension3designs {


GameThread::GameThread( const std::string &name, Game *pGame )
    :WorkerThread(name),
     _pGame(pGame)
{
    LOG4CXX_TRACE(_logger,"GameThread::GameThread: construct thread " << _name); 
    LOG4CXX_TRACE(_logger,"GameThread::GameThread: game " << _pGame); 
}

void GameThread::DoWork()
{
    try
    {
        LOG4CXX_TRACE(_logger,"GameThread::DoWork: starting work for thread " << _name);
        while (true)
        {
            LOG4CXX_TRACE(_logger,"GameThread::DoWork: sleeping for 2 seconds");
            boost::this_thread::sleep_for(boost::chrono::seconds(2)); 
            boost::this_thread::interruption_point();
        }
    }
    catch (const boost::thread_interrupted& e)
    {
        LOG4CXX_TRACE(_logger,"GameThread::DoWork: stopping");
    }
}


}}

