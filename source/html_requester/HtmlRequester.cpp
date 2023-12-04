#include "HtmlRequester.h"

#include "WebdriverCommand.h"
#include "Logger.h"

using Commands::WebdriverCommand;

namespace 
{
    constexpr size_t CodeTreshold = 250;
}

std::shared_ptr<const std::string> Html::HtmlRequester::Get(const std::string url, size_t retryCount)
{
    LOG_INFO("Requesting - " + url)
    WebdriverCommand command(url);

    for (size_t i = 0; i < retryCount; i++)
    {
        LOG_INFO("Request attempt - " + (i + 1))

        auto htmlCode = std::make_shared<const std::string>(command.Execute());
        if (htmlCode->size() > CodeTreshold)
        {
            LOG_INFO("Requesting success")
            return htmlCode;
        }
    }

    LOG_WARNING("Requesting failed")
    return std::make_shared<const std::string>();
}
