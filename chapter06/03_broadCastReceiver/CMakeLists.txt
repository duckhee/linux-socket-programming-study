cmake_minimum_required(VERSION 3.27)

project(chapter0603BroadCastReceiver LANGUAGES CXX VERSION 0.0.1)

message("chapter06-02 broad cast receiver")

add_executable(chapter0603BroadCastReceiver)

target_sources(chapter0603BroadCastReceiver PRIVATE
        main.cpp
)