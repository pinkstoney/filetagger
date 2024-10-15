#include <file-tagger/database-manager.h>

#include <filesystem>
#include <iostream>
#include <memory>
#include <stdexcept>

namespace filetagger
{

DatabaseManager::DatabaseManager(const std::filesystem::path& dbPath)
    : m_dbPath(dbPath)
    , m_db_handle(nullptr)
{
    if (sqlite3_open(m_dbPath.string().c_str(), &m_db_handle) != SQLITE_OK)
    {
        throw std::runtime_error("Failed to open database: " + std::string(sqlite3_errmsg(m_db_handle)));
    }
    initializeDatabase();
    prepareStatements();
}

DatabaseManager::~DatabaseManager()
{
    if (m_db_handle)
    {
        sqlite3_close(m_db_handle);
    }
}

void DatabaseManager::initializeDatabase()
{
    const std::string createTableSQL = "CREATE TABLE IF NOT EXISTS files ("
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
    if (sqlite3_prepare_v2(m_db_handle, insertSQL, -1, &insertStmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare insert statement: " + std::string(sqlite3_errmsg(m_db_handle)));
    m_insertStmt.reset(insertStmt);

    sqlite3_stmt* selectStmt = nullptr;
    if (sqlite3_prepare_v2(m_db_handle, selectSQL, -1, &selectStmt, nullptr) != SQLITE_OK)
        throw std::runtime_error("Failed to prepare select statement: " + std::string(sqlite3_errmsg(m_db_handle)));
    m_selectStmt.reset(selectStmt);
}

void DatabaseManager::insertFiles(const std::vector<FileInfo>& files)
{
    executeSQL("BEGIN TRANSACTION;");

    for (const auto& file : files)
    {
        // Bind file path to select statement
        if (sqlite3_bind_text(m_selectStmt.get(), 1, file.filePath.string().c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
        {
            std::cerr << "Failed to bind select statement: " << sqlite3_errmsg(m_db_handle) << std::endl;
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
            std::cerr << "Failed to execute select statement: " << sqlite3_errmsg(m_db_handle) << std::endl;
            sqlite3_reset(m_selectStmt.get());
            continue;
        }
        sqlite3_reset(m_selectStmt.get());

        // Bind category and file path to insert statement
        if (sqlite3_bind_text(m_insertStmt.get(), 1, file.category.c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK ||
            sqlite3_bind_text(m_insertStmt.get(), 2, file.filePath.string().c_str(), -1, SQLITE_TRANSIENT) != SQLITE_OK)
        {
            std::cerr << "Failed to bind insert statement: " << sqlite3_errmsg(m_db_handle) << std::endl;
            sqlite3_reset(m_insertStmt.get());
            continue;
        }

        // Execute insert
        if (sqlite3_step(m_insertStmt.get()) != SQLITE_DONE)
            std::cerr << "Failed to insert file: " << file.filePath.string() << " Error: " << sqlite3_errmsg(m_db_handle)
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
    if (sqlite3_exec(m_db_handle, sql.c_str(), nullptr, nullptr, &errMsg) != SQLITE_OK)
    {
        std::string error = errMsg;
        sqlite3_free(errMsg);
        throw std::runtime_error("SQL error: " + error);
    }
}

} // namespace filetagger