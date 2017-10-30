/*______________________________________________________________________________
 * CommandThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include <boost/assert.hpp>

#include "Game.hpp"
#include "LocalSocketServer.hpp"
#include "CommandMessageHandler.hpp"
#include "CommandThread.hpp"

namespace com { namespace dimension3designs {


CommandThread::CommandThread( const std::string &name, Game *pGame )
    :WorkerThread(name),
     _pGame(pGame)
{
    LOG4CXX_TRACE(_logger,"CommandThread::CommandThread: construct command server using socket file " << _pGame->_COMMAND_SOCKET_FILE);
    unlink(_pGame->_COMMAND_SOCKET_FILE.c_str());
    std::unique_ptr<LocalSocketSessionHandler> pSessionHandler(new CommandSessionHandler());
    _pCommandServer = std::unique_ptr<LocalSocketServer>(
        new LocalSocketServer(_io_service, _pGame->_COMMAND_SOCKET_FILE,pSessionHandler)
    );
    LOG4CXX_TRACE(_logger,"CommandThread::CommandThread: constructed thread " << _name << "(game " << _pGame << ")"); 
}
    
void CommandThread::Stop()
{
    LOG4CXX_TRACE(_logger,"CommandThread::Stop: stopping thread " << _name << " via io_service.stop()"); 
    _io_service.stop();
    //_thread.interrupt();
}


void CommandThread::DoWork()
{
    try
    {
        LOG4CXX_TRACE(_logger,"CommandThread: starting");
 
        // Tell the command message handler class about the game singleton
        CommandMessageHandler::SetGame(_pGame);

        // start the command server: will listen for commands
        LOG4CXX_TRACE(_logger,"CommandThread: starting command server (listening for commands)"); 
        _pCommandServer->Start();

        // run asynchronous IO: will block until all async IO is complete
        _io_service.run();
        LOG4CXX_TRACE(_logger,"CommandThread: exiting");
    }
    catch (const boost::thread_interrupted& e)
    {
        LOG4CXX_TRACE(_logger,"CommandThread: stopping");
    }
}


void CommandThread::DummyWork()
{
    while (true)
    {
        LOG4CXX_TRACE(_logger,"CommandThread: sleeping for 2 seconds");
        boost::this_thread::sleep_for(boost::chrono::seconds(2)); 
        boost::this_thread::interruption_point();
    }
}


}}

