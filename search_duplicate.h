#pragma once

#include <string>
#include <filesystem>
#include <iostream>
#include <vector>
#include <unordered_map>
#include <unordered_set>
#include <future>
#include<mutex>
#include<memory>


struct FilePoint
{
    std::size_t parent_index;
    std::unique_ptr<std::string> file_name;
};

struct DirPoint
{
    std::size_t section_index;
    std::size_t parent_index;
    std::unique_ptr<std::string> file_name;
};

struct Access {
    std::lock_guard<std::mutex> guard;
    std::vector<std::size_t>& ref_to_value;
};

//Access operator[](const Key& key) {
//    uint64_t set_number = (uint64_t)(key) % bucket_count_;
//    return { lock_guard(m_[set_number]), dictiorary_[set_number][key] };
//}

class FileCatalogue {
public:
    FileCatalogue();
    std::string FullPathName(std::size_t index);
    void OutputDuplicateCrc();
    std::string FullFileName(std::size_t index);
    void CalculateCRC();
    std::vector<std::unique_ptr<std::string>> section_;
    std::vector<DirPoint> dir_list_;
    std::vector<FilePoint> file_list_;
    std::unordered_map<uint32_t, std::vector<std::size_t>> crc_;
    std::unordered_set<uint32_t> duplicated_crc_;
};

struct Path
{
    std::filesystem::path file;
    bool is_dir;
};

FileCatalogue CreateCatalogue(std::ifstream& in);
uint32_t file_crc32(std::string file_name);