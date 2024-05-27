#include "crow/unicode.hpp"
#include "crow/unicode-data.hpp"
#include <algorithm>
#include <string>
#include <string_view>

namespace Crow {

    // Unicode Normalization Algorithm

    // References:
    // Unicode Standard 15.0, chapter 3.11, "Normalization Forms"
    // Unicode Standard Annex #15, "Unicode Normalization Forms"

    // We only care about NFC and NFD forms here.

    // Step 1: Decompose any characters that have canonical decompositions.
    // Decompose recursively until no more decompositions are possible.

    // Step 2: For each contiguous subsequence of non-starter characters, sort
    // the characters into ascending order by canonical combining class. This
    // is a stable sort, characters with the same combining class are not
    // reordered.

    // Step 3 (only for NFC): For each pair of characters that form a
    // canonical composing pair, replace the first with the composed
    // character and delete the second. A pair need not be adjacent to be
    // composable, if the characters between them are all non-starters with a
    // lower combining class than the second of the pair. Compose recursively
    // until no more compositions are possible.

    namespace {

        std::u32string apply_decomposition(std::u32string_view str) {
            using namespace Detail;
            std::u32string result;
            result.reserve(str.size() + str.size() / 10);
            auto it = str.begin();
            auto end = str.end();
            size_t pos = 0;
            while (it != end || pos < result.size()) {
                if (pos == result.size()) {
                    result += *it++;
                } else {
                    auto dec = canonical_decomposition(result[pos]);
                    if (dec.first == 0) {
                        ++pos;
                    } else {
                        result[pos] = dec.first;
                        if (dec.second != 0)
                            result.insert(pos + 1, 1, dec.second);
                    }
                }
            }
            return result;
        }

        void apply_ordering(std::u32string& str) {
            using namespace Detail;
            auto i = str.begin();
            auto j = i;
            auto end = str.end();
            while (j != end) {
                i = std::find_if(j, end, canonical_combining_class);
                if (i == end)
                    break;
                j = std::find_if_not(i, end, canonical_combining_class);
                std::stable_sort(i, j, [] (char32_t a, char32_t b) {
                    return canonical_combining_class(a) < canonical_combining_class(b);
                });
            }
        }

        void apply_composition(std::u32string& str) {
            using namespace Detail;
            if (str.size() < 2)
                return;
            auto i = str.begin();
            auto end = str.end();
            while (i != end) {
                i = std::find_if_not(i, end, canonical_combining_class);
                if (i == end)
                    break;
                bool combined = false;
                int prev_cc = 0;
                for (auto j = i + 1; j != end; ++j) {
                    int cc = canonical_combining_class(*j);
                    if (prev_cc == 0 || prev_cc < cc) {
                        char32_t c = canonical_composition(*i, *j);
                        if (c != 0) {
                            *i = c;
                            str.erase(j);
                            combined = true;
                            break;
                        }
                    }
                    if (cc == 0)
                        break;
                    prev_cc = cc;
                }
                if (! combined)
                    ++i;
            }
        }

    }

    std::u32string to_nfc(std::u32string_view str) {
        auto result = apply_decomposition(str);
        apply_ordering(result);
        apply_composition(result);
        return result;
    }

    std::u32string to_nfd(std::u32string_view str) {
        auto result = apply_decomposition(str);
        apply_ordering(result);
        return result;
    }

    std::string to_nfc(std::string_view str) {
        auto str32 = decode_string(str);
        auto nfc32 = to_nfc(str32);
        auto nfc8 = to_utf8(nfc32);
        return nfc8;
    }

    std::string to_nfd(std::string_view str) {
        auto str32 = decode_string(str);
        auto nfd32 = to_nfd(str32);
        auto nfd8 = to_utf8(nfd32);
        return nfd8;
    }

}
