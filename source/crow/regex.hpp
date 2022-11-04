#pragma once

#include "crow/enum.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <memory>
#include <ostream>
#include <stdexcept>
#include <string>
#include <string_view>
#include <vector>

namespace Crow {

    class Regex {

    public:

        class error;
        class match;
        class match_iterator;
        class split_iterator;
        class token_iterator;
        class transform;

        enum class flag_type: uint32_t {
            none             = 0,
            ascii            = 1u << 0,   // ASCII character properties           ~PCRE2_UCP
            byte             = 1u << 1,   // Match byte strings instead of UTF-8  ~(PCRE2_UCP|PCRE2_UTF)
            crlf             = 1u << 2,   // Lines end with CRLF                  PCRE2_NEWLINE_CRLF
            dollar_end       = 1u << 3,   // $ does not match line breaks at end  PCRE2_DOLLAR_ENDONLY
            dot_all          = 1u << 4,   // . matches line breaks                PCRE2_DOTALL
            extended         = 1u << 5,   // Ignore whitespace and # comments     PCRE2_EXTENDED
            first_line       = 1u << 6,   // Match must start in first line       PCRE2_FIRSTLINE
            icase            = 1u << 7,   // Case insensitive match               PCRE2_CASELESS
            line             = 1u << 8,   // Match whole line                     PCRE2_EXTRA_MATCH_LINE
            multiline        = 1u << 9,   // ^/$ match beginning/end of line      PCRE2_MULTILINE
            no_capture       = 1u << 10,  // No automatic capture                 PCRE2_NO_AUTO_CAPTURE
            word             = 1u << 11,  // Match whole word                     PCRE2_EXTRA_MATCH_WORD
            anchor           = 1u << 16,  // Anchor match at start                PCRE2_ANCHORED
            full             = 1u << 17,  // Match complete string                PCRE2_ANCHORED|PCRE2_ENDANCHORED
            global           = 1u << 18,  // Replace all matches                  --
            hard_fail        = 1u << 19,  // Throw on failed match                --
            not_empty        = 1u << 20,  // Do not match empty string            PCRE2_NOTEMPTY
            not_empty_start  = 1u << 21,  // Do not match empty string at start   PCRE2_NOTEMPTY_ATSTART
            not_line         = 1u << 22,  // Do not match ^/$ at start/end        PCRE2_NOTBOL|PCRE2_NOTEOL
            partial_hard     = 1u << 23,  // Hard partial match (partial>full)    PCRE2_PARTIAL_HARD
            partial_soft     = 1u << 24,  // Soft partial match (full>partial)    PCRE2_PARTIAL_SOFT
            compile_mask     = (1u << 12) - 1,
            runtime_mask     = (1u << 25) - (1u << 16),
            flags_mask       = uint32_t(compile_mask) | uint32_t(runtime_mask),
        };

        using enum flag_type;

        using version_type = std::vector<int>;

        struct partition_type {
            std::string_view left;
            std::string_view mid;
            std::string_view right;
        };

        class error:
        public std::runtime_error {
        public:
            explicit error(int code): std::runtime_error(message(code)), code_(code) {}
            int code() const noexcept { return code_; }
        private:
            int code_;
            static std::string message(int code);
        };

        class match {
        public:
            match() = default;
            bool full() const noexcept { return result_ >= 0; }
            bool partial() const noexcept;
            explicit operator bool() const noexcept { return full() || partial(); }
            bool matched(size_t i = 0) const noexcept;
            const char* begin(size_t i = 0) const noexcept;
            const char* end(size_t i = 0) const noexcept;
            size_t offset(size_t i = 0) const noexcept;
            size_t endpos(size_t i = 0) const noexcept;
            size_t count(size_t i = 0) const noexcept;
            std::string_view str(size_t i = 0) const noexcept;
            std::string_view operator[](size_t i) const noexcept { return str(i); }
            operator std::string() const { return std::string(str()); }
            operator std::string_view() const noexcept { return str(); }
            bool matched(std::string_view name) const { return matched(index_by_name(name)); }
            const char* begin(std::string_view name) const { return begin(index_by_name(name)); }
            const char* end(std::string_view name) const { return end(index_by_name(name)); }
            size_t offset(std::string_view name) const { return offset(index_by_name(name)); }
            size_t endpos(std::string_view name) const { return endpos(index_by_name(name)); }
            size_t count(std::string_view name) const { return count(index_by_name(name)); }
            std::string_view str(std::string_view name) const { return str(index_by_name(name)); }
            std::string_view operator[](std::string_view name) const { return str(name); }
            size_t first() const noexcept;
            size_t last() const noexcept;
            std::string_view last_mark() const noexcept;
        private:
            friend class Regex;
            friend class Regex::match_iterator;
            friend class Regex::split_iterator;
            friend class Regex::token_iterator;
            std::shared_ptr<void> data_; // pcre2_match_data
            std::string_view subject_;
            const Regex* regex_ = nullptr;
            flag_type flags_ = none;
            uint32_t options_ = 0;
            int result_ = -1; // PCRE2_ERROR_NOMATCH
            size_t offset_count_ = 0;
            size_t* offset_vector_ = nullptr;
            match(const Regex& re, std::string_view str, flag_type flags);
            size_t index_by_name(std::string_view name) const { return regex_ ? regex_->named(name) : npos; }
            void next() { next(endpos()); }
            void next(size_t pos);
        };

