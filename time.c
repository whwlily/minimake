#include <sys/stat.h>
#include <stdbool.h>
#include "time.h"

// 检查文件是否存在
inline bool file_exists(const char *filename) {
    struct stat buffer;
    return (stat(filename, &buffer) == 0);
}