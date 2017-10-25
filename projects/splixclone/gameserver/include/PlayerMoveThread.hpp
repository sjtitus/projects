#ifndef _PLAYERMOVETHREAD_H_
#define _PLAYERMOVETHREAD_H_
/*______________________________________________________________________________
 * PlayerMoveThread
 * Thread for handling game commands 
 *______________________________________________________________________________
*/

#include "WorkerThread.hpp"
#include <boost/asio.hpp>   

namespace com { namespace dimension3designs {

class Game;

class PlayerMoveThread: public WorkerThread 
{
    public:

    PlayerMoveThread( const std::string &name, Game *pGame );

    void DoWork() override; 
    
    private:
        Game *_pGame;
        boost::asio::io_service _io_service;    // io_service to handle async io
};


}}

#endif
