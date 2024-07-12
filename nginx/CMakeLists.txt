cmake_minimum_required(VERSION 3.0)
project(Server)

set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED on)


set(HIREDIS_INCLUDE_DIR "/usr/local/include")
set(HIREDIS_LIBRARIES "/usr/local/lib/libhiredis.so")


#包含头文件以及外部连接头文件
include_directories(
            ${CMAKE_SOURCE_DIR}/_include
            ${HIREDIS_INCLUDE_DIR}   
)
find_package(Threads REQUIRED)
set(SRC_DIR1 "${CMAKE_SOURCE_DIR}/app")
set(SRC_DIR2 "${CMAKE_SOURCE_DIR}/logic")
set(SRC_DIR3 "${CMAKE_SOURCE_DIR}/misc")
set(SRC_DIR4 "${CMAKE_SOURCE_DIR}/net")
set(SRC_DIR5 "${CMAKE_SOURCE_DIR}/proc")
set(SRC_DIR6 "${CMAKE_SOURCE_DIR}/signal")

aux_source_directory(${SRC_DIR1}
SRC_FILES1)
aux_source_directory(${SRC_DIR2}
SRC_FILES2)
aux_source_directory(${SRC_DIR3}
SRC_FILES3)
aux_source_directory(${SRC_DIR4}
SRC_FILES4)
aux_source_directory(${SRC_DIR5}
SRC_FILES5)
aux_source_directory(${SRC_DIR6}
SRC_FILES6)

set(SOURCES
${SRC_FILES1}
${SRC_FILES2}
${SRC_FILES3}
${SRC_FILES4}
${SRC_FILES5}
${SRC_FILES6}
)


add_executable(nginx_Server ${SOURCES})

target_link_libraries(nginx_Server
    ${HIREDIS_LIBRARIES}
    ${CMAKE_THREAD_LIBS_INIT}
    )