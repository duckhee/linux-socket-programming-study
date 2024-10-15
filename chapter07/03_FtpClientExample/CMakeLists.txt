cmake_minimum_required(VERSION 3.27)

project(chapter0703FtpClientExample LANGUAGES CXX VERSION 0.0.1)

add_executable(chapter0703FtpClientExample)

message("chapter07-03 FTP Client Example")

target_sources(chapter0703FtpClientExample PRIVATE
        main.cpp
)