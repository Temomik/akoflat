#pragma once

#include <string>
#include "Utils.h"

namespace Onliner::Url::Presets
{
    static inline const std::string Minsk = "bounds%5Blb%5D%5Blat%5D=53.8&bounds%5Blb%5D%5Blong%5D=27.39&bounds%5Brt%5D%5Blat%5D=54.01&bounds%5Brt%5D%5Blong%5D=27.72";

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
