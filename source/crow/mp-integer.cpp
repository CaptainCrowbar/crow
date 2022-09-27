#include "crow/mp-integer.hpp"
#include <bit>
#include <cstring>
#include <stdexcept>

namespace Crow {

    namespace {

        std::string roman(const MPN& n, bool lcase = false) {

            static constexpr std::pair<int, const char*> table[] = {
                { 900, "CM" }, { 500, "D" }, { 400, "CD" }, { 100, "C" },
                { 90, "XC" }, { 50, "L" }, { 40, "XL" }, { 10, "X" },
                { 9, "IX" }, { 5, "V" }, { 4, "IV" }, { 1, "I" },
            };

            std::string rom(size_t(n / 1000), 'M');
            auto m = n;
            m %= 1000;

            for (auto [num,str]: table) {
                for (MPN q = m / num; q > 0; --q)
                    rom += str;
                m %= num;
            }

            if (lcase)
                rom = ascii_lowercase(rom);

            return rom;

        }

    }

    // Unsigned integer class

    MPN::MPN(uint64_t x) {
        if (x > 0)
            rep_.push_back(uint32_t(x));
        if (x > mask32)
            rep_.push_back(uint32_t(x >> 32));
    }

    MPN& MPN::operator+=(const MPN& rhs) {
        size_t common = std::min(rep_.size(), rhs.rep_.size());
        rep_.resize(std::max(rep_.size(), rhs.rep_.size()), 0);
        uint64_t sum = 0;
        for (size_t i = 0; i < common; ++i) {
            sum += uint64_t(rep_[i]) + uint64_t(rhs.rep_[i]);
            rep_[i] = uint32_t(sum);
            sum >>= 32;
        }
        const auto* rptr = &rep_;
        if (rhs.rep_.size() > common)
            rptr = &rhs.rep_;
        for (size_t i = common; i < rep_.size(); ++i) {
            sum += uint64_t((*rptr)[i]);
            rep_[i] = uint32_t(sum);
            sum >>= 32;
        }
        if (sum)
            rep_.push_back(uint32_t(sum));
        trim();
        return *this;
    }

    MPN& MPN::operator-=(const MPN& rhs) {
        size_t common = std::min(rep_.size(), rhs.rep_.size());
        bool carry = false;
        for (size_t i = 0; i < common; ++i) {
            bool c = rep_[i] < rhs.rep_[i] || (carry && rep_[i] == rhs.rep_[i]);
            rep_[i] -= rhs.rep_[i];
            if (carry)
                --rep_[i];
            carry = c;
        }
        for (size_t i = common; carry && i < rep_.size(); ++i)
            carry = --rep_[i] == mask32;
        trim();
        return *this;
    }

    MPN& MPN::operator&=(const MPN& rhs) {
        size_t common = std::min(rep_.size(), rhs.rep_.size());
        rep_.resize(common);
        for (size_t i = 0; i < common; ++i)
            rep_[i] &= rhs.rep_[i];
        trim();
        return *this;
    }

    MPN& MPN::operator|=(const MPN& rhs) {
        size_t common = std::min(rep_.size(), rhs.rep_.size());
        rep_.resize(std::max(rep_.size(), rhs.rep_.size()));
        for (size_t i = 0; i < common; ++i)
            rep_[i] |= rhs.rep_[i];
        return *this;
    }

    MPN& MPN::operator^=(const MPN& rhs) {
        size_t common = std::min(rep_.size(), rhs.rep_.size());
        rep_.resize(std::max(rep_.size(), rhs.rep_.size()));
        for (size_t i = 0; i < common; ++i)
            rep_[i] ^= rhs.rep_[i];
        trim();
        return *this;
    }

    MPN& MPN::operator<<=(ptrdiff_t rhs) {
        if (rhs == 0)
            return *this;
        if (rhs < 0)
            return *this >>= - rhs;
        size_t words = rhs / 32;
        int bits = rhs % 32;
        uint32_t prev = 0;
        if (bits > 0) {
            for (auto& word: rep_) {
                uint32_t next = word >> (32 - bits);
                word = (word << bits) | prev;
                prev = next;
            }
        }
        if (prev)
            rep_.push_back(prev);
        rep_.insert(rep_.begin(), words, 0);
        return *this;
    }

