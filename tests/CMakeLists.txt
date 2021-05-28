set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

add_library(catchmain STATIC catchmain.cpp)
target_link_libraries(catchmain PUBLIC catch)

add_library(
    urlcheckertests
    OBJECT
    #${CMAKE_SOURCE_DIR}/src/application/common/settings.cpp
    ${CMAKE_SOURCE_DIR}/src/application/tools/proxytools.cpp
    ${CMAKE_SOURCE_DIR}/src/application/tools/ranktools.cpp
    ${CMAKE_SOURCE_DIR}/src/application/utils/requests.cpp
)
target_include_directories(urlcheckertests INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(urlcheckertests PUBLIC Qt5::Core)
target_link_libraries(urlcheckertests PUBLIC ${CPR_LIBRARIES})
target_link_libraries(urlcheckertests PUBLIC ${MY_LIBRARIES})

add_executable(
    tests

    testproxy.cpp
    testproxymanager.cpp
    testrand.cpp
    testproxytools.cpp
    testranktools.cpp
    testuseragentsmanager.cpp
)
target_link_libraries(tests PRIVATE catchmain)
target_link_libraries(tests PUBLIC urlcheckertests)
target_link_libraries(tests PUBLIC stdc++fs)

add_custom_target(copy_tests_data
    COMMAND ${CMAKE_COMMAND} -E copy_directory ${CMAKE_CURRENT_LIST_DIR}/data ${CMAKE_CURRENT_BINARY_DIR}/data
)
add_dependencies(tests copy_tests_data)

add_test(NAME urlcheckertests COMMAND tests)
