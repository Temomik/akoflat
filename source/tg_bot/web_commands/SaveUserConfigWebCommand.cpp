#include "SaveUserConfigWebCommand.h"

#include "TgUserConfigSerializator.h"
#include "UserConfig.h"

#include "BotStorage.h"

Telegram::Web::SaveUserConfigWebCommand::SaveUserConfigWebCommand(BotStorage& botStorage)
    : mBotStorage(botStorage)
{
}

bool Telegram::Web::SaveUserConfigWebCommand::Process(const std::string& id, const Json::Value& value)
{
    Telegram::User::Config config;

    if (Serializator<Telegram::User::Config>::Deserialize(config, value.toStyledString()))
    {
        auto status = mBotStorage.SaveId(id);
        status &= mBotStorage.SaveUserConfig(id, config);
    }

    return false;
}
