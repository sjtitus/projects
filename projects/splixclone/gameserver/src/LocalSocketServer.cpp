/*_____________________________________________________________________________
 * 
 * LocalSocketServer
 * Server that asynchronously handles connections to a specified Unix
 * domain socket endpoint (file). When a connection is made, the server
 * creates a LocalSocketSession with a specified handler for incoming
 * messages that is invoked asynchronously as messages arrive. 
 *_____________________________________________________________________________
*/
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   

#include "Logging.hpp"
#include "SimpleException.hpp"
#include "LocalSocketServer.hpp"

namespace com { namespace dimension3designs {

}}
