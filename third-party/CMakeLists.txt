# jthread
add_library(jthread INTERFACE)
target_sources(jthread
        INTERFACE
        "${CMAKE_CURRENT_SOURCE_DIR}/jthread/jthread.hpp"
        "${CMAKE_CURRENT_SOURCE_DIR}/jthread/stop_token.hpp"
        )

target_include_directories(jthread SYSTEM
        INTERFACE
        $<INSTALL_INTERFACE:jthread>
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/jthread>
        )