cmake_minimum_required(VERSION 3.10)
project(ServerClientApp)

# choosing c++ standart
set(CMAKE_CXX_STANDARD 11)

# adding directories with headers
include_directories(${CMAKE_SOURCE_DIR}/src/common)

# adding execution files
add_executable(server src/server/main.cpp src/server/server.h src/server/server.cpp)
add_executable(client src/client/main.cpp src/client/client.h src/client/client.cpp)

# add path to exec files
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/src/common)

# linking libs for win
if(WIN32)
    target_link_libraries(server ws2_32)
   target_link_libraries(client ws2_32)
endif()
