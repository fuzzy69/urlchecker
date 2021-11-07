set(test_lib corelib)

add_library(
    ${test_lib}
    OBJECT
    ${CMAKE_SOURCE_DIR}/src/application/core/toolsmanager.cpp
    ${CMAKE_SOURCE_DIR}/src/application/tools/tool.cpp
    ${CMAKE_SOURCE_DIR}/src/application/tools/dummy/dummytool.cpp
    ${CMAKE_SOURCE_DIR}/src/application/tools/test/testtool.cpp
)
target_include_directories(${test_lib} INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_lib} PRIVATE Qt5::Core)
target_link_libraries(${test_lib} PUBLIC Qt5::Widgets)
#target_link_libraries(${test_lib} PRIVATE ${CPR_LIBRARIES})

set(test_name testcore)

add_executable(
    ${test_name}

    test_toolsmanager.cpp
)
target_link_libraries(${test_name} PRIVATE catchmain)
target_link_libraries(${test_name} PRIVATE ${test_lib})
#target_link_libraries(${test_name} PRIVATE stdc++fs)
