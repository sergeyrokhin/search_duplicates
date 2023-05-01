#include "search_duplicate.h"

#include <fstream>
#include <algorithm>

using namespace std;
using filesystem::path;

void OutPathDir(ostream &dst, const path &p, const string space)
{
    vector<Path> files;
    for (const auto &dir_entry : filesystem::directory_iterator(p))
    {
        files.push_back({dir_entry.path(), (dir_entry.status().type() == filesystem::file_type::directory)});
    }
    sort(files.begin(), files.end(), [](const Path &l, const Path &r)
         { return l.file.filename() > r.file.filename(); });
    for (auto &file : files)
    {
        dst << space;
        dst << file.file.filename().string() << endl;
        if (file.is_dir)
        {
            OutPathDir(dst, file.file, (space + "  "s));
        }
    }
}

void PrintTree(ostream &dst, const path &p)
{
    dst << p.filename().string() << endl;
    OutPathDir(dst, p, "  "s);
}

void CreateCollection(FileCatalogue &catalogue, size_t parent_index, path collection_path, string ext)
{

    size_t section_index = catalogue.section_.size() - 1;
    size_t dir_index = catalogue.dir_list_.size() - 1;
    for (const auto &dir_entry : filesystem::directory_iterator(collection_path))
    {
        bool is_dir = (dir_entry.status().type() == filesystem::file_type::directory);
        if (is_dir)
        {
            catalogue.dir_list_.push_back({ section_index, parent_index, std::make_unique<std::string>(dir_entry.path().filename().string()) });
            CreateCollection(catalogue, catalogue.dir_list_.size() - 1,  dir_entry.path(), ext);
        }
        else if (dir_entry.path().extension() == ext)
        {
            catalogue.file_list_.push_back({ dir_index, std::make_unique<std::string>(dir_entry.path().filename().string()) });
        }
    }
}

FileCatalogue CreateCatalogue(ifstream& in)
{
    FileCatalogue catalogue;
    string collection_name;
    string ext;
    getline(in, ext);
    cout << "Search ext:" << ext << endl;
    while (getline(in, collection_name))
    {
        path collection_path(collection_name);
        if (exists(collection_path)) {
            cout << "Dir: " << collection_path << endl;
            catalogue.section_.push_back(std::make_unique<std::string>(collection_path.string()));
            CreateCollection(catalogue, 0, collection_path, ext);
        }
        else {
            cout << "not exist " << collection_path << endl;
        }
    }
    cout << "---Calc CRC for "s << catalogue.file_list_.size() << " files"s << endl;
    catalogue.CalculateCRC();
    return catalogue;
}

string FileCatalogue::FullFileName(size_t index)
{
    return FullPathName(file_list_[index].parent_index) + "/"s + *(file_list_[index].file_name.get());
}

void FileCatalogue::CalculateCRC()
{
    duplicated_crc_.clear();
    crc_.clear();
    for (size_t i = 0; i < file_list_.size(); i++)
    {
        uint32_t crc = file_crc32(FullFileName(i));

        const auto& v_crc = crc_.find(crc);
        if (v_crc == crc_.end())
        {
            crc_.insert({ crc,{i}});
        }
        else {
            duplicated_crc_.insert(crc);
            (*v_crc).second.push_back(i);
        }
    }
}

FileCatalogue::FileCatalogue()
{
    dir_list_.push_back({0, 0, std::make_unique<std::string>("root"s)});
}

std::string FileCatalogue::FullPathName(size_t index)
{
    if(dir_list_[index].parent_index == 0)
    {
        return *(section_[dir_list_[index].section_index].get()) + "/"s + *(dir_list_[index].file_name.get());
    }
    else
    {
        return  FullPathName(dir_list_[index].parent_index) + "/"s + *(dir_list_[index].file_name.get());
    }
}

void  FileCatalogue::OutputDuplicateCrc()
{
    for (auto crc : duplicated_crc_) {
        cout << hex << crc << endl;
        for (auto index : crc_.at(crc)) {
            cout << FullFileName(index) << endl;
        }
    }
    return;
}
