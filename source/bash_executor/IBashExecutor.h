#include <string>

class IBashExecutor
{
public:
    virtual const std::string Exec(const std::string &command) = 0;
};
