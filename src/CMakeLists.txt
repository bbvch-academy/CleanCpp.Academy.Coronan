cmake_minimum_required(VERSION 3.15...3.20)

project(
    coronan
    VERSION 0.1
    LANGUAGES CXX
)

set(HEADER_LIST
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/coronan/http_client.hpp"
    "${CMAKE_CURRENT_SOURCE_DIR}/../include/coronan/corona-api_parser.hpp"
)

add_library(${PROJECT_NAME} STATIC ${HEADER_LIST})

add_library(${PROJECT_NAME}::coronan_lib ALIAS ${PROJECT_NAME})

target_include_directories(
    ${PROJECT_NAME}
    PUBLIC $<BUILD_INTERFACE:${CMAKE_CURRENT_SOURCE_DIR}/../include>
           $<INSTALL_INTERFACE:${CMAKE_INSTALL_INCLUDEDIR}>
)

target_sources(
    ${PROJECT_NAME} PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/corona-api_parser.cpp
                            ${HEADER_LIST}
)

target_link_libraries(
    ${PROJECT_NAME}
    PUBLIC CONAN_PKG::poco
    PRIVATE CONAN_PKG::rapidjson
    PRIVATE coronan::compile_warnings
    PRIVATE coronan::compile_options
)

# allow for static analysis options

# IDE stuff
source_group(
    TREE "${CMAKE_CURRENT_SOURCE_DIR}/../include"
    PREFIX "Header Files"
    FILES ${HEADER_LIST}
)

set_target_properties(${PROJECT_NAME} PROPERTIES CXX_EXTENSIONS OFF)
