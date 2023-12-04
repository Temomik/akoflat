#include "SerializeUtils.h"

#include <string>
#include <vector>
#include <functional>
#include "TelegramWebCommandType.h"

using std::vector;
using std::string;
using std::function;

namespace
{
    template <typename T>
    vector<T> JsonToVector(Json::Value& root, const char* key, function<T(Json::Value)> castFunc)
    {
        auto items = root[key];
        vector<T> result;

        for (auto& item : items)
        {
            result.push_back(castFunc(item));
        }

        return result;
    }
}

namespace Serialize
{
    namespace Utils 
    {
        template <>
        vector<string> Deserializer::JsonToValue<vector<string>>(const char* key)
        {
            auto castFunc = [](Json::Value item) -> string
            {
                return static_cast<string>(item.asString());
            };

            return JsonToVector<string>(mRoot, key, castFunc);
        }

        template <>
        vector<size_t> Deserializer::JsonToValue<vector<size_t>>(const char* key)
        {
            auto castFunc = [](Json::Value item) -> size_t
            {
                return static_cast<size_t>(item.asUInt());
            };

            return JsonToVector<size_t>(mRoot, key, castFunc);
        }

        template <>
        Json::Value Deserializer::JsonToValue<Json::Value>(const char* key)
        {
            return mRoot[key];
        }

        template <>
        string Deserializer::JsonToValue<string>(const char* key)
        {
            return static_cast<string>(mRoot[key].asString());
        }

        template <>
        size_t Deserializer::JsonToValue<size_t>(const char* key)
        {
            return static_cast<size_t>(mRoot[key].asUInt());
        }

        template <>
        Telegram::Web::CommandType Deserializer::JsonToValue<Telegram::Web::CommandType>(const char* key)
        {
            return static_cast<Telegram::Web::CommandType>(mRoot[key].asUInt());
        }
    }
}
