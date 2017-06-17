
#include "SimpleException.hpp"
#include <iostream>
#include <string>
#include <cstdio>

// Constructor
SimpleException::SimpleException(const std::string &msg, const char *file, const char *function, int line)
: _file(file), _function(function), _line(line) 
{
  _msg[0] = '\0';
  snprintf ( _msg, 2048, "%s", msg.c_str() );
}

// Operator <<
std::ostream& operator<<(std::ostream& os, const SimpleException &se)  
{  
    os << "*** EXCEPTION ***: " << se.function() << " (" << se.file() << ", line " << se.line() << "): " << se.msg() << "\n";  
    return os;  
} 

