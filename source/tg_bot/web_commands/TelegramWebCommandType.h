#pragma once

#include <stddef.h>

namespace Telegram::Web
{
    enum CommandType
    {
        None = 0,
        SetUserConfig,
        Start,
        Stop
    };
}
