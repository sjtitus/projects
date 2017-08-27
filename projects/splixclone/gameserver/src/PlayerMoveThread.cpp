/*______________________________________________________________________________
 * PlayerMoveThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "PlayerMoveThread.hpp"

namespace com { namespace dimension3designs {


PlayerMoveThread::PlayerMoveThread( const std::string &name, Game *pGame )
    :WorkerThread(name),
     _pGame(pGame)
{
    LOG4CXX_TRACE(_logger,"PlayerMoveThread::PlayerMoveThread: construct thread " << _name); 
    LOG4CXX_TRACE(_logger,"PlayerMoveThread::PlayerMoveThread: game " << _pGame); 
}

void PlayerMoveThread::DoWork()
{
    try
    {
        LOG4CXX_TRACE(_logger,"PlayerMoveThread::DoWork: starting work for thread " << _name);
        while (true)
        {
            LOG4CXX_TRACE(_logger,"PlayerMoveThread::DoWork: sleeping for 2 seconds");
            boost::this_thread::sleep_for(boost::chrono::seconds(2)); 
            boost::this_thread::interruption_point();
        }
    }
    catch (const boost::thread_interrupted& e)
    {
        LOG4CXX_TRACE(_logger,"PlayerMoveThread::DoWork: stopping");
    }
}


}}

