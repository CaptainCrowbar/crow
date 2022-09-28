#pragma once

#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/regex.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <concepts>
#include <functional>
#include <iostream>
#include <iterator>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>
#include <utility>
#include <vector>

namespace Crow {

    namespace Detail {

        template <typename T>
        concept ScalarOptionType =
            ArithmeticType<T>
            || std::is_enum_v<T>
            || std::same_as<T, std::string>
            || (! SimpleContainerType<T>
                && (std::constructible_from<T, int>
                    || std::constructible_from<T, std::string>));

        template <typename T>
        concept ContainerOptionType =
            SimpleContainerType<T>
            && ScalarOptionType<typename T::value_type>
            && ! std::same_as<T, std::string>;

        template <typename T>
        concept OptionArgumentType = ScalarOptionType<T> || ContainerOptionType<T>;

        template <typename T>
        T parse_enum_unchecked(const std::string& arg) {
            // The string has already been validated
            T t = {};
            parse_enum(arg, t);
            return t;
        }

    }

    class Options {

    public:

        enum flag_type: int {
            anon        = 1,  // Arguments not claimed by other options are assigned to this
            no_default  = 2,  // Don't show default value in help
            required    = 4,  // Required option
        };

        class setup_error:
        public std::logic_error {
        public:
            explicit setup_error(const std::string& message);
        };

        class user_error:
        public std::runtime_error {
        public:
            explicit user_error(const std::string& message);
        };

        Options() = default;
        Options(const std::string& app, const std::string& version,
            const std::string& description, const std::string& extra = {});

        template <Detail::OptionArgumentType T> Options& add(T& var, const std::string& name, char abbrev,
            const std::string& description, int flags = 0, const std::string& group = {}, const std::string& pattern = {});
        void auto_help() noexcept { auto_help_ = true; }
        void set_colour(bool b) noexcept { colour_ = int(b); }
        bool parse(std::vector<std::string> args, std::ostream& out = std::cout);
        bool parse(int argc, char** argv, std::ostream& out = std::cout);
        bool found(const std::string& name) const;

    private:

        using setter_type = std::function<void(const std::string&)>;
        using validator_type = std::function<bool(const std::string&)>;

        enum class mode { boolean, single, multiple };

        struct option_info {
            setter_type setter;
            validator_type validator;
            std::string name;
            std::string description;
            std::string placeholder;
            std::string default_value;
            std::string group;
            char abbrev = '\0';
            mode kind = mode::single;
            bool is_anon = false;
            bool is_no_default = false;
            bool is_required = false;
            bool found = false;
        };

        std::vector<option_info> options_;
        std::string app_;
        std::string version_;
        std::string description_;
        std::string extra_;
        int colour_ = 0;
        bool allow_help_ = false;
        bool auto_help_ = false;

        void do_add(setter_type setter, validator_type validator, const std::string& name, char abbrev,
            const std::string& description, const std::string& placeholder, const std::string& default_value,
            mode kind, int flags, const std::string& group);
        std::string format_help() const;
        std::string group_list(const std::string& group) const;
        size_t option_index(const std::string& name) const;
        size_t option_index(char abbrev) const;

        template <Detail::ScalarOptionType T> static T parse_argument(const std::string& arg);
        template <Detail::ScalarOptionType T> static validator_type type_validator(const std::string& name, std::string pattern);
        template <Detail::ScalarOptionType T> static std::string type_placeholder();

    };

        template <Detail::OptionArgumentType T>
        Options& Options::add(T& var, const std::string& name, char abbrev,
                const std::string& description, int flags, const std::string& group, const std::string& pattern) {

            using namespace Detail;

            setter_type setter;
            validator_type validator;
            std::string placeholder;
            std::string default_value;
            mode kind;

            if constexpr (std::same_as<T, bool>) {

                setter = [&var] (const std::string& str) { var = to_boolean(str); };
                kind = mode::boolean;

            } else if constexpr (ScalarOptionType<T>) {

                setter = [&var] (const std::string& str) { var = parse_argument<T>(str); };
                validator = type_validator<T>(name, pattern);
                placeholder = type_placeholder<T>();
                kind = mode::single;

                if constexpr (std::same_as<T, std::string>)
                    if (validator && ! validator(var))
                        throw setup_error("Default value does not match pattern: --" + name);

                if ((flags & required) == 0 && (std::is_enum_v<T> || var != T())) {
                    default_value = format_object(var);
                    if constexpr (! ArithmeticType<T> && ! std::is_enum_v<T>)
                        if (! default_value.empty())
                            default_value = quote(default_value);
                }

            } else {

                using VT = typename T::value_type;

                if (! var.empty())
                    throw setup_error("Multi-valued options may not have default values: --" + name);

                setter = [&var] (const std::string& str) { var.insert(var.end(), parse_argument<VT>(str)); };
                validator = type_validator<VT>(name, pattern);
                placeholder = type_placeholder<VT>();
                kind = mode::multiple;

            }

            do_add(setter, validator, name, abbrev, description, placeholder, default_value, kind, flags, group);

            return *this;

        }

        template <Detail::ScalarOptionType T>
        T Options::parse_argument(const std::string& arg) {
            using namespace Detail;
            if constexpr (std::is_enum_v<T>)
                return parse_enum_unchecked<T>(arg);
            else if constexpr (std::same_as<T, std::string>)
                return arg;
            else if constexpr (std::same_as<T, bool>)
                return to_boolean(arg);
            else if constexpr (std::integral<T>)
                return to_integer<T>(arg);
            else if constexpr (std::floating_point<T>)
                return to_floating<T>(arg);
            else if constexpr (std::constructible_from<T, int>)
                return static_cast<T>(to_int64(arg));
            else
                return static_cast<T>(arg);
        }

        template <Detail::ScalarOptionType T>
        Options::validator_type Options::type_validator(const std::string& name, std::string pattern) {

            using namespace Literals;

            validator_type validator;

            if constexpr (! std::same_as<T, std::string>)
                if (! pattern.empty())
                    throw setup_error("Pattern is only allowed for string-valued options: {0:q}"_fmt("--" + name));

            if constexpr (std::is_enum_v<T>)
                validator = [] (const std::string& str) {
                    auto& names = list_enum_names(T());
                    return std::find(names.begin(), names.end(), str) != names.end();
                };
            else if constexpr (std::signed_integral<T>)
                pattern = R"([+-]?\d+)";
            else if constexpr (std::unsigned_integral<T>)
                pattern = R"(\+?\d+)";
            else if constexpr (std::floating_point<T>)
                pattern = R"([+-]?(\d+(\.\d*)?|\.\d+)([Ee][+-]?\d+)?)";

            if (! pattern.empty()) {
                Regex re;
                try {
                    re = Regex(pattern, Regex::full | Regex::no_capture);
                }
                catch (const Regex::error& ex) {
                    throw setup_error(ex.what());
                }
                validator = [re] (const std::string& str) { return re(str).matched(); };
            }

            return validator;

        }

        template <Detail::ScalarOptionType T>
        std::string Options::type_placeholder() {
            if constexpr (std::signed_integral<T>)
                return "<int>";
            else if constexpr (std::unsigned_integral<T>)
                return "<uint>";
            else if constexpr (std::floating_point<T>)
                return "<real>";
            else
                return "<arg>";
        }

}
