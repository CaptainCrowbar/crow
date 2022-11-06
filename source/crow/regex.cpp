#include "crow/regex.hpp"
#include <algorithm>
#include <cstdlib>
#include <iterator>
#include <new>
#include <utility>

#ifdef PCRE2_CODE_UNIT_WIDTH
    #undef PCRE2_CODE_UNIT_WIDTH
#endif

#define PCRE2_CODE_UNIT_WIDTH 8

#include <pcre2.h>

#if ! PCRE2_CONFIG_UNICODE
    #error PCRE2 must be compiled with Unicode support
#endif

namespace Crow {

    namespace {

        auto byte_ptr(std::string_view v) noexcept { return reinterpret_cast<const unsigned char*>(v.data()); }
        auto byte_ptr(std::string& s) noexcept { return reinterpret_cast<unsigned char*>(&s[0]); }

        void handle_error(int rc) {
            if (rc == PCRE2_ERROR_NOMEMORY)
                throw std::bad_alloc();
            else
                throw Regex::error(rc);
        }

        Regex::version_type get_version(uint32_t flag) noexcept {
            Regex::version_type v;
            char buf[24];
            if (pcre2_config(flag, buf) >= 0) {
                const char* ptr = buf;
                char* next = nullptr;
                for (;;) {
                    if (*ptr < '0' || *ptr > '9')
                        break;
                    v.push_back(int(std::strtol(ptr, &next, 10)));
                    if (*next != '.')
                        break;
                    ptr = next + 1;
                }
            }
            return v;
        }

        constexpr uint32_t translate_compile_flags(Regex::flag_type flags) noexcept {

            uint32_t options = 0;

            if (! (flags & Regex::byte)) {
                options |= PCRE2_UTF;
                if (! (flags & Regex::ascii))
                    options |= PCRE2_UCP;
            }

            if (!! (flags & Regex::anchor))      options |= PCRE2_ANCHORED;
            if (!! (flags & Regex::dollar_end))  options |= PCRE2_DOLLAR_ENDONLY;
            if (!! (flags & Regex::dot_all))     options |= PCRE2_DOTALL;
            if (!! (flags & Regex::extended))    options |= PCRE2_EXTENDED;
            if (!! (flags & Regex::first_line))  options |= PCRE2_FIRSTLINE;
            if (!! (flags & Regex::full))        options |= PCRE2_ANCHORED | PCRE2_ENDANCHORED;
            if (!! (flags & Regex::icase))       options |= PCRE2_CASELESS;
            if (!! (flags & Regex::multiline))   options |= PCRE2_MULTILINE;
            if (!! (flags & Regex::no_capture))  options |= PCRE2_NO_AUTO_CAPTURE;

            return options;

        }

        constexpr uint32_t translate_match_flags(Regex::flag_type flags) noexcept {

            uint32_t options = 0;

            if (!! (flags & Regex::anchor))           options |= PCRE2_ANCHORED;
            if (!! (flags & Regex::full))             options |= PCRE2_ANCHORED | PCRE2_ENDANCHORED;
            if (!! (flags & Regex::not_empty))        options |= PCRE2_NOTEMPTY;
            if (!! (flags & Regex::not_empty_start))  options |= PCRE2_NOTEMPTY_ATSTART;
            if (!! (flags & Regex::not_line))         options |= PCRE2_NOTBOL | PCRE2_NOTEOL;
            if (!! (flags & Regex::partial_hard))     options |= PCRE2_PARTIAL_HARD;
            if (!! (flags & Regex::partial_soft))     options |= PCRE2_PARTIAL_SOFT;

            return options;

        }

    }

    // Class Regex

    Regex::Regex(std::string_view pattern, flag_type flags) {

        if (!! (flags & ~ flags_mask))
            throw error(PCRE2_ERROR_BADOPTION);

        pattern_ = pattern;
        flags_ = flags;
        uint32_t compile_options = translate_compile_flags(flags);

        if (pattern.find("(*") != npos)
            compile_options |= PCRE2_NO_DOTSTAR_ANCHOR | PCRE2_NO_START_OPTIMIZE;

        auto context_ptr = pcre2_compile_context_create(nullptr);
        if (! context_ptr)
            throw std::bad_alloc();

        context_.reset(context_ptr, pcre2_compile_context_free);
        pcre2_set_bsr(context_ptr, !! (flags & byte) ? PCRE2_BSR_ANYCRLF : PCRE2_BSR_UNICODE);
        pcre2_set_newline(context_ptr, !! (flags & crlf) ? PCRE2_NEWLINE_CRLF : PCRE2_NEWLINE_LF);

        uint32_t extra = 0;
        if (!! (flags & Regex::line))
            extra |= PCRE2_EXTRA_MATCH_LINE;
        if (!! (flags & Regex::word))
            extra |= PCRE2_EXTRA_MATCH_WORD;
        pcre2_set_compile_extra_options(context_ptr, extra);

        int error_code = 0;
        size_t error_pos = 0;
        auto code_ptr = pcre2_compile(byte_ptr(pattern), pattern.size(), compile_options, &error_code, &error_pos, context_ptr);
        if (! code_ptr)
            handle_error(error_code);

        code_.reset(code_ptr, pcre2_code_free);

    }

