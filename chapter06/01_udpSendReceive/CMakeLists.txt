cmake_minimum_required(VERSION 3.27)

project(chapter0601UDPSendReceive LANGUAGES CXX VERSION 0.0.1)

message("chapter06-01 UDP sendto recvfrom")

add_executable(chapter0601UDPSendReceive)

target_sources(chapter0601UDPSendReceive PRIVATE
        main.cpp
)
