
/*______________________________________________________________________________
 * The board 
 * A rectangular grid that has dimensions (width,height) square blocks.
 *______________________________________________________________________________
*/

#include "Board.hpp" 
#include <boost/assert.hpp>

// Constructors
Board::Board(uint32_t width, uint32_t height) 
{
    BOOST_ASSERT_MSG(width <= 0x00ffffff, "Board width must be <= 0x00ffffff");
    BOOST_ASSERT_MSG(height <= 0x00ffffff, "Board height must be <= 0x00ffffff");
    _wlimit = 0x100 * width - 1;
    _hlimit = 0x100 * height - 1;
    _pArray = std::unique_ptr<Array2D<uint8_t>>( new Array2D<uint8_t>(height,width) );
}
        
        
