cmake_minimum_required(VERSION 3.10)
project(ServerClientApp)

# Вказуємо стандарт C++
set(CMAKE_CXX_STANDARD 11)

# Додаємо директорії з заголовковими файлами
include_directories(${CMAKE_SOURCE_DIR}/src/common)

# Створюємо виконувані файли
add_executable(server src/server/main.cpp src/server/server.h src/server/server.cpp)
add_executable(client src/client/main.cpp src/client/client.h src/client/client.cpp)

# Додаємо шлях до заголовкових файлів для кожного з виконуваних файлів
target_include_directories(server PRIVATE ${CMAKE_SOURCE_DIR}/src/common)
target_include_directories(client PRIVATE ${CMAKE_SOURCE_DIR}/src/common)

# Лінкування бібліотек для Windows
if(WIN32)
    target_link_libraries(server ws2_32)
   target_link_libraries(client ws2_32)
endif()
