#pragma once

#include <jsoncpp/json/json.h>
#include <string>

namespace Telegram::Web
{
    class ITelegramWebCommand
    {
    public:
        virtual bool Process(const std::string& id, const Json::Value& value) = 0;
    };
}