    size_t Regex::groups() const noexcept {
        if (is_null())
            return 0;
        uint32_t captures = 0;
        auto code_ptr = static_cast<pcre2_code*>(code_.get());
        pcre2_pattern_info(code_ptr, PCRE2_INFO_CAPTURECOUNT, &captures);
        return captures + 1;
    }

    size_t Regex::named(std::string_view name) const {
        if (is_null())
            return npos;
        auto code_ptr = static_cast<pcre2_code*>(code_.get());
        std::string name_str(name);
        int rc = pcre2_substring_number_from_name(code_ptr, byte_ptr(name_str));
        return rc == PCRE2_ERROR_NOSUBSTRING || rc == PCRE2_ERROR_NOUNIQUESUBSTRING ? npos : size_t(rc);
    }

    Regex::match Regex::search(std::string_view str, size_t pos, flag_type flags) const {
        match m(*this, str, flags);
        m.next(pos);
        return m;
    }

    Regex::match Regex::operator()(std::string_view str, size_t pos, flag_type flags) const {
        return search(str, pos, flags);
    }

    size_t Regex::count(std::string_view str, size_t pos, flag_type flags) const {
        auto range = grep(str, pos, flags);
        return size_t(std::distance(range.first, range.second));
    }

    Regex::match_range Regex::grep(std::string_view str, size_t pos, flag_type flags) const {
        return {{*this, str, pos, flags}, {}};
    }

    Regex::partition_type Regex::partition(std::string_view str, size_t pos, flag_type flags) const {
        match m = search(str, pos, flags);
        if (m)
            return {{str.data(), m.offset()}, m, {str.data() + m.endpos(), str.size() - m.endpos()}};
        else
            return {str, {str.data() + str.size(), 0}, {str.data() + str.size(), 0}};
    }

    void Regex::do_replace(std::string_view src, std::string& dst, std::string_view fmt, size_t pos, flag_type flags) const {

        static constexpr uint32_t default_options =
            PCRE2_SUBSTITUTE_EXTENDED | PCRE2_SUBSTITUTE_OVERFLOW_LENGTH | PCRE2_SUBSTITUTE_UNKNOWN_UNSET | PCRE2_SUBSTITUTE_UNSET_EMPTY;

        if (is_null()) {
            dst = src;
            return;
        }

        if (!! (flags & ~ runtime_mask))
            throw error(PCRE2_ERROR_BADOPTION);

        flags |= flags_;
        uint32_t replace_options = default_options | translate_match_flags(flags);
        if (!! (flags & global))
            replace_options |= PCRE2_SUBSTITUTE_GLOBAL;

        auto code_ptr = static_cast<pcre2_code*>(code_.get());
        dst.assign(src.size() + fmt.size() + 100, '\0');
        int rc = -1;

        while (rc < 0) {
            size_t dst_size = dst.size();
            rc = pcre2_substitute(code_ptr, byte_ptr(src), src.size(), pos, replace_options, nullptr, nullptr,
                byte_ptr(fmt), fmt.size(), byte_ptr(dst), &dst_size);
            if (rc < 0 && rc != PCRE2_ERROR_NOMATCH && rc != PCRE2_ERROR_NOMEMORY && rc != PCRE2_ERROR_PARTIAL)
                handle_error(rc);
            dst.resize(dst_size);
        }

    }

    std::string Regex::replace(std::string_view str, std::string_view fmt, size_t pos, flag_type flags) const {
        std::string dst;
        do_replace(str, dst, fmt, pos, flags);
        return dst;
    }

    void Regex::replace_in(std::string& str, std::string_view fmt, size_t pos, flag_type flags) const {
        std::string dst;
        do_replace(str, dst, fmt, pos, flags);
        str = std::move(dst);
    }

    Regex::split_range Regex::split(std::string_view str, size_t pos, flag_type flags) const {
        return {{*this, str, pos, flags}, {}};
    }

    Regex::version_type Regex::compile_version() noexcept {
        return {PCRE2_MAJOR, PCRE2_MINOR};
    }

