#ifndef __COMMON_H__
#define __COMMON_H__

#include <stdio.h>
#include <unistd.h>
#include <curl/curl.h>
#include <fcntl.h>
// 需要在linux上使用
#include <sys/mman.h>
#include <string.h>
#include <stdlib.h>
#include <pthread.h>
#include <signal.h>

#include "FileDownloader.h"

// 定义日志级别
#define LOG_LEVEL_INFO  0
#define LOG_LEVEL_WARN  1
#define LOG_LEVEL_ERROR 2

// 设置日志级别
#ifndef LOG_LEVEL
#define LOG_LEVEL LOG_LEVEL_INFO
#endif

// 定义日志宏
#define LOG_C(level, format, ...) \
    do { \
        if (level >= LOG_LEVEL) { \
            printf( format "| [%s:%d] " "\n", ##__VA_ARGS__, __FILE__, __LINE__); \
        } \
    } while(0)

class FileDownloader;

struct partWorkInfo {
    FileDownloader* downerInfo;

    int partNunber;
    uint64_t offset;
    uint64_t end;
    pthread_t thid;
};

#endif // __COMMON_H__