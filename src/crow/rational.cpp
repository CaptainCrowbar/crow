#include "crow/rational.hpp"

namespace Crow::Detail {

    bool parse_rational(const std::string& s, std::vector<std::string>& parts, bool& neg) noexcept {

        auto skipws = [] (auto& i, auto end) {
            while (i != end && ascii_isspace(*i))
                ++i;
        };

        auto getnum = [skipws] (auto& i, auto end, auto& out) {
            skipws(i, end);
            auto j = i;
            while (i != end && ascii_isdigit(*i))
                ++i;
            if (i == j)
                return false;
            std::string part(&*j, i - j);
            out.push_back(part);
            skipws(i, end);
            return true;
        };

        auto i = s.begin();
        skipws(i, s.end());

        if (i == s.end())
            return false;

        neg = false;

        if (*i == '+' || *i == '-')
            neg = *i++ == '-';

        if (! getnum(i, s.end(), parts))
            return false;

        if (i != s.end()) {
            if (*i != '/' && ! getnum(i, s.end(), parts))
                return false;
            if (*i != '/')
                return false;
            ++i;
            if (! getnum(i, s.end(), parts))
                return false;
        }

        return i == s.end();

    }

}
