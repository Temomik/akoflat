#pragma once

#include "IPersistenceStorage.h"

class PersistenceStorage : public IPersistenceStorage
{
public:
    PersistenceStorage(const std::string& pathToStorage);

    bool CreateFolders(const std::string& folder) const override;

    bool RemoveAll(const std::string& path) const override;

    bool Save(const std::string& folder, const std::string& fileName,
              const std::string& data) const override;

    bool Append(const std::string& folder, const std::string& fileName,
                const std::string& data) const override;

    std::string Load(const std::string& folder, const std::string& fileName) const override;

    std::deque<std::string> GetLines(const std::string& folder,
                                     const std::string& fileName) const override;

private:
    const std::string ConcatPath(const std::string& folder, const std::string& fileName) const;

    std::string mPathToStorage;
};
