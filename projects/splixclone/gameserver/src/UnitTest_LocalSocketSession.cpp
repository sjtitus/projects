
/*_____________________________________________________________________________
 * Unit test for LocalSocketSession class 
 *_____________________________________________________________________________
*/
 
#include "LocalSocketSession.hpp"
#include "LocalSocketServer.hpp"
#include "CommandMessageHandler.hpp"
#include "Logging.hpp"
#include "uds_client.hpp"


namespace com { namespace dimension3designs {


//_____________________________________________________________________________
// A test message handler for testing localsocketserver and localsocketsession.
// Will read an incoming message and save it, and will echo back the number
// of the message received.
class TestMessageHandler: public MessageHandler
{
    public:
        TestMessageHandler(boost::shared_ptr<LocalSocketSession> &pSession)
        :MessageHandler(pSession)
        {
            LOG4CXX_TRACE(logger_,"TestMessageHandler: construct: session " << pSession_.get());
        }
        
        virtual void HandleRead( std::unique_ptr<std::string> pMessage )
        {
            LOG4CXX_TRACE(logger_,"TestMessageHandler::HandleRead: session " << pSession_.get());
            lastMessage = *pMessage;
            pSession_->WriteMessage(std::string("received message ") + std::to_string(messageNumber++));
        }
        virtual void HandleWrite( size_t bytesWritten )
        {
            LOG4CXX_TRACE(logger_,"TestMessageHandler::HandleWrite: session " << pSession_.get());
            Close(); 
            //pSession_->ReadMessage();
        }
        virtual void HandleReadError( const boost::system::error_code& error )
        {
            LOG4CXX_TRACE(logger_,"TestMessageHandler:HandleReadError: session " << pSession_.get());
        }
        virtual void HandleWriteError( const boost::system::error_code& error )
        {
            LOG4CXX_TRACE(logger_,"TestMessageHandler::HandleWriteError: session " << pSession_.get());
        }
        virtual void Start()
        {
            LOG4CXX_TRACE(logger_,"TestMessageHandler::Start: session " << pSession_.get());
            started = true;
            pSession_->ReadMessage();
        }
        virtual void Close()
        { 
            LOG4CXX_TRACE(logger_,"TestMessageHandler::Close: session " << pSession_.get());
            pSession_->Close(); 
            pSession_.reset(); 
        }

    public:
        static bool started;
        static int messageNumber;
        static std::string lastMessage;
};

bool TestMessageHandler::started = false;
int TestMessageHandler::messageNumber = 0;
std::string TestMessageHandler::lastMessage;


}}


namespace {


using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;


// Test fixture for LocalSocketSession class 
class LocalSocketSessionTest : public ::testing::Test {
 protected:
  LocalSocketSessionTest()
    //:client("/tmp/LocalSocketServerTest")
    {}

  virtual ~LocalSocketSessionTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for LocalSocketSession.
        
  boost::asio::io_service io_service;
  //uds_client client;

};


//_____________________________________________________________________________
// Initial State
TEST_F(LocalSocketSessionTest, InitialState) {
    TMSG("LocalSocketSession Initial State\n");
    LocalSocketSession session(io_service); 
}

TEST_F(LocalSocketSessionTest, LocalSocketServer) {
    TMSG("LocalSocketServer\n");
    // set up a server using the test message handler 
    unlink("/tmp/LocalSocketServerTest");
    LocalSocketServer<TestMessageHandler> localServer(io_service,"/tmp/LocalSocketServerTest");
    // start the server 
    localServer.Start();
    // now local server is listening, create a client and connect to the server
    uds_client client("/tmp/LocalSocketServerTest");
    client.sconnect();
    // after server start, a readmessage() call is queued, so let's write a known message
    // to the server.   
    client.write_string("message1");
    // this run should cause a read of "message1" and a reply of "received message 0" 
    io_service.poll(); 
    EXPECT_EQ(TestMessageHandler::lastMessage,"message1\n");
    client.read_string();
    std::string rmsg(client.buf);
    EXPECT_EQ(rmsg,"received message 0\n");
}





}  // namespace
