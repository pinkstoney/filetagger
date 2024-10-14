#include <file-tagger/file-scanner.h>

#include <filesystem>
#include <iostream>
#include <stdexcept>
#include <vector>

namespace fs = std::filesystem;

namespace filetagger
{

FileScanner::FileScanner(const std::filesystem::path& directoryPath)
    : m_directoryPath(directoryPath)
{
    if (!fs::exists(m_directoryPath) || !fs::is_directory(m_directoryPath))
        throw std::runtime_error("Invalid directory path: " + m_directoryPath.string());
}

std::vector<FileInfo> FileScanner::scanDirectory()
{
    std::vector<FileInfo> files;
    try
    {
        for (const auto& entry : fs::recursive_directory_iterator(m_directoryPath))
        {
            if (entry.is_regular_file())
                files.emplace_back(FileInfo{.category = entry.path().parent_path().filename().string(), .filePath = entry.path()});
        }
    }
    catch (const fs::filesystem_error& e)
    {
        std::cerr << "Filesystem error: " << e.what() << '\n';
    }
    return files;
}

} // namespace filetagger
