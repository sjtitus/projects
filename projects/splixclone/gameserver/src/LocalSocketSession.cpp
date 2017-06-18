/*_____________________________________________________________________________
 *
 *  LocalSocketSession
 *
 *  Session for asynchronously passing string messages over a Unix  
 *  domain (local) socket.
 *______________________________________________________________________________
*/
#include "LocalSocketSession.hpp"
#include "Logging.hpp"
#include "SimpleException.hpp"

#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/assert.hpp>
#include <string> 

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;

namespace com { namespace dimension3designs {

// Logging 
//LoggerPtr LocalSocketSession::logger_(Logger::getLogger("com.dimension3designs.LocalSocketSession"));
    
}} // namespace
