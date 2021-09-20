
option(CPR_BUILD_TESTS "Set to ON to build cpr tests." ON)
set(CPR_BUILD_TESTS OFF)
add_subdirectory(cpr)
#cpr_option(CPR_BUILD_TESTS_SSL "Set to ON to build cpr tests." OFF)
add_subdirectory(my)
add_subdirectory(pugixml)
add_subdirectory(csv)
#add_library(tidy SHARED IMPORTED)
#set_property(
#    TARGET tidy
#    PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}libs/tidy-html5/libtidy.so
#)
