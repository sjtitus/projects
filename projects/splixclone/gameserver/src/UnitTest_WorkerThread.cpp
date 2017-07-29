
/*_____________________________________________________________________________
 * Unit test for WorkerThread class 
 *_____________________________________________________________________________
*/
#include "WorkerThread.hpp"
#include "CommandThread.hpp"


namespace com { namespace dimension3designs {


namespace {


using namespace log4cxx;
using namespace log4cxx::helpers;
using namespace com::dimension3designs;


// Test fixture for WorkerThread class 
class WorkerThreadTest : public ::testing::Test {
 protected:
  WorkerThreadTest()
    {}

  virtual ~WorkerThreadTest() {}
  virtual void SetUp() {}
  virtual void TearDown() {}
  // Objects declared here can be used by all tests in the test case for WorkerThread.
};


//_____________________________________________________________________________
// Initial State
TEST_F(WorkerThreadTest, InitialState) {
    TMSG("WorkerThread Initial State\n");
    WorkerThread testThread("Bob");
    testThread.Start();
    testThread.Thread().join();
}

TEST_F(WorkerThreadTest, CommandThreadInitialState) {
    TMSG("CommandThread Initial State\n");
    CommandThread testThread("Command Bob");
    testThread.Start();
    boost::this_thread::sleep_for(boost::chrono::seconds(10)); 
    TMSG("Stopping\n");
    testThread.Stop();
    boost::this_thread::sleep_for(boost::chrono::seconds(1)); 
}

}  // namespace

}}

