#pragma once

#ifdef FEATURE_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include "common.h"

extern "C" {
    // 提取单张图片的颜色直方图特征
    DLL_API int extract_feature(ImageInfo* img);
    
    // 批量提取数据库中所有图片的特征
    DLL_API void extract_all(ImageDB* db);
}