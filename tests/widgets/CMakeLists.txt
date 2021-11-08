cmake_minimum_required(VERSION 3.15)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

find_package(Qt5Core REQUIRED)
find_package(Qt5Widgets REQUIRED)
find_package(Qt5Test REQUIRED)

set(test_lib widgetslib)

add_library(
    ${test_lib}
    OBJECT
    ${CMAKE_SOURCE_DIR}/src/application/core/toolsmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/application/tools/tool.cpp
    ${CMAKE_SOURCE_DIR}/src/application/widgets/logwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/application/widgets/proxieswidget.cpp
    ${CMAKE_SOURCE_DIR}/src/application/widgets/sidebarwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/application/widgets/statusbarwidget.cpp
    ${CMAKE_SOURCE_DIR}/src/application/widgets/toolswidget.cpp
    ${CMAKE_SOURCE_DIR}/src/application/widgets/useragentswidget.cpp
)
target_include_directories(${test_lib} INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_lib} PRIVATE Qt5::Core)
target_link_libraries(${test_lib} PRIVATE Qt5::Widgets)

set(test_name testwidgets)

add_executable(
    ${test_name}

    main.cpp
    test_logwidget.cpp
    test_proxieswidget.cpp
    test_sidebarwidget.cpp
    test_statusbarwidget.cpp
    test_toolswidget.cpp
    test_useragentswidget.cpp
)

target_link_libraries(${test_name} PRIVATE Qt5::Core)
target_link_libraries(${test_name} PRIVATE Qt5::Widgets)
target_link_libraries(${test_name} PRIVATE Qt5::Test)
target_link_libraries(${test_name} PRIVATE ${test_lib})

set_target_properties(
    ${test_name}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests
)
