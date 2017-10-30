#ifndef _GAME_H_
#define _GAME_H_

/*______________________________________________________________________________
 * 
 *  Game
 *
 *  The splixclone game.
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
        // Singleton access
        static Game& Instance()
        {
            static Game gameInstance(100,100);
            return gameInstance;
        } 

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


        //_______________________________________________
        // I/O
        // Comm
        CircularMessageBuffer& CommandBuffer() { return _commandBuffer; }
        

    private:
        
        //_______________________________________________
        // Constructor is private because Game
        // is a singleton.
        Game(uint32_t board_width, uint32_t board_height);
       
        // Prevent copy construction and assignment 
        Game(Game const&); 
        void operator=(Game const&);


        //_______________________________________________
        // The board
        std::unique_ptr<com::dimension3designs::Board> _pBoard;

        //_______________________________________________
        // The players
        // Hash of player objects by unique player ID 
        std::unordered_map<std::string, std::unique_ptr<com::dimension3designs::Player>> _playerHash; 
        
        //_______________________________________________
        // Command Thread
        // Receives and queues game commands 
        std::unique_ptr<com::dimension3designs::CommandThread> _pCommandThread;

        //_______________________________________________
        // PlayerMove Thread 
        // Receives and queues player moves
        std::unique_ptr<com::dimension3designs::PlayerMoveThread> _pPlayerMoveThread;
 
        //_______________________________________________
        // Game Thread
        // Runs game main rendering loop
        std::unique_ptr<com::dimension3designs::GameThread> _pGameThread;
        
        //_______________________________________________
        // Circular Buffer that queues commands
        static const size_t COMMAND_BUFFER_SIZE = 32; 
        CircularMessageBuffer _commandBuffer;
       
        //_______________________________________________
        // Hash of circular buffers for player moves 
        // Each buffer queues player moves for 1 player 
        static const size_t PLAYERMOVE_BUFFER_SIZE = 512; 
        std::unordered_map<std::string, std::unique_ptr<com::dimension3designs::CircularMessageBuffer>>    _playerMoveBufferHash;
       
        //_______________________________________________
        // Logging 
        static log4cxx::LoggerPtr _logger;
 
};
 


}}

#endif
