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
    LOG4CXX_TRACE(_logger,"CommandThread::CommandThread: construct thread " << _name << "(game " << _pGame << ")"); 
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
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: starting work for thread " << _name);
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: command socket " << _pGame->_COMMAND_SOCKET_FILE);
 
        // Set up a local socket server to handle incoming commands 
        unlink(_pGame->_COMMAND_SOCKET_FILE.c_str());
        CommandMessageHandler::SetGame(_pGame);
        LocalSocketServer<CommandMessageHandler> localServer(_io_service, _pGame->_COMMAND_SOCKET_FILE);

        // start the server 
        localServer.Start();
        _io_service.run();
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: exiting");
    }
    catch (const boost::thread_interrupted& e)
    {
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: stopping");
    }
}


void CommandThread::DummyWork()
{
    while (true)
    {
        LOG4CXX_TRACE(_logger,"CommandThread::DummyWork: sleeping for 2 seconds");
        boost::this_thread::sleep_for(boost::chrono::seconds(2)); 
        boost::this_thread::interruption_point();
    }
}


}}

