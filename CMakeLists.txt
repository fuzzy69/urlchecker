cmake_minimum_required(VERSION 3.0)
project(urlchecker)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network REQUIRED)

set(urlchecker_SRC
  src/main.cpp
  src/application/core/mainwindow.cpp
  src/application/common/table.cpp
  src/application/utils/file.cpp
  src/application/utils/httpclient.cpp
  src/application/common/recentfiles.cpp
  src/application/core/sidebar.cpp
  src/application/utils/useragents.cpp
  src/application/core/proxieswidget.cpp
  src/application/common/applicationstate.cpp
  src/application/core/toolswidget.cpp
  assets.qrc
)

add_executable(urlchecker ${urlchecker_SRC})

target_link_libraries(urlchecker Qt5::Widgets)
target_link_libraries(urlchecker Qt5::Network)
