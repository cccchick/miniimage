cmake_minimum_required(VERSION 3.10)
project(ImageLoaderDLL)

# 设置 C++ 标准
set(CMAKE_CXX_STANDARD 11)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# 添加动态库（SHARED 表示生成 DLL）
add_library(ImageLoaderDLL SHARED
    src/image_loader.cpp  # 源文件路径
    include/image_loader.h  # 头文件（确保编译时能找到）
)

# 导出 include 目录，使链接本 DLL 的目标（如 App）能自动找到头文件
target_include_directories(ImageLoaderDLL
    PUBLIC
    ${CMAKE_CURRENT_SOURCE_DIR}/include
    ${CMAKE_SOURCE_DIR}
)

# 定义导出宏（与头文件中的 IMAGELOADER_EXPORTS 对应）
# 编译该 DLL 时，宏生效使函数导出；其他模块链接时自动使用导入宏
target_compile_definitions(ImageLoaderDLL PRIVATE IMAGELOADER_EXPORTS)
