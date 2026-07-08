#include "../include/feature_extract.h"
#include <opencv2/opencv.hpp>
#include <stdio.h>
#include <cmath>
#include <vector>

// ============================================================
// Uniform LBP 查找表（P=8，半径=1）
// 58 个 uniform 模式 + 1 个非 uniform 桶 = 59 bins
// ============================================================
static bool is_uniform_lbp(int code)
{
    int bits = code | (code << 8);
    int transitions = 0;
    for (int i = 0; i < 8; ++i) {
        int cur  = (bits >> i) & 1;
        int next = (bits >> (i + 1)) & 1;
        if (cur != next) ++transitions;
    }
    return transitions <= 2;
}

static void build_uniform_lbp_lookup(int table[256])
{
    // 第一遍：给 uniform 模式按顺序编号
    int idx = 0;
    for (int code = 0; code < 256; ++code) {
        if (is_uniform_lbp(code)) {
            table[code] = idx++;
        }
    }

    // 第二遍：非 uniform 模式全部归入最后一个桶
    for (int code = 0; code < 256; ++code) {
        if (!is_uniform_lbp(code)) {
            table[code] = LBP_BINS - 1;
        }
    }
}

// ============================================================
// 提取单张图片的多特征向量
// 包含：HSV 颜色直方图 + LBP 纹理直方图 + Hu 形状不变矩
// ============================================================
int extract_feature(ImageInfo* img)
{
    // 1. 读取图片
    cv::Mat mat = cv::imread(img->path);
    if (mat.empty()) {
        printf("[FeatureExtractDLL] 无法读取图片: %s\n", img->path);
        return -1;
    }

    img->width  = mat.cols;
    img->height = mat.rows;

    // 清零，避免旧数据残留
    for (int i = 0; i < FEATURE_DIM; ++i) {
        img->hist[i] = 0.0f;
    }

    // ========================================================
    // 2. HSV 颜色直方图（替代原来的 BGR）
    // ========================================================
    cv::Mat hsv;
    cv::cvtColor(mat, hsv, cv::COLOR_BGR2HSV);

    std::vector<cv::Mat> hsv_planes;
    cv::split(hsv, hsv_planes);

    int    hsvSize    = HSV_BINS;
    float  h_range[]  = {0, 180};   // Hue 范围
    float  sv_range[] = {0, 256};   // Saturation / Value 范围
    const float* ranges_h  = h_range;
    const float* ranges_sv = sv_range;

    cv::Mat h_hist, s_hist, v_hist;
    const int channel_0[] = {0};
    const int channel_1[] = {0};
    const int channel_2[] = {0};
    cv::calcHist(&hsv_planes[0], 1, channel_0, cv::Mat(), h_hist, 1, &hsvSize, &ranges_h);
    cv::calcHist(&hsv_planes[1], 1, channel_1, cv::Mat(), s_hist, 1, &hsvSize, &ranges_sv);
    cv::calcHist(&hsv_planes[2], 1, channel_2, cv::Mat(), v_hist, 1, &hsvSize, &ranges_sv);

    cv::normalize(h_hist, h_hist, 1, 0, cv::NORM_L1);
    cv::normalize(s_hist, s_hist, 1, 0, cv::NORM_L1);
    cv::normalize(v_hist, v_hist, 1, 0, cv::NORM_L1);

    for (int i = 0; i < HSV_BINS; ++i) {
        img->hist[HSV_OFFSET + i]              = h_hist.at<float>(i);
        img->hist[HSV_OFFSET + i + HSV_BINS]   = s_hist.at<float>(i);
        img->hist[HSV_OFFSET + i + 2 * HSV_BINS] = v_hist.at<float>(i);
    }

    // ========================================================
    // 3. Uniform LBP 纹理直方图
    // ========================================================
    cv::Mat gray;
    cv::cvtColor(mat, gray, cv::COLOR_BGR2GRAY);

    static int lbp_lookup[256];
    static bool lbp_lookup_ready = false;
    if (!lbp_lookup_ready) {
        build_uniform_lbp_lookup(lbp_lookup);
        lbp_lookup_ready = true;
    }

    float lbp_hist[LBP_BINS] = {0.0f};
    int valid_pixels = 0;

    // 8 邻域偏移（从左上角顺时针）
    const int dy[8] = {-1, -1, -1, 0, 1, 1,  1,  0};
    const int dx[8] = {-1,  0,  1, 1, 1, 0, -1, -1};

    for (int r = 1; r < gray.rows - 1; ++r) {
        for (int c = 1; c < gray.cols - 1; ++c) {
            uchar center = gray.at<uchar>(r, c);
            int code = 0;
            for (int n = 0; n < 8; ++n) {
                if (gray.at<uchar>(r + dy[n], c + dx[n]) >= center) {
                    code |= (1 << n);
                }
            }
            ++lbp_hist[lbp_lookup[code]];
            ++valid_pixels;
        }
    }

    if (valid_pixels > 0) {
        for (int i = 0; i < LBP_BINS; ++i) {
            lbp_hist[i] /= static_cast<float>(valid_pixels);
        }
    }
    for (int i = 0; i < LBP_BINS; ++i) {
        img->hist[LBP_OFFSET + i] = lbp_hist[i];
    }

    // ========================================================
    // 4. Hu 形状不变矩
    // ========================================================
    cv::Moments m = cv::moments(gray, false);
    double hu[7];
    cv::HuMoments(m, hu);

    float hu_log[HU_DIM];
    for (int i = 0; i < HU_DIM; ++i) {
        double v = std::fabs(hu[i]);
        if (v < 1e-10) v = 1e-10;  // 防止 log10(0) 得到 -inf
        hu_log[i] = static_cast<float>(std::log10(v));
    }

    // L2 归一化
    float norm = 0.0f;
    for (int i = 0; i < HU_DIM; ++i) {
        norm += hu_log[i] * hu_log[i];
    }
    norm = std::sqrt(norm);

    if (norm > 1e-10f) {
        for (int i = 0; i < HU_DIM; ++i) {
            img->hist[HU_OFFSET + i] = hu_log[i] / norm;
        }
    } else {
        for (int i = 0; i < HU_DIM; ++i) {
            img->hist[HU_OFFSET + i] = 0.0f;
        }
    }

    img->has_feature = 1;
    printf("[FeatureExtractDLL] 提取特征成功: %s (%dx%d), dim=%d\n",
           img->path, img->width, img->height, FEATURE_DIM);
    return 0;
}

// ============================================================
// 批量提取数据库中所有图片的特征
// ============================================================
void extract_all(ImageDB* db)
{
    for (int i = 0; i < db->count; i++) {
        extract_feature(&db->images[i]);
    }
    printf("[FeatureExtractDLL] 批量提取完成，共 %d 张\n", db->count);
}
