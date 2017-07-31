/*______________________________________________________________________________
 * Game 
 * The game consists of a board and a set of players that occupy the 
 * board.
 *______________________________________________________________________________
*/
#include "Game.hpp"
#include "Logging.hpp"
#include "SimpleException.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;

namespace com { namespace dimension3designs {

// Logging 
LoggerPtr Game::_logger(Logger::getLogger("com.dimension3designs.Game"));


// Constructors
Game::Game(uint32_t board_width, uint32_t board_height):
    _pBoard(std::unique_ptr<Board>( new Board(board_width,board_height))),
    _pCommandThread(std::unique_ptr<CommandThread>( new CommandThread("CommandThread"))),
    _pPlayerMoveThread(std::unique_ptr<PlayerMoveThread>( new PlayerMoveThread("PlayerMoveThread"))),
    _pGameThread(std::unique_ptr<GameThread>( new GameThread("GameThread"))),
    _commandBuffer(Game::COMMAND_BUFFER_SIZE)
{
    LOG4CXX_TRACE(_logger,"Game::Game: board width="<<_pBoard->width()<<", board height="<<_pBoard->height());
    LOG4CXX_TRACE(_logger,"Game::Game ok"); 
}

//______________________________________________________________________________
// AddPlayer
//    Add a player with a specified name to the game. Names need not 
//    be unique.
//    Returns the unique player identifier.
//______________________________________________________________________________
std::string Game::AddPlayer(const std::string &name)
{
    LOG4CXX_TRACE(_logger,"Game::AddPlayer: "<< name);
    
    // create a new player and grab his unique id 
    auto pPlayer = std::unique_ptr<Player>( new Player(name,_pBoard.get()) );
    auto id = pPlayer->GetId();

    // store the player by id
    auto iresult = _playerHash.insert(std::make_pair(id,std::move(pPlayer)));
    if (!iresult.second)
    {
        // non-unique id: abort
        RAISE_EXCEPTION("Game::AddPlayer: failed to add player "+id+" (non-unique id?)");
    }
    
    // add a circular buffer to store the new player's moves    
    LOG4CXX_TRACE(_logger,"Game::AddPlayer: adding move buffer for player "<< name);
    auto pPlayerMoveBuffer = std::unique_ptr<CircularMessageBuffer>( new CircularMessageBuffer(Game::PLAYERMOVE_BUFFER_SIZE) );
    auto iresult2 = _playerMoveBufferHash.insert(std::make_pair(id,std::move(pPlayerMoveBuffer)));
    if (!iresult2.second)
    {
        // failed to add buffer for player moves 
        RAISE_EXCEPTION("Game::AddPlayer: failed to add buffer for player "+id+" moves");
    }

    LOG4CXX_TRACE(_logger,"Game::AddPlayer: ok (name="<< name <<", id="<< id <<")"); 
    return id;
}




//______________________________________________________________________________
// RemovePlayer
// Remove a player from the game (by id).
//______________________________________________________________________________
void Game::RemovePlayer(const std::string &id)
{
    LOG4CXX_TRACE(_logger,"Game::RemovePlayer: id="<< id);

    // TODO: player cleanup? 

    // Remove from playerHash
    // Logic error if player is not present
    auto present = _playerHash.erase(id);
    BOOST_ASSERT_MSG(present==1, "Game::RemovePlayer: player not present");

    // Remove move buffer from playerMoveBufferHash
    auto present2 = _playerMoveBufferHash.erase(id);
    BOOST_ASSERT_MSG(present2==1, "Game::RemovePlayer: player move buffer not present");

    LOG4CXX_TRACE(_logger,"Game::RemovePlayer: ok");
}



void Game::Start()
{
    LOG4CXX_TRACE(_logger,"Game::Start: Starting"); 

    _pCommandThread->Start();

    _pPlayerMoveThread->Start();

    _pGameThread->Start();

    LOG4CXX_TRACE(_logger,"Game::Start: Started"); 
}



void Game::Stop()
{
    LOG4CXX_TRACE(_logger,"Game::Stop: Stopping"); 
    _pCommandThread->Stop();
    _pPlayerMoveThread->Stop();
    _pGameThread->Stop();
    _pCommandThread->Thread().join();
    _pPlayerMoveThread->Thread().join();
    _pGameThread->Thread().join();
    LOG4CXX_TRACE(_logger,"Game::Stop: Stopped"); 
}



//______________________________________________________________________________
// FindPlayers
// Find players with specified name.
// Returns list of all player IDs for players with the specified name, 
// or all players if name is not provided.
//______________________________________________________________________________
std::vector<std::string> Game::FindPlayers(const std::string &name)
{
    LOG4CXX_TRACE(_logger,"Game::FindPlayers: name="<<name);
    std::vector<std::string> playerList;
    if (name.size()==0)
    {
        // return all the players
        playerList.reserve(_playerHash.size());
        for(auto &kv : _playerHash) {
            playerList.push_back(kv.first);  
        }
    }
    else
    {
        // return all matching players
        for(auto &kv : _playerHash) {
            if (kv.second->Name() == name)
            {
                playerList.push_back(kv.first);
            }  
        }
    }
    
    LOG4CXX_TRACE(_logger,"Game::FindPlayers: returning "<<playerList.size()<<" id(s)");
    return playerList; 
}

}}
