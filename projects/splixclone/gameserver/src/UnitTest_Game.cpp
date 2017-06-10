
/*_____________________________________________________________________________
 * Unit test for Game class 
 *_____________________________________________________________________________
*/
 
#include "Game.hpp"

namespace {


// Test fixture for Game class 
class GameTest : public ::testing::Test {
 protected:
  GameTest() {}
  virtual ~GameTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for Game.
};


//_____________________________________________________________________________
// Initial State: all 0 
TEST_F(GameTest, InitialState) {
    Game g(1000,5000);
    TMSG("Game with 1000 x 5000 board\n");
    Board *b = g.GetBoard(); 
    EXPECT_EQ(1000,b->width());
    EXPECT_EQ(5000,b->height());
}


}  // namespace
