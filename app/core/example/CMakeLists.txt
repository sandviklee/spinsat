add_library(example STATIC example.c)
target_include_directories(example PUBLIC ${CMAKE_CURRENT_SOURCE_DIR})
