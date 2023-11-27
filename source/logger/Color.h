#include <ostream>
namespace Color
{
    enum Code
    {
        RESET = 0,
        FG_RED = 31,
        FG_GREEN = 32,
        FG_YELLOW = 33,
        FG_BLUE = 34,
        FG_WHITE = 37,
        FG_DEFAULT = 39,
        BG_RED = 41,
        BG_GREEN = 42,
        BG_YELLOW = 43,
        BG_BLUE = 44,
        BG_WHITE = 47,
        BG_DEFAULT = 49
    };

    class Modifier
    {
    public:
        Modifier(Code pCode) : code(pCode) {}
        friend std::ostream&
        operator<<(std::ostream& os, const Modifier& mod)
        {
            return os << "\033[" << mod.code << "m";
        }

    private:
        Code code;
    };
}
