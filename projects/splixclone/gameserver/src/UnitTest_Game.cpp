
/*_____________________________________________________________________________
 * Unit test for Game class 
 *_____________________________________________________________________________
*/
 
#include "Game.hpp"
#include "Logging.hpp"
#include "uds_client.hpp"

namespace {

using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;


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


TEST_F(GameTest, AddPlayer) {
    Game g(1000,5000);
    auto id = g.AddPlayer("stitus"); 
    EXPECT_EQ(1,g.NumPlayers());
    std::vector<std::string> pList = g.FindPlayers("stitus");
    EXPECT_EQ(1,pList.size());
    EXPECT_EQ(id,pList[0]);
}

TEST_F(GameTest, RemovePlayer) {
    Game g(1000,5000);
    auto a = g.AddPlayer("abby"); 
    auto b = g.AddPlayer("bernie"); 
    auto c = g.AddPlayer("carl"); 
    auto a2 = g.AddPlayer("abby"); 
    EXPECT_EQ(4,g.NumPlayers());
    std::vector<std::string> pList = g.FindPlayers("abby");
    EXPECT_EQ(2,pList.size());
    EXPECT_EQ(true,pList[0]==a || pList[1]==a);
    EXPECT_EQ(true,pList[0]==a2 || pList[1]==a2);
    g.RemovePlayer(c);
    EXPECT_EQ(3,g.NumPlayers());
    pList = g.FindPlayers("carl");
    EXPECT_EQ(0,pList.size());
    g.RemovePlayer(a);
    g.RemovePlayer(b);
    g.RemovePlayer(a2);
    EXPECT_EQ(0,g.NumPlayers());
}


TEST_F(GameTest, Start) {
    Game g(1000,5000);
    g.Start(); 
    boost::this_thread::sleep_for(boost::chrono::seconds(4)); 
    TMSG("Sending a message to command socket\n");
    uds_client client("/tmp/splixclone_command_socket");
    client.sconnect();
    client.write_string("command1");
    TMSG("Waiting prior to stopping\n");
    boost::this_thread::sleep_for(boost::chrono::seconds(6)); 
    g.Stop(); 
    boost::this_thread::sleep_for(boost::chrono::seconds(1)); 
}



}  // namespace
