#pragma once

#include <deque>
#include <optional>
#include <string>

#include "UserConfig.h"

class IBotStorage
{
    virtual void Init() = 0;

    virtual std::deque<std::string> GetIds() const = 0;
    virtual bool SaveId(const std::string& id) = 0;
    virtual bool RemoveId(const std::string& id) = 0;

    virtual std::optional<Telegram::User::Config> GetUserConfig(const std::string& id) const = 0;
    virtual bool SaveUserConfig(const std::string& id, const Telegram::User::Config& config) = 0;

    virtual bool SaveShownFlatId(const std::string& id, const std::string& flatId) = 0;
    virtual std::optional<std::deque<std::string>> GetShownFlatIds(const std::string& id) const = 0;
};
