
/*_____________________________________________________________________________
 * Unit test for Array2D class 
 *_____________________________________________________________________________
*/
 
#include "Array2D.hpp"

namespace {


// Test fixture for Array2D class 
class Array2DTest : public ::testing::Test {
 protected:
  Array2DTest() {}
  virtual ~Array2DTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for Array2D.
};


//_____________________________________________________________________________
// Initial State: all 0 
TEST_F(Array2DTest, InitialState) {
    Array2D<uint8_t> ary(1000,5000);
    TMSG("Initial width, height = 1000,5000\n");
    EXPECT_EQ(1000,ary.height());
    EXPECT_EQ(5000,ary.width());
    TMSG("Initial fill with 0\n");
    EXPECT_EQ(0,ary(0,0));
    EXPECT_EQ(0,ary(999,4999));
    EXPECT_EQ(0,ary(500,2500));
}

TEST_F(Array2DTest, ContiguousMemory) {
    Array2D<uint8_t> ary(100,500);
    EXPECT_EQ(100,ary.height());
    EXPECT_EQ(500,ary.width());

    TMSG("Contiguous memory 100x500\n");
    uint8_t *pByte = &(ary(0,0));
    for (size_t r=0; r<ary.height(); r++)
        for (size_t c=0; c<ary.width(); c++)
        {
            EXPECT_EQ(pByte,&(ary(r,c)));
            pByte++;
        }

}


}  // namespace
