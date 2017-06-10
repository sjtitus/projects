#ifndef _LOCALSOCKETSERVER_H_
#define _LOCALSOCKETSERVER_H_
/*_____________________________________________________________________________
 * 
 * LocalSocketServer
 *
 * Server that asynchronously handles communications for a specified Unix
 * domain socket. The server establishes a LocalSocketSession for each
 * incoming connection, and then passes ownership of the connected
 * session to a specified LocalSocketSessionHandler.  
 *_____________________________________________________________________________
*/

#include "Logger.hpp"
#include "Exception.hpp"
#include "LocalSocketSession.hpp"
#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/asio.hpp>   
    
//_____________________________________________________________________________
// LocalSocketServer
class LocalSocketServer
{
  public:
    
    //_________________________________________________________________________
    // Constructor
    LocalSocketServer(boost::asio::io_service& io_service, 
            const std::string& file, 
            LocalSocketSessionHandler &sessionHandler);

    //_________________________________________________________________________
    // Start: start accepting connections 
    void Start();
    
    //_________________________________________________________________________
    // Stop: stop accepting connections
    void Stop();

  private:
    //_________________________________________________________________________
    // AcceptHandler: async callback handling new incoming sessions. 
    void AcceptHandler(LocalSocketSession::Ptr &new_session,
            const boost::system::error_code& error);

  private:
    boost::asio::io_service& io_service_;
    stream_protocol::acceptor acceptor_;
    LocalSocketSessionHandler &sessionHandler_;
    std::string file_;
};

#endif

