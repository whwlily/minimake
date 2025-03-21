#include <sys/stat.h>
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include "line.h"
#include "time.h"

// 获取文件的修改时间
time_t get_file_mod_time(const char *filename) {
    struct stat buffer;
    if (stat(filename, &buffer) == 0) {
        return buffer.st_mtime;
    }
    return -1; 
}

// 检查目标是否重复定义
bool is_duplicate_target(const char *target) {
    int count = 0;
    for (int i = 0; i < rule_count; i++) {
        if (strcmp(rules[i].target, target) == 0) {
            count++;
        }
    }
    return count > 1;
}

// timecheck 函数：检查目标文件是否需要重新构建
void timecheck(const char *target) {
    for (int i = 0; i < rule_count; i++) {
        if (target != NULL && strcmp(rules[i].target, target) != 0) {
            continue; 
        }

        // 检查目标是否重复定义
        if (is_duplicate_target(rules[i].target)) {
            printf("错误: Duplicate target definition '%s'\n", rules[i].target);
            return; // 立即停止后续处理
        }

        printf("检查目标: %s\n", rules[i].target);

        // 检查目标文件是否存在
        if (!file_exists(rules[i].target)) {
            printf("目标文件 '%s' 不存在。\n", rules[i].target);

            // 检查依赖文件是否存在
            bool all_deps_exist = true;
            for (int j = 0; j < rules[i].dep_count; j++) {
                if (!file_exists(rules[i].dependencies[j])) {
                    printf("错误: 依赖文件 '%s' 不存在。\n", rules[i].dependencies[j]);
                    all_deps_exist = false;
                }
            }

            // 如果依赖文件不存在，直接退出
            if (!all_deps_exist) {
                printf("构建失败: 目标 '%s' 的依赖文件缺失。\n", rules[i].target);
                return; // 立即停止后续处理
            }

            // 如果所有依赖文件都存在，则构建目标
            printf("所有依赖文件均存在，开始构建目标 '%s'...\n", rules[i].target);
            if (strlen(rules[i].commands) > 0) {
                system(rules[i].commands);
            } else {
                printf("错误: 目标 '%s' 没有构建命令。\n", rules[i].target);
            }
        } else {
            // 如果目标文件存在，检查依赖文件的修改时间
            time_t target_time = get_file_mod_time(rules[i].target);
            bool needs_rebuild = false;

            for (int j = 0; j < rules[i].dep_count; j++) {
                time_t dep_time = get_file_mod_time(rules[i].dependencies[j]);
                if (dep_time > target_time) {
                    printf("依赖文件 '%s' 比目标文件 '%s' 更新，需要重新构建。\n",
                           rules[i].dependencies[j], rules[i].target);
                    needs_rebuild = true;
                }
            }

            // 如果需要重新构建，则执行构建命令
            if (needs_rebuild) {
                printf("开始重新构建目标 '%s'...\n", rules[i].target);
                if (strlen(rules[i].commands) > 0) {
                    system(rules[i].commands); 
                } else {
                    printf("错误: 目标 '%s' 没有构建命令。\n", rules[i].target);
                }
            } else {
                printf("目标文件 '%s' 已是最新，无需重新构建。\n", rules[i].target);
            }
        }
    }
}