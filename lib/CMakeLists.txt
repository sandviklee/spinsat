# SPDX-License-Identifier: Apache-2.0
add_subdirectory_ifdef(CONFIG_BLUETOOTH bluetooth)
add_subdirectory_ifdef(CONFIG_STATE_MACHINE state_machine)
add_subdirectory_ifdef(CONFIG_IMU imu)
add_subdirectory_ifdef(CONFIG_MOTOR motor)
