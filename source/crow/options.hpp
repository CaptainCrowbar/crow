#pragma once

#include "crow/binary.hpp"
#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/path.hpp"
#include "crow/regex.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <concepts>
#include <functional>
#include <iostream>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <type_traits>
#include <vector>

namespace Crow {

    namespace Detail {

        template <typename T>
        concept ScalarOptionType =
            (ArithmeticType<T>
            || std::is_enum_v<T>
            || std::same_as<T, std::string>
            || (! SimpleContainerType<T>
                && (std::constructible_from<T, int>
                    || std::constructible_from<T, std::string>)));

        template <typename T>
        concept ContainerOptionType =
            (SimpleContainerType<T>
            && ScalarOptionType<typename T::value_type>
            && ! std::same_as<T, std::string>);

        template <typename T>
        concept OptionArgumentType = (ScalarOptionType<T> || ContainerOptionType<T>);

        template <typename T>
        concept FileOptionType =
            (std::same_as<T, std::string>
            || std::same_as<T, Path>
            || (ContainerOptionType<T>
                && (std::same_as<typename T::value_type, std::string>
                    || std::same_as<typename T::value_type, Path>)));

    }

    class Options {

    public:

        enum class flag_type: uint32_t {
            none           = 0,
            anon           = 1u << 0,  // Arguments not claimed by other options are assigned to this
            no_default     = 1u << 1,  // Don't show default value in help
            required       = 1u << 2,  // Required option
            dir_exists     = 1u << 3,  // Must be an existing directory
            file_exists    = 1u << 4,  // Must be an existing file
            not_exists     = 1u << 5,  // Must not be an existing file or directory
            parent_exists  = 1u << 6,  // Parent directory must exist
        };

        using enum flag_type;

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

        template <Detail::OptionArgumentType T>
        Options& add(T& var, const std::string& name, char abbrev, const std::string& description,
            flag_type flags = none, const std::string& group = {}, const std::string& pattern = {});

        void auto_help() noexcept { auto_help_ = true; }
        void set_colour(bool b) noexcept { colour_ = int(b); }
        bool parse(std::vector<std::string> args, std::ostream& out = std::cout);
        bool parse(int argc, char** argv, std::ostream& out = std::cout);
        bool found(const std::string& name) const;

    private:

        using setter_callback = std::function<void(const std::string&)>;
        using validator_callback = std::function<bool(const std::string&)>;
        using void_callback = std::function<void()>;

        enum class mode { boolean, single, multiple };

        struct option_info {
            void_callback reset;
            setter_callback setter;
            validator_callback validator;
            std::string name;
            std::string description;
            std::string placeholder;
            std::string default_value;
            std::string group;
            char abbrev = '\0';
            mode kind = mode::single;
            flag_type flags = none;
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

        void do_add(void_callback reset, setter_callback setter, validator_callback validator,
            const std::string& name, char abbrev, const std::string& description,
            const std::string& placeholder, const std::string& default_value,
            mode kind, flag_type flags, const std::string& group);
        std::string format_help() const;
        std::string group_list(const std::string& group) const;
        size_t option_index(const std::string& name) const;
        size_t option_index(char abbrev) const;

        static void validate_path(const std::string& name, flag_type flags);

        template <Detail::ScalarOptionType T>
            static T parse_argument(const std::string& arg);
        template <Detail::ScalarOptionType T>
            static T parse_enum_unchecked(const std::string& arg);
        template <Detail::ScalarOptionType T>
            static validator_callback type_validator(const std::string& name, std::string pattern);
        template <Detail::ScalarOptionType T>
            static std::string type_placeholder(flag_type flags);

    };

        CROW_BITMASK_OPERATORS(Options::flag_type)

