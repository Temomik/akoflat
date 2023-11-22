#include "OnlinerHtmlParser.h"

#include "HtmlParserUtils.h"

#include <cstdlib>

using Telegram::FlatDto;
using std::string;

Onliner::HtmlParser::HtmlParser(std::shared_ptr<const std::string> htmlCode)
    : mHtmlCode(htmlCode)
{
}

bool Onliner::HtmlParser::Parse(std::vector<Telegram::FlatDto>& flatsDto)
{
    ParseHtml(mHtmlCode);
    auto root = GetHtmlDotRoot();
    auto flatsRoot = Html::FindAll("div", {{"id", "search-filter-results"}}, root);
    bool status = true;

    std::vector<std::string> pages;
    status &= Html::ExportDataArray(pages, "a", {{"class", "pagination-pages__link"}}, root);
    
    //TODO sort asc
    if (!status || pages.size() == 0 || pages.front().size() == 0)
    {
        return false;
    }

    if (flatsRoot.size() == 1)
    {
        mPagesCount = std::stoi(pages.front());

        auto flats = Html::FindAll("a", {{"class", "classified"}}, flatsRoot.front());
        
        for (auto& flat : flats)
        {
            string image;
            string usdPrice;
            string rubPrice;
            string pagesCount;
            FlatDto dto;

            status &= Html::ExportData(dto.Address, "span", {{"class", "classified__caption-item classified__caption-item_adress"}}, flat);
            status &= Html::ExportAttributeValue(image, "src", "img", {}, flat);
            dto.Images.push_back(image);
            status &= Html::ExportData(dto.Info, "span", {{"class", "classified__caption-item classified__caption-item_type"}}, flat);
            status &= Html::ExportAttributeValue(dto.Link, "href", "a", {{"class", "classified"}}, flat);
            
            status &= Html::ExportData(usdPrice, "span", {{"data-bind", "text: SearchApartments.formatPrice(apartment.price, 'USD')"}}, flat);
            status &= Html::ExportData(rubPrice, "span", {{"data-bind", "text: SearchApartments.formatPrice(apartment.price, 'BYN')"}}, flat);
            dto.Price = usdPrice + " $\n" + rubPrice + " p.";

            if (status)
            {
                flatsDto.push_back(dto);
            }
            else
            {
                //TODO ADD LOGGING
                return false;
            }
        }

        return true;
    }

    return false;
}

size_t Onliner::HtmlParser::GetPagesCount() const
{
    return mPagesCount;
}
