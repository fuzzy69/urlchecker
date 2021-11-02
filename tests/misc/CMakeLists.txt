set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(catchmain STATIC catchmain.cpp)
target_link_libraries(catchmain PUBLIC catch)

set(test_lib urlcheckerlib)

add_library(
    ${test_lib}
    OBJECT
    ${CMAKE_SOURCE_DIR}/src/application/tools/alexarank/utilities.cpp
    ${CMAKE_SOURCE_DIR}/src/application/tools/scrapeproxies/utilities.cpp
    ${CMAKE_SOURCE_DIR}/src/application/utils/requests.cpp
)
target_include_directories(${test_lib} INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_lib} PRIVATE Qt5::Core)
target_link_libraries(${test_lib} PRIVATE ${CPR_LIBRARIES})
target_link_libraries(${test_lib} PUBLIC ${MY_LIBRARIES})

set(test_name testmisc)

add_executable(
    ${test_name}

    testproxy.cpp
    testproxymanager.cpp
    testrand.cpp
    testproxytools.cpp
    testranktools.cpp
    testuseragentsmanager.cpp
)
target_link_libraries(${test_name} PRIVATE catchmain)
target_link_libraries(${test_name} PRIVATE ${test_lib})
target_link_libraries(${test_name} PRIVATE stdc++fs)

add_custom_target(copy_tests_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
)
add_dependencies(${test_name} copy_tests_data)

#add_test(NAME ${test_lib} COMMAND ${test_name})
