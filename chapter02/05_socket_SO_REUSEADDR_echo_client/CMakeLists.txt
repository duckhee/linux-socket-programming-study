cmake_minimum_required(VERSION 3.27)

project(chapter0204SocketSO_REUSEADDR_Client LANGUAGES CXX VERSION 0.0.1)

message("chapter02-03 Socket SO_REUSEADDR Echo Client")

add_executable(chapter0204SocketSO_REUSEADDR_Client)

target_sources(chapter0204SocketSO_REUSEADDR_Client PRIVATE
    main.cpp
)