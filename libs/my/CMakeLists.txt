cmake_minimum_required(VERSION 3.15)
project(my LANGUAGES CXX)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MY_LIBRARIES my CACHE INTERNAL "")

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

add_subdirectory(my)
add_subdirectory(include)

target_link_libraries(my Qt5::Core)
target_link_libraries(my Qt5::Network)
