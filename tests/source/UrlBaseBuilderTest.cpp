#include <gtest/gtest.h>
#include <UrlBaseBuilder.h>
#include <string>
#include <vector>

using std::string;
using std::vector;
using Url::BaseBuilder;

TEST(UrlBaseBuilder, SetupSimpleLink)
{
    string url = "url";
    string expectedLink = "url";
    BaseBuilder builder;

    builder.SetUrl(url);
    auto link = builder.Build();

    EXPECT_EQ(link, expectedLink);
}

TEST(UrlBaseBuilder, SetupLinkWithQuerry)
{
    string url = "url";
    vector querryItems = {"1", "2", "3"};
    string ExpectedLink = url + "?1&2&3";
    
    BaseBuilder builder;

    builder.SetUrl(url);
    for (auto& item : querryItems)
    {
        builder.AddQuerryItem(item);
    }

    auto link = builder.Build();

    EXPECT_EQ(link, ExpectedLink);
}

TEST(UrlBaseBuilder, SetupLinkWithAnchor)
{
    string url = "url";
    vector anchortItems = {"1", "2", "3"};
    string ExpectedLink = url + "#1&2&3";
    
    BaseBuilder builder;

    builder.SetUrl(url);
    for (auto& item : anchortItems)
    {
        builder.AddAnchorItem(item);
    }

    auto link = builder.Build();

    EXPECT_EQ(link, ExpectedLink);
}

TEST(UrlBaseBuilder, SetupLinkWithAnchorAndQuerry)
{
    string url = "url";
    vector querryItems = {"1", "2", "3"};
    vector anchortItems = {"11", "22", "33"};
    string ExpectedLink = url + "?1&2&3#11&22&33";
    
    BaseBuilder builder;

    builder.SetUrl(url);
    for (auto& item : anchortItems)
    {
        builder.AddAnchorItem(item);
    }
    for (auto& item : querryItems)
    {
        builder.AddQuerryItem(item);
    }

    auto link = builder.Build();

    EXPECT_EQ(link, ExpectedLink);
}
