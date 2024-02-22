#include <string.h>
#include <iostream>
#include "FileDownloader.h"

FileDownloader::FileDownloader() : threadPool(16) {
    partWorkInfos.clear();
    url = "";
    std::string outFileName = "";
    threadNumber = 16;
    fileSize = 0.0;
}


FileDownloader::FileDownloader(const int t_threadNumber)
    : threadNumber(t_threadNumber), threadPool(t_threadNumber) {
    partWorkInfos.clear();
    url = "";
    std::string outFileName = "";
    fileSize = 0.0;
}

bool FileDownloader::download(const std::string& url, const std::string& fileName) {
    bool ret = false;
    outFileName = fileName;
    this->url = url;
    ret = getDownloadFileLength(url);
    if (!ret) {
        LOG_C(LOG_LEVEL_ERROR, "downloadFileLength fail.");
        return false;
    }
    LOG_C(LOG_LEVEL_INFO, "Expected content length: %.0f bytes", fileSize);

    outFile.open(outFileName, std::ios::binary);
    if (!outFile.is_open()) {
        LOG_C(LOG_LEVEL_ERROR, "downloadFileLength fail, open outfile fail.");
        return false;
    }

    uint64_t partSize = fileSize / threadNumber;

    partWorkInfos.resize(threadNumber + 1);
    std::vector<std::future<bool>> futures;

    // 3.创建多个线程，每个线程一个文件下载器
    for (int i = 0;i < threadNumber;i ++) {
        partWorkInfos[i].offset = i * partSize;
        partWorkInfos[i].downerInfo = this;
        partWorkInfos[i].partNunber = i + 1;
        if (i < threadNumber) {
            partWorkInfos[i].end = (i+1) * partSize - 1;
        } else {
            partWorkInfos[i].end = fileSize - 1;
        }

        auto downloadTask = std::bind(&FileDownloader::downloadEachPart, this, std::placeholders::_1);
        try {
            futures.push_back(threadPool.enqueue(downloadTask, &partWorkInfos[i]));
        } catch (const std::runtime_error& e) {
            LOG_C(LOG_LEVEL_ERROR, "add task fail.");
        }
    }

    // 等待所有任务完成
    for (auto& fut : futures) {
        try {
            ret |= fut.get();
        } catch (const std::runtime_error& e) {
            LOG_C(LOG_LEVEL_ERROR, "download fail.");
        }
    }
    
    if (ret) {
        std::cout << "下载完成" << std::endl;
    } else {
        std::cout << "下载失败" << std::endl;
    }
    outFile.close();
    return 0;
}

std::string FileDownloader::getUrl() {
    return url;
}