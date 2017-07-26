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
Game::Game(uint32_t board_width, uint32_t board_height)
{
    LOG4CXX_TRACE(_logger,"Game::Game: board width="<<board_width<<", board height="<<board_height);
    _pBoard = std::unique_ptr<Board>( new Board(board_width,board_height) );
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
    auto iresult = _PlayerHash.insert(std::make_pair(id,std::move(pPlayer)));
    if (!iresult.second)
    {
        // non-unique id: abort
        RAISE_EXCEPTION("Game::AddPlayer: failed to add player "+id+" (non-unique id?)");
    }

    LOG4CXX_TRACE(_logger,"Game::AddPlayer: ok (id="<< id <<")"); 
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
    // Logic error if player is not present
    auto present = _PlayerHash.erase(id);
    BOOST_ASSERT_MSG(present==1, "Game::RemovePlayer: player not present");
    LOG4CXX_TRACE(_logger,"Game::RemovePlayer: ok");
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
        playerList.reserve(_PlayerHash.size());
        for(auto &kv : _PlayerHash) {
            playerList.push_back(kv.first);  
        }
    }
    else
    {
        // return all matching players
        for(auto &kv : _PlayerHash) {
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
