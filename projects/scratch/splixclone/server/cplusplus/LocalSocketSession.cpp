/*_____________________________________________________________________________
 *
 *  LocalSocketSession
 *
 *  Session for asynchronously passing string messages over a Unix  
 *  domain (local) socket.
 *______________________________________________________________________________
*/
#include "LocalSocketSession.hpp"
#include "Logger.hpp"
#include "Exception.hpp"

#include <cstdio>
#include <iostream>
#include <boost/bind.hpp>
#include <boost/assert.hpp>
#include <string> 
 

    //__________________________________________________________________________
    // Constructors
    LocalSocketSession::LocalSocketSession( boost::asio::io_service& io_service ) 
        : socket_(io_service),
          pMessageHandler_(NULL)
    {
        L_DEBUG << "construct no handler:" << this;
    }
    
    LocalSocketSession::~LocalSocketSession()
    {
        L_DEBUG << "LocalSocketSession: destruct " << this;
    }
    
    //__________________________________________________________________________
    // Set the handler that provides async callbacks 
    void LocalSocketSession::SetMessageHandler( LocalSocketSession::MessageHandler::Ptr &pMessageHandler )
    {
        L_DEBUG << "setting handler = shared message handler: " << pMessageHandler.get();
        pMessageHandler_ = pMessageHandler;
    }


    //__________________________________________________________________________
    // Accessors 
    stream_protocol::socket& LocalSocketSession::Socket()
    {
        return socket_;
    }
    
    //__________________________________________________________________________
    // Close
    void LocalSocketSession::Close()
    {
        L_DEBUG << "shutting down and closing socket";
        socket_.shutdown(boost::asio::ip::tcp::socket::shutdown_both);
        socket_.close(); 
    }
   
    
    //__________________________________________________________________________
    // Connect: Sync connect to a server
    void LocalSocketSession::Connect(const std::string &localFile)
    {
        L_DEBUG << "sync connect to " << localFile;
        try 
        {
            socket_.connect(stream_protocol::endpoint(localFile));
        } 
        catch(std::exception &e) 
        {
            RETHROW_SYSTEM_EXCEPTION(e); 
        }
    } 


    //__________________________________________________________________________
    // ReadMesssage: Async read message. Reads from the socket until
    // the delimiter is seen (delimiter is part of received message).
    void LocalSocketSession::ReadMessage()
    {
        BOOST_ASSERT_MSG(pMessageHandler_,"*** MessageHandler must be defined ***");
        L_DEBUG << "asynch read message";
        try
        {
            boost::asio::async_read_until(
                socket_, 
                response_, 
                LocalSocketSession::DELIMITER,
                boost::bind(
                    &LocalSocketSession::ReadCallback, 
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                )
            );
        }
        catch(std::exception &e) 
        {
            RETHROW_SYSTEM_EXCEPTION(e); 
        }
        
    }


    //__________________________________________________________________________
    // ReadCallback: internal read callback
    void LocalSocketSession::ReadCallback(const boost::system::error_code& error,
                    std::size_t bytes_transferred)
    {
        L_DEBUG << "receiving message";
        if (error)
        {
            L_ERROR << "ERROR reading messaage: " << error.message();
            if (pMessageHandler_)
            { 
                L_DEBUG << "handling error";
                pMessageHandler_->HandleReadError(error);
            }
        }
        else if (bytes_transferred > 0) 
        {
            L_DEBUG << "read: " << bytes_transferred << " bytes";
            try 
            { 
                // read entire socket stream into a new message (string) 
                std::istream response_stream(&response_);
                std::unique_ptr<std::string> pMessage = std::unique_ptr<std::string>(
                    new std::string(std::istreambuf_iterator<char>(response_stream), {})
                );
                if (response_stream.fail())
                {
                    L_ERROR << "getline stream fail";
                } 
                if (response_stream.eof())
                {
                    L_ERROR << "getline stream eof";
                } 
            
                // invoke the message handler, passing message ownership
                L_DEBUG << "incoming message: '" <<  *pMessage << "'";
                if (pMessageHandler_)
                { 
                    L_DEBUG << "invoking incoming message handler";
                    pMessageHandler_->HandleRead(std::move(pMessage));
                }
            }
            catch(std::exception &e) 
            {
                RETHROW_SYSTEM_EXCEPTION(e); 
            }
        }
        else
        {
            L_DEBUG << "0 bytes transferred (socket closed?)";
        }
    }

    
    //__________________________________________________________________________
    // WriteMesssage: Async write message. Writes the specified string
    // message to the socket, appending the delimiter. 
    void LocalSocketSession::WriteMessage(const std::string &message)
    {
        BOOST_ASSERT_MSG(pMessageHandler_,"*** MessageHandler must be defined ***");
        L_DEBUG << "asynch write message, message='" << message << "'";
        try
        {
            // TODO: make sure message does not contain delimeter
            std::ostream request_stream(&request_);
            request_stream << message << LocalSocketSession::DELIMITER; 
            L_DEBUG << "queued message size: " << request_.size(); 

            boost::asio::async_write(
                socket_, 
                request_,
                boost::bind(
                    &LocalSocketSession::WriteCallback,
                    shared_from_this(),
                    boost::asio::placeholders::error,
                    boost::asio::placeholders::bytes_transferred
                )
            );
        }
        catch(std::exception &e) 
        {
            L_DEBUG << "error: asynch write message: throwing system exception";
            RETHROW_SYSTEM_EXCEPTION(e); 
        }
    }


    //__________________________________________________________________________
    // WriteCallback: invoked after a message is written to the socket
    void LocalSocketSession::WriteCallback(const boost::system::error_code& error, size_t bytes_transferred)
    {
        if (error)
        {
            L_ERROR << "ERROR during write: " << error.message();
            if (pMessageHandler_)
            {
                L_DEBUG << "handling error";
                pMessageHandler_->HandleWriteError(error);
            }
            //THROW_SYSTEM_EXCEPTION("ERROR writing message: " + error.message());
        }
        else if (bytes_transferred > 0)
        { 
            L_DEBUG << "written message size: " << bytes_transferred;
            // empty the write buffer? Seems like it's not needed..? 
            // request_.consume(request_.size()); 
            if (pMessageHandler_)
            { 
                L_DEBUG << "invoking write callback";
                pMessageHandler_->HandleWrite(bytes_transferred);
            }
        }
        else
        {
            L_DEBUG << "0 bytes transferred (socket closed?)";
        }
    }
    
    
#ifdef _UNIT_TEST_LOCALSOCKETSESSION_
int main(int argc, const char **argv)
{
    return 0;
}
#endif
