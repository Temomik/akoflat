#pragma once

#include "FlatDto.h"
#include "UserConfig.h"

class IWebFlatAdapter
{
public:
    virtual std::vector<Telegram::FlatDto> GetFlats(const Telegram::User::Config& config) const = 0;
};
