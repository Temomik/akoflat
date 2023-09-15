#include <gtest/gtest.h>
#include <UrlBaseBuilder.h>
#include <string>
#include <vector>

using std::string;

// Demonstrate some basic assertions.
TEST(UrlBaseBuilder, SetupSimpleUrl)
{
    string url = "url";
    UrlBuilder::BaseBuilder builder;

    builder.SetUrl(url);
    auto link = builder.Build();

    EXPECT_EQ(link, url);
}

TEST(UrlBaseBuilder, SetupLink)
{
    string url = "url";
    vector querryItems = {"1", "2", "3"};
    string ExpectedLink = url + "/1?2&3";
    
    UrlBuilder::BaseBuilder builder;

    builder.SetUrl(url);
    for (auto& item : querryItems)
    {
        builder.AddQuerryItem(item);
    }

    auto link = builder.Build();

    EXPECT_EQ(link, ExpectedLink);
}
