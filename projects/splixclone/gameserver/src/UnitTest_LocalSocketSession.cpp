
/*_____________________________________________________________________________
 * Unit test for LocalSocketSession class 
 *_____________________________________________________________________________
*/
 
#include "LocalSocketSession.hpp"
#include "CommandMessageHandler.hpp"
#include "Logging.hpp"

namespace {

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;


// Test fixture for LocalSocketSession class 
class LocalSocketSessionTest : public ::testing::Test {
 protected:
  LocalSocketSessionTest() {}
  virtual ~LocalSocketSessionTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for LocalSocketSession.
        
  boost::asio::io_service io_service;

};


//_____________________________________________________________________________
// Initial State: all 0 
TEST_F(LocalSocketSessionTest, InitialState) {
    LocalSocketSession lss(io_service); 
    TMSG("LocalSocketSession Initial State\n");
}


}  // namespace
