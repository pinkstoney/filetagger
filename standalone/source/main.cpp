#include <file-tagger/file-tagger.h>
#include <cxxopts.hpp>

#include <iostream>
#include <stdexcept>

int main(int argc, char* argv[])
{
    cxxopts::Options options("FileTagger", "A tool for tagging files");

    options.add_options()
        ("p,path", "Path to the directory to tag", cxxopts::value<std::string>())
        ("h,help", "Print usage")
    ;

    auto result = options.parse(argc, argv);

    if (result.count("help"))
    {
        std::cout << options.help() << std::endl;
        return 0;
    }

    if (!result.count("path"))
    {
        std::cerr << "Error: Path argument is required." << std::endl;
        std::cout << options.help() << std::endl;
        return 1;
    }

    std::string path = result["path"].as<std::string>();

    try
    {
        filetagger::FileTagger ft(path);
        ft.populateDatabase();
        std::cout << "Database populated successfully." << std::endl;
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }

    return 0;
}
