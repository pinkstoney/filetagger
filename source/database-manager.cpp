#include <file-tagger/database-manager.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace filetagger
{

DatabaseManager::DatabaseManager(const std::filesystem::path& dbPath)
    : m_dbPath(dbPath)
    , m_db_handle(m_dbPath.string())
{
    initializeDatabase();
    prepareStatements();
}

void DatabaseManager::initializeDatabase()
{
    const char* createTableSQL = "CREATE TABLE IF NOT EXISTS files ("
                                 "id INTEGER PRIMARY KEY AUTOINCREMENT, "
                                 "category TEXT NOT NULL, "
                                 "file_path TEXT NOT NULL UNIQUE);";
    executeSQL(createTableSQL);
}

void DatabaseManager::prepareStatements()
{
    const char* insertSQL = "INSERT INTO files (category, file_path) VALUES (?, ?);";
    const char* selectSQL = "SELECT id FROM files WHERE file_path = ?;";

    sqlite3_stmt* insertStmt = nullptr;
    if (sqlite3_prepare_v2(m_db_handle.get(), insertSQL, -1, &insertStmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare insert statement: " + std::string(sqlite3_errmsg(m_db_handle.get())));
    m_insertStmt.reset(insertStmt);

    sqlite3_stmt* selectStmt = nullptr;
    if (sqlite3_prepare_v2(m_db_handle.get(), selectSQL, -1, &selectStmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(m_db_handle.get())));
    m_selectStmt.reset(selectStmt);
}

void DatabaseManager::insertFiles(const std::vector<FileInfo>& files)
{
    executeSQL("BEGIN TRANSACTION;");

    for (const auto& file : files)
    {
        if (sqlite3_bind_text(m_selectStmt.get(), 1, file.filePath.string().c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
        {
            std::cerr << "Failed to bind select statement: " << sqlite3_errmsg(m_db_handle.get()) << std::endl;
            sqlite3_reset(m_selectStmt.get());
            continue;
        }

        int selectResult = sqlite3_step(m_selectStmt.get());
        if (selectResult == SQLITE_ROW)
        {
            std::cout << "File already exists in database: " << file.filePath.string() << std::endl;
            sqlite3_reset(m_selectStmt.get());
            continue;
        }
        else if (selectResult != SQLITE_DONE && selectResult != SQLITE_ROW)
        {
            std::cerr << "Failed to execute select statement: " << sqlite3_errmsg(m_db_handle.get()) << std::endl;
            sqlite3_reset(m_selectStmt.get());
            continue;
        }
        sqlite3_reset(m_selectStmt.get());

        if (sqlite3_bind_text(m_insertStmt.get(), 1, file.category.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(m_insertStmt.get(), 2, file.filePath.string().c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
        {
            std::cerr << "Failed to bind insert statement: " << sqlite3_errmsg(m_db_handle.get()) << std::endl;
            sqlite3_reset(m_insertStmt.get());
            continue;
        }

        if (sqlite3_step(m_insertStmt.get()) != SQLITE_DONE)
            std::cerr << "Failed to insert file: " << file.filePath.string() << " Error: " << sqlite3_errmsg(m_db_handle.get())
                      << std::endl;
        else
            std::cout << "Inserted file into database: " << file.filePath.string() << std::endl;

        sqlite3_reset(m_insertStmt.get());
    }

    executeSQL("END TRANSACTION;");
}

void DatabaseManager::executeSQL(const std::string& sql)
{
    char* errMsg = nullptr;
    if (sqlite3_exec(m_db_handle.get(), sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + error);
    }
}

} // namespace filetagger
