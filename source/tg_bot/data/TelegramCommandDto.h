#pragma once

#include <jsoncpp/json/json.h>

#include "TelegramWebCommandType.h"

namespace Telegram
{
    class CommandDto
    {
    public:
        Web::CommandType Command = Web::CommandType::None;
        Json::Value Value;
    };
}
