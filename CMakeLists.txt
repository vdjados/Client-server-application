cmake_minimum_required(VERSION 3.10)
project(NetworkProject)

set(CMAKE_CXX_STANDARD 17)


add_executable(DisplayServer
    src/main/DisplayServerMain.cpp
    src/net/DisplayServer.cpp
    src/net/ServerBase.cpp
    src/net/SocketWrapper.cpp
)

add_executable(Client
    src/main/ClientMain.cpp
    src/net/Client.cpp
    src/net/SocketWrapper.cpp
)

add_executable(ProcessingServer
    src/main/ProcessingServerMain.cpp
    src/net/ProcessingServer.cpp
    src/net/ServerBase.cpp
    src/net/SocketWrapper.cpp
)

target_include_directories(ProcessingServer PRIVATE include)
target_include_directories(Client PRIVATE include)
target_include_directories(DisplayServer PRIVATE include)

if(WIN32)
    target_link_libraries(DisplayServer PRIVATE ws2_32)
    target_link_libraries(Client PRIVATE ws2_32)
    target_link_libraries(ProcessingServer PRIVATE ws2_32)
endif()