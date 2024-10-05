#pragma once

#include "FlatDto.h"
#include "IWebFlatAdapter.h"
#include "UserConfig.h"

namespace Onliner
{
    class WebAdapter : public IWebFlatAdapter
    {
    public:
        WebAdapter(size_t retryCount = 3);

        std::vector<Telegram::FlatDto> GetFlats(const Telegram::User::Config& config) const override;

    private:
        size_t mRetryCount;
    };
}
