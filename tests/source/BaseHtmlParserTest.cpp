#include <fstream>
#include <gtest/gtest.h>
#include <string>

#include "BaseHtmlParser.h"
#include "HtmlParserUtils.h"

using std::ifstream;
using std::string;
using std::stringstream;

namespace
{
    const string GetDataFromFile(const string& path)
    {
        ifstream htmlStream;
        stringstream htmlDataStream;
        htmlStream.open(path);
        bool isFileOpened = htmlStream.is_open();
        string res;

        if (isFileOpened)
        {
            htmlDataStream << htmlStream.rdbuf();
            res = htmlDataStream.str();
            htmlStream.close();
        }

        return res;
    }
}

TEST(BaseHtmlParser, ParseSimpleHtml)
{
    Html::BaseParser parser;
    string code = GetDataFromFile("../tests/data/simple.html");
    parser.ParseHtml(code.c_str());

    EXPECT_EQ(parser.GetHtmlDotRoot()->childs.size(), 2);
}

TEST(BaseHtmlParser, ParseSimpleHtml_WithSkipTags)
{
    Html::BaseParser parser;
    string code = GetDataFromFile("../tests/data/simple_with_skip.html");
    parser.ParseHtml(code.c_str());

    EXPECT_EQ(parser.GetHtmlDotRoot()->childs.size(), 2);
}

TEST(BaseHtmlParser, ParseSimpleHtml_ParseOnlinerFlats)
{
    Html::BaseParser parser;
    string code = GetDataFromFile("../tests/data/onliner.html");
    parser.ParseHtml(code.c_str());

    auto as = Html::FindAll("a", {{"class", "classified"}}, parser.GetHtmlDotRoot());

    for (auto a : as)
    {
        auto priceUsd = Html::FindAll("span", {{"data-bind", "text: SearchApartments.formatPrice(apartment.price, 'USD')"}}, a).front();
        auto priceByn = Html::FindAll("span", {{"data-bind", "text: SearchApartments.formatPrice(apartment.price, 'BYN')"}}, a).front();
        auto flatCount = Html::FindAll("span", {{"data-bind", "text: SearchApartments.formatRentType(apartment.rent_type)"}}, a).front();
        auto address = Html::FindAll("span", {{"data-bind", "text: apartment.location.user_address"}}, a).front();
        auto photoTag = Html::FindAll("img", {{"data-bind", "attr: {src: apartment.photo}"}}, a).front();

        auto link = a->attributes.find("href")->second;
        auto photo = photoTag->attributes.find("src")->second;

        // std::cout <<
        //     priceUsd->data << std::endl <<
        //     priceByn->data << std::endl <<
        //     flatCount->data << std::endl <<
        //     address->data << std::endl <<
        //     link << std::endl <<
        //     photo << std::endl << std::endl;
    }
}
