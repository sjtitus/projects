#ifndef _BOARD_H_
#define _BOARD_H_

/*______________________________________________________________________________
 * The board 
 * A rectangular grid that has dimensions (width,height) square blocks.
 * Origin is upper left, +x is right, +y is down.
 *
 * NOTE: width and height must be <= 0x00ffffff to accommodate fixed-point
 * positions on the board, which have 8 bits of fraction (i.e. a position
 * change of +1 in x or y represents movement of 1/256 of a block), see
 * Position class. 
 *______________________________________________________________________________
*/

#include <cstdint>
#include <memory>
#include "Array2D.hpp"
#include "Logging.hpp"

namespace com { namespace dimension3designs {

class Board
{
    public:

        // Constructors
        Board(uint32_t width, uint32_t height); 
        
        // Accessors
        uint32_t width()  { return _pArray->width();    }  
        uint32_t height() { return _pArray->height();   }
        uint32_t wlimit() { return _wlimit;             }
        uint32_t hlimit() { return _hlimit;             }
        
    private:
        std::unique_ptr<com::dimension3designs::Array2D<uint8_t>>   _pArray;    // board owns the array of blocks 
        uint32_t                                                    _wlimit;    // int width limit 
        uint32_t                                                    _hlimit;    // int height limit
        static log4cxx::LoggerPtr                                   _logger;    // logging
    
    // Move semantics only
    private:
        Board(const Board&); 
        Board& operator=(const Board&);
    public:
        Board(Board&& x) = default;   
        Board& operator=(Board&& a) = default;

};

}
}

#endif
