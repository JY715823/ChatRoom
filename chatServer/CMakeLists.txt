# 设置CMake最低标准
cmake_minimum_required(VERSION 2.8)

# 设置项目名称
project(chatServer)

# 设置C++标准
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

# 添加可执行文件
add_executable(${PROJECT_NAME}
    ${PROJECT_SOURCE_DIR}/src/main.cpp
    ${PROJECT_SOURCE_DIR}/src/chatServer.cpp
    ${PROJECT_SOURCE_DIR}/src/threadPool.cpp
    ${PROJECT_SOURCE_DIR}/../MSG_CLASS/MSG.cpp
)

# 添加头文件路径
target_include_directories(${PROJECT_NAME}
    PRIVATE
    ${PROJECT_SOURCE_DIR}/header
    ${PROJECT_SOURCE_DIR}/../MSG_CLASS
)

# 链接线程动态库
target_link_libraries(${PROJECT_NAME} -lpthread)

