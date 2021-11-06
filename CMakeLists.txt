cmake_minimum_required(VERSION 3.15)
project(urlchecker)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTORCC ON)

#set(CMAKE_POSITION_INDEPENDENT_CODE ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

find_package(Qt5Widgets CONFIG REQUIRED)
find_package(Qt5Help REQUIRED)

add_subdirectory(docs)
add_subdirectory(libs)

set(tidy_source_dir ${CMAKE_CURRENT_SOURCE_DIR}/libs/tidy-html5)
set(tidy_build_dir ${tidy_source_dir}/build/cmake)
if (WIN32)
    include_directories(${tidy_source_dir}/include)
    link_directories(${tidy_build_dir}/Release)
    set(tidy_static_lib ${tidy_build_dir}/Release/tidy_static.lib)
elseif(UNIX)
    set(tidy_static_lib ${tidy_build_dir}/libtidy.a)
endif()

if (NOT EXISTS ${tidy_static_lib})
    message(STATUS "Compiling Tidy library ...")
    execute_process(COMMAND cmake -S ${tidy_source_dir} -B ${tidy_build_dir} -DCMAKE_BUILD_TYPE=Release -DCMAKE_POSITION_INDEPENDENT_CODE:BOOL=true)
    if(WIN32)
        execute_process(COMMAND cmake --build ${tidy_build_dir} --config Release)
    elseif(UNIX)
        execute_process(COMMAND make -C ${tidy_build_dir} -f Makefile)
    endif()
endif()

option(ENABLE_TESTS "Turn on to enable tests" ON)

