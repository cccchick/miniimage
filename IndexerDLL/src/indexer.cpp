#include "../include/indexer.h"
#include <stdio.h>

int build_index(ImageDB* db) {
    if (db == nullptr) {
        printf("[IndexerDLL] 数据库指针为空\n");
        return -1;
    }

    int valid_count = 0;
    for (int i = 0; i < db->count; i++) {
        if (db->images[i].has_feature) {
            valid_count++;
        }
    }

    printf("[IndexerDLL] 索引建立完成，有效特征 %d / %d\n",
           valid_count, db->count);
    return valid_count;
}
