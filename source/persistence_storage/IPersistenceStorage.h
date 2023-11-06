#pragma once

#include <deque>
#include <string>

class IPersistenceStorage
{
    /// @brief recursive creation of folders
    virtual bool CreateFolders(const std::string& folder) const = 0;

    /// @brief recursive deletion of folders and files
    virtual bool RemoveAll(const std::string& path) const = 0;

    /// @brief Save and overrite all data in file
    virtual bool Save(const std::string& folder, const std::string& fileName,
                      const std::string& data) const = 0;

    /// @brief Append all data to file
    virtual bool Append(const std::string& folder, const std::string& fileName,
                        const std::string& data) const = 0;

    /// @brief load all data from file
    virtual std::string Load(const std::string& folder, const std::string& fileName) const = 0;

    /// @brief load all lines from file
    virtual std::deque<std::string> GetLines(const std::string& folder,
                                             const std::string& fileName) const = 0;
};
