#pragma once

#include <string_view>

namespace Utils
{
    bool EqualsIC(const std::string_view& lhs, const std::string_view& rhs, const size_t length, const size_t offset = 0);
    size_t FindIC(const std::string_view & source, const std::string_view & target, const size_t length,  const size_t offset = 0);
}
