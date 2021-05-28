target_include_directories(
    my PUBLIC
    $<INSTALL_INTERFACE:include>
    $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}>
)

target_sources(
    my PRIVATE

    my/browserutils.h
    my/file.h
    my/httpclient.h
    my/httpproxy.h
    my/proxymanager.h
    my/proxyutils.h
    my/random.h
    my/text.h
    my/urlutils.h
    my/useragents.h
)

install(DIRECTORY my DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})

