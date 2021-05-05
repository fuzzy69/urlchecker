cmake_minimum_required(VERSION 3.15)
project(urlchecker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network REQUIRED)
find_package(Qt5Help REQUIRED)
# find_package(Qt5 COMPONENTS Help REQUIRED)

add_subdirectory(libs)
add_subdirectory(src)

target_link_libraries(urlchecker Qt5::Widgets)
target_link_libraries(urlchecker Qt5::Network)
target_link_libraries(urlchecker Qt5::Help)
# target_link_libraries(urlchecker PRIVATE Qt5::Help)
target_link_libraries(urlchecker ${CPR_LIBRARIES})
target_link_libraries(urlchecker ${MY_LIBRARIES})

# Tests
option(ENABLE_TEST "Turn on to enable tests" ON)
if (ENABLE_TEST)
    enable_testing()

    set(CATCH2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/catch2)  
    add_library(catch INTERFACE)
    target_include_directories(catch INTERFACE ${CATCH2_INCLUDE_DIR})

    add_subdirectory(tests)
endif()
