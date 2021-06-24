cmake_minimum_required(VERSION 3.15)
project(urlchecker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Help REQUIRED)

add_subdirectory(docs)
add_subdirectory(libs)

add_executable(
    ${PROJECT_NAME}

    assets.qrc

    src/main.cpp
    src/application/mainwindow.cpp

    src/application/core/applicationstatemachine.cpp
    src/application/core/basemainwindow.cpp
    src/application/core/helpbrowser.cpp
    src/application/core/misc.cpp
    src/application/core/recentfiles.cpp
    src/application/core/settings.cpp
    src/application/core/table.cpp
    src/application/core/thread.cpp
    
    src/application/tools/proxytools.cpp
    src/application/tools/ranktools.cpp
    src/application/tools/tool.cpp

    src/application/utils/requests.cpp

    src/application/widgets/filesystemwidget.cpp
    src/application/widgets/helpwidget.cpp
    src/application/widgets/logwidget.cpp
    src/application/widgets/proxieswidget.cpp
    src/application/widgets/settingswidget.cpp
    src/application/widgets/sidebarwidget.cpp
    src/application/widgets/tableswidget.cpp
    src/application/widgets/toolswidget.cpp
    src/application/widgets/workspacewidget.cpp
    
    src/application/workers/checkalexarank.cpp
    src/application/workers/checkurlstatusworker.cpp
    src/application/workers/dummyworker.cpp
    src/application/workers/scrapeproxies.cpp
    src/application/workers/testworker.cpp
    src/application/workers/worker.cpp
)

target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Help)
target_link_libraries(${PROJECT_NAME} PRIVATE ${CPR_LIBRARIES})
target_link_libraries(${PROJECT_NAME} PRIVATE ${MY_LIBRARIES})

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/docs/help.qhc
        ${CMAKE_BINARY_DIR}/bin/docs/help.qhc
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/docs/help.qch
        ${CMAKE_BINARY_DIR}/bin/docs/help.qch
)

# Tests
option(ENABLE_TEST "Turn on to enable tests" ON)
if (ENABLE_TEST)
    enable_testing()

    set(CATCH2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/catch2)  
    add_library(catch INTERFACE)
    target_include_directories(catch INTERFACE ${CATCH2_INCLUDE_DIR})

    add_subdirectory(tests)
endif()
