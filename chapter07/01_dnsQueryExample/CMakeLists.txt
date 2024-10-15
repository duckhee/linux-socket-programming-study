cmake_minimum_required(VERSION 3.27)

project(chapter0701DNSQueryExample LANGUAGES CXX VERSION 0.0.1)

message("chapter07-01 DNS Query Example")

add_executable(chapter0701DNSQueryExample)

target_sources(chapter0701DNSQueryExample PRIVATE
        main.cpp
)