    Regex::version_type Regex::runtime_version() noexcept {
        static const version_type v = get_version(PCRE2_CONFIG_VERSION);
        return v;
    }

    Regex::version_type Regex::unicode_version() noexcept {
        static const version_type v = get_version(PCRE2_CONFIG_UNICODE_VERSION);
        return v;
    }

    std::string Regex::escape(std::string_view str) {

        static const auto digit = [] (auto x) { return char(x < 10 ? '0' + x : 'a' + x - 10); };

        static const std::string table = [] {
            static const std::string meta = "$()*+.?[\\]^{|}";
            std::string tab(256, 'c');
            auto b = tab.begin();
            std::fill(b, b + 9, 'x');
            std::fill(b + 14, b + 32, 'x');
            for (char m: meta)
                tab[uint8_t(m)] = 'm';
            tab[127] = 'x';
            return tab;
        }();

        std::string esc;

        for (char c: str) {
            auto b = uint8_t(c);
            char t = table[b];
            if (t == 'm')
                esc += {'\\', c};
            else if (t == 'x')
                esc += {'\\', 'x', digit(b / 16), digit(b % 16)};
            else
                esc += c;
        }

        return esc;

    }

    Regex::token_range Regex::tokenize(const Regex& token, const Regex& delimiter, std::string_view str, size_t pos, flag_type flags) {
        return {{token, delimiter, str, pos, flags}, {}};
    }

    // Class Regex::error

    std::string Regex::error::message(int code) {
        std::string msg = "PCRE2 error " + std::to_string(code);
        if (code == 0)
            return msg;
        std::string buf(120, '\0');
        int rc = pcre2_get_error_message(code, byte_ptr(buf), buf.size());
        if (rc > 0) {
            msg += ": ";
            msg += buf.data();
        }
        return msg;
    }

    // Class Regex::token_error

    std::string Regex::token_error::message(size_t pos) {
        return "Tokenization error at byte " + std::to_string(pos);
    }

    // Class Regex::match

    bool Regex::match::partial() const noexcept {
        return result_ == PCRE2_ERROR_PARTIAL;
    }

    bool Regex::match::matched(size_t i) const noexcept {
        if (result_ < 0 && result_ != PCRE2_ERROR_PARTIAL)
            return false;
        else if (i == 0)
            return true;
        else if (i < offset_count_)
            return offset_vector_[2 * i] != PCRE2_UNSET;
        else
            return false;
    }

    const char* Regex::match::begin(size_t i) const noexcept {
        size_t ofs = offset(i);
        return ofs == npos ? nullptr : subject_.data() + ofs;
    }

    const char* Regex::match::end(size_t i) const noexcept {
        size_t ofs = endpos(i);
        return ofs == npos ? nullptr : subject_.data() + ofs;
    }

    size_t Regex::match::offset(size_t i) const noexcept {
        if (i >= offset_count_)
            return npos;
        size_t ofs = offset_vector_[2 * i];
        return ofs == PCRE2_UNSET ? npos : ofs;
    }

    size_t Regex::match::endpos(size_t i) const noexcept {
        if (i >= offset_count_)
            return npos;
        size_t ofs = offset_vector_[2 * i + 1];
        return ofs == PCRE2_UNSET ? npos : ofs;
    }

    size_t Regex::match::count(size_t i) const noexcept {
        if (i >= offset_count_)
            return 0;
        size_t start = offset_vector_[2 * i], stop = offset_vector_[2 * i + 1];
        return start == PCRE2_UNSET || stop < start ? 0 : stop - start;
    }

    std::string_view Regex::match::str(size_t i) const noexcept {
        if (i >= offset_count_)
            return {};
        size_t start = offset_vector_[2 * i], stop = offset_vector_[2 * i + 1];
        if (start == PCRE2_UNSET)
            return {};
        else if (start <= stop)
            return std::string_view(subject_.data() + start, stop - start);
        else
            return std::string_view(subject_.data() + start, 0);
    }

    size_t Regex::match::first() const noexcept {
        for (size_t i = 1; i < offset_count_; ++i) {
            size_t start = offset_vector_[2 * i], stop = offset_vector_[2 * i + 1];
            if (start != PCRE2_UNSET && start < stop)
                return i;
        }
        return npos;
    }

    size_t Regex::match::last() const noexcept {
        if (offset_count_ > 1) {
            for (size_t i = offset_count_ - 1; i > 0; --i) {
                size_t start = offset_vector_[2 * i], stop = offset_vector_[2 * i + 1];
                if (start != PCRE2_UNSET && start < stop)
                    return i;
            }
        }
        return npos;
    }

