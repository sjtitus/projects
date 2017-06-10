#ifndef _SIMPLELOGGER_H_ 
#define _SIMPLELOGGER_H_ 

//#define BOOST_LOG_DYN_LINK 1 // necessary when linking the boost_log library dynamically

#include <boost/log/trivial.hpp>
#include <boost/log/sources/global_logger_storage.hpp>
#include <iostream>
#include <iomanip>

// the logs are also written to LOGFILE

// just log messages with severity >= SEVERITY_THRESHOLD are written
#define SEVERITY_THRESHOLD logging::trivial::debug

// register a global logger
BOOST_LOG_GLOBAL_LOGGER(logger, boost::log::sources::severity_logger_mt<boost::log::trivial::severity_level>)

// just a helper macro used by the macros below - don't use it in your code
#define LOG(severity) BOOST_LOG_SEV(logger::get(),boost::log::trivial::severity)

// ===== log macros =====
#define LOG_TRACE   LOG(trace)
#define LOG_DEBUG   LOG(debug)
#define LOG_INFO    LOG(info)
#define LOG_WARNING LOG(warning)
#define LOG_ERROR   LOG(error)
#define LOG_FATAL   LOG(fatal)
        
#define L_TRACE   LOG_TRACE << std::setw(24) << __FUNCTION__ << ": "
#define L_DEBUG   LOG_DEBUG << std::setw(24) <<  __FUNCTION__ << ": "
#define L_INFO    LOG_INFO << std::setw(24) << __FUNCTION__ << ": "
#define L_WARNING LOG_WARNING << std::setw(24) << __FUNCTION__ << ": "
#define L_ERROR   LOG_ERROR << std::setw(24) << __FUNCTION__ << ": "
#define L_FATAL   LOG_FATAL << std::setw(24) << __FUNCTION__ << ": "

#endif
