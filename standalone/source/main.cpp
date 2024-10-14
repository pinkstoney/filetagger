#include <file-tagger/file-tagger.h>

#include <iostream>
#include <stdexcept>

int main()
{
    try
    {
        filetagger::FileTagger ft("/Users/hryniv/Documents/data/");
        ft.populateDatabase();
    }
    catch (const std::exception& e)
    {
        std::cerr << "Error: " << e.what() << std::endl;
        return 1;
    }
}
