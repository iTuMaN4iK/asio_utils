
set(ASIO_SEARCH_PATHS
        ${ASIO_ROOT})

find_package(Threads QUIET)

find_path(ASIO_INCLUDE_DIR asio.hpp
        HINTS
        $ENV{ASIO_ROOT}
        PATH_SUFFIXES
        include
        include/asio
        PATHS
        ${ASIO_SEARCH_PATHS}
        ~/Library/Frameworks
        /Library/Frameworks
        /usr/local/include
        /usr/include
        /sw/include # Fink
        /opt/local/include # DarwinPorts
        /opt/csw/include # Blastwave
        /opt/include
        /usr/freeware/include
        )

include(FindPackageHandleStandardArgs)
find_package_handle_standard_args(ASIO
        REQUIRED_VARS ASIO_INCLUDE_DIR
        FAIL_MESSAGE "ASIO was not found")

if (${ASIO_FOUND})
    if (NOT TARGET ASIO::ASIO)
        add_library(ASIO::ASIO INTERFACE IMPORTED)
        set_target_properties(ASIO::ASIO PROPERTIES
                INTERFACE_COMPILE_DEFINITIONS "ASIO_STANDALONE"
                INTERFACE_INCLUDE_DIRECTORIES ${ASIO_INCLUDE_DIR})

        if(WIN32)
            target_link_libraries(ASIO::ASIO INTERFACE
                    Threads::Threads
                    ws2_32.lib
                    mswsock.lib
                    )
            target_compile_definitions(ASIO::ASIO INTERFACE _WIN32_WINDOWS _WIN32_WINNT=0x0601)
        endif()
        message(STATUS "Asio include dir: ${ASIO_INCLUDE_DIR}")
    endif()
endif ()