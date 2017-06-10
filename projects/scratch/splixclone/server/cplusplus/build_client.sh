#!/bin/bash
echo "Building client"
g++ -ggdb -std=c++11 -D_UNIT_TEST_LOCALSOCKETSESSION_CLIENT_ -DLOGFILE=\"logfile_client\" -Wall LocalSocketSession.cpp Logger.cpp ClientMain.cpp -o client -lboost_system -lboost_serialization -lboost_log -lpthread -lboost_thread -lrt


