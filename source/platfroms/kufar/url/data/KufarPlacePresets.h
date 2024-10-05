#pragma once

#include "Utils.h"
#include <string>

namespace Kufar::Url::Presets
{
    static inline const std::string Minsk = "minsk";

    std::string GetPresetForCity(const std::string& city)
    {
        auto length = city.length();

        if (Utils::EqualsIC(city, "minsk", length))
        {
            return Minsk;
        }

        return "";
    }
}
