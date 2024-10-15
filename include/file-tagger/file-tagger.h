#pragma once

#include <string>
#include <filesystem>
#include <vector>
#include <sqlite3.h>
#include "file-scanner.h"
#include "database-manager.h"

namespace filetagger
{

class FileTagger
{
public:
    explicit FileTagger(const std::filesystem::path& directoryPath);
    ~FileTagger() = default;

    void populateDatabase();

private:
    std::filesystem::path m_directoryPath;
    FileScanner m_scanner;
    DatabaseManager m_dbManager;
};

} // namespace filetagger