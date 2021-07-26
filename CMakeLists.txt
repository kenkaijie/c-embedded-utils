cmake_minimum_required(VERSION 3.13)

project(cemb LANGUAGES C)

# Project Options
option(CEMB_CFG_OWN_CMAKE "This project uses a target cmocka as the target for internal library testing. If your own cmocka is used, cmocka will be fetched from your provided target." OFF)
option(CEMB_CFG_PRODUCE_UNIT_TESTS "Produces unit testing for library" ON)

# Inclusions should be done after options are set.

add_library(cemb)
add_library(cemb::cemb ALIAS cemb)
add_subdirectory(cemb)

include(cmake/AddCEmbeddedModule.cmake)

if (CEMB_CFG_PRODUCE_UNIT_TESTS)
    enable_testing()
endif()

add_subdirectory(extern)

add_subdirectory(lib)

add_custom_target(cemb_dependency_graph ALL
        COMMAND ${CMAKE_COMMAND} "--graphviz=cemb.dot" .
        COMMAND dot -Tpng -o cemb_dependency_graph.png cemb.dot
        WORKING_DIRECTORY "${CMAKE_BINARY_DIR}")