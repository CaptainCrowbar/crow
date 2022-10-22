#include "crow/enum.hpp"
#include "crow/types.hpp"
#include <map>
#include <unordered_map>
#include <vector>

namespace Crow {

    CROW_ENUM_CLASS(GC, int, 1,
        Cc,  // Control
        Cf,  // Format
        Cn,  // Unassigned
        Co,  // Private_Use
        Cs,  // Surrogate
        Ll,  // Lowercase_Letter
        Lm,  // Modifier_Letter
        Lo,  // Other_Letter
        Lt,  // Titlecase_Letter
        Lu,  // Uppercase_Letter
        Mc,  // Spacing_Mark
        Me,  // Enclosing_Mark
        Mn,  // Nonspacing_Mark
        Nd,  // Decimal_Number
        Nl,  // Letter_Number
        No,  // Other_Number
        Pc,  // Connector_Punctuation
        Pd,  // Dash_Punctuation
        Pe,  // Close_Punctuation
        Pf,  // Final_Punctuation
        Pi,  // Initial_Punctuation
        Po,  // Other_Punctuation
        Ps,  // Open_Punctuation
        Sc,  // Currency_Symbol
        Sk,  // Modifier_Symbol
        Sm,  // Math_Symbol
        So,  // Other_Symbol
        Zl,  // Line_Separator
        Zp,  // Paragraph_Separator
        Zs   // Space_Separator
    )

    namespace Detail {

        using GeneralCategoryTable                = std::map<char32_t, GC>;
        using CanonicalCombiningClassTable        = std::map<char32_t, int>;
        using CanonicalDecompositionMappingTable  = std::unorderedMap<char32_t, std::vector<char32_t>>;
        using BooleanPropertyTable                = std::map<char32_t, bool>;

        const GeneralCategoryTable&                general_category_table();
        const CanonicalCombiningClassTable&        canonical_combining_class_table();
        const CanonicalDecompositionMappingTable&  canonical_decomposition_mapping_table();
        const BooleanPropertyTable&                xid_continue_table();
        const BooleanPropertyTable&                xid_start_table();
        const BooleanPropertyTable&                xid_nonstart_table();
        const BooleanPropertyTable&                full_composition_exclusion_table();
        const BooleanPropertyTable&                pattern_syntax_table();

    }

}
