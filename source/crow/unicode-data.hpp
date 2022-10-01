#include "crow/types.hpp"
#include <map>
#include <unordered_map>
#include <utility>

namespace Crow::Detail {

    enum class BitShift: int {
        Canonical_Combining_Class   = 0,
        Control                     = 8,
        Full_Composition_Exclusion  = 9,
        Pattern_Syntax              = 10,
        XID_Continue                = 11,
        XID_Start                   = 12,
    };

    using DecompositionMap = std::unordered_map<char32_t, std::pair<char32_t, char32_t>>;
    using PropertyMap = std::map<char32_t, uint32_t>;

    const DecompositionMap& decomposition_map();
    const PropertyMap& property_map();

    inline bool get_property(char32_t c, int bit) {
        auto it = property_map().upper_bound(c);
        --it;
        return ((it->second >> bit) & 1) != 0;
    }

}
