#include "PersistenceStorage.h"

#include <filesystem>
#include <fstream>
#include <sstream>

using std::ifstream;
using std::ofstream;
using std::stringstream;
namespace fs = std::filesystem;

PersistenceStorage::PersistenceStorage(const std::string& pathToStorage)
    : mPathToStorage(pathToStorage)
{
}

bool PersistenceStorage::CreateFolders(const std::string& folder) const
{
    fs::path path(mPathToStorage);
    path /= folder;

    if (!fs::exists(path) && !fs::create_directories(path))
    {
        return false;
    }

    return true;
}

bool PersistenceStorage::RemoveAll(const std::string& folder) const
{
    fs::path path(mPathToStorage);
    path /= folder;

    return fs::remove_all(path);
}

bool PersistenceStorage::Save(const std::string& folder, const std::string& fileName,
                              const std::string& data) const
{
    ofstream file;
    auto path = ConcatPath(folder, fileName);

    CreateFolders(folder);
    file.open(path, ofstream::trunc);

    if (file.is_open())
    {
        file << data;
        file.close();

        return true;
    }

    return false;
}

bool PersistenceStorage::Append(const std::string& folder, const std::string& fileName,
                                const std::string& data) const
{
    ofstream file;
    auto path = ConcatPath(folder, fileName);

    CreateFolders(folder);
    file.open(path, ofstream::app);

    if (file.is_open())
    {
        file << data;
        file.close();

        return true;
    }

    return false;
}

std::string PersistenceStorage::Load(const std::string& folder, const std::string& fileName) const
{
    ifstream file;
    stringstream dataStream;

    auto path = ConcatPath(folder, fileName);
    file.open(path);

    if (file.is_open())
    {
        dataStream << file.rdbuf();
        file.close();

        return dataStream.str();
    }

    return std::string();
}

std::deque<std::string> PersistenceStorage::GetLines(const std::string& folder,
                                                     const std::string& fileName) const
{
    ifstream file;
    std::deque<std::string> res;

    auto path = ConcatPath(folder, fileName);
    file.open(path);

    if (file.is_open())
    {
        for (std::string line; std::getline(file, line);)
        {
            res.push_back(line);
        }

        file.close();
    }

    return res;
}

const std::string PersistenceStorage::ConcatPath(const std::string& folder,
                                                 const std::string& fileName) const
{
    fs::path path(mPathToStorage);
    path /= folder;
    path /= fileName;

    return path;
}
