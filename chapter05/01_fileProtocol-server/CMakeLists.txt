cmake_minimum_required(VERSION 3.27)

project(chapter0501FileProtocolServer LANGUAGES CXX VERSION 0.0.1)

message("chapter05-01 file protocol server")

add_executable(chapter0501FileProtocolServer)

target_sources(chapter0501FileProtocolServer PRIVATE
        AppProtocol.h
        main.cpp
)