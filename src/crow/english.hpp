#pragma once

#include "crow/string.hpp"
#include "crow/types.hpp"
#include <iterator>
#include <string>
#include <unordered_set>
#include <vector>

namespace Crow {

    // Constants

    const std::unordered_set<std::string>& stopwords();

    // Case conversion functions

    std::string extended_titlecase(const std::string& str, bool initial = true);
    std::string extended_titlecase(const std::string& str, const std::unordered_set<std::string>& stop,
        bool initial = true);
    std::string sentence_case(const std::string& str);

    // List formatting functions

    namespace Detail {

        std::string comma_list_helper(const std::vector<std::string>& list, const std::string& conj);

    }

    template <typename Range>
    std::string comma_list(const Range& range, const std::string& conj = {}) {
        using std::begin;
        using std::end;
        std::vector<std::string> vec(begin(range), end(range));
        return Detail::comma_list_helper(vec, conj);
    }

    // Number formatting functions

    std::string cardinal(size_t n, size_t threshold = std::string::npos);
    std::string ordinal(size_t n, size_t threshold = std::string::npos);
    std::string format_count(double n, int prec);
    std::string number_of(size_t n, const std::string& name, const std::string& plural_name = {}, size_t threshold = 21);

    // Pluralization functions

    std::string plural(const std::string& noun);

    // Text generators

    class LoremIpsum {
    public:
        using result_type = std::string;
        LoremIpsum() = default;
        explicit LoremIpsum(size_t bytes, bool paras = true): bytes_(bytes), paras_(paras) {}
        template <typename RNG> std::string operator()(RNG& rng) const;
    private:
        size_t bytes_ = 1000;
        bool paras_ = true;
        static const std::vector<std::string>& classic();
    };

        template <typename RNG>
        std::string LoremIpsum::operator()(RNG& rng) const {

            if (bytes_ == 0)
                return {};

            std::string text;
            text.reserve(bytes_ + 20);

            for (size_t i = 0; i < classic().size() && text.size() <= bytes_; ++i)
                text += classic()[i];

            if (paras_)
                text.replace(text.size() - 1, 1, "\n\n");

            while (text.size() <= bytes_) {
                size_t n_para = paras_ ? rng() % 7 + 1 : std::string::npos;
                for (size_t i = 0; i < n_para && text.size() <= bytes_; ++i)
                    text += classic()[rng() % classic().size()];
                if (paras_)
                    text.replace(text.size() - 1, 1, "\n\n");
            }

            size_t cut = text.find_first_of("\n .,", bytes_);

            if (cut != std::string::npos)
                text.resize(cut);

            while (! ascii_isalpha(text.back()))
                text.pop_back();

            text += ".";

            if (paras_)
                text += "\n";

            return text;

        }

}
