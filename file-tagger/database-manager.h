#pragma once

#include <string>
#include <vector>
#include "file-scanner.h"
#include <sqlite3.h>

namespace filetagger
{

class DatabaseManager
{
public:
    explicit DatabaseManager(const std::string& dbPath);
    ~DatabaseManager();

    void insertFiles(const std::vector<FileInfo>& files);

private:
    sqlite3* db_;

    void initializeDatabase();
};

} // namespace filetagger
