/*______________________________________________________________________________
 * Main 
 * Main program for splixclone game. 
 *______________________________________________________________________________
*/
#include "CommandSessionHandler.hpp"
#include "LocalSocketServer.hpp"
#include <boost/asio.hpp>
#include "Logging.hpp"

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;

int main( int argc, const char **argv )
{
    log4cxx::LoggerPtr _logger(Logger::getLogger("com.dimension3designs"));

    LOG4CXX_TRACE(_logger,"Main: starting");
 
    LOG4CXX_TRACE(_logger,"_____________________________________________________");
    LOG4CXX_TRACE(_logger,"Main: creating a command session handler");
    std::unique_ptr<LocalSocketSessionHandler> pSessionHandler(new CommandSessionHandler());
    
    LOG4CXX_TRACE(_logger,"_____________________________________________________");
    LOG4CXX_TRACE(_logger,"Main: creating a local socket server on /tmp/LocalSocketServerTest");
    unlink("/tmp/LocalSocketServerTest");
    boost::asio::io_service io_service; 
    LocalSocketServer server(io_service,"/tmp/LocalSocketServerTest",pSessionHandler);
    
    LOG4CXX_TRACE(_logger,"_____________________________________________________");
    LOG4CXX_TRACE(_logger,"Main: starting the server"); 
    server.Start();

    LOG4CXX_TRACE(_logger,"_____________________________________________________");
    LOG4CXX_TRACE(_logger,"Main: running the io service"); 
    io_service.run();
 
    return 0;

/*
    LOG4CXX_TRACE(_logger,"Main: creating a game on a 100x100 board");
    Game game(100,100);
    
    LOG4CXX_TRACE(_logger,"_____________________________________________________");
    LOG4CXX_TRACE(_logger,"Main: starting game");
    game.Start(); 
    
    LOG4CXX_TRACE(_logger,"_____________________________________________________");
    LOG4CXX_TRACE(_logger,"Main: joining game");
    game.Join(); 
 
    LOG4CXX_TRACE(_logger,"Main: exiting"); 
*/
}


