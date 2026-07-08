#pragma once

#ifdef SEARCHER_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include "common.h"

// 可调整的检索权重（会自动归一化为和为 1）
struct SearchWeights {
    float color;    // HSV 颜色特征权重
    float texture;  // LBP 纹理特征权重
    float shape;    // Hu 形状特征权重
};

extern "C" {
    // 设置检索权重；传入 nullptr 则恢复默认权重
    DLL_API void set_search_weights(const SearchWeights* w);

    // 按加权多特征相似度检索 Top-K 图片
    // exclude_path：需要排除的图片路径（例如查询图片本身），nullptr 表示不排除
    DLL_API void search_similar(ImageDB* db, float* query_hist, int k,
                                SearchResult* results, int* result_count,
                                const char* exclude_path = nullptr);
}
