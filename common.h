#pragma once

#define MAX_IMAGES 100
#define HIST_BINS 16
#define MAX_PATH_LEN 256

// ============================================================
// 多特征向量定义
// 将 ImageInfo.hist[] 扩展为扁平向量：HSV 颜色 + LBP 纹理 + Hu 形状
// ============================================================

// 1) HSV 颜色直方图：H/S/V 各 16 个 bin，共 48 维
#define HSV_BINS 16
#define HSV_DIM  (HSV_BINS * 3)

// 2) Uniform LBP 纹理直方图：P=8 时 58 个 uniform 模式 + 1 个非 uniform 桶，共 59 维
#define LBP_BINS 59
#define LBP_DIM  LBP_BINS

// 3) Hu 不变矩：7 维
#define HU_DIM   7

// 各特征块在 hist[] 中的起始偏移
#define HSV_OFFSET 0
#define LBP_OFFSET (HSV_DIM)                    // 48
#define HU_OFFSET  (HSV_DIM + LBP_DIM)          // 107
#define FEATURE_DIM (HSV_DIM + LBP_DIM + HU_DIM) // 114

// 单张图片的所有信息
struct ImageInfo {
    char path[MAX_PATH_LEN];
    int width;
    int height;
    float hist[FEATURE_DIM];   // 多特征向量：HSV + LBP + Hu
    int has_feature;           // 是否已提取
};

// 整个图片库
struct ImageDB {
    ImageInfo images[MAX_IMAGES];
    int count;
};

// 检索结果
struct SearchResult {
    int image_id;
    float similarity;  // 越大越相似
    char path[MAX_PATH_LEN];
};
