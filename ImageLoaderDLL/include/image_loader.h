#pragma once
#ifdef IMAGELOADER_EXPORTS  // 修改为 IMAGELOADER_EXPORTS
#define DLL_API __declspec(dllexport)
#else
#define DLL_API __declspec(dllimport)
#endif

#include "common.h"

extern "C" {
    DLL_API int load_image_paths(const char* folder_path, ImageDB* db);
}
