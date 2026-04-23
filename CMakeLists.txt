cmake_minimum_required(VERSION 4.2)
project(DataEncryptionStandard C)

include_directories(include)

set(CMAKE_C_STANDARD 11)

add_executable(DataEncryptionStandard
        src/key_schedule.c
        include/key_schedule.h
        src/utilities.c
        include/utilities.h
        tests/key_schedule_test.c
        include/des.h
        src/des.c
        include/f_function.h
        src/f_function.c
)
