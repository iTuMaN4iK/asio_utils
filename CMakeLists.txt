cmake_minimum_required(VERSION 3.7.2 FATAL_ERROR)
cmake_policy(SET CMP0074 OLD)

project(asio_connection_tools LANGUAGES CXX C)

set(CMAKE_CXX_STANDARD 14)
set(CMAKE_CXX_STANDARD_REQUIRED TRUE)

option(CLEAN_BUILD "Treat all warnings as errors" ON)
option(BUILD_TESTS "Build Tests" ON)

if (MSVC)
    add_compile_options(/W4)
    # warnings as errors
    if (${CLEAN_BUILD})
        add_compile_options(/WX)
        #exclude deprecated
        if (NOT ${ALLOW_DEPRECATED})
            add_compile_options(-wd4996)
        endif()
    endif ()
else ()
    add_compile_options(-Wall -Wextra -pedantic)
    # warnings as errors
    if (${CLEAN_BUILD})
        add_compile_options(-Werror)
        # exclude deprecated
        if (${ALLOW_DEPRECATED})
            add_compile_options(-Wno-error=deprecated-declarations)
        endif ()
    endif ()
endif ()

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY bin)

# FindXXX.cmake scripts for external packages
list(APPEND CMAKE_MODULE_PATH
        ${CMAKE_CURRENT_LIST_DIR}/cmake)

find_package(ASIO REQUIRED)

add_library(asio_utils INTERFACE)
target_include_directories(asio_utils INTERFACE
        $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/include>
        $<INSTALL_INTERFACE:include>
        )
target_link_libraries(asio_utils INTERFACE ASIO::ASIO)

add_library(eld::asio_utils ALIAS asio_utils)

get_directory_property(IS_EXCLUDED EXCLUDE_FROM_ALL)

# tests
if (NOT IS_EXCLUDED AND BUILD_TESTS)
    find_package(GTest CONFIG REQUIRED)
    enable_testing()
    add_subdirectory(test)

    # sandbox
    add_executable(sandbox sandbox/sandbox.cpp)
    target_link_libraries(sandbox PRIVATE eld::asio_utils)

    add_executable(sb.find_deadlock sandbox/find_deadlock.cpp)
    target_link_libraries(sb.find_deadlock PRIVATE eld::asio_utils)

    add_executable(example_6 sandbox/example_6.cpp)
    target_link_libraries(example_6 PRIVATE eld::asio_utils)
endif()
