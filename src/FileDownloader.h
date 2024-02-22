#ifndef __FILE_DOWNLOADER_H__
#define __FILE_DOWNLOADER_H__

#include <string>
#include <fstream>
#include "ThreadPool.h"
#include "common.h"

class FileDownloader
{
public:
    virtual bool getDownloadFileLength(const std::string url) = 0;
    FileDownloader();
    FileDownloader(const int t_threadNumber);
    bool download(const std::string& url, const std::string& destination);
    std::string getUrl();
    ~FileDownloader() {}

protected:
    std::vector<struct partWorkInfo> partWorkInfos;
    std::string url;
    std::ofstream outFile;
    std::string outFileName;
    int threadNumber;
    ThreadPool threadPool;
    std::mutex mtx;
    double fileSize;
    virtual bool downloadEachPart(partWorkInfo *partInfo) = 0;
};

#endif // __FILE_DOWNLOADER_H__