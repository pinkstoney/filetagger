#include <file-tagger/file-tagger.h>

#include <filesystem>
#include <iostream>

namespace filetagger
{

FileTagger::FileTagger(const std::filesystem::path& directoryPath)
    : m_directoryPath(directoryPath)
    , m_scanner(directoryPath)
    , m_dbManager("database.db")
{
}

void FileTagger::populateDatabase()
{
    auto files = m_scanner.scanDirectory();
    try
    {
        m_dbManager.insertFiles(files);
        std::cout << "Database populated successfully with " << files.size() << " files.\n";
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error populating database: " << e.what() << '\n';
    }
}

} // namespace filetagger
