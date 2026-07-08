#include "../include/searcher.h"
#include <math.h>
#include <stdlib.h>
#include <string.h>

// 默认检索权重：颜色 35%、纹理 35%、形状 30%
static SearchWeights g_weights = {0.35f, 0.35f, 0.30f};

// 将权重归一化，使三者和为 1
static void normalize_weights(SearchWeights* w)
{
    float sum = w->color + w->texture + w->shape;
    if (sum > 0.0f) {
        w->color   /= sum;
        w->texture /= sum;
        w->shape   /= sum;
    }
}

DLL_API void set_search_weights(const SearchWeights* w)
{
    if (w) {
        g_weights = *w;
    } else {
        g_weights = {0.35f, 0.35f, 0.30f};
    }
    normalize_weights(&g_weights);
}

// 计算两个等长向量的余弦相似度
static float cosine_block(const float* a, const float* b, int dim)
{
    float dot  = 0.0f;
    float na   = 0.0f;
    float nb   = 0.0f;

    for (int i = 0; i < dim; ++i) {
        dot += a[i] * b[i];
        na  += a[i] * a[i];
        nb  += b[i] * b[i];
    }

    if (na <= 0.0f || nb <= 0.0f) {
        return 0.0f;
    }

    return dot / (sqrtf(na) * sqrtf(nb));
}

static int compare_result(const void* a, const void* b)
{
    float sa = ((SearchResult*)a)->similarity;
    float sb = ((SearchResult*)b)->similarity;

    if (sb > sa) return 1;
    if (sb < sa) return -1;
    return 0;
}

DLL_API void search_similar(ImageDB* db, float* query_hist, int k,
                            SearchResult* results, int* result_count,
                            const char* exclude_path)
{
    SearchResult temp[MAX_IMAGES];
    int valid_count = 0;

    const float* q = query_hist;

    for (int i = 0; i < db->count; i++) {
        if (!db->images[i].has_feature) {
            continue;
        }

        // 排除查询图片本身，避免它自己排第一
        if (exclude_path && strcmp(db->images[i].path, exclude_path) == 0) {
            continue;
        }

        const float* d = db->images[i].hist;

        // 分别计算三种特征的余弦相似度
        float sim_hsv = cosine_block(q + HSV_OFFSET, d + HSV_OFFSET, HSV_DIM);
        float sim_lbp = cosine_block(q + LBP_OFFSET, d + LBP_OFFSET, LBP_DIM);
        float sim_hu  = cosine_block(q + HU_OFFSET,  d + HU_OFFSET,  HU_DIM);

        // 加权融合（权重已归一化，结果仍在 [0, 1] 区间）
        float sim = g_weights.color   * sim_hsv
                  + g_weights.texture * sim_lbp
                  + g_weights.shape   * sim_hu;

        temp[valid_count].image_id   = i;
        temp[valid_count].similarity = sim;
        strcpy(temp[valid_count].path, db->images[i].path);
        valid_count++;
    }

    qsort(temp, valid_count, sizeof(SearchResult), compare_result);

    *result_count = (valid_count < k) ? valid_count : k;
    for (int i = 0; i < *result_count; i++) {
        results[i] = temp[i];
    }
}
