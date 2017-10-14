#ifndef _POSITION_H_
#define _POSITION_H_

/*______________________________________________________________________________
 * A player's position on the board.
 * Contains x and y coordinates, each is a 32-bit fixed-point with 24 bits 
 * of integer and 8 bits of fraction. Coordinates "clamp" at 0 and 0xffffffff.
 * 
 *______________________________________________________________________________
*/

#include <cstdint>

namespace com { namespace dimension3designs {

class Position
{
    public:

        // Constructors
        Position() : _x(0x00000080), _y(0x00000080)                 {}
        Position(uint32_t x, uint32_t y) : _x(x), _y(y) {}
        
        // Accessors
        uint32_t    x()                 const       { return _x; }
        uint32_t    y()                 const       { return _y; }
        void        set_x(uint32_t x)               { _x=x; }
        void        set_y(uint32_t y)               { _y=y; }
        
        // Accessors for integer values 
        uint32_t    int_x()                 const   { return (_x >> 8);  }
        uint32_t    int_y()                 const   { return (_y >> 8);  }
        void        set_int_x(uint32_t x)   { _x=(x << 8) + 0x80; }     // center of block 
        void        set_int_y(uint32_t y)   { _y=(y << 8) + 0x80; }     // center of block 

        // Change x,y coords, clamp at 0 and max
        void move_x(uint32_t dx, bool neg) 
        { 
            uint32_t new_x = (neg) ? _x-dx:_x+dx; 
            _x = (neg) ? ((new_x > _x) ? 0:new_x):((new_x < _x) ? 0xffffffff:new_x);
        }
        
        void move_y(uint32_t dy, bool neg) 
        { 
            uint32_t new_y = (neg) ? _y-dy:_y+dy; 
            _y = (neg) ? ((new_y > _y) ? 0:new_y):((new_y < _y) ? 0xffffffff:new_y);
        }
   
    private:
        uint32_t _x;
        uint32_t _y;
    
    // Move semantics only
    private:
        Position(const Position&); 
        Position& operator=(const Position&);
    public:
        Position(Position&& x) = default;   
        Position& operator=(Position&& a) = default;
};

}}

#endif
