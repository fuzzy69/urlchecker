add_library(catchmain STATIC catchmain.cpp)

target_link_libraries(catchmain PUBLIC catch)

add_library(urlcheckertests OBJECT ${CMAKE_SOURCE_DIR}/src/application/utils/httpproxy.cpp)
target_include_directories(urlcheckertests INTERFACE ${CMAKE_SOURCE_DIR}/src)

add_executable(tests tests.cpp)
target_link_libraries(tests PRIVATE catchmain)
target_link_libraries(tests PUBLIC urlcheckertests)

add_test(NAME urlcheckertests COMMAND tests)
