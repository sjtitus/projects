/*_____________________________________________________________________________
 *  LocalSocketSessionHandler
 *  Wires a LocalSocketSession to an application-specific MessageHandler via
 *  templating.
 * ____________________________________________________________________________
*/

#include "LocalSocketSessionHandler.hpp"
#include "Logging.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;

namespace com { namespace dimension3designs {

// Logging
log4cxx::LoggerPtr LocalSocketSessionHandler::logger_(log4cxx::Logger::getLogger("com.dimension3designs.LocalSocketSession"));

LocalSocketSessionHandler::LocalSocketSessionHandler()
{
    LOG4CXX_TRACE(logger_,"LocalSocketSessionHandler: Construct");
}

}}  // namespace

