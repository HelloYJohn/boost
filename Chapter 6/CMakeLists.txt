cmake_minimum_required(VERSION 2.8)
project(task_process)
FILE(GLOB SRC_LIST "*.cpp")
add_executable(task_process ${SRC_LIST})
target_link_libraries(task_process boost_system boost_thread pthread)
