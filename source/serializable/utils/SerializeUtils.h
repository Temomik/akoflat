#pragma once

#include <jsoncpp/json/json.h>

namespace Serialize
{
    namespace Utils
    {
        class Serializer
        {
        public:
            Serializer();

            std::string GetString() const;

            template <typename T>
            void ValueToJson(const char* key, const T& value);

        private:
            Json::Value mRoot;
            Json::StreamWriterBuilder mBuilder;
            const std::unique_ptr<Json::StreamWriter> mWriter;
        };

        class Deserializer
        {
        public:
            Deserializer(const std::string& raw);

            void Initialize();

            template <typename T>
            T JsonToValue(const char* key);

        private:
            Json::Value mRoot;
            Json::Reader mReader;
            const std::string mRaw;
        };
    }
}
