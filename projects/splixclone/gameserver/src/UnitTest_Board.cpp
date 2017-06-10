
/*_____________________________________________________________________________
 * Unit test for Board class 
 *_____________________________________________________________________________
*/
 
#include "Board.hpp"

namespace {


// Test fixture for Board class 
class BoardTest : public ::testing::Test {
 protected:
  BoardTest() {}
  virtual ~BoardTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for Board.
};


//_____________________________________________________________________________
// Initial State: all 0 
TEST_F(BoardTest, InitialState) {
    Board b(1000,5000);
    TMSG("Initial width, height = 1000,5000\n");
    EXPECT_EQ(1000,b.width());
    EXPECT_EQ(5000,b.height());
}

//_____________________________________________________________________________
// Accessors 
TEST_F(BoardTest, Accessors) {
    Board b(1000,5000);
    TMSG("Accessors\n");
    uint32_t w = b.width();
    uint32_t h = b.height();
    EXPECT_EQ(1000,w);
    EXPECT_EQ(5000,h);
}



}  // namespace
