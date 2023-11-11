#pragma once

#include "IBotStorage.h"
#include "PersistenceStorage.h"

#include <map>
#include <set>

class BotStorage : public IBotStorage
{
public:
    BotStorage(const std::string& rootFolder);

    void Init() override;

    std::deque<std::string> GetIds() const override;
    bool SaveId(const std::string& id) override;
    bool RemoveId(const std::string& id) override;

    std::optional<Telegram::User::Config> GetUserConfig(const std::string& id) const override;
    bool SaveUserConfig(const std::string& id, const Telegram::User::Config& config) override;

    bool SaveShownFlatId(const std::string& id, const std::string& flatId) override;
    std::optional<std::deque<std::string>> GetShownFlatIds(const std::string& id) const override;

private:
    bool IsIdExist(const std::string& id) const;
    bool IsFlatIdExist(const std::string& id, const std::string& flatId) const;
    bool SaveIdInternal(const std::string& id) const;
    bool SaveShownFlatIdInternal(const std::string& id, const std::string& flatId) const;

    std::set<std::string> mIds;
    std::map<std::string, std::set<std::string>> mIdsData;
    std::string mRootFolder;

    PersistenceStorage mPersistenceStorage;
};
