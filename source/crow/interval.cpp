#include "crow/interval.hpp"
#include <unordered_map>

using namespace std::literals;

namespace Crow {

    namespace Detail {

        std::pair<IntervalBound, IntervalBound> decode_interval_bounds(std::string_view mode) {

            static const std::unordered_map<std::string_view, std::pair<IntervalBound, IntervalBound>> map = {
                { "()"sv,  { IntervalBound::open,     IntervalBound::open     }},
                { "(]"sv,  { IntervalBound::open,     IntervalBound::closed   }},
                { "[)"sv,  { IntervalBound::closed,   IntervalBound::open     }},
                { "[]"sv,  { IntervalBound::closed,   IntervalBound::closed   }},
                { ">"sv,   { IntervalBound::open,     IntervalBound::unbound  }},
                { "<"sv,   { IntervalBound::unbound,  IntervalBound::open     }},
                { ">="sv,  { IntervalBound::closed,   IntervalBound::unbound  }},
                { "<="sv,  { IntervalBound::unbound,  IntervalBound::closed   }},
                { "*"sv,   { IntervalBound::unbound,  IntervalBound::unbound  }},
            };

            auto it = map.find(mode);
            if (it == map.end())
                throw std::invalid_argument("Invalid interval mode: " + std::string(mode));
            return it->second;

        }

    }

}
