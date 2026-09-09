# 设置cmake版本
cmake_minimum_required(VERSION 4.0)

# 设置项目名称
project(Nokia_Snake VERSION 1.0.0 LANGUAGES C CXX)

# msvc解析utf-8 BOM编码
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 设置c++版本
set(CMAKE_CXX_STANDARD 17)
# 指定标准编译
set(CMAKE_CXX_STANDARD_REQUIRED ON)
# 禁用扩展
set(CMAKE_CXX_EXTENSIONS OFF)

# 添加c++源文件
add_executable(${PROJECT_NAME}
    source/main.cpp
    source/Utils.cpp
    source/Nokia_Snake.cpp
)

# 引入 raygui
FetchContent_Declare(
    raygui
    URL https://github.com/raysan5/raygui/archive/refs/heads/master.tar.gz
    DOWNLOAD_EXTRACT_TIMESTAMP ON
)

FetchContent_MakeAvailable(raygui)

# 添加 raygui 头文件路径（下载后会生成 raygui_SOURCE_DIR）
target_include_directories(${PROJECT_NAME} PRIVATE
    ${raygui_SOURCE_DIR}/src
)

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
