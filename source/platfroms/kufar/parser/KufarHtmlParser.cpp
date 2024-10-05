#include "KufarHtmlParser.h"

#include "HtmlParserUtils.h"

#include <cstdlib>

#include "Logger.h"

using Telegram::FlatDto;
using std::string;

Kufar::HtmlParser::HtmlParser(std::shared_ptr<const std::string> htmlCode)
    : mHtmlCode(htmlCode)
{
}

bool Kufar::HtmlParser::Parse(std::vector<Telegram::FlatDto>& flatsDto)
{
    LOG_INFO("Start parsing html")
    ParseHtml(mHtmlCode);
    LOG_INFO("End parsing html")
    auto root = GetHtmlDotRoot();

    LOG_INFO("Start parsing Kufar")
    auto flatsRoot = Html::FindAll("div", {{"class", "styles_cards__HMGBx"}}, root);
    bool status = true;

    if (flatsRoot.size() == 1)
    {
        // mPagesCount = std::stoi(pages.front());

        auto flats = Html::FindAll("a", {{"target", "_blank"}}, flatsRoot.front());
        
        for (auto& flat : flats)
        {
            string image;
            string usdPrice;
            string rubPrice;
            string pagesCount;
            FlatDto dto;

            status &= Html::ExportData(dto.Address, "span", {{"class", "styles_address__l6Qe_"}}, flat);
            
            auto imgsDot = Html::FindAll("div", {{"class", "styles_container__uQSzP styles_image_container__fCS2v"}}, flat);

            if (!imgsDot.empty())
            {
                status &= Html::ExportAttributeValueArray(dto.Images, "src", "img", {}, imgsDot.front());
            }

            status &= Html::ExportData(dto.Info, "div", {{"class", "styles_parameters__7zKlL"}}, flat);
            status &= Html::ExportAttributeValue(dto.Link, "href", "a", {{"target", "_blank"}}, flat);
            
            status &= Html::ExportData(usdPrice, "span", {{"class", "styles_price__usd__HpXMa"}}, flat);
            status &= Html::ExportData(rubPrice, "span", {{"class", "styles_price__byr__lLSfd"}}, flat);
            dto.Price = usdPrice + "\n" + rubPrice;

            auto linkView = std::string_view(dto.Link);
            auto idStartIndex = linkView.find_last_of("/");
            auto idEndIndex = linkView.find_first_of("?");

            if (idStartIndex != std::string_view::npos && idEndIndex != std::string_view::npos)
            {
                dto.id = linkView.substr(idStartIndex + 1, idEndIndex - idStartIndex - 1);
                dto.id += "K";
            }
            else
            {
                status = false;
            }

            if (status)
            {
                flatsDto.push_back(dto);
            }
            else
            {
                LOG_ERROR("Failed to parse Kufar")
                return false;
            }
        }

        LOG_INFO("Success to parse Kufar")
        return true;
    }

    LOG_ERROR("Failed to parse Kufar")
    return false;
}

std::string Kufar::HtmlParser::GetPageLink(const size_t pageCount) const
{
    bool status = true;
    auto root = GetHtmlDotRoot();

    std::vector<std::string> pages;

    status &= Html::ExportAttributeValueArray(pages, "href", "a", {{"data-testid", "realty-pagination-page-" + std::to_string(pageCount)}}, root);

    if (status && !pages.empty())
    {
        return pages.front();
    }

    return "";
}
