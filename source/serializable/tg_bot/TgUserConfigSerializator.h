#pragma once

#include "Serializator.h"
#include "SerializeUtils.h"
#include "UserConfig.h"

#include <jsoncpp/json/json.h>
#include <sstream>

template <>
class Serializator<Telegram::User::Config>
{
public:
    constexpr static char MaxPriceKey[] = "maxPrice";
    constexpr static char MinPriceKey[] = "minPrice";
    constexpr static char CityKey[] = "city";
    constexpr static char FloorCountKey[] = "floorCount";
    constexpr static char PlatformsKey[] = "platforms";

    static std::string Serialize(const Telegram::User::Config& dto)
    {
        try
        {
            Serialize::Utils::Serializer serializer;

            serializer.ValueToJson(MinPriceKey, dto.Price.first);
            serializer.ValueToJson(MaxPriceKey, dto.Price.second);
            serializer.ValueToJson(CityKey, dto.City);
            serializer.ValueToJson(FloorCountKey, dto.FloorCount);
            serializer.ValueToJson(PlatformsKey, dto.Platforms);

            return serializer.GetString();
        }
        catch (const std::exception& e)
        {
            return std::string();
        }
    }

    static bool Deserialize(Telegram::User::Config& dto, const std::string& str)
    {
        try
        {
            Serialize::Utils::Deserializer deserializer(str);
            deserializer.Initialize();

            dto.Platforms = deserializer.JsonToValue<std::vector<std::string>>(PlatformsKey);
            dto.FloorCount = deserializer.JsonToValue<std::vector<size_t>>(FloorCountKey);
            dto.City = deserializer.JsonToValue<std::string>(CityKey);
            dto.Price.first = deserializer.JsonToValue<size_t>(MinPriceKey);
            dto.Price.second = deserializer.JsonToValue<size_t>(MaxPriceKey);
        }
        catch (const std::exception& e)
        {
            return false;
        }

        return true;
    }
};
