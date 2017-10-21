/*_____________________________________________________________________________
 * Unit tests for splixclone project
 * ____________________________________________________________________________
 */

#include "UnitTest.hpp"

#include "UnitTest_Game.cpp"

/*
#include "UnitTest_CircularMessageBuffer.cpp"
#include "UnitTest_WorkerThread.cpp"
#include "UnitTest_LocalSocketSession.cpp"
#include "UnitTest_Position.cpp"
#include "UnitTest_Board.cpp"
#include "UnitTest_Player.cpp"
#include "UnitTest_Array2D.cpp"
*/

int main(int argc, char **argv) {
  ::testing::InitGoogleTest(&argc, argv);
  return RUN_ALL_TESTS();
}


