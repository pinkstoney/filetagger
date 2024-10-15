#pragma once

#include "file-scanner.h"
#include <sqlite3.h>
#include <string>
#include <vector>
#include <filesystem>
#include <memory>

namespace filetagger
{

class DatabaseManager
{
public:
    explicit DatabaseManager(const std::filesystem::path& dbPath);
    ~DatabaseManager();

public:
    std::filesystem::path getDatabasePath() const { return m_dbPath; }

public:
    void insertFiles(const std::vector<FileInfo>& files);

private:
    std::filesystem::path m_dbPath; 
    sqlite3* m_db_handle;

private:
    void initializeDatabase();
    void prepareStatements();
    void executeSQL(const std::string& sql);

private:
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> m_insertStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> m_selectStmt{nullptr, sqlite3_finalize};
};

} // namespace filetagger
