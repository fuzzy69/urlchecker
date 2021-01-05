cmake_minimum_required(VERSION 3.0)
project(urlchecker)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)

set(urlchecker_SRC
  src/main.cpp
  src/mainwindow.cpp
  src/table.cpp
  assets.qrc
)

add_executable(urlchecker ${urlchecker_SRC})

target_link_libraries(urlchecker Qt5::Widgets)
