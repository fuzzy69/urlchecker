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
#add_subdirectory(libs/random)

set(urlchecker_SRC
  assets.qrc

  src/main.cpp

  src/application/common/applicationstatemachine.cpp
  src/application/common/basemainwindow.cpp
  src/application/common/recentfiles.cpp
  src/application/common/settings.cpp
  src/application/common/table.cpp
  src/application/common/thread.cpp

  src/application/core/helpwidget.cpp
  src/application/core/mainwindow.cpp
  src/application/core/proxieswidget.cpp
  src/application/core/settingswidget.cpp
  src/application/core/sidebar.cpp
  src/application/core/tool.cpp
  src/application/core/toolswidget.cpp
  src/application/core/workspacewidget.cpp

  src/application/utils/file.cpp
  src/application/utils/useragents.cpp
  src/application/utils/rand.cpp

  src/application/workers/worker.cpp
  src/application/workers/checkurlstatusworker.cpp
  src/application/workers/checkalexarank.cpp
  src/application/workers/dummyworker.cpp
  src/application/workers/scrapeproxies.cpp
  src/application/workers/testworker.cpp
)

add_executable(urlchecker ${urlchecker_SRC})

target_link_libraries(urlchecker Qt5::Widgets)
target_link_libraries(urlchecker Qt5::Network)
target_link_libraries(urlchecker ${CPR_LIBRARIES})
#target_link_libraries(urlchecker effolkronium_random)
