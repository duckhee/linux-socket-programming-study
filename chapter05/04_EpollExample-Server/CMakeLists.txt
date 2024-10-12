cmake_minimum_required(VERSION 3.27)

project(chapter0504EpollExampleServer LANGUAGES CXX VERSION 0.0.1)

message("chapter05-04 epoll example server")

add_executable(chapter0504EpollExampleServer)

target_sources(chapter0504EpollExampleServer PRIVATE
        main.cpp
)