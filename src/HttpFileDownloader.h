#ifndef __HTPP_FAIL_DOWNLOADER_H__
#define __HTPP_FAIL_DOWNLOADER_H__

#include "FileDownloader.h"
#include <fstream>

class HttpFileDownloader : public FileDownloader {
public:
    HttpFileDownloader(const int threadNumber)
        : FileDownloader(threadNumber) {}
    size_t writeFunc(void *ptr, size_t size, size_t memb, void *userdata);
    static size_t writeFuncCallBack(void *ptr, size_t size, size_t memb, void *userdata);
private:
    bool getDownloadFileLength(const std::string url) override;
    bool downloadEachPart(partWorkInfo *partInfo) override;
};

#endif // __HTPP_FAIL_DOWNLOADER_H__