        template <Detail::OptionArgumentType T>
        Options& Options::add(T& var, const std::string& name, char abbrev, const std::string& description,
                flag_type flags, const std::string& group, const std::string& pattern) {

            using namespace Detail;

            void_callback reset;
            setter_callback setter;
            validator_callback validator;
            std::string placeholder;
            std::string default_value;
            mode kind;

            if constexpr (! FileOptionType<T>)
                if (has_bit(flags, dir_exists | file_exists | parent_exists | not_exists))
                    throw setup_error("Invalid variable type for a file or directory option: --" + name);

            if constexpr (std::same_as<T, bool>) {

                setter = [&var] (const std::string& str) { var = to_boolean(str); };
                kind = mode::boolean;

            } else if constexpr (ScalarOptionType<T>) {

                setter = [&var] (const std::string& str) { var = parse_argument<T>(str); };
                validator = type_validator<T>(name, pattern);
                placeholder = type_placeholder<T>(flags);
                kind = mode::single;

                if constexpr (std::same_as<T, std::string>)
                    if (validator && ! validator(var))
                        throw setup_error("Default value does not match pattern: --" + name);

                if (! has_bit(flags, required) && (std::is_enum_v<T> || var != T())) {
                    default_value = format_object(var);
                    if constexpr (! ArithmeticType<T> && ! std::is_enum_v<T>)
                        if (! default_value.empty())
                            default_value = quote(default_value);
                }

            } else {

                using V = typename T::value_type;

                reset = [&var] { var.clear(); };
                setter = [&var] (const std::string& str) { var.insert(var.end(), parse_argument<V>(str)); };
                validator = type_validator<V>(name, pattern);
                placeholder = type_placeholder<V>(flags);
                kind = mode::multiple;

                if constexpr (std::same_as<V, std::string>)
                    if (validator)
                        for (auto& v: var)
                            if (! validator(v))
                                throw setup_error("Default value does not match pattern: --" + name);

                if (! has_bit(flags, required)) {
                    if (var.size() == 1) {
                        default_value = format_object(*var.begin());
                        if constexpr (! ArithmeticType<V> && ! std::is_enum_v<V>)
                            if (! default_value.empty())
                                default_value = quote(default_value);
                    } else if (var.size() > 1) {
                        default_value = format_object(var);
                    }
                }

            }

            do_add(reset, setter, validator, name, abbrev, description, placeholder, default_value,
                kind, flags, group);

            return *this;

        }

        template <Detail::ScalarOptionType T>
        T Options::parse_argument(const std::string& arg) {
            using namespace Detail;
            try {
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
            catch (const std::invalid_argument& ex) {
                throw setup_error(ex.what());
            }
        }

        template <Detail::ScalarOptionType T>
        T Options::parse_enum_unchecked(const std::string& arg) {
            // The string and type have already been validated
            T t = {};
            parse_enum(arg, t);
            return t;
        }

        template <Detail::ScalarOptionType T>
        Options::validator_callback Options::type_validator(const std::string& name, std::string pattern) {

            if constexpr (std::is_enum_v<T>) {
                if (! pattern.empty())
                    throw setup_error(fmt("Patterns are not allowed for enumeration options: {0:q}", "--" + name));
                return [] (const std::string& str) {
                    auto& names = list_enum_names(T());
                    return std::find(names.begin(), names.end(), str) != names.end();
                };
            }

            if (pattern.empty()) {
                return {};
            } else {
                try {
                    auto re = Regex(pattern, Regex::full | Regex::no_capture);
                    return [re] (const std::string& str) { return re(str).matched(); };
                }
                catch (const Regex::error& ex) {
                    throw setup_error(ex.what());
                }
            }

        }

        template <Detail::ScalarOptionType T>
        std::string Options::type_placeholder(flag_type flags) {

            if constexpr (std::signed_integral<T>)
                return "<int>";
            if constexpr (std::unsigned_integral<T>)
                return "<uint>";
            if constexpr (std::floating_point<T>)
                return "<real>";

            if constexpr (std::same_as<T, std::string>) {
                if (has_bits(flags, dir_exists | file_exists))
                    return "<file>";
                if (has_bit(flags, dir_exists))
                    return "<dir>";
                if (has_bit(flags, dir_exists | file_exists | not_exists | parent_exists))
                    return "<file>";
            }

            return "<arg>";

        }

}
