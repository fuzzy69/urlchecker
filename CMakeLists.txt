cmake_minimum_required(VERSION 3.0)
project(urlchecker)

set(CMAKE_CXX_STANDARD 11)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Network REQUIRED)
# include(FetchContent)
# # FetchContent_Declare(cpr GIT_REPOSITORY https://github.com/whoshuu/cpr.git GIT_TAG c8d33915dbd88ad6c92b258869b03aba06587ff9) # the commit hash for 1.5.0
# FetchContent_MakeAvailable(cpr)
add_subdirectory(libs/cpr)
cpr_option(BUILD_CPR_TESTS "Set to ON to build cpr tests." OFF)

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
  src/application/workers/checkurlstatusworker.cpp
  assets.qrc
)

add_executable(urlchecker ${urlchecker_SRC})

target_link_libraries(urlchecker Qt5::Widgets)
target_link_libraries(urlchecker Qt5::Network)
target_link_libraries(urlchecker ${CPR_LIBRARIES})
