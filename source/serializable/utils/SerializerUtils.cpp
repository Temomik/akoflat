#include "SerializeUtils.h"

#include <string>

using std::string;

namespace Serialize
{
    namespace Utils
    {

        Serializer::Serializer()
            : mBuilder(), mWriter(mBuilder.newStreamWriter())
        {
        }

        string Serializer::GetString() const
        {
            std::stringstream ret;
            mWriter->write(mRoot, &ret);

            return ret.str();
        }
    }
}
