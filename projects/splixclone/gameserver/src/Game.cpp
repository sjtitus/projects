/*______________________________________________________________________________
 * Game 
 * The game consists of a board and a set of players that occupy the 
 * board.
 *______________________________________________________________________________
*/
#include "Game.hpp"


// Constructors
Game::Game(uint32_t board_width, uint32_t board_height)
{
    _pBoard = std::unique_ptr<Board>( new Board(board_width,board_height) );
}

// Add a player with a specified name to the game
// Names need not be unique
// Returns the unique player identifier        
std::string Game::AddPlayer(const std::string &name)
{
    return "";
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




