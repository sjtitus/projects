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

namespace com { namespace dimension3designs {

class Game
{
    public:

        //_______________________________________________
        // Constructors
        Game(uint32_t board_width, uint32_t board_height);

        //_______________________________________________
        // Accessors
        com::dimension3designs::Board* GetBoard() { return _pBoard.get(); }

        //_______________________________________________
        // Player Management
        std::string                 AddPlayer(const std::string &name);
        std::vector<std::string>    FindPlayers(const std::string &name=""); 
        void                        RemovePlayer(const std::string &id);  
        size_t                      NumPlayers()                                { return _PlayerHash.size(); }

        //_______________________________________________
        // Game Management
        void Start();
        void Stop();         


    private:
        std::unique_ptr<com::dimension3designs::Board>                                      _pBoard;        // game owns the board
        std::unordered_map<std::string, std::unique_ptr<com::dimension3designs::Player>>    _PlayerHash;    // hash of players 
        static log4cxx::LoggerPtr                                                           _logger;        // logging

};

}}

#endif
