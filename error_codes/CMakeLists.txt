add_library(error_codes INTERFACE)

target_include_directories(error_codes INTERFACE ${CMAKE_CURRENT_SOURCE_DIR})

target_link_libraries(cemb PUBLIC error_codes)