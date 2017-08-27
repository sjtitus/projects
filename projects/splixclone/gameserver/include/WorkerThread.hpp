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
        :_name(name)
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::WorkerThread: construct thread " << _name); 
    } 


    // Do the work
    virtual void DoWork()
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::DoWork: starting work for thread " << _name);
    }

    void Start()
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::Start: start thread " << _name); 
        _thread = boost::thread( boost::bind( &WorkerThread::DoWork, this ));
        LOG4CXX_TRACE(_logger,"WorkerThread::Start: thread started, id: " << _thread.get_id()); 
    }
    
    // Stop the thread
    // Note: DoWork must implement interruption points for stopping to work 
    virtual void Stop()
    {
        LOG4CXX_TRACE(_logger,"WorkerThread::Stop: stopping thread " << _name); 
        _thread.interrupt();
    }

    // Accessor for underlying thread
    boost::thread& Thread()
    {
        return _thread;
    }

    protected:
        boost::thread _thread;
        std::string _name;
        static log4cxx::LoggerPtr _logger;
};


}}

#endif