if (EXISTS ${tidy_static_lib})
    add_executable(
        ${PROJECT_NAME}

        assets.qrc

        src/main.cpp
        src/application/actions.cpp
        src/application/mainwindow.cpp

        src/application/core/actionsmanager.cpp
        src/application/core/applicationbridge.cpp
        src/application/core/applicationstatemachine.cpp
        src/application/core/misc.cpp
        src/application/core/recentfiles.cpp
        src/application/core/settings.cpp
        src/application/core/table.cpp
        src/application/core/thread.cpp
        src/application/core/toolsmanager.cpp
        src/application/core/worker.cpp
        src/application/core/workermanager.cpp
    #    src/application/core/useragents/useragentsmanager.cpp

        src/application/tools/alexarank/alexaranktool.cpp
        src/application/tools/alexarank/checkalexarankworker.cpp
        src/application/tools/alexarank/utilities.cpp
        src/application/tools/dummy/dummyworker.cpp
        src/application/tools/dummy/dummytool.cpp

        src/application/tools/scrapelinks/scrapelinkssettings.ui
        src/application/tools/scrapelinks/scrapelinkssettingspage.cpp
        src/application/tools/scrapelinks/scrapelinkstool.cpp
        src/application/tools/scrapelinks/scrapelinksworker.cpp
        src/application/tools/scrapelinks/utilities.cpp

        src/application/tools/scrapeimages/scrapeimagessettings.ui
        src/application/tools/scrapeimages/scrapeimagessettingspage.cpp
        src/application/tools/scrapeimages/scrapeimagestool.cpp
        src/application/tools/scrapeimages/scrapeimagesworker.cpp
        src/application/tools/scrapeimages/utilities.cpp

        src/application/tools/scrapeproxies/scrapeproxiestool.cpp
        src/application/tools/scrapeproxies/scrapeproxiesworker.cpp
        src/application/tools/scrapeproxies/utilities.cpp
        src/application/tools/test/testtool.cpp
        src/application/tools/test/testworker.cpp
        src/application/tools/urlstatus/checkurlstatusworker.cpp
        src/application/tools/urlstatus/urlstatustool.cpp
        src/application/tools/tool.cpp
        src/application/tools/workerfactory.cpp

        src/application/tools/scrapesitemaps/scrapesitemapssettings.ui
        src/application/tools/scrapesitemaps/scrapesitemapssettingspage.cpp
        src/application/tools/scrapesitemaps/scrapesitemapstool.cpp
        src/application/tools/scrapesitemaps/scrapesitemapsworker.cpp
        src/application/tools/scrapesitemaps/utilities.cpp

        src/application/tools/scrapehtml/settings.ui
        src/application/tools/scrapehtml/settingspage.cpp
        src/application/tools/scrapehtml/tool.cpp
        src/application/tools/scrapehtml/worker.cpp
        src/application/tools/scrapehtml/utilities.cpp

        src/application/tools/scrapemeta/tool.cpp
        src/application/tools/scrapemeta/worker.cpp

        src/application/tools/scrapeemails/scrapeemailstool.cpp
        src/application/tools/scrapeemails/scrapeemailsworker.cpp
        src/application/tools/scrapeemails/utilities.cpp
        src/application/tools/scrapephonenumbers/scrapephonenumberstool.cpp
        src/application/tools/scrapephonenumbers/scrapephonenumbersworker.cpp
        src/application/tools/scrapephonenumbers/utilities.cpp
        src/application/tools/checksearchengineindex/checksearchengineindextool.cpp
        src/application/tools/checksearchengineindex/checksearchengineindexworker.cpp
        src/application/tools/checksearchengineindex/utilities.cpp

        src/application/utils/requests.cpp
        src/application/utils/simpledom.cpp
        src/application/utils/tidyhtml.cpp

        src/application/widgets/settingswidget/settingswidget.cpp
        src/application/widgets/settingswidget/generalsettingspage.cpp
        src/application/widgets/settingswidget/connectionsettingspage.cpp
        src/application/widgets/settingswidget/useragentssettingspage.cpp
        src/application/widgets/settingswidget/proxiessettingspage.cpp
        src/application/widgets/settingswidget/generalsettings.ui
        src/application/widgets/settingswidget/connectionsettings.ui
        src/application/widgets/settingswidget/useragentssettings.ui
        src/application/widgets/settingswidget/proxiessettings.ui
        src/application/widgets/filesystemwidget.cpp
        src/application/widgets/helpbrowser.cpp
        src/application/widgets/helpwidget.cpp
        src/application/widgets/logwidget.cpp
        src/application/widgets/mainwindowbase.cpp
        src/application/widgets/proxieswidget.cpp
        src/application/widgets/sidebarwidget.cpp
        src/application/widgets/statusbarwidget.cpp
        src/application/widgets/tableswidget.cpp
        src/application/widgets/toolswidget.cpp
        src/application/widgets/useragentswidget.cpp
        src/application/widgets/workspacewidget.cpp

        src/common/file.cpp
        src/common/httpproxy.cpp
        src/common/proxymanager.cpp
        src/common/proxyutils.cpp
        src/common/random.cpp
        src/common/text.cpp
        src/common/urlutils.cpp
        src/common/useragentmanager.cpp

    )

if (ENABLE_TESTS)
    add_definitions(-DUNIT_TESTING=1)
endif()

    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Widgets)
    target_link_libraries(${PROJECT_NAME} PRIVATE Qt5::Help)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${CPR_LIBRARIES})
    target_link_libraries(${PROJECT_NAME} PRIVATE pugixml)
    target_link_libraries(${PROJECT_NAME} PRIVATE ${tidy_static_lib})
    target_link_libraries(${PROJECT_NAME} PRIVATE csv)

add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/docs/help.qhc
        ${CMAKE_BINARY_DIR}/bin/docs/help.qhc
)
add_custom_command(
    TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/docs/help.qch
        ${CMAKE_BINARY_DIR}/bin/docs/help.qch
)

# Tests
if (ENABLE_TESTS)
    enable_testing()

    set(CATCH2_INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/libs/catch2)
    add_library(catch INTERFACE)
    target_include_directories(catch INTERFACE ${CATCH2_INCLUDE_DIR})

    add_subdirectory(tests)
endif()

else()
    message(FATAL_ERROR "Failed to locate Tidy static lib '${tidy_static_lib}'!")
endif()
