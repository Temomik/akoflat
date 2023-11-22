#pragma once

#include "FlatDto.h"
#include "UserConfig.h"

namespace Onliner
{
    class WebAdapter
    {
    public:
        WebAdapter(size_t retryCount = 4);

        std::vector<Telegram::FlatDto> GetFlats(const Telegram::User::Config& config) const;

    private:
        size_t mRetryCount;
    };
}
