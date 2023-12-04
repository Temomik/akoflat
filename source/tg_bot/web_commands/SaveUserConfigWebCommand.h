#pragma once

#include "ITelegramWebCommand.h"

class BotStorage;

namespace Telegram::Web
{
    class SaveUserConfigWebCommand : public ITelegramWebCommand
    {
    public:
        SaveUserConfigWebCommand(BotStorage& botStorage);

        bool Process(const std::string& id, const Json::Value& value) override;

    private:
        BotStorage& mBotStorage;
    };
}
