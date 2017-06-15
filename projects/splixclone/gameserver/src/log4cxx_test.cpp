#include <log4cxx/logger.h>
#include <log4cxx/basicconfigurator.h>
#include <log4cxx/helpers/exception.h>
#include <log4cxx/ndc.h>
 
using namespace log4cxx;
using namespace log4cxx::helpers;
 
LoggerPtr logger(Logger::getLogger("log4cxx_test"));
 
int main(int argc, char **argv)
{
    int result = EXIT_SUCCESS;
    try
    {
        // Set up a simple configuration that logs on the console.
        //BasicConfigurator::configure();
        LOG4CXX_INFO(logger, "Entering application.")
        {
            NDC::push("block1");
            LOG4CXX_TRACE(logger, "Doing something.")
            NDC::pop();
        }
        LOG4CXX_INFO(logger, "Exiting application.")
    }
    catch(Exception&)
    {
        result = EXIT_FAILURE;
    }

    NDC::remove(); 
    return result;
}
