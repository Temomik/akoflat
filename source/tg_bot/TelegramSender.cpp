#include "TelegramSender.h"

#define BOOST_BIND_GLOBAL_PLACEHOLDERS

#include <tgbot/Bot.h>

Telegram::Sender::Sender(TgBot::Bot& bot)
    : mBot(bot)
{
}

bool Telegram::Sender::SendFlatDto(const std::int64_t id)
{
    return false;
}
