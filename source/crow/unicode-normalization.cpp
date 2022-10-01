#include "crow/unicode.hpp"
#include "crow/unicode-data.hpp"
#include <algorithm>

namespace Crow {

    // Unicode Normalization Algorithm:

    // References:
    // Unicode Standard 13.0, chapter 3.11, "Normalization Forms"
    // Unicode Standard Annex #15, "Unicode Normalization Forms"

    // Step 1: Decompose any decomposable characters. For NFC and NFD, apply
    // only canonical decompositions; for NFKC and NFKD, also apply
    // compatibility decompositions. Decompose characters recursively until
    // no more decompositions are possible.

    // Step 2: For each contiguous subsequence of non-starter characters, sort
    // the characters into ascending order by canonical combining class.
    // Characters with the same combining class are not reordered(i.e. stable
    // sort).

    // Step 3 (only for NFC and NFKC): For each pair of characters that form a
    // canonical composing pair, replace the first with the composed
    // character and delete the second. A pair need not be adjacent to be
    // composable, if the characters between them are all non-starters with a
    // lower combining class than the second of the pair. Compose characters
    // recursively until no more compositions are possible.

    namespace {

        void apply_decomposition(std::u32string& str) {
            using namespace Detail;
            std::u32string out;
            out.reserve(str.size());
            std::pair<char32_t, char32_t> dec;
            auto i = str.begin(), end = str.end();
            size_t j = 0;
            while (i != end || j < out.size()) {
                if (j == out.size()) {
                    out += *i++;
                } else {
                    dec = canonical_decomposition(out[j]);
                    if (dec.first == 0) {
                        ++j;
                    } else {
                        out[j] = dec.first;
                        if (dec.second != 0)
                            out.insert(j + 1, 1, dec.second);
                    }
                }
            }
            str.swap(out);
        }

        void apply_ordering(std::u32string& str) {
            using namespace Detail;
            auto i = str.begin(), j = i, end = str.end();
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
            auto i = str.begin(), end = str.end();
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
                        if (c) {
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

    std::u32string to_nfc(std::u32string str) {
        apply_decomposition(str);
        apply_ordering(str);
        apply_composition(str);
        return str;
    }

    std::u32string to_nfd(std::u32string str) {
        apply_decomposition(str);
        apply_ordering(str);
        return str;
    }

    std::string to_nfc(std::string str) {
        auto str32 = decode_string(str);
        auto nfc32 = to_nfc(str32);
        auto nfc8 = to_utf8(nfc32);
        return nfc8;
    }

    std::string to_nfd(std::string str) {
        auto str32 = decode_string(str);
        auto nfd32 = to_nfd(str32);
        auto nfd8 = to_utf8(nfd32);
        return nfd8;
    }

}
