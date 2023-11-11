#include "SerializeUtils.h"

namespace Serialize::Utils
{
    Deserializer::Deserializer(const std::string& raw)
        : mRaw(raw)
    {
    }

    void Deserializer::Initialize()
    {
        mReader.parse(mRaw, mRoot);
    }
}
