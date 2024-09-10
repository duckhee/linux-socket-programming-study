cmake_minimum_required(VERSION 3.27)

project(chapter0204SocketSO_REUSEADDR_echo_server LANGUAGES CXX VERSION 0.0.1)

message("chapter02-03 SO_REUSEADDR Socket Echo Server")

add_executable(chapter0204SocketSO_REUSEADDR_echo_server)

target_sources(chapter0204SocketSO_REUSEADDR_echo_server PRIVATE
    main.cpp
)