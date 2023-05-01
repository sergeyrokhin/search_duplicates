#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <map>
#include <set>

struct FilePoint
{
    std::size_t parent_index;
    std::string file_name;
};

struct DirPoint
{
    std::size_t section_index;
    std::size_t parent_index;
    std::string file_name;
};

class FileCatalogue {
public:
    FileCatalogue();
    std::string FullPathName(std::size_t index);
    void OutputDuplicateCrc();
    std::string FullFileName(std::size_t index);
    void CalculateCRC();
    std::vector<std::string> section_;
    std::vector<DirPoint> dir_list_;
    std::vector<FilePoint> file_list_;
    std::map<uint32_t, std::vector<std::size_t>> crc_;
    std::set<uint32_t> duplicated_crc_;
};

struct Path
{
    std::filesystem::path file;
    bool is_dir;
};

FileCatalogue CreateCatalogue(std::ifstream& in);
uint32_t file_crc32(std::string file_name);