
/*_____________________________________________________________________________
 * Unit test for Position class
 *_____________________________________________________________________________
*/
 
#include "Position.hpp"

namespace {


// Test fixture for Position class 
class PositionTest : public ::testing::Test {
 protected:
  PositionTest() {
    //std::cout << "PositionTest Constructor\n";
  }

  virtual ~PositionTest() {
    //std::cout << "PositionTest Destructor\n";
  }

  virtual void SetUp() {
    //std::cout << "PositionTest SetUp\n";
  }

  virtual void TearDown() {
    //std::cout << "PositionTest TearDown\n";
  }

  // Objects declared here can be used by all tests in the test case for Position.
};


//_____________________________________________________________________________
// Initial State: all 0 
TEST_F(PositionTest, InitialState) {
    Position f;
    TMSG("Initial x = 0x80\n");
    EXPECT_EQ(0x80, f.x());
    TMSG("Initial y = 0x80\n");
    EXPECT_EQ(0x80, f.y());
    TMSG("Initial int_x = 0\n");
    EXPECT_EQ(0, f.int_x());
    TMSG("Initial int_y = 0\n");
    EXPECT_EQ(0, f.int_y());
    
    TMSG("Init x,y to 0xffffffff,0xffffffff\n");
    Position g(0xffffffff,0xffffffff);
    EXPECT_EQ(0xffffffff, g.x());
    EXPECT_EQ(0xffffffff, g.y());
    EXPECT_EQ(0x00ffffff, g.int_x());
    EXPECT_EQ(0x00ffffff, g.int_y());
}
    

//_____________________________________________________________________________
// Set Position 
TEST_F(PositionTest, SetPosition) {
    Position g;
    
    TMSG("Set x,y to 0xffffffff,0xffffffff\n");
    g.set_x(0xffffffff);
    g.set_y(0xffffffff);
    EXPECT_EQ(0xffffffff, g.x());
    EXPECT_EQ(0xffffffff, g.y());
    EXPECT_EQ(0x00ffffff, g.int_x());
    EXPECT_EQ(0x00ffffff, g.int_y());
    
    TMSG("Set int_x,int_y\n");
    g.set_int_x(15);
    EXPECT_EQ(15, g.int_x());
    EXPECT_EQ((15<<8)+0x80, g.x());
    g.set_int_y(23);
    EXPECT_EQ(23, g.int_y());
    EXPECT_EQ((23<<8)+0x80, g.y());
    g.set_int_x(15);
    g.move_x(0x00000100,false);
    EXPECT_EQ(16, g.int_x());
    g.move_x(0x00000100,true);
    EXPECT_EQ(15, g.int_x());
}



//_____________________________________________________________________________
// Move up
TEST_F(PositionTest, MoveUp) {
    Position f;
    
    TMSG("Move up x,y = 1/0\n");
    f.move_x(1,false);
    EXPECT_EQ(0x81, f.x());
    EXPECT_EQ(0, f.int_x());
    f.move_y(1,false);
    EXPECT_EQ(0x81, f.y());
    EXPECT_EQ(0, f.int_y());
    
    TMSG("Move up x,y by 254 = 255/0\n");
    for (int i=0; i<254; i++)
    {
        f.move_x(1,false);
        f.move_y(1,false);
    }
    EXPECT_EQ(0x81+254, f.x());
    EXPECT_EQ(1, f.int_x());
    EXPECT_EQ(0x81+254, f.y());
    EXPECT_EQ(1, f.int_y());
    
    TMSG("Clamp x,y to 0xffffffff/0x00ffffff\n");
    Position g(0xffffffff,0xffffffff);
    EXPECT_EQ(0xffffffff, g.x());
    EXPECT_EQ(0xffffffff, g.y());
    EXPECT_EQ(0x00ffffff, g.int_x());
    EXPECT_EQ(0x00ffffff, g.int_y());
    g.move_x(1,false);
    g.move_y(1,false);
    EXPECT_EQ(0xffffffff, g.x());
    EXPECT_EQ(0xffffffff, g.y());
    EXPECT_EQ(0x00ffffff, g.int_x());
    EXPECT_EQ(0x00ffffff, g.int_y());
    g.move_x(10,false);
    g.move_y(10,false);
    EXPECT_EQ(0xffffffff, g.x());
    EXPECT_EQ(0xffffffff, g.y());
    EXPECT_EQ(0x00ffffff, g.int_x());
    EXPECT_EQ(0x00ffffff, g.int_y());
}


//_____________________________________________________________________________
// Move down 
TEST_F(PositionTest, MoveDown) {
    Position f(0,0);
    
    TMSG("Clamp x,y to 0/0\n");
    f.move_x(1,true);
    f.move_y(1,true);
    EXPECT_EQ(0, f.x());
    EXPECT_EQ(0, f.int_x());
    EXPECT_EQ(0, f.y());
    EXPECT_EQ(0, f.int_y());
    f.move_x(10,true);
    f.move_y(10,true);
    EXPECT_EQ(0, f.x());
    EXPECT_EQ(0, f.int_x());
    EXPECT_EQ(0, f.y());
    EXPECT_EQ(0, f.int_y());

    TMSG("Up 512, down 256 = 256/1\n");
    f.move_x(512,false);
    f.move_x(256,true);
    f.move_y(512,false);
    f.move_y(256,true);
    EXPECT_EQ(256, f.x());
    EXPECT_EQ(1, f.int_x());
    EXPECT_EQ(256, f.y());
    EXPECT_EQ(1, f.int_y());
}



}  // namespace
