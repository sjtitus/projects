#ifndef _POSITION_H_
#define _POSITION_H_

/*______________________________________________________________________________
 * A Position on the board.
 * Contains x and y coordinates, each is a 32-bit fixed-point with 24 bits 
 * of integer and 8 bits of fraction.
 *______________________________________________________________________________
*/

#include <cstdint>

class Position
{
    public:

        // Constructors
        Position() : _x(0), _y(0)                       {}
        Position(uint32_t x, uint32_t y) : _x(x), _y(y) {}
        
        // Accessors
        uint32_t x() { return _x; }
        uint32_t y() { return _y; }
        
        // Accessors for integer values 
        uint32_t int_x() { return (_x >> 8); }
        uint32_t int_y() { return (_y >> 8); }

        // Change x,y coords
        void move_x(uint32_t dx, bool neg);
        void move_y(uint32_t dy, bool neg);
   
    private:
        uint32_t _x;
        uint32_t _y;
};

#endif
