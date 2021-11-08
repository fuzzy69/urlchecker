set(test_lib commonlib)

add_library(
    ${test_lib}
    OBJECT
    ${CMAKE_SOURCE_DIR}/src/common/httpproxy.cpp
    ${CMAKE_SOURCE_DIR}/src/common/proxymanager.cpp
    ${CMAKE_SOURCE_DIR}/src/common/random.cpp
    ${CMAKE_SOURCE_DIR}/src/common/useragentmanager.cpp
)
target_include_directories(${test_lib} INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_lib} PRIVATE Qt5::Core)
target_link_libraries(${test_lib} PRIVATE ${CPR_LIBRARIES})

set(test_name testcommon)

add_executable(
    ${test_name}

    test_httpproxy.cpp
    test_proxymanager.cpp
    test_httpproxy.cpp
    test_useragentmanager.cpp
)
target_link_libraries(${test_name} PRIVATE catchmain)
target_link_libraries(${test_name} PRIVATE ${test_lib})
target_link_libraries(${test_name} PRIVATE stdc++fs)

set_target_properties(
    ${test_name}
    PROPERTIES
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_RUNTIME_OUTPUT_DIRECTORY}/tests
)
