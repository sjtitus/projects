#ifndef _GAME_H_
#define _GAME_H_

/*______________________________________________________________________________
 * Game 
 * The game consists of a board and a set of players that occupy the 
 * board.
 *______________________________________________________________________________
*/

#include <cstdint>
#include <memory>
#include <unordered_map>
#include <string>
#include <vector>

#include "Board.hpp"
#include "Player.hpp"
#include "Logging.hpp"
#include "CommandThread.hpp"
#include "PlayerMoveThread.hpp"
#include "GameThread.hpp"
#include "CircularMessageBuffer.hpp"


namespace com { namespace dimension3designs {

class Game
{
    public:

        //_______________________________________________
        // Constructors
        Game(uint32_t board_width, uint32_t board_height);

        //_______________________________________________
        // Accessors
        com::dimension3designs::Board* GetBoard() 
        { 
            return _pBoard.get(); 
        }

        //_______________________________________________
        // Player Management
        std::string                 AddPlayer(const std::string &name);
        void                        RemovePlayer(const std::string &id);
        std::vector<std::string>    FindPlayers(const std::string &name=""); 
        size_t                      NumPlayers()                                { return _playerHash.size(); }
    
        //_______________________________________________
        // Game Management
        void Start();
        void Stop();
        void Join();         
       
        //_______________________________________________
        // Constants 
        const std::string _COMMAND_SOCKET_FILE;
        const std::string _PLAYERMOVE_SOCKET_FILE;


        CircularMessageBuffer& CommandBuffer() { return _commandBuffer; }
        

    private:
        std::unique_ptr<com::dimension3designs::Board>                                      _pBoard;        // the board
        std::unordered_map<std::string, std::unique_ptr<com::dimension3designs::Player>>    _playerHash;    // hash of players by id
        static log4cxx::LoggerPtr                                                           _logger;        // logging

        //_______________________________________________
        // Command Thread
        // Thread that receives and queues game commands 
        // (start/stop game, add a player, remove a player, ...)
        std::unique_ptr<com::dimension3designs::CommandThread>      _pCommandThread;

        //_______________________________________________
        // PlayerMove Thread 
        // Thread that receives and queues player moves
        // coming from the web application 
        std::unique_ptr<com::dimension3designs::PlayerMoveThread>   _pPlayerMoveThread;
 
        //_______________________________________________
        // Game Thread 
        // Thread that runs the main game loop
        std::unique_ptr<com::dimension3designs::GameThread>         _pGameThread;
        
        //_______________________________________________
        // Circular Buffer that queues commands
        static const size_t COMMAND_BUFFER_SIZE = 32; 
        CircularMessageBuffer _commandBuffer;
       
        //_______________________________________________
        // Hash of circular buffers 
        // Each buffer queues player moves for a single player 
        static const size_t PLAYERMOVE_BUFFER_SIZE = 512; 
        std::unordered_map<std::string, std::unique_ptr<com::dimension3designs::CircularMessageBuffer>>    _playerMoveBufferHash;
 
};
 


}}

#endif
