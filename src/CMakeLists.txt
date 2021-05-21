add_executable(
    urlchecker

    ../assets.qrc

    main.cpp
    application/config.h
    application/icons.h

    application/common/applicationstatemachine.cpp
    application/common/basemainwindow.cpp
    application/common/recentfiles.cpp
    application/common/settings.cpp
    application/common/table.cpp
    application/common/thread.cpp
  
    application/core/helpbrowser.cpp
    application/core/helpwidget.cpp
    application/core/mainwindow.cpp
    application/core/proxieswidget.cpp
    application/core/settingswidget.cpp
    application/core/sidebar.cpp
    application/core/tool.cpp
    application/core/toolswidget.cpp
    application/core/workspacewidget.cpp

    application/tools/proxytools.cpp
    application/tools/ranktools.cpp

    application/utils/requests.cpp

    application/workers/worker.cpp
    application/workers/checkurlstatusworker.cpp
    application/workers/checkalexarank.cpp
    application/workers/dummyworker.cpp
    application/workers/scrapeproxies.cpp
    application/workers/testworker.cpp
)

add_custom_command(
    TARGET urlchecker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/docs/help.qhc
        ${CMAKE_BINARY_DIR}/bin/docs/help.qhc
)
add_custom_command(
    TARGET urlchecker POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy
        ${CMAKE_SOURCE_DIR}/docs/help.qch
        ${CMAKE_BINARY_DIR}/bin/docs/help.qch
)
