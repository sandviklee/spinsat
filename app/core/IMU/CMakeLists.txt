add_module(IMU STATIC IMU.c)
target_link_libraries(app PUBLIC IMU)