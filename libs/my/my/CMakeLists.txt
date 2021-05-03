cmake_minimum_required(VERSION 3.15)

add_library(
    my

    browserutils.cpp
    file.cpp
    httpclient.cpp
    httpproxy.cpp
    proxymanager.cpp
    proxyutils.cpp
    random.cpp
    text.cpp
    urlutils.cpp
)

add_library(my::my ALIAS my)

# install(TARGETS my)
