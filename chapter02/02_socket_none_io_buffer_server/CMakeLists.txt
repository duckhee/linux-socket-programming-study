cmake_minimum_required(VERSION 3.27)

project(chapter0202SocketNoneIOBufferServer LANGUAGES CXX VERSION 0.0.1)

message("chapter02-02 socket use none IO Buffer server -> Kernel Socket IO Buffer")

add_executable(chapter0202SocketNoneIOBufferServer)

target_sources(chapter0202SocketNoneIOBufferServer PRIVATE
        main.cpp
)