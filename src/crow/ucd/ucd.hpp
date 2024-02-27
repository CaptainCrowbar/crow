#include "crow/enum.hpp"
#include "crow/types.hpp"
#include <unordered_map>
#include <utility>
#include <vector>

namespace Crow {

    CROW_ENUM_SCOPED(GC, int,
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
        Zs,  // Space_Separator
    )

    namespace Detail {

        CROW_ENUM_SCOPED(East_Asian_Width, int,
            A,   // Ambiguous
            F,   // Fullwidth
            H,   // Halfwidth
            N,   // Neutral
            Na,  // Narrow
            W    // Wide
        )

        CROW_ENUM_SCOPED(Grapheme_Cluster_Break, int,
            Control,
            CR,
            Extend,
            L,
            LF,
            LV,
            LVT,
            Other,
            Prepend,
            Regional_Indicator,
            SpacingMark,
            T,
            V,
            ZWJ
        )

        CROW_ENUM_SCOPED(Hangul_Syllable_Type, int,
            L,
            LV,
            LVT,
            NA,
            T,
            V
        )

        template <typename T> using UcdPropertyTable  = std::vector<std::pair<char32_t, T>>;
        using BooleanPropertyTable                    = UcdPropertyTable<bool>;
        using CanonicalCombiningClassTable            = UcdPropertyTable<int>;
        using EastAsianWidthTable                     = UcdPropertyTable<East_Asian_Width>;
        using GeneralCategoryTable                    = UcdPropertyTable<GC>;
        using GraphemeClusterBreakTable               = UcdPropertyTable<Grapheme_Cluster_Break>;
        using CanonicalDecompositionMappingTable      = std::unordered_map<char32_t, std::pair<char32_t, char32_t>>;

        const GeneralCategoryTable&                general_category_table();
        const CanonicalCombiningClassTable&        canonical_combining_class_table();
        const CanonicalDecompositionMappingTable&  canonical_decomposition_mapping_table();
        const EastAsianWidthTable&                 east_asian_width_table();
        const GraphemeClusterBreakTable&           grapheme_cluster_break_table();
        const BooleanPropertyTable&                full_composition_exclusion_table();
        const BooleanPropertyTable&                pattern_syntax_table();
        const BooleanPropertyTable&                xid_continue_table();
        const BooleanPropertyTable&                xid_start_table();
        const BooleanPropertyTable&                extended_pictographic_table();

    }

}
