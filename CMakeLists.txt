cmake_minimum_required(VERSION 3.15)
project(MiniImageRetrieval)

set(CMAKE_CXX_STANDARD 17)

# MinGW 下统一使用 UTF-8 编码，避免中文乱码
if(MINGW)
    add_compile_options(-fexec-charset=UTF-8 -finput-charset=UTF-8)
endif()

# ========== Qt 配置 ==========
# 优先从环境变量 QT6_DIR 读取，否则使用默认路径
if(NOT DEFINED Qt6_DIR)
    if(DEFINED ENV{QT6_DIR})
        set(Qt6_DIR "$ENV{Qt6_DIR}" CACHE PATH "Qt6 cmake directory")
    else()
        set(Qt6_DIR "D:/QT/6.11.1/mingw_64/lib/cmake/Qt6" CACHE PATH "Qt6 cmake directory")
    endif()
endif()

set(CMAKE_AUTOMOC ON)
find_package(Qt6 REQUIRED COMPONENTS Core Gui Widgets)

# ========== OpenCV 配置 ==========
# 优先从环境变量 OpenCV_DIR 读取，否则使用默认路径
if(NOT DEFINED OpenCV_DIR)
    if(DEFINED ENV{OpenCV_DIR})
        set(OpenCV_DIR "$ENV{OpenCV_DIR}" CACHE PATH "OpenCV install directory")
    else()
        set(OpenCV_DIR "C:/Users/chick/opencv-project/opencv-install" CACHE PATH "OpenCV install directory")
    endif()
endif()
find_package(OpenCV REQUIRED)

# 打印确认（调试用）
message(STATUS "Qt6 found: ${Qt6_DIR}")
message(STATUS "OpenCV found: ${OpenCV_VERSION}")
message(STATUS "OpenCV include: ${OpenCV_INCLUDE_DIRS}")
message(STATUS "OpenCV libs: ${OpenCV_LIBS}")

include_directories(${CMAKE_SOURCE_DIR})

add_subdirectory(ImageLoaderDLL)
add_subdirectory(FeatureExtractorDLL)
add_subdirectory(IndexerDLL)
add_subdirectory(SearcherDLL)
add_subdirectory(App)
