
cmake_minimum_required(VERSION 3.15)
project(o_negative LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(CMAKE_WINDOWS_EXPORT_ALL_SYMBOLS ON)
set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(O_NEGATIVE_ROOT "${CMAKE_CURRENT_LIST_DIR}")

add_library(dinput8 SHARED 
        ${O_NEGATIVE_ROOT}/src/common.cpp
        ${O_NEGATIVE_ROOT}/src/dinput_proxy.cpp
)

target_include_directories(dinput8 PUBLIC
    ${O_NEGATIVE_ROOT}/src
#${O_NEGATIVE_ROOT}/include
)

# Output as dinput8.dll
set_target_properties(dinput8 PROPERTIES
    OUTPUT_NAME "dinput8"
    SUFFIX ".dll"
)

# This helps ensure correct exports (especially if game links statically)
set_target_properties(dinput8 PROPERTIES LINK_FLAGS "/DEF:${O_NEGATIVE_ROOT}/dinput8.def")

# Use static runtime (optional, for portability)
# set(CMAKE_MSVC_RUNTIME_LIBRARY "MultiThreaded$<$<CONFIG:Debug>:Debug>")

# Disable unnecessary warnings
if (MSVC)
    target_compile_options(dinput8 PRIVATE /W3 /wd4996)
else()
    target_compile_options(dinput8 PRIVATE -Wall)
endif()
