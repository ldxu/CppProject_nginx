cmake_minimum_required(VERSION 3.0)
project(CliForNginx)

# 设置编译器标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 找到 spdlog 库和头文件
find_package(spdlog REQUIRED)

if(spdlog_FOUND)
    message(STATUS "Found spdlog: ${spdlog_DIR}")
else()
    message(FATAL_ERROR "spdlog not found")
endif()
# 手动设置 libconfig++ 库的路径
# 如果 libconfig++ 库在系统路径中无法找到，请手动指定库的路径
# 假设库在 /usr/local/lib 和头文件在 /usr/local/include
set(LIBCONFIGPP_LIBRARY /usr/local/lib/libconfig++.so)
set(LIBCONFIGPP_INCLUDE_DIR /usr/local/include)

# 设置spdlog路径
# set(SPDLOG_INCLUDE_DIR /usr/local/include)
# set(SPDLOG_LIBRARY /usr/local/lib/libspdlog.so)


# 设置头文件目录
include_directories(
            ${CMAKE_SOURCE_DIR}/_include 
            ${spdlog_INCLUDE_DIR}
            ${LIBCONFIGPP_INCLUDE_DIR}
        )

# 添加源文件
set(SOURCES
    _src/cli.cpp
    _src/client.cpp
    _src/cli_config.cpp
    _src/log.cpp
    _src/crc32.cpp
)

# 添加可执行文件
add_executable(CliForNginx ${SOURCES})


# 链接 spdlog 库
# 链接 libconfig++ 库
target_link_libraries(CliForNginx
    ${LIBCONFIGPP_LIBRARY}
    spdlog::spdlog
)