#include <gtest/gtest.h>
#include <BaseHtmlParser.h>
#include <fstream>
#include <string>

using std::string;
using std::stringstream;
using std::ifstream;

TEST(BaseHtmlParser, ParseSimpleUrl)
{
    Html::BaseParser parser;
    ifstream htmlStream;
    string htmlFilePath = "../tests/data/html.html";
    stringstream htmlDataStream;
    htmlStream.open(htmlFilePath);
    bool isFileOpened = htmlStream.is_open();

    if (isFileOpened)
    {
        htmlDataStream << htmlStream.rdbuf();

        parser.ParseHtml(htmlDataStream.str().c_str());

        htmlStream.close();
    }

    EXPECT_TRUE(isFileOpened);
}

//the same for comments empty comment and code
// TEST(BaseHtmlParser, ParseUrlWithScriptTag_shouldBeSkipped)
// {
//     Html::BaseParser parser;
//     ifstream htmlStream;
//     string htmlFilePath = "../tests/data/skip_script.html";
//     stringstream htmlDataStream;
//     htmlStream.open(htmlFilePath);
//     bool isFileOpened = htmlStream.is_open();

//     if (isFileOpened)
//     {
//         htmlDataStream << htmlStream.rdbuf();

//         parser.ParseHtml(htmlDataStream.str().c_str());

//         htmlStream.close();
//     }

//     EXPECT_TRUE(isFileOpened);
// }
