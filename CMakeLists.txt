cmake_minimum_required(VERSION 3.11.2)
# project(server)
project(log)
include_directories(base)
include_directories(net)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(server
                base/Timestamp.cpp
                base/Timestamp.h
                base/LogStream.cpp
                base/LogStream.h
                base/Logging.cpp
                base/Logging.h
                net/Buffer.cpp
                net/Buffer.h
                net/CallBacks.h
                net/Channel.cpp
                net/Channel.h
                net/Socket.cpp
                net/Socket.h
                net/Poller.cpp
                net/Poller.h
                net/EventLoop.cpp
                net/EventLoop.h
                net/Acceptor.cpp
                net/Acceptor.h
                net/TcpConnection.cpp
                net/TcpConnection.h
                net/TcpServer.cpp
                net/TcpServer.h
                test/echo.cpp
                test/echo.h
                test/main.cpp
                )          
