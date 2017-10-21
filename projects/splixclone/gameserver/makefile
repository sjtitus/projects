#______________________________________________________________________________
# gameserver
# The game server for my splix clone game. 
ifndef DEV_HOME
  $(error makefile must set variable DEV_HOME to the root of the dev env)
endif

#______________________________________________________________________________
# Where is make env located 
MAKE_ROOT = $(DEV_HOME)/tools/make

# detect architecture details
ifndef ARCH
include $(MAKE_ROOT)/makefile-archdetect
endif

#______________________________________________________________________________
# Project settings 
PROJECT_NAME = gameserver 
PROJECT_TYPE = exe
PROJECT_PLATFORMS = x86_64linux
USE_64 = 1
OUTPUT = bin/$(PROJECT_NAME)
CXXFLAGS = -std=c++11
INC_DIRS = include /usr/local/include
LIB_DIRS = lib /usr/local/lib
LIBS = m gtest pthread boost_system boost_thread rt

SRC_FILES = src/Board.cpp                   \
            src/Game.cpp                    \
            src/Array2D.cpp                 \
            src/Player.cpp                  \
            src/SimpleException.cpp         \
            src/CommandMessageHandler.cpp   \
            src/LocalSocketSession.cpp      \
            src/MessageHandler.cpp          \
            src/LocalSocketServer.cpp       \
            src/WorkerThread.cpp       		\
            src/CommandThread.cpp    		\
            src/PlayerMoveThread.cpp   		\
            src/GameThread.cpp       		\
            src/CircularMessageBuffer.cpp	\
 			src/Main.cpp
# 			src/UnitTest.cpp
# src/log4cxx_test.cpp
    
#______________________________________________________________________________
# Include the master makefile
# Defines targets 'all' and 'clean'
include $(MAKE_ROOT)/makefile-master
LDFLAGS += /usr/local/lib/liblog4cxx.a /usr/local/apr/lib/libapr-1.a /usr/local/apr/lib/libaprutil-1.a /usr/lib64/libexpat.a
