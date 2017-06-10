/*_____________________________________________________________________________
 * 
 * ServerMain 
 * Main program for the SplixClone server. 
 *_____________________________________________________________________________
*/

#include "CommandMessageHandler.hpp"
#include "PlayerMessageHandler.hpp"
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   
#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp> 


//_____________________________________________________________________________
// CommandWorker 
// Worker thread that handles command endpoint communication 
class CommandWorker 
{
    public:
        
    void CommandWorkerThreadMain()
    {
        const char *COMMAND_ENDPOINT = "/tmp/splixcommand";
       
        // Log thread start and id 
        boost::asio::io_service io_service;
        boost::thread::id tid = boost::this_thread::get_id();
        L_INFO << "CommandWorker [" << tid << "]: started";
       
        //______________________________
        // Start the command server (listens over unix domain socket for commands) 
        L_INFO << "CommandWorker [" << tid << "]: starting command server";
        
        // remove the unix domain socket local path
        unlink(COMMAND_ENDPOINT);
        
        // instantiate a session handler
        CommandSessionHandler commandSessionHandler;
        const std::string socket_file(COMMAND_ENDPOINT);
        LocalSocketServer CommandServer(
            io_service, 
            socket_file, 
            commandSessionHandler
        );
        CommandServer.Start(); 
        L_INFO << "CommandWorker [" << tid << "]: starting async IO";
        io_service.run();
        L_ERROR << "CommandWorker [" << tid << "]: *** returned from io_Service.run() ***";
        L_ERROR << "CommandWorker [" << tid << "]: *** command worker thread exiting ***";
    }

    void Start()
    {
        L_INFO << "CommandWorker: spawning command worker thread";
        thread_ = boost::thread( boost::bind( &CommandWorker::CommandWorkerThreadMain, this) );
    }
    
    boost::thread & Thread()
    {
        return thread_;
    }

    private:
        boost::thread thread_;
};



//_____________________________________________________________________________
// PlayerWorker 
// Worker thread that handles player endpoint communication 
class PlayerWorker 
{
    public:
        
    void PlayerWorkerThreadMain()
    {
        const char *PLAYER_ENDPOINT = "/tmp/splixplayer";
        
        boost::asio::io_service io_service;
        boost::thread::id tid = boost::this_thread::get_id();
        L_INFO << "PlayerWorker [" << tid << "]: started";
        
        L_INFO << "PlayerWorker [" << tid << "]: starting player server";
        unlink(PLAYER_ENDPOINT);
        const std::string pe(PLAYER_ENDPOINT);
        PlayerSessionHandler playerSessionHandler;
        LocalSocketServer PlayerServer(
            io_service, 
            pe, 
            playerSessionHandler
        );
        PlayerServer.Start(); 
        L_INFO << "PlayerWorker [" << tid << "]: starting async IO";
        io_service.run();
        L_ERROR << "PlayerWorker [" << tid << "]: *** returned from io_Service.run() ***";
        L_ERROR << "PlayerWorker [" << tid << "]: *** player worker thread exiting ***";
    }

    void Start()
    {
        L_INFO << "PlayerWorker: spawning player worker thread";
        thread_ = boost::thread( boost::bind( &PlayerWorker::PlayerWorkerThreadMain, this) );
    }

    boost::thread & Thread()
    {
        return thread_;
    }

    private:
        boost::thread thread_;
};






/*_____________________________________________________________________________
 * Main Program
 *_____________________________________________________________________________
*/
int main(int argc, const char **argv)
{
    try
    {
        // get id of main thread 
        boost::thread::id thread_id_main = boost::this_thread::get_id();
        L_INFO << "Server [" << thread_id_main << "]: started";
        
        // start the command handling worker thread 
        L_INFO << "Server [" << thread_id_main << "]: starting command worker";
        CommandWorker commandWorker;
        commandWorker.Start();
        
        // start the player handling worker thread 
        L_INFO << "Server [" << thread_id_main << "]: starting player worker";
        PlayerWorker playerWorker;
        playerWorker.Start();
        
        L_INFO << "Server [" << thread_id_main << "]: servers started, joining children";

        // wait for all child threads to finish
        commandWorker.Thread().join();
        playerWorker.Thread().join();

    }
    catch (SystemException& e)
    {
        L_FATAL <<  e.Info() << "\n";
    }
    catch (DomainException& e)
    {
        L_FATAL <<  e.Info() << "\n";
    }

    L_INFO << "exiting with status 0";
    return 0;
}

