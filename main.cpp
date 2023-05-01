#include "search_duplicate.h"

#include <fcntl.h>
#include <locale.h>
#include <fstream>
#include <iostream>
#include <sstream>
#include <string_view>
#include <vector>
#include <windows.h>
#include <utility>

using namespace std;
using filesystem::path;

namespace fs = std::filesystem;

path operator""_p(const wchar_t *data, std::size_t sz)
{
    return path(data, data + sz);
}

void PrintUsage(std::ostream &stream = std::cerr)
{
    stream << "Usage: search_duplicates <filename_with_catalogue_list>\n"sv;
    stream << "\n<filename_with_catalogue_list> contains:\n"sv;
    stream << "<.extention>\n"sv;
    stream << "<dir1>\n<dir2>\n...\n"sv;
}

int main(int argc, char *argv[])
{
    setlocale(LC_ALL, ".UTF8");
    setlocale(LC_NUMERIC, "us_US.UTF-8"); //чтобы не путать в числах '.' ','

    if (argc != 2)
    {
        PrintUsage();
        return 1;
    }
    if (!exists(path(argv[1])))
    {
        std::cout << "not exists list_file\n"s;
        PrintUsage();
        return 1;
    }
    ifstream in(argv[1]);
    if (!(in))
    {
        cout << "Error open input file: " << argv[1] << " \n"s;
    }
    FileCatalogue catalogue(CreateCatalogue(in));
    cout << "===Found duplicates:"<< endl;
    catalogue.OutputDuplicateCrc();
    cout << "===finish" << endl;
}
