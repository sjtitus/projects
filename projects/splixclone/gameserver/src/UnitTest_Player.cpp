
/*_____________________________________________________________________________
 * Unit test for Player class 
 *_____________________________________________________________________________
*/
 
#include "Player.hpp"
#include "Logging.hpp"
#include <vector>

namespace {

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;


// Test fixture for Player class 
class PlayerTest : public ::testing::Test {
 protected:
  PlayerTest() 
  {
    b = new Board(10,5);
  }
  virtual ~PlayerTest() 
  {
    delete b;
  }
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for Player.
  Board *b;
};


//_____________________________________________________________________________
// Initial State: all 0 
TEST_F(PlayerTest, Container) {
    Player p("stitus",b);
    std::vector<Player> playerVector;
    playerVector.push_back(std::move(p));
}

TEST_F(PlayerTest, InitialState) {
    Player p("stitus",b);
    TMSG("Player init position\n");
    EXPECT_EQ(0x80,p.GetPositionX());
    EXPECT_EQ(0x80,p.GetPositionY());
    EXPECT_EQ(0,p.GetPositionIntX());
    EXPECT_EQ(0,p.GetPositionIntY());
}


TEST_F(PlayerTest, WalkLeftToRight) {
    Player player("stitus",b);
    player.SetPosition(0,0); 
    TMSG("Player walk left to right\n");
    for (uint32_t i=0x1; i<=0x9FF; i++)
    {
        player.MoveRight(1); 
        EXPECT_EQ(i,player.GetPositionX());
        if      (i<0x100)       { EXPECT_EQ(0,player.GetPositionIntX()); }
        else if (i<0x200)       { EXPECT_EQ(1,player.GetPositionIntX()); }
        else if (i<0x300)       { EXPECT_EQ(2,player.GetPositionIntX()); }
        else if (i<0x400)       { EXPECT_EQ(3,player.GetPositionIntX()); }
        else if (i<0x500)       { EXPECT_EQ(4,player.GetPositionIntX()); }
        else if (i<0x600)       { EXPECT_EQ(5,player.GetPositionIntX()); }
        else if (i<0x700)       { EXPECT_EQ(6,player.GetPositionIntX()); }
        else if (i<0x800)       { EXPECT_EQ(7,player.GetPositionIntX()); }
        else if (i<0x900)       { EXPECT_EQ(8,player.GetPositionIntX()); }
        else if (i<=0x9FF)      { EXPECT_EQ(9,player.GetPositionIntX()); }
    }
    // we should be up against the far right boundary
    player.MoveRight(1); 
    EXPECT_EQ(0x9FF,player.GetPositionX());
    EXPECT_EQ(9,player.GetPositionIntX());
    player.MoveRight(100); 
    EXPECT_EQ(0x9FF,player.GetPositionX());
    EXPECT_EQ(9,player.GetPositionIntX());
    
    TMSG("Player walk back right to left\n");
    for (uint32_t i=0x9FE; i>0; i--)
    {
        player.MoveLeft(1); 
        EXPECT_EQ(i,player.GetPositionX());
        if      (i<0x100)       { EXPECT_EQ(0,player.GetPositionIntX()); }
        else if (i<0x200)       { EXPECT_EQ(1,player.GetPositionIntX()); }
        else if (i<0x300)       { EXPECT_EQ(2,player.GetPositionIntX()); }
        else if (i<0x400)       { EXPECT_EQ(3,player.GetPositionIntX()); }
        else if (i<0x500)       { EXPECT_EQ(4,player.GetPositionIntX()); }
        else if (i<0x600)       { EXPECT_EQ(5,player.GetPositionIntX()); }
        else if (i<0x700)       { EXPECT_EQ(6,player.GetPositionIntX()); }
        else if (i<0x800)       { EXPECT_EQ(7,player.GetPositionIntX()); }
        else if (i<0x900)       { EXPECT_EQ(8,player.GetPositionIntX()); }
        else if (i<=0x9FF)      { EXPECT_EQ(9,player.GetPositionIntX()); }
    }
    player.MoveLeft(1); 
    // we should be up against the left boundary
    EXPECT_EQ(0,player.GetPositionX());
    EXPECT_EQ(0,player.GetPositionIntX());
    player.MoveLeft(1); 
    EXPECT_EQ(0,player.GetPositionX());
    EXPECT_EQ(0,player.GetPositionIntX());
    player.MoveLeft(100); 
    EXPECT_EQ(0,player.GetPositionX());
    EXPECT_EQ(0,player.GetPositionIntX());
}


TEST_F(PlayerTest, WalkTopToBottom) {
    Player player("stitus",b);
    player.SetPosition(0,0); 
    TMSG("Player walk top to bottom\n");
    for (uint32_t i=0x1; i<=0x4FF; i++)
    {
        player.MoveDown(1); 
        EXPECT_EQ(i,player.GetPositionY());
        if      (i<0x100)       { EXPECT_EQ(0,player.GetPositionIntY()); }
        else if (i<0x200)       { EXPECT_EQ(1,player.GetPositionIntY()); }
        else if (i<0x300)       { EXPECT_EQ(2,player.GetPositionIntY()); }
        else if (i<0x400)       { EXPECT_EQ(3,player.GetPositionIntY()); }
        else if (i<=0x4FF)      { EXPECT_EQ(4,player.GetPositionIntY()); }
    }
    // we should be up against the bottom boundary
    player.MoveDown(1); 
    EXPECT_EQ(0x4FF,player.GetPositionY());
    EXPECT_EQ(4,player.GetPositionIntY());
    player.MoveDown(100); 
    EXPECT_EQ(0x4FF,player.GetPositionY());
    EXPECT_EQ(4,player.GetPositionIntY());
    
    TMSG("Player walk bottom to top\n");
    for (uint32_t i=0x4FE; i>0; i--)
    {
        player.MoveUp(1); 
        EXPECT_EQ(i,player.GetPositionY());
        if      (i<0x100)       { EXPECT_EQ(0,player.GetPositionIntY()); }
        else if (i<0x200)       { EXPECT_EQ(1,player.GetPositionIntY()); }
        else if (i<0x300)       { EXPECT_EQ(2,player.GetPositionIntY()); }
        else if (i<0x400)       { EXPECT_EQ(3,player.GetPositionIntY()); }
        else if (i<=0x4FF)      { EXPECT_EQ(4,player.GetPositionIntY()); }
    }
    player.MoveUp(1); 
    // we should be up against the top boundary
    EXPECT_EQ(0,player.GetPositionY());
    EXPECT_EQ(0,player.GetPositionIntY());
    player.MoveUp(1); 
    EXPECT_EQ(0,player.GetPositionY());
    EXPECT_EQ(0,player.GetPositionIntY());
    player.MoveUp(100); 
    EXPECT_EQ(0,player.GetPositionY());
    EXPECT_EQ(0,player.GetPositionIntY());
}



}  // namespace
