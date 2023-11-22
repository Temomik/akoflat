#pragma once

#include "BashExecutor.h"
#include "ICommand.h"

namespace Commands
{
    class WebdriverCommand : public ICommand
    {
    public:
        WebdriverCommand(const std::string url)
            : mUrl(url){};

        std::string Execute() override
        {
            BashExecutor executor;

            return executor.Exec(baseCommand + '\'' + mUrl + '\'');
        }

    private:
        static inline const std::string baseCommand = "timeout 10 chromium --log-level=0 --alsa-check-close-timeout=1000 --noerrdialogs --validate-input-event-stream --headless=new --disable-gpu --no-sandbox --dump-dom ";

        const std::string mUrl;
    };
}
