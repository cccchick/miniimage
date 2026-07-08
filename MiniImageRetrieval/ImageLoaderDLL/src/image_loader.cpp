#include "image_loader.h"
#include <windows.h>  // Windows API（FindFirstFile等）
#include <stdio.h>    // 标准输入输出
#include <string>     // 字符串处理（C++标准库）

int load_image_paths(const char* folder_path, ImageDB* db) {
    db->count = 0;  // 初始化图片数量为0

    // 构造搜索模式（如 "folder_path\*" 匹配所有文件）
    std::string search_path = std::string(folder_path) + "\\*";
    WIN32_FIND_DATAA find_file_data;  // 存储文件信息
    HANDLE h_find = FindFirstFileA(search_path.c_str(), &find_file_data);

    // 检查文件夹是否存在或无文件
    if (h_find == INVALID_HANDLE_VALUE) {
        return 0;
    }

    // 遍历文件夹中的所有文件
    do {
        // 获取文件名（如 "cat1.jpg"）
        std::string file_name = find_file_data.cFileName;

        // 筛选.jpg和.png文件（忽略其他类型）
        if (file_name.size() > 4 && 
            (file_name.substr(file_name.size() - 4) == ".jpg" || 
             file_name.substr(file_name.size() - 4) == ".png")) {
            
            // 检查是否超过最大图片数量限制
            if (db->count < MAX_IMAGES) {
                // 构造完整路径（如 "folder_path\cat1.jpg"）
                std::string full_path = std::string(folder_path) + "\\" + file_name;
                // 将路径复制到ImageDB的ImageInfo结构体中
                strcpy_s(db->images[db->count].path, MAX_PATH_LEN, full_path.c_str());
                db->count++;  // 图片数量+1
            }
        }
    } while (FindNextFileA(h_find, &find_file_data) != 0);  // 继续查找下一个文件

    FindClose(h_find);  // 关闭文件查找句柄
    return db->count;   // 返回找到的图片数量
}