        class match_iterator:
        public ForwardIterator<Regex::match_iterator, const Regex::match> {
        public:
            match_iterator() = default;
            const match& operator*() const noexcept { return current_; }
            match_iterator& operator++() { current_.next(); return *this; }
            bool operator==(const match_iterator& rhs) const noexcept { return current_.offset() == rhs.current_.offset(); }
        private:
            friend class Regex;
            match current_;
            match_iterator(const Regex& re, std::string_view str, size_t pos, flag_type flags): current_(re, str, flags) { current_.next(pos); }
        };

        class split_iterator:
        public ForwardIterator<Regex::split_iterator, const std::string_view> {
        public:
            split_iterator() = default;
            const std::string_view& operator*() const noexcept { return span_; }
            split_iterator& operator++();
            bool operator==(const split_iterator& rhs) const noexcept { return span_.data() == rhs.span_.data(); }
        private:
            friend class Regex;
            Regex::match after_;
            const char* end_ = nullptr;
            std::string_view span_;
            split_iterator(const Regex& re, std::string_view str, size_t pos, flag_type flags);
        };

        class token_iterator:
        public ForwardIterator<Regex::token_iterator, const Regex::match> {
        public:
            token_iterator() = default;
            const match& operator*() const noexcept { return current_; }
            token_iterator& operator++();
            bool operator==(const token_iterator& rhs) const noexcept { return current_.offset() == rhs.current_.offset(); }
        private:
            friend class Regex;
            const Regex* token_ = nullptr;
            const Regex* delimiter_ = nullptr;
            match current_;
            flag_type flags_;
            token_iterator(const Regex& token, const Regex& delimiter, std::string_view str, size_t pos, flag_type flags);
        };

        class transform {
        public:
            transform() = default;
            explicit transform(const Regex& pattern, std::string_view fmt, flag_type flags = none);
            explicit transform(std::string_view pattern, std::string_view fmt, flag_type flags = none);
            std::string pattern() const;
            std::string format() const { return format_; }
            flag_type flags() const noexcept;
            std::string replace(std::string_view str, size_t pos = 0, flag_type flags = none) const;
            void replace_in(std::string& str, size_t pos = 0, flag_type flags = none) const;
            std::string operator()(std::string_view str, size_t pos = 0, flag_type flags = none) const;
        private:
            std::shared_ptr<Regex> regex_;
            std::string format_;
            flag_type flags_ = none;
        };

        using match_range = Irange<match_iterator>;
        using split_range = Irange<split_iterator>;
        using token_range = Irange<token_iterator>;

        Regex() = default;
        explicit Regex(std::string_view pattern, flag_type flags = none);

        std::string pattern() const { return pattern_; }
        flag_type flags() const noexcept { return flags_; }
        bool empty() const noexcept { return pattern_.empty(); }
        bool is_null() const noexcept { return ! code_; }
        size_t groups() const noexcept;
        size_t named(std::string_view name) const;
        match search(std::string_view str, size_t pos = 0, flag_type flags = none) const;
        match operator()(std::string_view str, size_t pos = 0, flag_type flags = none) const;
        size_t count(std::string_view str, size_t pos = 0, flag_type flags = none) const;
        match_range grep(std::string_view str, size_t pos = 0, flag_type flags = none) const;
        partition_type partition(std::string_view str, size_t pos = 0, flag_type flags = none) const;
        std::string replace(std::string_view str, std::string_view fmt, size_t pos = 0, flag_type flags = none) const;
        void replace_in(std::string& str, std::string_view fmt, size_t pos = 0, flag_type flags = none) const;
        split_range split(std::string_view str, size_t pos = 0, flag_type flags = none) const;

        static version_type compile_version() noexcept;
        static version_type runtime_version() noexcept;
        static version_type unicode_version() noexcept;
        static std::string escape(std::string_view str);
        static token_range tokenize(const Regex& token, const Regex& delimiter, std::string_view str, size_t pos = 0, flag_type flags = none);

        friend std::ostream& operator<<(std::ostream& out, const Regex& re) { return out << re.pattern(); }

    private:

        std::shared_ptr<void> context_; // pcre2_compile_context
        std::shared_ptr<void> code_; // pcre2_code
        std::string pattern_;
        flag_type flags_ = none;

        void do_replace(std::string_view src, std::string& dst, std::string_view fmt, size_t pos, flag_type flags) const;

    };

    CROW_BITMASK_OPERATORS(Regex::flag_type)

}
