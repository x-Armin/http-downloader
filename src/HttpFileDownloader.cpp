#include "HttpFileDownloader.h"
#include "common.h"
#include <curl/curl.h>
#include <iostream>

bool HttpFileDownloader::getDownloadFileLength(const std::string url) {
    bool ret = false;
    CURL *curl = curl_easy_init();

    LOG_C(LOG_LEVEL_INFO, "url: %s" , url.c_str());
    curl_easy_setopt(curl, CURLOPT_URL, url.c_str());
    curl_easy_setopt(curl, CURLOPT_HEADER, 1);
    curl_easy_setopt(curl, CURLOPT_NOBODY, 1);

    CURLcode curlRet = curl_easy_perform(curl);
    if (curlRet == CURLE_OK) {
        curlRet = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &fileSize);
        if (curlRet == CURLE_OK) {
            LOG_C(LOG_LEVEL_INFO, "Expected content length: %.0f bytes", fileSize);
            ret = true;
        } else {
            LOG_C(LOG_LEVEL_ERROR, "downloadFileLength error");
        }
    } else {
        LOG_C(LOG_LEVEL_ERROR, "downloadFileLength error, curlRet=%d", curlRet);
    }
    curl_easy_cleanup(curl);

    return ret;
}

size_t HttpFileDownloader::writeFuncCallBack(void *ptr, size_t size, size_t memb, void *userdata) {
    size_t ret = 0;
    struct partWorkInfo *info = (struct partWorkInfo *)userdata;
    HttpFileDownloader* HttpFileDownloaderPtr = dynamic_cast<HttpFileDownloader*>(info->downerInfo);

    if (HttpFileDownloaderPtr != nullptr) {
        ret = HttpFileDownloaderPtr->writeFunc(ptr, size, memb, userdata);
    } else {
        LOG_C(LOG_LEVEL_ERROR, "Conversion failed");
    }

    return ret;
}

// 写入回调
size_t HttpFileDownloader::writeFunc(void *ptr, size_t size, size_t memb, void *userdata) {
    struct partWorkInfo *info = (struct partWorkInfo *)userdata;
    std::lock_guard<std::mutex> lock(mtx);  // 同时只有一个在写文件
    
    outFile.seekp(info->offset);
    outFile.write((char *)ptr, size * memb);
    info->offset += size * memb;

    return size * memb;
}

bool HttpFileDownloader::downloadEachPart(partWorkInfo *partInfo) {
    LOG_C(LOG_LEVEL_INFO, "partInfo thid=%lu offset=%lu, end=%lu, url=%s",
        partInfo->thid, partInfo->offset, partInfo->end, partInfo->downerInfo->getUrl().c_str());
    bool ret = false;

    char range[64] = {0};
    snprintf(range, 64, "%lu-%lu", partInfo->offset, partInfo->end);

    CURL *curl = curl_easy_init();
    curl_easy_setopt(curl, CURLOPT_URL, partInfo->downerInfo->getUrl().c_str());
    curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, HttpFileDownloader::writeFuncCallBack);
    curl_easy_setopt(curl, CURLOPT_WRITEDATA, partInfo);
    curl_easy_setopt(curl, CURLOPT_RANGE, range);

    CURLcode res = curl_easy_perform(curl);
    if (res != CURLE_OK) {
        LOG_C(LOG_LEVEL_ERROR, "ret = %d", res);
    }
    curl_easy_cleanup(curl);

    return ret;
}

