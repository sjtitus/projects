/*______________________________________________________________________________
 * Game 
 * The game consists of a board and a set of players that occupy the 
 * board.
 *______________________________________________________________________________
*/
#include "Game.hpp"
#include "Logging.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;

namespace com { namespace dimension3designs {

// Logging 
LoggerPtr Game::_logger(Logger::getLogger("com.dimension3designs.Game"));


// Constructors
Game::Game(uint32_t board_width, uint32_t board_height)
{
    LOG4CXX_TRACE(_logger,"contruct Game: board width="<<board_width<<", board height="<<board_height);
    _pBoard = std::unique_ptr<Board>( new Board(board_width,board_height) );
    LOG4CXX_TRACE(_logger,"contruct Game: ok"); 
}

// Add a player with a specified name to the game
// Names need not be unique
// Returns the unique player identifier        
std::string Game::AddPlayer(const std::string &name)
{
    LOG4CXX_TRACE(_logger,"Game: AddPlayer: "<<name); 
    auto pPlayer = std::unique_ptr<Player>( new Player(name,_pBoard.get()) );
    auto id = pPlayer->GetId();
    //auto iresult = _PlayerHash.insert(std::make_pair(id,std::move(pPlayer)));
    LOG4CXX_TRACE(_logger,"Game: AddPlayer: ok (id="<< id <<")"); 
    return id;
}

// Remove a player from the game (by id)
void Game::RemovePlayer(const std::string &id)
{
}

// Find players with specified name
// Returns list of all matching players, or all players if name is not provided
std::vector<std::string> Game::FindPlayers(const std::string &name)
{
    std::vector<std::string> playerList;
    return playerList; 
}

}}




