/*______________________________________________________________________________
 * WorkerThread
 * Simple class that wraps boost::thread.
 *______________________________________________________________________________
*/

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp>
#include "Logging.hpp"

namespace com { namespace dimension3designs {

class WorkerThread
{
    public:
    
    // Constructor
    WorkerThread( const std::string &name  )
        :name_(name)
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::WorkerThread: construct thread " << name_); 
    } 


    // Do the work
    virtual void DoWork()
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::ThreadMain: starting work for thread " << name_); 
    }

    void Start()
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::Start: start thread " << name_); 
        thread_ = boost::thread( boost::bind( &WorkerThread::DoWork, this) );
    }

    // Accessor for underlying thread
    boost::thread& Thread()
    {
        return thread_;
    }

    private:
        boost::thread thread_;
        std::string name_;
        static log4cxx::LoggerPtr _logger;
};

// Logging 
using namespace log4cxx;
using namespace log4cxx::helpers;
LoggerPtr WorkerThread::_logger(Logger::getLogger("com.dimension3designs.WorkerThread"));

}}
