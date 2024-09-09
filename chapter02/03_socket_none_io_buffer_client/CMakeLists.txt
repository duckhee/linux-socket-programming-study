cmake_minimum_required(VERSION 3.27)

project(chapter0202SocketNoneIOBufferClient LANGUAGES CXX VERSION 0.0.1)

message("chapter02-02 socket none io buffer client")

add_executable(chapter0202SocketNoneIOBufferClient)

target_sources(chapter0202SocketNoneIOBufferClient PRIVATE
        main.cpp
)