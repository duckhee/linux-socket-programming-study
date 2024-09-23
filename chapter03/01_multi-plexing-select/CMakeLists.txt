cmake_minimum_required(VERSION 3.27)

project(chapter0301MultiPlexingChattingServer LANGUAGES CXX VERSION 0.0.1)

message("chapter03-01 multi plexing chatting server using select")

add_executable(chapter0301MultiPlexingChattingServer)

target_sources(chapter0301MultiPlexingChattingServer PRIVATE
        main.cpp
)