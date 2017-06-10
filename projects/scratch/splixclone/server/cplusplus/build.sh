#!/bin/bash

#echo "Building Logger"
#g++ -std=c++11 Logger.cpp -D_UNIT_TEST_SIMPLELOGGER_ -DLOGFILE=\"logfile_logger\" -lboost_system -lboost_log -lpthread -lboost_thread -lrt -o Logger

echo "Building server"
g++ -std=c++11 -DLOGFILE=\"logfile_server\" -D_UNIT_TEST_LOCALSOCKETSERVER_ -Wall LocalSocketSession.cpp Message.cpp Logger.cpp LocalSocketServer.cpp -o server -lboost_system -lboost_serialization -lboost_log -lpthread -lboost_thread -lrt

echo "Building client"
g++ -std=c++11 -D_UNIT_TEST_LOCALSOCKETSESSION_CLIENT_ -DLOGFILE=\"logfile_client\" -Wall LocalSocketSession.cpp Message.cpp Logger.cpp ClientMain.cpp -o client -lboost_system -lboost_serialization -lboost_log -lpthread -lboost_thread -lrt


