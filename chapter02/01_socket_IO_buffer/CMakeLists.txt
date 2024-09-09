cmake_minimum_required(VERSION 3.27)

project(chapter0201SocketIOBufferSize LANGUAGES CXX VERSION 0.0.1)

message("chapter02-01 socket IO Buffer size get")

add_executable(chapter0201SocketIOBufferSize)

target_sources(chapter0201SocketIOBufferSize PRIVATE
        main.cpp
)