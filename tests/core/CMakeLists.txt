set(test_lib corelib)

add_library(
    ${test_lib}
    OBJECT
    ${CMAKE_SOURCE_DIR}/src/application/core/settings.cpp
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
    test_settings.cpp
)
target_link_libraries(${test_name} PRIVATE catchmain)
target_link_libraries(${test_name} PRIVATE ${test_lib})
#target_link_libraries(${test_name} PRIVATE stdc++fs)

set_target_properties(
    ${test_name}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests
)

add_custom_target(copy_tests_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests/data
)
add_dependencies(${test_name} copy_tests_data)
