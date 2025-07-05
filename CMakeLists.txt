cmake_minimum_required(VERSION 3.10)

project(cliengine-cpp VERSION 0.1.0 LANGUAGES CXX)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

set(INCLUDE_DIR ${CMAKE_CURRENT_SOURCE_DIR}/include)
set(SRC_DIR ${CMAKE_CURRENT_SOURCE_DIR}/src)

set(HEADERS
    ${INCLUDE_DIR}/cliengine/Defines.hpp
    ${INCLUDE_DIR}/cliengine/Engine.hpp
    ${INCLUDE_DIR}/cliengine/Parser.hpp
)

set(SOURCES
    ${SRC_DIR}/Engine.cpp
    ${SRC_DIR}/Parser.cpp
)

add_library(cliengine-cpp STATIC ${SOURCES} ${HEADERS})

target_include_directories(cliengine-cpp PUBLIC ${INCLUDE_DIR})

option(CLIENGINE_USE_PRECOMPILED_HEADER "Enable inclusion of precompiled header" OFF)
set(CLIENGINE_PRECOMPILED_HEADER_PATH "" CACHE STRING "Path to precompiled header")

if(CLIENGINE_USE_PRECOMPILED_HEADER AND NOT "${CLIENGINE_PRECOMPILED_HEADER_PATH}" STREQUAL "")
    target_compile_definitions(cliengine-cpp PUBLIC
        CLIENGINE_PRECOMPILED_HEADER_PATH="${CLIENGINE_PRECOMPILED_HEADER_PATH}"
    )
endif()
