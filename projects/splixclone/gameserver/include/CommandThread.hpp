#ifndef _COMMANDTHREAD_H_
#define _COMMANDTHREAD_H_
/*______________________________________________________________________________
 * CommandThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"


namespace com { namespace dimension3designs {

class Game;

class CommandThread: public WorkerThread 
{
    public:
        CommandThread( const std::string &name, Game *pGame );
        void DoWork() override; 

    private:
        Game *_pGame;       // game thread is running in
        
        void DummyWork();
};


}}

#endif
