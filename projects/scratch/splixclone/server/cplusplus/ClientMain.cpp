#include "Logger.hpp"
#include "Exception.hpp"
#include "LocalSocketSession.hpp"
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   
#include <memory>

#include <boost/thread.hpp>
#include <boost/thread/mutex.hpp> 
#include <boost/format.hpp>

using boost::format;


class ClientHandler:  public LocalSocketSessionHandler
{
  public:
    void HandleRead( std::unique_ptr<std::string> pMessage )
    {
        L_INFO << "ClientHandler: message received: '" << *pMessage << "'";
    }
    void HandleWrite( size_t bytesWritten )
    {
        L_INFO << "ClientHandler: message written: " << bytesWritten << " bytes";
        L_INFO << "ClientHandler: reading response";
        pSession_->ReadMessage();
    }
    void HandleReadError( const boost::system::error_code& error )
    {
        L_INFO << "ClientHandler: read error: " << error.message();
    }
    void HandleWriteError( const boost::system::error_code& error )
    {
        L_INFO << "ClientHandler: write error: " << error.message();
    }
    void Start()
    {
        std::string message("Hello, client world!");
        pSession_->WriteMessage(message);
    }
};



class Worker 
{
    public:
    
    void ThreadMain(const char *sname)
    {
        boost::asio::io_service io_service;

        boost::thread::id id = boost::this_thread::get_id();
        L_INFO << "new worker threadmain: " << id; 
        
        LocalSocketSession::Ptr pSession(new LocalSocketSession(io_service));
        L_INFO << "connecting to " << sname;
        pSession->Connect(sname);
       
        // client handler handles the new session 
        ClientHandler handler; 
        handler.HandleSession(pSession);
        
        L_INFO << "running IO service";
        io_service.run();
        L_INFO << "IO service done";
    }

    void StartThread(const char *sname)
    {
        L_INFO << "worker starting thread";
        thread_ = boost::thread( boost::bind( &Worker::ThreadMain, this, sname) );
    }

    public:
        boost::thread thread_;
};



int main(int argc, const char **argv)
{
  try
  {
    L_INFO << "client main start";
    
    if (argc != 2)
    {
      std::cerr << "Usage: client <file>\n";
      std::cerr << "*** WARNING: existing file is removed ***\n";
      return 1;
    }
    
    Worker w;
    L_INFO << "starting worker thread";
    w.StartThread(argv[1]);
    
    // blocking call
    L_INFO << "joining worker";
    w.thread_.join();
    L_INFO << "done";

  }
  catch (SystemException& e)
  {
    L_FATAL <<  "Caught SystemException: " << e.Info() << "\n";
  }
  catch (DomainException& e)
  {
    L_FATAL <<  "Caught DomainException: " << e.Info() << "\n";
  }

  return 0;
}

