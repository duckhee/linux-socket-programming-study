cmake_minimum_required(VERSION 3.27)

project(chapter0102EchoClient LANGUAGES C VERSION 0.0.1)

message("chapter01-02 echo client")

add_executable(chapter0102EchoClient)

target_sources(chapter0102EchoClient PRIVATE
        main.c
)