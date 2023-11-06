#include "BotStorage.h"

#include <filesystem>

using std::string;
namespace fs = std::filesystem;

namespace
{
    static string ShownIdsFolder = "shown_flat_ids";
    static string KnownIdsFile = "Known_ids";
    static string KnownIdsFolder = "ids";
}

BotStorage::BotStorage(const std::string& rootFolder)
    : mRootFolder(rootFolder), mPersistenceStorage(rootFolder)
{
}

void BotStorage::Init()
{
    auto ids = mPersistenceStorage.GetLines(KnownIdsFolder, KnownIdsFile);

    for (auto& id : ids)
    {
        mIds.insert(id);

        auto lines = mPersistenceStorage.GetLines(ShownIdsFolder, id);
        std::set<std::string> shownFlats(lines.begin(), lines.end());

        mIdsData.insert(std::pair(id, shownFlats));
    }
}

std::deque<std::string> BotStorage::GetIds() const
{
    return std::deque<std::string>(mIds.begin(), mIds.end());
}

bool BotStorage::SaveId(const std::string& id)
{
    auto ret = SaveIdInternal(id);

    mIds.insert(id);
    mIdsData.insert(std::pair(id, std::set<std::string>()));

    return ret;
}

bool BotStorage::RemoveId(const std::string& id)
{
    if (!IsIdExist(id))
    {
        return false;
    }

    bool ret = true;
    mIds.erase(id);
    mIdsData.erase(id);

    fs::path path(KnownIdsFolder);
    path /= KnownIdsFile;

    ret &= mPersistenceStorage.RemoveAll(path);

    for (auto& id : mIds)
    {
        ret &= SaveIdInternal(id);
    }

    return ret;
}

bool BotStorage::SaveShownFlatId(const std::string& id, const std::string& flatId)
{
    if (IsFlatIdExist(id, flatId))
    {
        return false;
    }

    auto ret = SaveShownFlatIdInternal(id, flatId);
    mIdsData.find(id)->second.insert(flatId);

    return ret;
}

std::optional<std::deque<std::string>> BotStorage::GetShownFlatIds(const std::string& id) const
{
    if (!IsIdExist(id))
    {
        return {};
    }

    auto& shownIds = mIdsData.find(id)->second;

    return std::deque<std::string>(shownIds.begin(), shownIds.end());
}

bool BotStorage::IsIdExist(const string& id) const
{
    return mIds.find(id) != mIds.end() &&
           mIdsData.find(id) != mIdsData.end();
}

bool BotStorage::IsFlatIdExist(const string& id, const string& flatId) const
{
    auto it = mIdsData.find(id);

    return mIds.find(id) != mIds.end() &&
           mIdsData.find(id) != mIdsData.end() &&
           it->second.find(flatId) != it->second.end();
}

bool BotStorage::SaveIdInternal(const string& id) const
{
    return mPersistenceStorage.Append(KnownIdsFolder, KnownIdsFile, id + "\n");
}

bool BotStorage::SaveShownFlatIdInternal(const string& id, const string& flatId) const
{
    return mPersistenceStorage.Append(ShownIdsFolder, id, flatId + "\n");
}
