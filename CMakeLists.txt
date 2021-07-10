cmake_minimum_required(VERSION 3.19)
project(Raft)

set(CMAKE_CXX_STANDARD 14)

include_directories(/Users/jingyan/Apps/boost_1_76_0)
link_directories(/Users/jingyan/Apps/boost_1_76_0/stage/lib)

add_executable(Raft test/TimerTest.cpp util/Client.cpp util/Client.h util/Session.cpp util/Session.h util/Server.cpp util/Server.h util/util.h util/Log.h util/Log.cpp state/ServerContext.cpp state/ServerContext.h storage/LogStore.cpp storage/LogStore.h state/AbstractState.cpp state/AbstractState.h util/timer.h)