#pragma once

#include <deque>
#include <optional>
#include <string>

class IBotStorage
{
    virtual void Init() = 0;

    virtual std::deque<std::string> GetIds() const = 0;
    virtual bool SaveId(const std::string& id) = 0;
    virtual bool RemoveId(const std::string& id) = 0;

    virtual bool SaveShownFlatId(const std::string& id, const std::string& flatId) = 0;
    virtual std::optional<std::deque<std::string>> GetShownFlatIds(const std::string& id) const = 0;
};
