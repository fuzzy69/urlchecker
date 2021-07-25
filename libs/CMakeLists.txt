add_subdirectory(cpr)
cpr_option(BUILD_CPR_TESTS "Set to ON to build cpr tests." OFF)
add_subdirectory(my)
add_subdirectory(pugixml)
add_library(tidy SHARED IMPORTED)
set_property(
    TARGET tidy
    PROPERTY IMPORTED_LOCATION ${CMAKE_CURRENT_SOURCE_DIR}libs/tidy-html5/libtidy.so
)
