/*______________________________________________________________________________
 * WorkerThread
 * Simple class that wraps boost::thread.
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;

namespace com { namespace dimension3designs {

LoggerPtr WorkerThread::_logger(Logger::getLogger("com.dimension3designs.WorkerThread"));

}}
