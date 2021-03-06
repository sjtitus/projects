#ifndef _PLAYER_H_
#define _PLAYER_H_

/*______________________________________________________________________________
 * Player 
 * A player exists on a board, has a position, and can move. 
 *______________________________________________________________________________
*/

#include <cstdint>
#include <memory>
#include "Position.hpp"
#include "Board.hpp"

namespace com { namespace dimension3designs {

class Player
{
    public:
        // Types
        enum class Direction
        {
            Up,
            Down,
            Left,
            Right,
            Stop        // stationary 
        };

        typedef std::vector<std::pair<uint32_t,uint32_t>> Path;

        // Constructors
        Player(const std::string &name, com::dimension3designs::Board * const pBoard);

        // Accessors
        std::string             Name()              const   { return _name;             }
        std::string             GetId()             const   { return _id;               } 
        uint32_t                GetPositionX()      const   { return _Position.x();     }
        uint32_t                GetPositionY()      const   { return _Position.y();     }
        uint32_t                GetPositionIntX()   const   { return _Position.int_x(); }
        uint32_t                GetPositionIntY()   const   { return _Position.int_y(); }
        Player::Path&           GetPath()                   { return _path;             }  
        
        
        void   SetPosition(uint32_t x, uint32_t y)      { _Position.set_x(x); _Position.set_y(y);           }
        void   SetPositionInt(uint32_t x, uint32_t y)   { _Position.set_int_x(x); _Position.set_int_y(y);   }
        void   SetColor(uint32_t color)                 { _rgbColor = color; }
       
        // Movement
        void MoveUp(uint32_t step); 
        void MoveDown(uint32_t step); 
        void MoveLeft(uint32_t step); 
        void MoveRight(uint32_t step); 


    private:
        std::string                                     _name;              // player name
        std::string                                     _id;                // unique player id
        com::dimension3designs::Position                _Position;          // player position on the board
        com::dimension3designs::Board *                 _pBoard;            // the game board (owned by the game) 
        uint32_t                                        _rgbColor;          // player color (32 bit RGB value)
        bool                                            _inside;            // inside his own color?
        Direction                                       _direction;         // current direction player is moving
        Path                                            _path;              // path


        // helper functions
        std::string                                     _GenerateId();     // generate unique player id 


    // Enforce move semantics only
    private:
        Player(const Player&); 
        Player& operator=(const Player&);
    public:
        Player(Player&& x) = default;   
        Player& operator=(Player&& a) = default;
};

}}

#endif

