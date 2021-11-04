set(test_lib toolslib)

add_library(
    ${test_lib}
    OBJECT
    ${CMAKE_SOURCE_DIR}/src/application/tools/alexarank/utilities.cpp
        ${CMAKE_SOURCE_DIR}/src/application/tools/scrapeproxies/utilities.cpp
)
target_include_directories(${test_lib} INTERFACE ${CMAKE_SOURCE_DIR}/src)
target_link_libraries(${test_lib} PRIVATE Qt5::Core)
target_link_libraries(${test_lib} PRIVATE ${CPR_LIBRARIES})

set(test_name testtools)

add_executable(
    ${test_name}

    test_alexarank.cpp
    test_scrapeproxies.cpp
)
target_link_libraries(${test_name} PRIVATE catchmain)
target_link_libraries(${test_name} PRIVATE ${test_lib})
target_link_libraries(${test_name} PRIVATE stdc++fs)
