# 设置cmake版本
cmake_minimum_required(VERSION 3.20)

# 设置项目名称
project(Nokia_Snake VERSION 1.0.0 LANGUAGES C)

# msvc解析utf-8 BOM编码
if(MSVC)
    add_compile_options(/utf-8)
endif()

# 设置功能（指定使用 C11 标准）
# 需要更老编译器时，可以改成 C99。
# 之前的注释：指定 C 语言标准。
set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)
set(CMAKE_C_EXTENSIONS OFF)

# 设置功能（添加 C 源文件）
# 增加新的游戏模块时，在这里添加对应的源文件。
# 之前的注释：添加项目源文件。
add_executable(${PROJECT_NAME}
    source/main.c
    source/Utils.c
    source/Renderer.c
    source/Nokia_Snake.c
)

# 添加头文件路径
target_include_directories(${PROJECT_NAME} PRIVATE
    ${CMAKE_CURRENT_SOURCE_DIR}/include
)
