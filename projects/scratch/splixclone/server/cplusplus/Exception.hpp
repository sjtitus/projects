#ifndef _EXCEPTION_H_
#define _EXCEPTION_H_

#include <stdexcept>
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <iostream>
#include <sstream>



//______________________________________________________________________________
// Exception Macros

// Throw a System or Domain exception, save (function, file, line) 
// information, and the exception message.
#define THROW_SYSTEM_EXCEPTION(msg) (throw SystemException(__FILE__,__PRETTY_FUNCTION__,__LINE__,msg)) 
#define THROW_DOMAIN_EXCEPTION(msg) (throw DomainException(__FILE__,__PRETTY_FUNCTION__,__LINE__,msg)) 


// "ReThrow" a System or Domain exception corresponding to an existing exception
// (by using it's 'what' member as the message.
#define RETHROW_SYSTEM_EXCEPTION(e) (throw SystemException(__FILE__,__PRETTY_FUNCTION__,__LINE__,e.what())) 
#define RETHROW_DOMAIN_EXCEPTION(e) (throw DomainException(__FILE__,__PRETTY_FUNCTION__,__LINE__,e.what())) 


//______________________________________________________________________________
// ExceptionBase
// Base class for exceptions, saves (function, file, line) where the 
// exception was thrown.
class ExceptionBase
{
    public:
    ExceptionBase()
    {
        file_[0] = func_[0] = '\0';
    }

    ExceptionBase(const char *file, const char *func, int line)
    {
        size_t len; 
        file_[0] = func_[0] = '\0';
        if (file)
        {
            len = std::min((size_t) 511,strlen(file));
            strncpy(file_,file,len);
            file_[len] = '\0';
        }
        if (func)
        {
            len = std::min((size_t) 511,strlen(func));
            strncpy(func_,func,len);
            func_[len] = '\0';
        }
        line_ = line;
    }

    public: 
        char file_[512];
        char func_[512];
        int line_;
};


//______________________________________________________________________________
// SystemException
// Exception that is the result of a system/platform/technical condition
// as opposed to a business domain exception. 
class SystemException: public ExceptionBase, public std::runtime_error
{
    public:
    SystemException(const char *file, const char *func, int line, const char *what)
        :ExceptionBase(file,func,line),
        std::runtime_error(what)
    {
    }
    
    SystemException(const char *file, const char *func, int line, std::string what)
        :ExceptionBase(file,func,line),
        std::runtime_error(what)
    {
    }
    
    const std::string Info()
    {
        std::ostringstream is; 
        is <<  "\n*** SystemException ***\n" << func_ << " [" << file_ << ", line " << line_ << "]: " << what() << "\n";
        return is.str(); 
    }
};


//______________________________________________________________________________
// DomainException
// Exception that is the result of an application/domain level problem/error as
// as opposed to a system/platform/technical exception. 
class DomainException: public ExceptionBase, public std::logic_error
{
    public:
    DomainException(const char *file, const char *func, int line, const char *what)
        :ExceptionBase(file,func,line),
        std::logic_error(what)
    {
    }
    
    DomainException(const char *file, const char *func, int line, std::string what)
        :ExceptionBase(file,func,line),
        std::logic_error(what)
    {
    }
    
    const std::string Info()
    {
        std::ostringstream is; 
        is <<  "\n*** DomainException ***\n" << func_ << " [" << file_ << ", line " << line_ << "]: " << what() << "\n";
        return is.str(); 
    }
}; 

#endif
