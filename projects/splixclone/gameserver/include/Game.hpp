#ifndef _GAME_H_
#define _GAME_H_

/*______________________________________________________________________________
 * Game 
 * The game consists of a board and a set of players that occupy the 
 * board.
 *______________________________________________________________________________
*/

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>
#include "Board.hpp"
#include "Player.hpp"

class Game
{
    public:

        // Constructors
        Game(uint32_t board_width, uint32_t board_height);

        // Accessors
        Board* GetBoard() { return _pBoard.get(); }

        // Player Management
        std::string                 AddPlayer(const std::string &name);
        std::vector<std::string>    FindPlayers(const std::string &name=""); 
        void                        RemovePlayer(const std::string &id);  

    private:
        std::unique_ptr<Board> _pBoard;                             // game owns the game board
        std::unordered_map<std::string, Player> _PlayerHash;        // hash of players 
};

#endif
