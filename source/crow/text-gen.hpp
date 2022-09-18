#pragma once

#include "crow/enum.hpp"
#include "crow/random.hpp"
#include "crow/types.hpp"
#include <functional>
#include <initializer_list>
#include <memory>
#include <string>
#include <string_view>
#include <type_traits>
#include <utility>
#include <vector>

namespace Crow {

    class TextGen;

    using StringFunction = std::function<std::string(const std::string&)>;
    using StringList = std::vector<std::string>;
    using TextList = std::vector<TextGen>;
    using TextWeights = std::vector<std::pair<TextGen, double>>;

    namespace Detail {

        class TextBase;

        using SharedBase = std::shared_ptr <TextBase>;
        using BaseList = std::vector<SharedBase>;
        using BaseWeights = std::vector<std::pair<SharedBase, double>>;

        template <typename T, typename... Args> TextGen base2gen(Args&&... args);
        SharedBase gen2base(const TextGen& g);

    }

    class TextGen {

    public:

        using result_type = std::string;

        enum class option: int {
            none      = 0,
            lower     = 1,
            upper     = 2,
            title     = 4,
            xtitle    = 8,
            sentence  = 16,
        };

        TextGen() = default;
        TextGen(const std::string& str);
        TextGen(const std::string_view& str): TextGen(std::string(str)) {}
        TextGen(const char* str): TextGen(std::string(str)) {}
        TextGen(char c): TextGen(std::string{c}) {}
        TextGen(char32_t c);

        std::string operator()(StdRng& rng) const;

        void set(option opt);

        static TextGen number(int min, int max);
        static TextGen choice(const std::string& list);
        static TextGen choice(const StringList& list);
        static TextGen choice(const TextList& list);
        static TextGen choice(std::initializer_list<TextGen> list);
        static TextGen choice(const TextWeights& weights);
        static TextGen choice(std::initializer_list<std::pair<TextGen, double>> weights);

    private:

        template <typename T, typename... Args> friend TextGen Detail::base2gen(Args&&... args);
        friend Detail::SharedBase Detail::gen2base(const TextGen& g);

        Detail::SharedBase base_;
        option options_ = option::none;

    };

    CROW_BITMASK_OPERATORS(TextGen::option)

    namespace Detail {

        class TextBase {
        public:
            virtual ~TextBase() = default;
            virtual std::string gen(StdRng& rng) const = 0;
        };

        template <typename T, typename... Args>
        TextGen base2gen(Args&&... args) {
            static_assert(std::is_base_of_v<TextBase, T>);
            TextGen g;
            g.base_ = std::make_shared<T>(std::forward<Args>(args)...);
            return g;
        }

        class FixedText:
        public TextBase {
        public:
            explicit FixedText(const std::string& str): str_(str) {}
            std::string gen(StdRng& /*rng*/) const override { return str_; }
        private:
            std::string str_;
        };

        class NumberText:
        public TextBase {
        public:
            NumberText(int min, int max);
            std::string gen(StdRng& rng) const override;
        private:
            UniformInteger<int> random_int_;
        };

        class SelectText:
        public TextBase {
        public:
            explicit SelectText(const BaseList& list);
            std::string gen(StdRng& rng) const override;
            const BaseList& list() const noexcept { return list_; }
        private:
            BaseList list_;
            RandomChoice<SharedBase> choice_;
        };

        class WeightedText:
        public TextBase {
        public:
            explicit WeightedText(const BaseWeights& weights);
            std::string gen(StdRng& rng) const override;
        private:
            WeightedChoice<SharedBase> choice_;
        };

        class SequenceText:
        public TextBase {
        public:
            SequenceText(SharedBase left, SharedBase right, bool dups): left_(left), right_(right), dups_(dups) {}
            std::string gen(StdRng& rng) const override;
        private:
            SharedBase left_;
            SharedBase right_;
            bool dups_;
        };

        class OptionalText:
        public TextBase {
        public:
            OptionalText(SharedBase base, double p);
            std::string gen(StdRng& rng) const override;
        private:
            SharedBase base_;
            BernoulliDistribution option_;
        };

        class RepeatText:
        public TextBase {
        public:
            RepeatText(SharedBase base, int n): base_(base), random_int_(n, n) {}
            RepeatText(SharedBase base, int m, int n);
            std::string gen(StdRng& rng) const override;
            SharedBase base() const noexcept { return base_; }
            int min() const noexcept { return random_int_.min(); }
        private:
            SharedBase base_;
            UniformInteger<int> random_int_;
        };

        class TransformText:
        public TextBase {
        public:
            TransformText(SharedBase base, StringFunction f): base_(base), fun_(f) {}
            std::string gen(StdRng& rng) const override;
        private:
            SharedBase base_;
            StringFunction fun_;
        };

    }

    TextGen operator+(const TextGen& a, const TextGen& b);
    TextGen& operator+=(TextGen& a, const TextGen& b);
    TextGen operator&(const TextGen& a, const TextGen& b);
    TextGen& operator&=(TextGen& a, const TextGen& b);
    TextGen operator|(const TextGen& a, const TextGen& b);
    TextGen& operator|=(TextGen& a, const TextGen& b);
    TextGen operator*(const TextGen& g, int n);
    TextGen& operator*=(TextGen& g, int n);
    TextGen operator%(const TextGen& g, double p);
    TextGen& operator%=(TextGen& g, double p);
    TextGen operator>>(const TextGen& g, StringFunction f);
    TextGen& operator>>=(TextGen& g, StringFunction f);

    namespace Literals {

        TextGen operator""_tg(const char* ptr, size_t len);
        TextGen operator""_tg(unsigned long long n);

    }

}
