#pragma once

#include "Serializator.h"
#include "SerializeUtils.h"
#include "TelegramCommandDto.h"

#include <jsoncpp/json/json.h>

template <>
class Serializator<Telegram::CommandDto>
{
public:
    constexpr static char CommandKey[] = "command";
    constexpr static char ValueKey[] = "value";

    static std::string Serialize(const Telegram::CommandDto& dto)
    {
        try
        {
            Serialize::Utils::Serializer serializer;

            serializer.ValueToJson<Telegram::Web::CommandType>(CommandKey, dto.Command);
            serializer.ValueToJson(ValueKey, dto.Value);

            return serializer.GetString();
        }
        catch (const std::exception& e)
        {
            return std::string();
        }
    }

    static bool Deserialize(Telegram::CommandDto& dto, const std::string& str)
    {
        try
        {
            Serialize::Utils::Deserializer deserializer(str);
            deserializer.Initialize();

            dto.Command = deserializer.JsonToValue<Telegram::Web::CommandType>(CommandKey);
            dto.Value = deserializer.JsonToValue<Json::Value>(ValueKey);
        }
        catch (const std::exception& e)
        {
            return false;
        }

        return true;
    }
};
