#pragma once

#ifdef INDEXER_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include "common.h"

extern "C" {
    // 建立索引：校验图库中所有图片是否已提取特征
    DLL_API int build_index(ImageDB* db);
}
