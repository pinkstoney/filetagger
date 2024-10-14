#pragma once

#include <string>
#include <vector>

namespace filetagger
{

struct FileInfo
{
    std::string category;
    std::string filePath;
};

class FileScanner
{
public:
    explicit FileScanner(const std::string& directoryPath);
    std::vector<FileInfo> scanDirectory();

private:
    std::string directoryPath_;
};

} // namespace filetagger