    std::string_view Regex::match::last_mark() const noexcept {
        if (! data_)
            return {};
        auto match_ptr = static_cast<pcre2_match_data*>(data_.get());
        auto ptr = pcre2_get_mark(match_ptr);
        if (! ptr)
            return {};
        auto mark_ptr = reinterpret_cast<const char*>(ptr);
        size_t count = ptr[-1];
        return {mark_ptr, count};
    }

    Regex::match::match(const Regex& re, std::string_view str, flag_type flags) {
        if (!! (flags & ~ runtime_mask))
            throw error(PCRE2_ERROR_BADOPTION);
        if (re.is_null() || ! str.data())
            return;
        subject_ = str;
        regex_ = &re;
        flags_ = flags | re.flags();
        options_ = translate_match_flags(flags_);
    }

    void Regex::match::next(size_t pos) {

        if (! regex_)
            return;

        if (pos > subject_.size())
            return;

        auto code_ptr = static_cast<pcre2_code*>(regex_->code_.get());

        if (data_.use_count() != 1) {
            auto new_data = pcre2_match_data_create_from_pattern(code_ptr, nullptr);
            if (! new_data)
                throw std::bad_alloc();
            data_.reset(new_data, pcre2_match_data_free);
        }

        result_ = PCRE2_ERROR_NOMATCH;
        offset_count_ = 0;
        offset_vector_ = nullptr;
        auto match_ptr = static_cast<pcre2_match_data*>(data_.get());
        result_ = pcre2_match(code_ptr, byte_ptr(subject_), subject_.size(), pos, options_, match_ptr, nullptr);

        if (result_ == PCRE2_ERROR_NOMATCH && ! (flags_ & hard_fail))
            return;
        else if (result_ < 0 && result_ != PCRE2_ERROR_PARTIAL)
            handle_error(result_);

        offset_vector_ = pcre2_get_ovector_pointer(match_ptr);
        offset_count_ = pcre2_get_ovector_count(match_ptr);

    }

    // Class Regex::split_iterator

    Regex::split_iterator& Regex::split_iterator::operator++() {
        if (after_) {
            auto start = after_.end();
            after_.next();
            if (after_)
                span_ = {start, size_t(after_.begin() - start)};
            else
                span_ = {start, size_t(end_ - start)};
        } else {
            span_ = {};
        }
        return *this;
    }

    Regex::split_iterator::split_iterator(const Regex& re, std::string_view str, size_t pos, flag_type flags):
    after_(re, str, flags), end_(str.data() + str.size()), span_(str) {
        after_.next(pos);
        if (after_)
            span_ = std::string_view(str.data() + pos, after_.offset() - pos);
    }

    // Class Regex::token_iterator

    Regex::token_iterator& Regex::token_iterator::operator++() {
        if (current_.endpos() >= current_.subject_.size()) {
            current_ = {};
        } else {
            auto next_delimiter = delimiter_->search(current_.subject_, current_.endpos(), flags_);
            if (! next_delimiter)
                throw token_error(current_.endpos());
            current_ = token_->search(current_.subject_, next_delimiter.endpos(), flags_);
            if (! current_)
                throw token_error(next_delimiter.endpos());
        }
        return *this;
    }

    Regex::token_iterator::token_iterator(const Regex& token, const Regex& delimiter, std::string_view str, size_t pos, flag_type flags):
    token_(&token), delimiter_(&delimiter), current_(), flags_(flags | Regex::anchor) {
        if (pos < str.size()) {
            current_ = token_->search(str, pos, flags_);
            if (! current_)
                throw token_error(pos);
        }
    }

    // Class Regex::transform

    Regex::transform::transform(const Regex& pattern, std::string_view fmt, flag_type flags):
    regex_(std::make_shared<Regex>(pattern)), format_(fmt), flags_(flags) {}

    Regex::transform::transform(std::string_view pattern, std::string_view fmt, flag_type flags):
    regex_(std::make_shared<Regex>(pattern, flags)), format_(fmt), flags_(flags & runtime_mask) {}

    std::string Regex::transform::pattern() const {
        return regex_ ? regex_->pattern() : std::string();
    }

    Regex::flag_type Regex::transform::flags() const noexcept {
        return regex_ ? regex_->flags() | flags_ : none;
    }

    std::string Regex::transform::replace(std::string_view str, size_t pos, flag_type flags) const {
        if (regex_)
            return regex_->replace(str, format_, pos, flags | flags_);
        else
            return std::string(str);
    }

    void Regex::transform::replace_in(std::string& str, size_t pos, flag_type flags) const {
        if (regex_)
            regex_->replace_in(str, format_, pos, flags | flags_);
    }

    std::string Regex::transform::operator()(std::string_view str, size_t pos, flag_type flags) const {
        return replace(str, pos, flags);
    }

}
