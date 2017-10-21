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
        LOG4CXX_TRACE(_logger,"PlayerMoveThread:: starting");
        while (true)
        {
            boost::this_thread::sleep_for(boost::chrono::seconds(1)); 
            boost::this_thread::interruption_point();
        }
    }
    catch (const boost::thread_interrupted& e)
    {
        LOG4CXX_TRACE(_logger,"PlayerMoveThread:: stopping");
    }
}


}}

