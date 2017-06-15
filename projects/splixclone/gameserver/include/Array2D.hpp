#ifndef _ARRAY2D_H_
#define _ARRAY2D_H_

/*______________________________________________________________________________
 * The array2D 
 * A template class for a generic memory-continuous 2D array. 
 *______________________________________________________________________________
*/

#include <cstdint>
#include <cstdlib>
#include <cstring>
#include <boost/assert.hpp>

namespace com { namespace dimension3designs {

template< typename T >
class Array2D
{
    public:

        Array2D(size_t height, size_t width)
        :_height(height), _width(width)
        {
            _ary = new T*[height];           
            T* pool = new T[width*height];
            memset((void *) pool, 0, sizeof(T)*width*height);    
            for (size_t i=0; i<height; ++i, pool += width )
                _ary[i] = pool;
        }

        ~Array2D()
        {
            delete [] _ary[0];
            delete [] _ary;
        }
        
        // Accessors
        size_t height() { return _height; }
        size_t width()  { return _width;  }

    
        T& operator()(size_t r, size_t c)
        {
            BOOST_ASSERT_MSG(r<_height && c<_width,  "Array2D index(s) OOB");
            return _ary[r][c]; 
        }
        
    private:
        size_t _height;
        size_t _width;
        T** _ary;

};

}}


#endif
