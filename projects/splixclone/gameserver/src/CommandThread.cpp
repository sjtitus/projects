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


void CommandThread::DoWork()
{
    try
    {
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: starting work for thread " << _name);
        LOG4CXX_TRACE(_logger,"CommandThread::DoWork: command socket " << _pGame->_COMMAND_SOCKET_FILE);
 
        // Set up a local socket server to handle incoming commands 
        boost::asio::io_service io_service;
        unlink(_pGame->_COMMAND_SOCKET_FILE.c_str());
        LocalSocketServer<CommandMessageHandler> localServer(io_service, _pGame->_COMMAND_SOCKET_FILE);

        // start the server 
        localServer.Start();
        io_service.run();
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

