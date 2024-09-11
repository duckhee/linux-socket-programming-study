cmake_minimum_required(VERSION 3.27)

project(chapter0207MultiThreadEchoClient LANGUAGES CXX VERSION 0.0.1)

message("chapter02-04 multi thread echo client")

add_executable(chapter0207MultiThreadEchoClient)

target_sources(chapter0207MultiThreadEchoClient PRIVATE
        main.cpp
)