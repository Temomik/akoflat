#include "HtmlRequester.h"

#include "WebdriverCommand.h"

using Commands::WebdriverCommand;

namespace 
{
    constexpr size_t CodeTreshold = 250;
}

std::shared_ptr<const std::string> Html::HtmlRequester::Get(const std::string url, size_t retryCount)
{
    WebdriverCommand command(url);

    for (size_t i = 0; i < retryCount; i++)
    {
        auto htmlCode = std::make_shared<const std::string>(command.Execute());

        if (htmlCode->size() > CodeTreshold)
        {
            return htmlCode;
        }
    }

    return std::make_shared<const std::string>();
}
