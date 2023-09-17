namespace Html
{
    class IParser
    {
    public:
        virtual void ParseHtml(const char* htmlCode) = 0;
    };
}
