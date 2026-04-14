cmake_minimum_required(VERSION 3.15)
project(o_negative LANGUAGES CXX)

if(MSVC)
  # Release => /MD, Debug => /MDd
  set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>DLL")
endif()

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS OFF)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(O_NEGATIVE_ROOT "${CMAKE_CURRENT_LIST_DIR}")

option(BUILD_XINPUT_PROXY "Build xinput1_4 proxy instead of dinput8" OFF)

if(BUILD_XINPUT_PROXY)
    set(PROXY_TARGET xinput1_4)
    set(PROXY_SOURCE ${O_NEGATIVE_ROOT}/src/xinput_proxy.cpp)
    set(PROXY_DEF    ${O_NEGATIVE_ROOT}/xinput1_4.def)
    set(PROXY_OUTPUT xinput1_4)
else()
    set(PROXY_TARGET dinput8)
    set(PROXY_SOURCE ${O_NEGATIVE_ROOT}/src/dinput_proxy.cpp)
    set(PROXY_DEF    ${O_NEGATIVE_ROOT}/dinput8.def)
    set(PROXY_OUTPUT dinput8)
endif()

add_library(${PROXY_TARGET} SHARED
    ${O_NEGATIVE_ROOT}/src/common.cpp
    ${PROXY_SOURCE}
)

target_include_directories(${PROXY_TARGET} PUBLIC
    ${O_NEGATIVE_ROOT}/src
)

set_target_properties(${PROXY_TARGET} PROPERTIES
    OUTPUT_NAME "${PROXY_OUTPUT}"
    SUFFIX ".dll"
    LINK_FLAGS "/DEF:${PROXY_DEF}"
    RUNTIME_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    LIBRARY_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
    ARCHIVE_OUTPUT_DIRECTORY "${CMAKE_BINARY_DIR}/bin/$<CONFIG>"
)

add_library(test_mod SHARED
    ${O_NEGATIVE_ROOT}/src/test_mod.cpp
)

set_target_properties(test_mod PROPERTIES
    OUTPUT_NAME "test_mod"
    SUFFIX ".dll"
    RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/mods
)

if (MSVC)
    target_compile_options(test_mod PRIVATE /W3 /wd4996)
else()
    target_compile_options(test_mod PRIVATE -Wall)
endif()


if(MSVC)
    target_compile_options(${PROXY_TARGET} PRIVATE /W3 /wd4996)
else()
    target_compile_options(${PROXY_TARGET} PRIVATE -Wall)
endif()
