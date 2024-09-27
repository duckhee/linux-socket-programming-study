cmake_minimum_required(VERSION 3.27)

project(chapter0402NotProtocolFileSendClient LANGUAGES CXX VERSION 0.0.1)

message("chapter04-01 not protocol file send client")

add_executable(chapter0402NotProtocolFileSendClient)


target_sources(chapter0402NotProtocolFileSendClient PRIVATE
        main.cpp
)