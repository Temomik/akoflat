#pragma once

#include <IBashExecutor.h>

class BashExecutor : public IBashExecutor
{
public:
    const std::string Exec(const std::string &command) override;
};
