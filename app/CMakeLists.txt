cmake_minimum_required(VERSION 3.13.1)
find_package(Zephyr REQUIRED HINTS $ENV{ZEPHYR_BASE})
project(spinsat VERSION 1.0 DESCRIPTION "SpinSat Software" LANGUAGES C CXX)

target_sources(app PRIVATE src/main.c)
target_include_directories(app PRIVATE core)

add_subdirectory(core)
target_link_libraries(app PRIVATE core_lib)
