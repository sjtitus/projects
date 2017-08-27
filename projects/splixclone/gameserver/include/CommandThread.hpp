#ifndef _COMMANDTHREAD_H_
#define _COMMANDTHREAD_H_
/*______________________________________________________________________________
 * CommandThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"
#include <boost/asio.hpp>   


namespace com { namespace dimension3designs {

// forward decl for Game 
// Game includes a CommandThread and passes itself to it 
class Game;

class CommandThread: public WorkerThread 
{
    public:
        CommandThread( const std::string &name, Game *pGame );
        void DoWork() override; 
        void Stop() override;

    private:
        Game *_pGame;                           // game thread is running in
        boost::asio::io_service _io_service;    // io_service to handle async io
        
        void DummyWork();
};


}}

#endif
