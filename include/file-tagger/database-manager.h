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
    ~DatabaseManager() = default; 

public:
    std::filesystem::path getDatabasePath() const { return m_dbPath; }

public:
    void insertFiles(const std::vector<FileInfo>& files);


private:
    class SQLiteHandle
    {
    public:
        SQLiteHandle(const std::string& dbPath)
        {
            if (sqlite3_open(dbPath.c_str(), &m_db) != SQLITE_OK)
                throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(m_db)));
        }

        ~SQLiteHandle()
        {
            if (m_db)
                sqlite3_close(m_db);
        }

        sqlite3* get() const { return m_db; }

    private:
        sqlite3* m_db;
    };

private:
    std::filesystem::path m_dbPath; 
    SQLiteHandle m_db_handle;

private:
    void initializeDatabase();
    void prepareStatements();
    void executeSQL(const std::string& sql);

private:
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> m_insertStmt{nullptr, sqlite3_finalize};
    std::unique_ptr<sqlite3_stmt, decltype(&sqlite3_finalize)> m_selectStmt{nullptr, sqlite3_finalize};
};

} // namespace filetagger
