#ifndef _COMMANDTHREAD_H_
#define _COMMANDTHREAD_H_
/*______________________________________________________________________________
 * CommandThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"
#include "LocalSocketServer.hpp"
#include "CommandMessageHandler.hpp"
#include <memory>
#include <boost/asio.hpp>   


namespace com { namespace dimension3designs {

// forward decl for Game 
// Game includes a CommandThread and passes itself to it 
class Game;

class CommandThread: public WorkerThread 
{
    public:
        CommandThread( const std::string &name, Game *pGame );
        LocalSocketServer* CommandServer() {  return _pCommandServer.get(); }
        
        void DoWork() override; 
        void Stop() override;

    private:
        Game *_pGame;                                            // game thread is running in
        boost::asio::io_service _io_service;                     // io_service to handle async io
        std::unique_ptr<LocalSocketServer> _pCommandServer;      // game command server 
        
        void DummyWork();
};


}}

#endif
