set(CMAKE_CXX_STANDARD 17)

add_library(catchmain STATIC catchmain.cpp)

target_link_libraries(catchmain PUBLIC catch)

add_library(
    urlcheckertests
    OBJECT
    ${CMAKE_SOURCE_DIR}/src/application/utils/httpproxy.cpp
    ${CMAKE_SOURCE_DIR}/src/application/utils/proxymanager.cpp
    ${CMAKE_SOURCE_DIR}/src/application/utils/rand.cpp
    ${CMAKE_SOURCE_DIR}/src/application/utils/text.cpp
)
target_include_directories(urlcheckertests INTERFACE ${CMAKE_SOURCE_DIR}/src)

add_executable(
    tests

    # tests.cpp
    testproxy.cpp
    testproxymanager.cpp
    testrand.cpp
)
target_link_libraries(tests PRIVATE catchmain)
target_link_libraries(tests PUBLIC urlcheckertests)

add_test(NAME urlcheckertests COMMAND tests)
