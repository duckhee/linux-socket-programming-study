cmake_minimum_required(VERSION 3.27)

project(chapter0602BroadCastSender LANGUAGES CXX VERSION 0.0.1)

message("chapter06-02 broad cast sender")

add_executable(chapter0602BroadCastSender)

target_sources(chapter0602BroadCastSender PRIVATE
        main.cpp
)