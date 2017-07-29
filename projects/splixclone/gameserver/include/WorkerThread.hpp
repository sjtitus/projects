#ifndef _WORKERTHREAD_H_
#define _WORKERTHREAD_H_

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
        LOG4CXX_TRACE(_logger,"WorkerThread::DoWork: starting work for thread " << name_); 
    }

    void Start()
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::Start: start thread " << name_); 
        thread_ = boost::thread( boost::bind( &WorkerThread::DoWork, this) );
        LOG4CXX_TRACE(_logger,"WorkerThread::Start: thread started, id: " << thread_.get_id()); 
    }
    
    // Stop the thread
    // Note: DoWork must implement interruption points for stopping to work 
    void Stop()
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::Stop: stopping thread " << name_); 
        thread_.interrupt();
    }

    // Accessor for underlying thread
    boost::thread& Thread()
    {
        return thread_;
    }

    protected:
        boost::thread thread_;
        std::string name_;
        static log4cxx::LoggerPtr _logger;
};


}}

#endif
