cmake_minimum_required(VERSION 3.27)

project(chapter0206MultiThreadEchoServer LANGUAGES CXX VERSION 0.0.1)

message("chapter02-04 multi thread echo server")

add_executable(chapter0206MultiThreadEchoServer)

target_sources(chapter0206MultiThreadEchoServer PRIVATE
        main.cpp
)