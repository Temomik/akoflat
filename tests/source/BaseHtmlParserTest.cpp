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
    std::shared_ptr<const std::string> codePtr = std::make_shared<const std::string>(code);

    parser.ParseHtml(codePtr);

    EXPECT_EQ(parser.GetHtmlDotRoot()->childs.size(), 2);
}

TEST(BaseHtmlParser, ParseSimpleHtml_WithSkipTags)
{
    Html::BaseParser parser;
    string code = GetDataFromFile("../tests/data/simple_with_skip.html");
    std::shared_ptr<const std::string> codePtr = std::make_shared<const std::string>(code);

    parser.ParseHtml(codePtr);

    EXPECT_EQ(parser.GetHtmlDotRoot()->childs.size(), 2);
}

