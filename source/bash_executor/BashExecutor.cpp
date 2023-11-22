#include <BashExecutor.h>
#include <memory>
#include <stdio.h>

using std::shared_ptr;
using std::string;

namespace
{
    constexpr size_t bufferSize = 1024;
}

const string BashExecutor::Exec(const string& command)
{
    std::string suppressedCommand = command + " 2>/dev/null";
    shared_ptr<FILE> pipe(popen(suppressedCommand.c_str(), "r"), pclose);
    if (!pipe)
    {
        // TODO replace with static string from utils;
        return "";
    }

    char buffer[bufferSize];

    string result = "";
    while (!feof(pipe.get()))
    {
        if (fgets(buffer, bufferSize, pipe.get()) != NULL)
            result += buffer;
    }

    return result;
}
