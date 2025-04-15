cmake_minimum_required(VERSION 3.10)
project(NetworkProject)

set(CMAKE_CXX_STANDARD 17)

add_executable(Server
    main/ServerMain.cpp
    src/net/Server.cpp
    src/net/SocketWrapper.cpp
)

add_executable(Client
    main/ClientMain.cpp
    src/net/Client.cpp
    src/net/SocketWrapper.cpp
)

target_include_directories(Server PRIVATE include)
target_include_directories(Client PRIVATE include)

if(WIN32)
    target_link_libraries(Server PRIVATE ws2_32)
    target_link_libraries(Client PRIVATE ws2_32)
endif()
