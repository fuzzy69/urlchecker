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
  
    application/core/helpwidget.cpp
    application/core/mainwindow.cpp
    application/core/proxieswidget.cpp
    application/core/settingswidget.cpp
    application/core/sidebar.cpp
    application/core/tool.cpp
    application/core/toolswidget.cpp
    application/core/workspacewidget.cpp
  
    application/utils/file.cpp
    application/utils/useragents.cpp
    application/utils/rand.cpp
    application/utils/httpproxy.cpp
  
    application/workers/worker.cpp
    application/workers/checkurlstatusworker.cpp
    application/workers/checkalexarank.cpp
    application/workers/dummyworker.cpp
    application/workers/scrapeproxies.cpp
    application/workers/testworker.cpp
)

target_link_libraries(urlchecker Qt5::Widgets)
target_link_libraries(urlchecker Qt5::Network)