    MPN& MPN::operator>>=(ptrdiff_t rhs) {
        if (rhs == 0)
            return *this;
        if (rhs < 0)
            return *this <<= - rhs;
        size_t words = rhs / 32;
        int bits = rhs % 32;
        if (words >= rep_.size()) {
            rep_.clear();
        } else {
            rep_.erase(rep_.begin(), rep_.begin() + words);
            if (bits > 0) {
                uint32_t prev = 0;
                for (size_t i = rep_.size() - 1; i != std::string::npos; --i) {
                    uint32_t next = rep_[i] << (32 - bits);
                    rep_[i] = (rep_[i] >> bits) | prev;
                    prev = next;
                }
            }
        }
        trim();
        return *this;
    }

    size_t MPN::bits() const noexcept {
        size_t n = 32 * rep_.size();
        if (! rep_.empty())
            n -= 32 - std::bit_width(rep_.back());
        return n;
    }

    size_t MPN::bits_set() const noexcept {
        size_t n = 0;
        for (auto i: rep_)
            n += std::popcount(i);
        return n;
    }

    size_t MPN::bytes() const noexcept {
        if (rep_.empty())
            return 0;
        else
            return 4 * (rep_.size() - 1) + size_t(rep_.back() > 0) + size_t(rep_.back() > size_t(0xff))
                + size_t(rep_.back() > size_t(0xffff)) + size_t(rep_.back() > size_t(0xffffff));
    }

    bool MPN::get_bit(size_t i) const noexcept {
        if (i < 32 * rep_.size())
            return (rep_[i / 32] >> (i % 32)) & 1;
        else
            return false;
    }

    uint8_t MPN::get_byte(size_t i) const noexcept {
        if (i < 4 * rep_.size())
            return (rep_[i / 4] >> (i % 4 * 8)) & 0xff;
        else
            return 0;
    }

    void MPN::set_bit(size_t i, bool b) {
        bool in_rep = i < 32 * rep_.size();
        if (b) {
            if (! in_rep)
                rep_.resize(i / 32 + 1, 0);
            rep_[i / 32] |= uint32_t(1) << (i % 32);
        } else if (in_rep) {
            rep_[i / 32] &= ~ (uint32_t(1) << (i % 32));
            trim();
        }
    }

    void MPN::set_byte(size_t i, uint8_t b) {
        if (i >= 4 * rep_.size())
            rep_.resize(i / 4 + 1, 0);
        rep_[i / 4] |= uint32_t(b) << (i % 4 * 8);
        trim();
    }

    void MPN::flip_bit(size_t i) {
        if (i >= 32 * rep_.size())
            rep_.resize(i / 32 + 1, 0);
        rep_[i / 32] ^= uint32_t(1) << (i % 32);
        trim();
    }

    size_t MPN::hash() const noexcept {
        return hash_mix(rep_);
    }

    MPN MPN::pow(const MPN& n) const {
        MPN x = *this, y = n, z = 1;
        while (y) {
            if (y.is_odd())
                z *= x;
            x *= x;
            y >>= 1;
        }
        return z;
    }

    std::string MPN::str(FormatSpec spec) const {

        if (spec.lcmode() == 'r') {
            if (*this < 1)
                throw std::invalid_argument("No Roman numeral for " + str());
            return roman(*this, spec.mode() == 'r');
        }

        if (spec.lcmode() >= 'd' && spec.lcmode() <= 'g')
            return format_floating_point(static_cast<long double>(*this), spec);

        MPN base;

        switch (spec.lcmode()) {
            case 'b':  base = 2; break;
            case 'x':  base = 16; break;
            default:   base = 10; break;
        }

        spec.default_prec(1);
        auto xdigits = spec.mode() == 'X' ? Detail::hex_digits_uc : Detail::hex_digits_lc;
        std::string result;

        for (MPN n = *this; n != 0 || result.size() < size_t(spec.prec()); n /= base)
            result += xdigits[unsigned(n % base)];

        std::reverse(result.begin(), result.end());
        Detail::expand_formatted_number(result, spec);

        return result;

    }

