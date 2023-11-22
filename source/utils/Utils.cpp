#include "Utils.h"

#include <cctype>
#include <algorithm>

namespace
{
    bool ICharEquals(char a, char b)
    {
        return std::toupper(static_cast<unsigned char>(a)) ==
               std::toupper(static_cast<unsigned char>(b));
    }
}

// TODO cover with tests
bool Utils::EqualsIC(const std::string_view &lhs, const std::string_view &rhs, const size_t length, const size_t offset)
{
    if (lhs.length() < length + offset || rhs.length() < length)
    {
        return false;
    }

    size_t minLength = std::min<size_t>(lhs.length() - offset, rhs.length());
    auto lengthConstrain = std::clamp<size_t>(length, 0, minLength);
    return std::equal(lhs.begin() + offset, lhs.begin() + offset + lengthConstrain, rhs.begin(),
                      rhs.begin() + lengthConstrain, ICharEquals);
}

size_t Utils::FindIC(const std::string_view &source, const std::string_view &target, const size_t length, const size_t offset)
{
    if (offset + length > source.length())
    {
        return std::string_view::npos;
    }

    const char* sourceBegin = source.begin() + offset;
    const char* sourceEnd= sourceBegin + length;

    auto it = std::search(
        sourceBegin, sourceEnd,
        target.begin(), target.end(),
        [](unsigned char lhs, unsigned char rhs)
        { return std::toupper(lhs) == std::toupper(rhs); });

    if (it == sourceEnd)
    {
        return std::string_view::npos;
    }
    else
    {
        return (it - source.begin()) / sizeof(char);
    }
}
