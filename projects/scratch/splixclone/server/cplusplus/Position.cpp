/*______________________________________________________________________________
 * A Position on the board.
 * Contains x and y coordinates, each is a 32-bit fixed-point with 24 bits 
 * of integer and 8 bits of fraction.
 *______________________________________________________________________________
*/

#include "Position.hpp"
#include <cstdio>
#include <cstdlib>


void Position::move_x(uint32_t dx, bool neg)
{
    if (neg)
    {
        uint32_t new_x = _x - dx;
        _x = (new_x >= _x) ? _x:new_x;
    }
    else
    {
        uint32_t new_x = _x + dx;
        _x = (new_x <= _x) ? _x:new_x;
    }
}

// Change y coord
void Position::move_y(uint32_t dy, bool neg)
{
    if (neg)
    {
        uint32_t new_y = _y - dy;
        _y = (new_y >= _y) ? _y:new_y;
    }
    else
    {
        uint32_t new_y = _y + dy;
        _y = (new_y <= _y) ? _y:new_y;
    }
}

#ifdef _UNIT_TEST_POSITION_

int main( int argc, const char **argv )
{
    printf("Hello, world\n");
    Position p; 
    exit(0);
}

#endif

