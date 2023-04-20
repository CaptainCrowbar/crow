#pragma once

#include "crow/types.hpp"
#include <stdexcept>
#include <string>
#include <string_view>

namespace Crow {

    class EncodingError:
    public std::runtime_error {
    public:
        explicit EncodingError(const std::string& encoding);
    };

    class Encoding {
    public:
        virtual ~Encoding() noexcept {}
        virtual std::string name() const = 0;
        std::string encode(const void* in, size_t len) const;
        std::string encode(std::string_view in) const { return encode(in.data(), in.size()); }
        size_t decode(std::string_view in, void* out, size_t maxlen) const { return do_decode(in, out, maxlen); }
        std::string decode(std::string_view in) const;
        size_t line() const noexcept { return line_; }
    protected:
        explicit Encoding(size_t line): line_(line) {}
        virtual bool can_break(std::string_view /*encoded*/, size_t /*pos*/) const noexcept { return true; }
        virtual std::string do_encode(const void* in, size_t len) const = 0;
        virtual size_t do_decode(std::string_view in, void* out, size_t maxlen) const = 0;
    private:
        size_t line_ = npos;
    };

    class Hexcode:
    public Encoding {
    public:
        Hexcode() noexcept: Encoding(npos) {}
        explicit Hexcode(size_t line) noexcept: Encoding(line) {}
        std::string name() const override { return "Hexcode"; }
    protected:
        bool can_break(std::string_view /*encoded*/, size_t pos) const noexcept override { return pos % 2 == 0; }
        std::string do_encode(const void* in, size_t len) const override;
        size_t do_decode(std::string_view in, void* out, size_t maxlen) const override;
    };

    class Base64:
    public Encoding {
    public:
        Base64() noexcept: Base64(npos, default_last2, true) {}
        explicit Base64(size_t line, const std::string& last2 = default_last2, bool padding = true);
        std::string name() const override { return "Base64"; }
    protected:
        std::string do_encode(const void* in, size_t len) const override;
        size_t do_decode(std::string_view in, void* out, size_t maxlen) const override;
    private:
        static constexpr const char* default_last2 = "+/";
        std::string last2_;
        bool padding_;
    };

}
