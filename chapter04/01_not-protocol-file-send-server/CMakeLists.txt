cmake_minimum_required(VERSION 3.27)

project(chapter0401NotProtocolFileSendServer LANGUAGES CXX VERSION 0.0.1)

message("chapter04-01 not protocol use file send server")

add_executable(chapter0401NotProtocolFileSendServer)

target_sources(chapter0401NotProtocolFileSendServer PRIVATE
        main.cpp
)