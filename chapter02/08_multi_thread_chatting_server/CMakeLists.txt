cmake_minimum_required(VERSION 3.27)

project(chapter0208MultiThreadChattingServer LANGUAGES CXX VERSION 0.0.1)

message("chapter02-05 Multi Thread Chatting Server")

add_executable(chapter0208MultiThreadChattingServer)

target_sources(chapter0208MultiThreadChattingServer PRIVATE
        main.cpp
)