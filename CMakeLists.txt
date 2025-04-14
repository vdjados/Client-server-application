cmake_minimum_required(VERSION 3.10)
project(SocketServer)

# Установка стандарта C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Указываем путь к исходникам
add_executable(ServerApp Server.cpp)

# Линкуем библиотеку ws2_32 для Windows
if(WIN32)
    target_link_libraries(ServerApp ws2_32)
endif()


add_executable(ClientApp Client.cpp)
if(WIN32)
    target_link_libraries(ClientApp ws2_32)
endif()
