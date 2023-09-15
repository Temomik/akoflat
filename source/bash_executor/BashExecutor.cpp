#include <BashExecutor.h>
#include <stdio.h>
#include <memory>

using std::string;
using std::shared_ptr;

namespace
{
    constexpr size_t bufferSize = 1024; 
    constexpr char* errorMessage = "";
}

const string BashExecutor::Exec(const string& command)
{
    shared_ptr<FILE> pipe(popen(command.c_str(), "r"), pclose);
    if (!pipe)
    {
        return errorMessage;
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
