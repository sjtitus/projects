#ifndef _GAMETHREAD_H_
#define _GAMETHREAD_H_
/*______________________________________________________________________________
 * GameThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"

namespace com { namespace dimension3designs {

class Game;

class GameThread: public WorkerThread 
{
    public:

    GameThread( const std::string &name, Game *pGame );

    void DoWork() override; 
    
    private:
        Game *_pGame;
};


}}

#endif
