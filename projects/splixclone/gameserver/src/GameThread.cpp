/*______________________________________________________________________________
 * GameThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "GameThread.hpp"
#include "Game.hpp"

namespace com { namespace dimension3designs {


GameThread::GameThread( const std::string &name, Game *pGame )
    :WorkerThread(name),
     _pGame(pGame)
{
    LOG4CXX_TRACE(_logger,"GameThread::GameThread: construct thread " << _name); 
    LOG4CXX_TRACE(_logger,"GameThread::GameThread: game " << _pGame); 
}

//______________________________________________________________________________
// Game Thread Work
//     Execute main game loop (per-frame)
//          1. See if there are any commmands pending; if so, execute them
//          2. Iterate through all players and get/make their moves
//          3. Render the board
//          4. Broadcast the board to the players 
void GameThread::DoWork()
{
    try
    {
        LOG4CXX_TRACE(_logger,"GameThread:: starting");
        while (true)
        {
            // Fetch and process commands
            std::vector<std::unique_ptr<std::string>> commandVector = FetchCommands();
            ProcessCommands(commandVector);

            // 
            boost::this_thread::sleep_for(boost::chrono::seconds(1)); 
            boost::this_thread::interruption_point();
        }
    }
    catch (const boost::thread_interrupted& e)
    {
        LOG4CXX_TRACE(_logger,"GameThread:: stopping");
    }
}



//______________________________________________________________________________
// Fetch and Process any pending game commands 
void GameThread::ProcessCommands( std::vector<std::unique_ptr<std::string>> &commandVector )
{
    LOG4CXX_TRACE(_logger,"GameThread:: processing commands");
    for (auto& i : commandVector)
    {
        LOG4CXX_TRACE(_logger,"GameThread:: command: " << *i); 
    }
}


//______________________________________________________________________________
// Fetch  
std::vector<std::unique_ptr<std::string>> GameThread::FetchCommands()
{
    LOG4CXX_TRACE(_logger,"GameThread:: fetching commands");
    std::vector<std::unique_ptr<std::string>> commandVector; 
    std::unique_ptr<std::string> pCommand;
    while ((pCommand =  _pGame->CommandBuffer().PopFront()))
    {
        commandVector.push_back(std::move(pCommand)); 
    }
    LOG4CXX_TRACE(_logger,"GameThread:: got " << commandVector.size() << " commands");
    return commandVector; 
}




}}

