# Use cpr CMake files as a reference guide
cmake_minimum_required(VERSION 3.15)
project(my LANGUAGES CXX)

# Only change the folder behaviour if cpr is not a subproject
# if(${CMAKE_PROJECT_NAME} STREQUAL ${PROJECT_NAME})
#     set_property(GLOBAL PROPERTY USE_FOLDERS ON)
#     set_property(GLOBAL PROPERTY PREDEFINED_TARGETS_FOLDER "CMake")
#     set(EXECUTABLE_OUTPUT_PATH ${CMAKE_BINARY_DIR}/bin)
#     set(LIBRARY_OUTPUT_PATH ${CMAKE_BINARY_DIR}/lib)
# endif()

# set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(MY_LIBRARIES my CACHE INTERNAL "")

option(BUILD_SHARED_LIBS "Build libraries as shared libraries" ON)

# include(GNUInstallDirs)
# include(FetchContent)

find_package(Qt5Core REQUIRED)
find_package(Qt5Network REQUIRED)

add_subdirectory(my)
add_subdirectory(include)

target_link_libraries(my Qt5::Core)
target_link_libraries(my Qt5::Network)
