#include <gtest/gtest.h>

#include "OnlinerHtmlParser.h"
#include "OnlinerUrlBuilder.h"
#include "HtmlRequester.h"
#include "UserConfig.h"

TEST(OnlinerHtmlParser, GetFromLinkAndParse)
{
    // This test is not stable, OnlinerWebAdapter cover this functionality

    Telegram::User::Config config;

    config.City = "Minsk";
    config.Price.first = 100;
    config.Price.second = 320;
    config.FloorCount = {2, 3, 4};

    Onliner::UrlBuilder builder;   
    
    builder.BuildLink(config);
    auto link = builder.GetNextPage();

    Html::HtmlRequester requester;

    std::cout << "LINK _: " << link << std::endl;

    auto htmlCode = requester.Get(link);

    ASSERT_FALSE(htmlCode->size() == 0);

    Onliner::HtmlParser parser(htmlCode);  
    
    std::vector<Telegram::FlatDto> flatsDto;
    parser.Parse(flatsDto);

    ASSERT_FALSE(flatsDto.size() == 0);
}
