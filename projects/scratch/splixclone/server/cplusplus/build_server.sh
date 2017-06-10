#!/bin/bash

echo "Building server"
#g++ -std=c++11 -DLOGFILE=\"logfile_server\" -D_UNIT_TEST_LOCALSOCKETSERVER_ -Wall -ggdb LocalSocketSession.cpp Message.cpp Logger.cpp LocalSocketServer.cpp CommandMessageHandler.cpp PlayerMessageHandler.cpp CircularMessageBuffer.cpp Player.cpp -o server -lboost_system -lboost_serialization -lboost_log -lpthread -lboost_thread -lrt
g++ -std=c++11 -DLOGFILE=\"logfile_server\" -Wall -ggdb LocalSocketSession.cpp Message.cpp Logger.cpp LocalSocketServer.cpp CommandMessageHandler.cpp PlayerMessageHandler.cpp CircularMessageBuffer.cpp ServerMain.cpp -o server -lboost_system -lboost_serialization -lboost_log -lpthread -lboost_thread -lrt
