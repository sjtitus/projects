/*_____________________________________________________________________________
 * Unit test for CircularMessageBuffer class 
 *_____________________________________________________________________________
*/
 
#include "CircularMessageBuffer.hpp"

namespace {

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;


// Test fixture for CircularMessageBuffer class 
class CircularMessageBufferTest : public ::testing::Test {
 protected:
  CircularMessageBufferTest() 
  {
  }
  virtual ~CircularMessageBufferTest() 
  {
  }
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for CircularMessageBuffer.
};


TEST_F(CircularMessageBufferTest, InitialState) {
    TMSG("CircularMessageBuffer init\n");
    CircularMessageBuffer c(1024);
    TMSG("Size 0\n");
    EXPECT_EQ(0,c.Size());
    TMSG("Capacity 1024\n");
    EXPECT_EQ(1024,c.Capacity());
}

TEST_F(CircularMessageBufferTest, PushBack) {
    TMSG("CircularMessageBuffer PushBack\n");
    CircularMessageBuffer c(3);
    c.PushBack("Abby");
    c.PushBack("Burt");
    c.PushBack("Carl");
    TMSG("Size 3\n");
    EXPECT_EQ(3,c.Size());
    
    bool p = c.PushBack("OneTooMany");
    TMSG("CircularMessageBuffer Push full\n");
    EXPECT_EQ(true,p);

    std::unique_ptr<std::string> pMsg = c.PopFront();
    TMSG("CircularMessageBuffer PopFront Burt\n");
    EXPECT_EQ("Burt",*pMsg);
    
    pMsg = c.PopFront();
    TMSG("CircularMessageBuffer PopFront Carl\n");
    EXPECT_EQ("Carl",*pMsg);
        
    pMsg = c.PopFront();
    TMSG("CircularMessageBuffer PopFront OneTooMany\n");
    EXPECT_EQ("OneTooMany",*pMsg);
    
    pMsg = c.PopFront();
    TMSG("CircularMessageBuffer PopFront empty\n");
    EXPECT_EQ(pMsg.get(),nullptr);
}

}  // namespace
