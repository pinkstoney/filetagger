#pragma once

#include <string>
#include <vector>
#include <filesystem>

namespace filetagger
{

struct FileInfo
{
    std::string category;
    std::filesystem::path filePath;
};

class FileScanner
{
public:
    explicit FileScanner(const std::filesystem::path& directoryPath);

public:
    std::vector<FileInfo> scanDirectory();

private:
    std::filesystem::path m_directoryPath;
};

} // namespace filetagger