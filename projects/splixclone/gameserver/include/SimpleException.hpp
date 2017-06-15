#ifndef _SIMPLE_EXCEPTION_H_
#define _SIMPLE_EXCEPTION_H_

#include <stdexcept>
#include <iostream>
//#include <string>
//#include <cstdio>

#define RAISE_EXCEPTION(msg) (throw SimpleException(msg,__FILE__,__FUNCTION__,__LINE__))

class SimpleException
{
  public:

    // Constructor
    SimpleException(const std::string &msg, const char *file, const char *function, int line);

   
    // Accessors 
    int             line()      const   { return _line;     }
    const char*     file()      const   { return _file;     }
    const char*     function()  const   { return _function; }
    const char*     msg()       const   { return _msg;      }
   
    // Operator << to output a formatted string  
    friend std::ostream& operator<<(std::ostream& os, const SimpleException& dt);  
    
  private:
    const char *    _file;          // originating source file
    const char *    _function;      // originating function
    int             _line;          // originating source line
    char            _msg[2048];     // message describing exception
};

#endif