    void MPN::write_be(void* ptr, size_t n) const noexcept {
        size_t nb = std::min(n, bytes());
        std::memset(ptr, 0, n - nb);
        auto bp = static_cast<uint8_t*>(ptr) + n - nb, end = bp + nb;
        while (bp != end)
            *bp++ = get_byte(--nb);
    }

    void MPN::write_le(void* ptr, size_t n) const noexcept {
        auto bp = static_cast<uint8_t*>(ptr);
        size_t nb = std::min(n, bytes());
        for (size_t i = 0; i < nb; ++i)
            bp[i] = get_byte(i);
        std::memset(bp + nb, 0, n - nb);
    }

    MPN MPN::from_double(double x) {
        int e = 0;
        double m = frexp(fabs(x), &e);
        MPN n = uint64_t(floor(ldexp(m, 64)));
        n <<= e - 64;
        return n;
    }

    MPN MPN::read_be(const void* ptr, size_t n) {
        MPN result;
        result.rep_.resize((n + 3) / 4);
        auto bp = static_cast<const uint8_t*>(ptr);
        for (size_t i = 0, j = n - 1; i < n; ++i, --j)
            result.set_byte(j, bp[i]);
        result.trim();
        return result;
    }

    MPN MPN::read_le(const void* ptr, size_t n) {
        MPN result;
        result.rep_.resize((n + 3) / 4);
        auto bp = static_cast<const uint8_t*>(ptr);
        for (size_t i = 0; i < n; ++i)
            result.set_byte(i, bp[i]);
        result.trim();
        return result;
    }

    std::strong_ordering MPN::compare(const MPN& rhs) const noexcept {
        using namespace Detail;
        auto c = rep_.size() <=> rhs.rep_.size();
        if (c != 0)
            return c;
        for (size_t i = rep_.size() - 1; i != std::string::npos; --i) {
            c = rep_[i] <=> rhs.rep_[i];
            if (c != 0)
                return c;
        }
        return SO::equal;
    }

    void MPN::init(std::string_view s, int base) {

        if (base < 0 || base == 1 || base > 36)
            throw std::invalid_argument("Invalid base: " + std::to_string(base));

        if (s.empty())
            return;

        auto ptr = s.data(), end = ptr + s.size();

        if (base == 0) {
            if (s[0] != '0' || s.size() < 3)
                base = 10;
            else if (s[1] == 'B' || s[1] == 'b')
                base = 2;
            else if (s[1] == 'X' || s[1] == 'x')
                base = 16;
            else
                base = 10;
            if (base != 10)
                ptr += 2;
        }

        MPN nbase = base;
        int digit = 0;
        int (*get_digit)(char c);

        if (base <= 10)
            get_digit = [] (char c) noexcept { return c >= '0' && c <= '9' ? int(c - '0') : 64; };
        else
            get_digit = [] (char c) noexcept { return c >= '0' && c <= '9' ? int(c - '0') :
                c >= 'A' && c <= 'Z' ? int(c - 'A') + 10 : c >= 'a' && c <= 'z' ? int(c - 'a') + 10 : 64; };

        for (; ptr != end; ++ptr) {
            if (*ptr == '\'')
                continue;
            digit = get_digit(*ptr);
            if (digit >= base)
                throw std::invalid_argument(fmt("Invalid base {0} integer: {1:q}", base, s));
            *this *= nbase;
            *this += digit;
        }

    }

    void MPN::trim() noexcept {
        size_t i = rep_.size() - 1;
        while (i != std::string::npos && rep_[i] == 0)
            --i;
        rep_.resize(i + 1);
    }

    void MPN::do_divide(const MPN& x, const MPN& y, MPN& q, MPN& r) {

        MPN quo, rem = x;

        if (x >= y) {

            size_t shift = x.bits() - y.bits();
            MPN rsub = y;
            rsub <<= shift;

            if (rsub > x) {
                --shift;
                rsub >>= 1;
            }

            MPN qadd = 1;
            qadd <<= shift;

            while (qadd) {
                if (rem >= rsub) {
                    rem -= rsub;
                    quo += qadd;
                }
                rsub >>= 1;
                qadd >>= 1;
            }

        }

        q = std::move(quo);
        r = std::move(rem);

    }

