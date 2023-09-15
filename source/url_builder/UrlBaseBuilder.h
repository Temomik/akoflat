#include <string>
#include <vector>
#include <IUrlBuilder.h>

using std::string;
using std::vector;

namespace UrlBuilder
{
    class BaseBuilder : public IUrlBuilder
    {
        public:
            void SetUrl(const string& url) override; 
            void AddQuerryItem(const string& item) override; 
            void Reset() override;
            const string Build() const override;

        protected:
            string mUrl;
            vector<string> mQuerryItems;
    };
}
