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
    FileTagger(const std::string& directoryPath);
    void tag();
    
    void populateDatabase();

private:
    std::string directoryPath_;
    FileScanner scanner_;
    DatabaseManager dbManager_;
};

} // namespace filetagger
