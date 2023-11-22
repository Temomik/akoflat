#pragma once

#include <string>
#include <cstdint>

namespace Telegram
{
    namespace TgBot
    {
        class Bot;
    }

    class Sender
    {
    public:
        Sender(TgBot::Bot& bot);

        bool SendFlatDto(const std::int64_t id);

    private:
        TgBot::Bot& mBot;
    };
}
