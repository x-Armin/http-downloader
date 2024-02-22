#include "common.h"
#include "HttpFileDownloader.h"
#include <string>

int main() {
    // std::string url = "https://releases.ubuntu.com/22.04/ubuntu-22.04.3-desktop-amd64.iso.zsync";
    // std::string outFileName = "ubuntu.zenger";
    // int threadNumber = 2;

    std::string url = "";
    std::string outFileName = "";
    int threadNumber = 1;

    std::cout << "请输入下载链接:" << std::endl;
    std::cin >> url;

    std::cout << "请输入本地保存文件名:" << std::endl;
    std::cin >> outFileName;

    std::cout << "请输入并发数:" << std::endl;
    std::cin >> threadNumber;

    HttpFileDownloader downloader(threadNumber);

    downloader.download(url, outFileName);

    return 0;
}
