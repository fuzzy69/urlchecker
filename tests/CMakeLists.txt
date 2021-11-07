add_library(catchmain STATIC catchmain.cpp)
target_link_libraries(catchmain PUBLIC catch)

add_subdirectory(common)
add_subdirectory(core)
add_subdirectory(tools)
add_subdirectory(widgets)
