cmake_minimum_required(VERSION 3.27)

project(chapter0209MultiThreadChattingClient LANGUAGES CXX VERSION 0.0.1)

message("chapter02-05 Multi Thread Chatting Client")

add_executable(chapter0209MultiThreadChattingClient)

target_sources(chapter0209MultiThreadChattingClient PRIVATE
        main.cpp
)