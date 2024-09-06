cmake_minimum_required(VERSION 3.27)

project(chapter0103SocketIOBufferSize LANGUAGES CXX VERSION 0.0.1)

message("chapter01-03 socket IO Buffer size get")

add_executable(chapter0103SocketIOBufferSize)

target_sources(chapter0103SocketIOBufferSize PRIVATE
        main.cpp
)