#pragma once

#include "crow/binary.hpp"
#include "crow/random.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <map>
#include <optional>
#include <set>
#include <stdexcept>
#include <string>
#include <vector>

namespace Crow {

    namespace Detail {

        template <typename T> struct DefaultSequence { using type = std::vector<T>; };
        template <> struct DefaultSequence<char> { using type = std::string; };
        template <> struct DefaultSequence<char16_t> { using type = std::u16string; };
        template <> struct DefaultSequence<char32_t> { using type = std::u32string; };
        template <> struct DefaultSequence<wchar_t> { using type = std::wstring; };

    }

    enum class MarkovFlags: int {
        none       = 0,
        exclusive  = 1,
    };

    CROW_BITMASK_OPERATORS(MarkovFlags)

    template <typename T, typename S = typename Detail::DefaultSequence<T>::type>
    class Markov {

    public:

        using string_type = S;
        using value_type = T;

        Markov() = default;
        explicit Markov(size_t context, size_t min_length = 1, size_t max_length = npos, MarkovFlags flags = MarkovFlags::none);

        void add(const S& example);
        template <typename RNG> S operator()(RNG& rng) const;

    private:

        // Unordered containers would require custom hash

        std::set<S> corpus_;
        std::map<S, WeightedChoice<std::optional<T>>> freqs_;

        size_t context_ = 2;
        size_t min_length_ = 1;
        size_t max_length_ = npos;
        MarkovFlags flags_ = MarkovFlags::none;

        bool accept_result(const S& s) const;

    };

        template <typename T, typename S>
        Markov<T, S>::Markov(size_t context, size_t min_length, size_t max_length, MarkovFlags flags):
        context_(context),
        min_length_(min_length),
        max_length_(max_length),
        flags_(flags) {
            if (context == 0)
                throw std::invalid_argument("Markov generator context length must be at least 1");
            if (max_length == 0 || min_length > max_length)
                throw std::invalid_argument("Invalid output length range for Markov generator");
        }

        template <typename T, typename S>
        void Markov<T, S>::add(const S& example) {

            if (example.empty())
                return;

            if (has_bit(flags_, MarkovFlags::exclusive))
                corpus_.insert(example);

            auto i = example.begin();
            auto j = i;

            auto add_next = [&] {
                S prefix(i, j);
                std::optional<T> suffix;
                if (j != example.end())
                    suffix = *j;
                freqs_[prefix].add(1, suffix);
            };

            size_t n1 = std::min(context_, example.size());
            size_t n2 = example.size() + 1 - n1;

            for (size_t k = 0; k < n1; ++j, ++k)
                add_next();
            for (size_t k = 0; k < n2; ++i, ++j, ++k)
                add_next();

        }

        template <typename T, typename S>
        template <typename RNG>
        S Markov<T, S>::operator()(RNG& rng) const {

            S prefix, result;

            do {

                prefix.clear();
                result.clear();

                for (;;) {

                    auto suffix = freqs_.find(prefix)->second(rng);

                    if (! suffix)
                        break;

                    prefix.push_back(*suffix);
                    result.push_back(*suffix);

                    if (prefix.size() > context_)
                        prefix.erase(prefix.begin());

                }

            } while (! accept_result(result));

            return result;

        }

        template <typename T, typename S>
        bool Markov<T, S>::accept_result(const S& s) const {
            if (s.size() < min_length_ || s.size() > max_length_)
                return false;
            else if (! (flags_ & MarkovFlags::exclusive))
                return true;
            else
                return corpus_.count(s) == 0;
        }

    using CMarkov = Markov<char>;
    using SMarkov = Markov<std::string>;

}
