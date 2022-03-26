cmake_minimum_required(VERSION 3.11.2)
project(server)
include_directories(base)
include_directories(net)

SET(CMAKE_BUILD_TYPE "Debug")
SET(CMAKE_CXX_FLAGS_DEBUG "$ENV{CXXFLAGS} -O0 -Wall -g2 -ggdb -pthread")
SET(CMAKE_CXX_FLAGS_RELEASE "$ENV{CXXFLAGS} -O3 -Wall")

add_executable(httpserver
                base/FileUtil.h
                base/FileUtil.cpp
                base/LogFile.h
                base/LogFile.cpp
                base/Timestamp.cpp
                base/Timestamp.h
                base/LogStream.cpp
                base/LogStream.h
                base/AsyncLogging.h
                base/AsyncLogging.cpp
                base/Logging.cpp
                base/Logging.h
                base/Thread.h
                base/Thread.cpp
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
                net/EventLoopThread.h
                net/EventLoopThread.cpp
                net/EventLoopThreadPool.h
                net/EventLoopThreadPool.cpp
                net/TcpServer.cpp
                net/TcpServer.h
                http/HttpRequest.h
                http/HttpResponse.h
                http/HttpPraser.h
                http/HttpPraser.cpp
                http/HttpServer.h
                http/HttpServer.cpp
                http/testt.cpp
                )          
