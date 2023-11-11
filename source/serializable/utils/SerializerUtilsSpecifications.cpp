#include "SerializeUtils.h"

#include <string>
#include <vector>

using std::vector;
using std::string;

namespace Serialize
{
    namespace Utils
    {
        template <>
        void Serializer::ValueToJson<size_t>(const char* key, const size_t& value)
        {
            mRoot[key] = value;
        }
        
        template <>
        void Serializer::ValueToJson<string>(const char* key, const string& value)
        {
            mRoot[key] = value;
        }

        template <>
        void Serializer::ValueToJson<vector<string>>(const char* key, const vector<string>& values)
        {
            Json::Value json;
            for (const auto& it : values)
            {
                json.append(it);
            }
            
            mRoot[key] = json;
        }

        template <>
        void Serializer::ValueToJson<vector<size_t>>(const char* key, const vector<size_t>& values)
        {
            Json::Value json;
            for (const auto& it : values)
            {
                json.append(it);
            }
            
            mRoot[key] = json;
        }
    }
}
