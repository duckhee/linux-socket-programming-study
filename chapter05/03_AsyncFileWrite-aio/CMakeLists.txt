cmake_minimum_required(VERSION 3.27)

project(chapter0503AsyncFileIO-AIO LANGUAGES CXX VERSION 0.0.1)

message("chapter05-02 async file IO -aio")

add_executable(chapter0503AsyncFileIO-AIO)

target_sources(chapter0503AsyncFileIO-AIO PRIVATE
        main.cpp
)