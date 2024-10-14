#pragma once

#include "database-manager.h"
#include "file-scanner.h"
#include <filesystem>
#include <sqlite3.h>
#include <string>

namespace filetagger
{

class FileTagger
{
public:
    explicit FileTagger(const std::filesystem::path& directoryPath);

public:
    void populateDatabase();

private:
    std::filesystem::path m_directoryPath;
    FileScanner m_scanner;
    DatabaseManager m_dbManager;

private:
    std::string deriveDatabasePath(const std::string& directoryPath);
};

} // namespace filetagger