    void MPN::do_multiply(const MPN& x, const MPN& y, MPN& z) {

        if (! x || ! y) {

            z.rep_.clear();

        } else {

            size_t m = x.rep_.size(), n = y.rep_.size();
            z.rep_.assign(m + n, 0);
            uint64_t carry = 0;

            for (size_t k = 0; k <= m + n - 2; ++k) {

                carry += uint64_t(z.rep_[k]);
                z.rep_[k] = uint32_t(carry);
                carry >>= 32;
                size_t i_min = k < n ? 0 : k - n + 1;
                size_t i_max = k < m ? k : m - 1;

                for (size_t i = i_min; i <= i_max; ++i) {
                    uint64_t p = uint64_t(x.rep_[i]) * uint64_t(y.rep_[k - i]) + uint64_t(z.rep_[k]);
                    z.rep_[k] = uint32_t(p);
                    carry += uint32_t(p >> 32);
                }

            }

            z.rep_[m + n - 1] = uint32_t(carry);
            z.trim();

        }

    }

    // Signed integer class

    MPZ& MPZ::operator+=(const MPZ& rhs) {
        using namespace Detail;
        if (! rhs.mag_) {
            // do nothing
        } else if (! mag_) {
            mag_ = rhs.mag_;
            neg_ = rhs.neg_;
        } else if (neg_ == rhs.neg_) {
            mag_ += rhs.mag_;
        } else {
            auto c = mag_ <=> rhs.mag_;
            if (c == SO::less) {
                mag_ = rhs.mag_ - mag_;
                neg_ = ! neg_;
            } else if (c == SO::greater) {
                mag_ -= rhs.mag_;
            } else {
                mag_ = {};
                neg_ = false;
            }
        }
        return *this;
    }

    size_t MPZ::hash() const noexcept {
        return hash_mix(mag_, neg_);
    }

    MPZ MPZ::pow(const MPZ& n) const {
        MPZ z;
        z.mag_ = mag_.pow(n.mag_);
        z.neg_ = neg_ && n.mag_.is_odd();
        return z;
    }

    std::string MPZ::str(FormatSpec spec) const {
        if (spec.lcmode() == 'r') {
            if (*this < 1)
                throw std::invalid_argument("No Roman numeral for " + str());
            return roman(mag_, spec.mode() == 'r');
        }
        if (spec.lcmode() >= 'd' && spec.lcmode() <= 'g')
            return format_floating_point(static_cast<long double>(*this), spec);
        auto result = mag_.str(spec);
        if (sign() == -1)
            result.insert(0, 1, '-');
        return result;
    }

    MPZ MPZ::from_double(double x) {
        MPZ i = MPN::from_double(x);
        if (x < 0)
            i = - i;
        return i;
    }

    std::strong_ordering MPZ::compare(const MPZ& rhs) const noexcept {
        auto c = rhs.neg_ <=> neg_;
        if (c != 0)
            return c;
        else if (neg_)
            return rhs.mag_ <=> mag_;
        else
            return mag_ <=> rhs.mag_;
    }

    void MPZ::init(std::string_view s, int base) {
        if (base < 0 || base == 1 || base > 36)
            throw std::invalid_argument("Invalid base: " + std::to_string(base));
        if (s.empty())
            return;
        auto b = s.data(), e = b + s.size();
        neg_ = *b == '-';
        if (*b == '+' || *b == '-')
            ++b;
        std::string_view u(b, e - b);
        mag_.init(u, base);
        neg_ &= bool(mag_);
    }

    void MPZ::do_divide(const MPZ& x, const MPZ& y, MPZ& q, MPZ& r) {
        MPZ quo, rem;
        MPN::do_divide(x.mag_, y.mag_, quo.mag_, rem.mag_);
        if (rem.mag_ && (x.neg_ || y.neg_)) {
            if (x.neg_) {
                ++quo.mag_;
                rem.mag_ = y.mag_ - rem.mag_;
            }
        }
        quo.neg_ = bool(quo.mag_) && x.neg_ != y.neg_;
        q = std::move(quo);
        r = std::move(rem);
    }

    void MPZ::do_multiply(const MPZ& x, const MPZ& y, MPZ& z) {
        MPN::do_multiply(x.mag_, y.mag_, z.mag_);
        z.neg_ = bool(x) && bool(y) && x.neg_ != y.neg_;
    }

}
