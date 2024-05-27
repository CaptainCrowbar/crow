#pragma once

#include "crow/colour-space.hpp"
#include "crow/enum.hpp"
#include "crow/format.hpp"
#include "crow/maths.hpp"
#include "crow/string.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <compare>
#include <concepts>
#include <limits>
#include <optional>
#include <ostream>
#include <stdexcept>
#include <string>
#include <type_traits>

namespace Crow {

    CROW_ENUM_SCOPED(ColourLayout, int,
        forward,
        forward_alpha,
        alpha_forward,
        reverse,
        reverse_alpha,
        alpha_reverse
    )

    CROW_ENUM_BITMASK(Pma, int,
        none    = 0,
        first   = 1,
        second  = 2,
        result  = 4,
        all     = first | second | result,
    )

    template <ArithmeticType VT, ColourSpace CS, ColourLayout CL> class Colour;

    namespace Detail {

        template <typename CT> struct IsColourType: std::false_type {};
        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL> struct IsColourType<Colour<VT, CS, CL>>: std::true_type {};

        template <ColourSpace CS, char CH, int Offset = 0>
        struct ColourSpaceChannelIndex {
            static constexpr int get() noexcept {
                if constexpr (Offset >= CS::count)
                    return -1;
                else if constexpr (CS::channels[Offset] == CH)
                    return Offset;
                else
                    return ColourSpaceChannelIndex<CS, CH, Offset + 1>::get();
            }
        };

        template <ColourSpace CS, char CH, ColourLayout CL>
        struct ColourChannelIndex {
            static constexpr int cs_index = ColourSpaceChannelIndex<CS, CH>::get();
            static constexpr int get() noexcept {
                if constexpr (cs_index == -1)
                    return -1;
                else if constexpr (CL == ColourLayout::forward || CL == ColourLayout::forward_alpha)
                    return cs_index;
                else if constexpr (CL == ColourLayout::alpha_forward)
                    return cs_index + 1;
                else if constexpr (CL == ColourLayout::reverse || CL == ColourLayout::reverse_alpha)
                    return CS::count - cs_index - 1;
                else
                    return CS::count - cs_index;
            }
        };

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL, bool IsLinear = LinearColourSpace<CS>>
        class ColourArithmetic {

        private:

            static constexpr bool ca_has_alpha = CL != ColourLayout::forward && CL != ColourLayout::reverse;
            static constexpr int ca_channels = CS::count + int(ca_has_alpha);

            using ca_colour = Colour<VT, CS, CL>;
            using ca_vector = Vector<VT, ca_channels>;

        public:

            friend constexpr ca_colour operator+(ca_colour c) noexcept { return c; }
            friend constexpr ca_colour operator-(ca_colour c) noexcept { return ca_colour(- c.as_vector()); }
            friend constexpr ca_colour operator+(ca_colour a, ca_colour b) noexcept { return ca_colour(a.as_vector() + b.as_vector()); }
            friend constexpr ca_colour operator-(ca_colour a, ca_colour b) noexcept { return ca_colour(a.as_vector() - b.as_vector()); }
            friend constexpr ca_colour operator*(ca_colour a, VT b) noexcept { return ca_colour(a.as_vector() * b); }
            friend constexpr ca_colour operator*(VT a, ca_colour b) noexcept { return ca_colour(a * b.as_vector()); }
            friend constexpr ca_colour operator/(ca_colour a, VT b) noexcept { return ca_colour(a.as_vector() / b); }
            friend constexpr ca_colour operator*(ca_colour a, ca_vector b) noexcept { return ca_colour(a.as_vector() * b); }
            friend constexpr ca_colour operator*(ca_vector a, ca_colour b) noexcept { return ca_colour(a * b.as_vector()); }
            friend constexpr ca_colour operator/(ca_colour a, ca_vector b) noexcept { return ca_colour(a.as_vector() / b); }
            friend constexpr ca_colour& operator+=(ca_colour& a, ca_colour b) noexcept { return a = a + b; }
            friend constexpr ca_colour& operator-=(ca_colour& a, ca_colour b) noexcept { return a = a - b; }
            friend constexpr ca_colour& operator*=(ca_colour& a, VT b) noexcept { return a = a * b; }
            friend constexpr ca_colour& operator/=(ca_colour& a, VT b) noexcept { return a = a / b; }
            friend constexpr ca_colour& operator*=(ca_colour& a, ca_vector b) noexcept { return a = a * b; }
            friend constexpr ca_colour& operator/=(ca_colour& a, ca_vector b) noexcept { return a = a / b; }

        };

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        class ColourArithmetic<VT, CS, CL, false> {};

        template <ArithmeticType T>
        using FloatingChannelType =
            std::conditional_t<std::is_floating_point_v<T>, T,
            std::conditional_t<(sizeof(T) < sizeof(float)), float,
            std::conditional_t<(sizeof(T) < sizeof(double)), double, long double>>>;

        template <ArithmeticType T1, ArithmeticType T2>
        using WorkingChannelType =
            std::conditional_t<(sizeof(FloatingChannelType<T1>) >= sizeof(FloatingChannelType<T2>)),
            FloatingChannelType<T1>, FloatingChannelType<T2>>;

        template <ArithmeticType WT, ArithmeticType T1>
        constexpr WT channel_to_working_type(T1 c, T1 scale) noexcept {
            return WT(c) / WT(scale);
        }

        template <ArithmeticType T2, ArithmeticType WT>
        constexpr T2 working_type_to_channel(WT w, T2 scale) noexcept {
            w *= WT(scale);
            if constexpr (std::is_integral_v<T2>)
                return const_round<T2>(w);
            else
                return T2(w);
        }

        template <ArithmeticType T2, ArithmeticType T1>
        constexpr T2 round_channel(T1 t) noexcept {
            if constexpr (std::is_integral_v<T2>)
                return const_round<T2>(t);
            else
                return T2(t);
        }

    }

    template <typename CT> concept ColourType = Detail::IsColourType<CT>::value;
    template <typename CT> concept LinearColourType = ColourType<CT> && CT::is_linear;

    // Don't use single letter template parameters here

    template <ArithmeticType VT = float, ColourSpace CS = LinearRGB, ColourLayout CL = ColourLayout::forward_alpha>
    class Colour:
    public Detail::ColourArithmetic<VT, CS, CL> {

    public:

        static constexpr int colour_space_channels = CS::count;
        static constexpr int alpha_index = CL == ColourLayout::alpha_forward || CL == ColourLayout::alpha_reverse ? 0 :
            CL == ColourLayout::forward_alpha || CL == ColourLayout::reverse_alpha ? colour_space_channels : -1;
        static constexpr bool has_alpha = alpha_index != -1;
        static constexpr bool can_premultiply = has_alpha && LinearColourSpace<CS> && RgbColourSpace<CS>
            && UnitColourSpace<CS> && ! PolarColourSpace<CS>;
        static constexpr int channels = colour_space_channels + int(has_alpha);
        static constexpr bool is_hdr = std::is_floating_point_v<VT>;
        static constexpr bool is_linear = LinearColourSpace<CS>;
        static constexpr ColourLayout layout = CL;
        static constexpr VT scale = is_hdr || (! PolarColourSpace<CS> && ! UnitColourSpace<CS>) ?
            VT(1) : std::numeric_limits<VT>::max();

    private:

        template <ArithmeticType VT2, char Lit> using if_channel_t =
            std::enable_if<SfinaeTrue<VT2, Detail::ColourChannelIndex<CS, Lit, CL>::get() != -1>::value>;
        template <typename... Args> using if_alpha_args_t =
            std::enable_if_t<sizeof...(Args) + 2 == channels, VT>;
        template <typename... Args> using if_nonalpha_args_t =
            std::enable_if_t<has_alpha && sizeof...(Args) + 2 == colour_space_channels, VT>;
        template <ArithmeticType VT2> using if_rgb_t = std::enable_if<SfinaeTrue<VT2, RgbColourSpace<CS>>::value>;
        template <ArithmeticType VT2> using if_rgba_t = std::enable_if<SfinaeTrue<VT2, RgbColourSpace<CS> && has_alpha>::value>;

    public:

        using colour_space = CS;
        using iterator = VT*;
        using const_iterator = const VT*;
        using value_type = VT;
        using vector_type = Vector<VT, channels>;
        using partial_vector_type = Vector<VT, colour_space_channels>;

        constexpr Colour() noexcept {}
        explicit constexpr Colour(VT x) noexcept: vec_(x) { if constexpr (has_alpha) alpha() = scale; }
        template <ArithmeticType VT2 = VT> constexpr Colour(VT2 x, std::enable_if_t<has_alpha, VT2> a) noexcept: vec_(VT(x)) { alpha() = VT(a); }
        explicit constexpr Colour(vector_type v) noexcept: vec_(v) {}
        template <typename... Args> constexpr Colour(VT x, if_alpha_args_t<Args...> y, Args... args) noexcept;
        template <typename... Args> constexpr Colour(VT x, if_nonalpha_args_t<Args...> y, Args... args) noexcept;
        explicit Colour(const std::string& str);

        #define CROW_COLOUR_CHANNEL_(Ch, Lit) \
            template <ArithmeticType VT2 = VT> \
            constexpr VT& Ch(if_channel_t<VT2, Lit>* = nullptr) noexcept { \
                return vec_[Detail::ColourChannelIndex<CS, Lit, CL>::get()]; \
            } \
            template <ArithmeticType VT2 = VT> \
            constexpr const VT& Ch(if_channel_t<VT2, Lit>* = nullptr) const noexcept { \
                return vec_[Detail::ColourChannelIndex<CS, Lit, CL>::get()]; \
            }

        CROW_COLOUR_CHANNEL_(a, 'a')  CROW_COLOUR_CHANNEL_(A, 'A')
        CROW_COLOUR_CHANNEL_(b, 'b')  CROW_COLOUR_CHANNEL_(B, 'B')
        CROW_COLOUR_CHANNEL_(c, 'c')  CROW_COLOUR_CHANNEL_(C, 'C')
        CROW_COLOUR_CHANNEL_(d, 'd')  CROW_COLOUR_CHANNEL_(D, 'D')
        CROW_COLOUR_CHANNEL_(e, 'e')  CROW_COLOUR_CHANNEL_(E, 'E')
        CROW_COLOUR_CHANNEL_(f, 'f')  CROW_COLOUR_CHANNEL_(F, 'F')
        CROW_COLOUR_CHANNEL_(g, 'g')  CROW_COLOUR_CHANNEL_(G, 'G')
        CROW_COLOUR_CHANNEL_(h, 'h')  CROW_COLOUR_CHANNEL_(H, 'H')
        CROW_COLOUR_CHANNEL_(i, 'i')  CROW_COLOUR_CHANNEL_(I, 'I')
        CROW_COLOUR_CHANNEL_(j, 'j')  CROW_COLOUR_CHANNEL_(J, 'J')
        CROW_COLOUR_CHANNEL_(k, 'k')  CROW_COLOUR_CHANNEL_(K, 'K')
        CROW_COLOUR_CHANNEL_(l, 'l')  CROW_COLOUR_CHANNEL_(L, 'L')
        CROW_COLOUR_CHANNEL_(m, 'm')  CROW_COLOUR_CHANNEL_(M, 'M')
        CROW_COLOUR_CHANNEL_(n, 'n')  CROW_COLOUR_CHANNEL_(N, 'N')
        CROW_COLOUR_CHANNEL_(o, 'o')  CROW_COLOUR_CHANNEL_(O, 'O')
        CROW_COLOUR_CHANNEL_(p, 'p')  CROW_COLOUR_CHANNEL_(P, 'P')
        CROW_COLOUR_CHANNEL_(q, 'q')  CROW_COLOUR_CHANNEL_(Q, 'Q')
        CROW_COLOUR_CHANNEL_(r, 'r')  CROW_COLOUR_CHANNEL_(R, 'R')
        CROW_COLOUR_CHANNEL_(s, 's')  CROW_COLOUR_CHANNEL_(S, 'S')
        CROW_COLOUR_CHANNEL_(t, 't')  CROW_COLOUR_CHANNEL_(T, 'T')
        CROW_COLOUR_CHANNEL_(u, 'u')  CROW_COLOUR_CHANNEL_(U, 'U')
        CROW_COLOUR_CHANNEL_(v, 'v')  CROW_COLOUR_CHANNEL_(V, 'V')
        CROW_COLOUR_CHANNEL_(w, 'w')  CROW_COLOUR_CHANNEL_(W, 'W')
        CROW_COLOUR_CHANNEL_(x, 'x')  CROW_COLOUR_CHANNEL_(X, 'X')
        CROW_COLOUR_CHANNEL_(y, 'y')  CROW_COLOUR_CHANNEL_(Y, 'Y')
        CROW_COLOUR_CHANNEL_(z, 'z')  CROW_COLOUR_CHANNEL_(Z, 'Z')

        VT& operator[](int i) noexcept { return vec_[i]; }
        const VT& operator[](int i) const noexcept { return vec_[i]; }
        template <ArithmeticType VT2 = VT> constexpr VT& alpha(std::enable_if<SfinaeTrue<VT2, has_alpha>::value>* = nullptr) noexcept;
        constexpr const VT& alpha() const noexcept;
        constexpr VT& cs(int i) noexcept { return vec_[space_to_layout_index(i)]; }
        constexpr const VT& cs(int i) const noexcept { return vec_[space_to_layout_index(i)]; }
        constexpr vector_type as_vector() const noexcept { return vec_; }
        constexpr partial_vector_type partial_vector() const noexcept { return partial_vector_type(vec_.begin()); }
        constexpr VT* begin() noexcept { return vec_.begin(); }
        constexpr const VT* begin() const noexcept { return vec_.begin(); }
        constexpr VT* end() noexcept { return vec_.end(); }
        constexpr const VT* end() const noexcept { return vec_.end(); }
        constexpr void clamp() noexcept;
        constexpr Colour clamped() const noexcept;
        constexpr bool is_clamped() const noexcept;
        constexpr bool empty() const noexcept { return false; }
        size_t hash() const noexcept { return vec_.hash(); }
        std::string hex() const;
        template <ArithmeticType VT2 = VT> constexpr Colour
            multiply_alpha(std::enable_if<SfinaeTrue<VT2, can_premultiply>::value>* = nullptr) const noexcept;
        template <ArithmeticType VT2 = VT> constexpr Colour
            unmultiply_alpha(std::enable_if<SfinaeTrue<VT2, can_premultiply>::value>* = nullptr) const noexcept;
        constexpr size_t size() const noexcept { return size_t(channels); }
        std::string str(FormatSpec spec = {}) const { return vec_.str(spec); }
        friend std::ostream& operator<<(std::ostream& out, const Colour& c) { return out << c.str(); }

        template <ArithmeticType VT2 = VT> static Colour clear(if_rgba_t<VT2>* = nullptr) noexcept { return Colour(0, 0); }
        template <ArithmeticType VT2 = VT> static Colour black(if_rgb_t<VT2>* = nullptr) noexcept { return Colour(0); }
        template <ArithmeticType VT2 = VT> static Colour white(if_rgb_t<VT2>* = nullptr) noexcept { return Colour(scale); }
        template <ArithmeticType VT2 = VT> static Colour red(if_rgb_t<VT2>* = nullptr) noexcept { return {scale, 0, 0}; }
        template <ArithmeticType VT2 = VT> static Colour yellow(if_rgb_t<VT2>* = nullptr) noexcept { return {scale, scale, 0}; }
        template <ArithmeticType VT2 = VT> static Colour green(if_rgb_t<VT2>* = nullptr) noexcept { return {0, scale, 0}; }
        template <ArithmeticType VT2 = VT> static Colour cyan(if_rgb_t<VT2>* = nullptr) noexcept { return {0, scale, scale}; }
        template <ArithmeticType VT2 = VT> static Colour blue(if_rgb_t<VT2>* = nullptr) noexcept { return {0, 0, scale}; }
        template <ArithmeticType VT2 = VT> static Colour magenta(if_rgb_t<VT2>* = nullptr) noexcept { return {scale, 0, scale}; }

        friend constexpr bool operator==(Colour a, Colour b) noexcept { return a.vec_ == b.vec_; }

    private:

        friend class Detail::ColourArithmetic<VT, CS, CL>;

        vector_type vec_;

        static constexpr int space_to_layout_index(int cs) noexcept;

    };

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        template <typename... Args>
        constexpr Colour<VT, CS, CL>::Colour(VT x, if_alpha_args_t<Args...> y, Args... args) noexcept:
        vec_(VT(x), VT(y), VT(args)...) {}

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        template <typename... Args>
        constexpr Colour<VT, CS, CL>::Colour(VT x, if_nonalpha_args_t<Args...> y, Args... args) noexcept:
        vec_(VT(x), VT(y), VT(args)..., scale) {}

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        Colour<VT, CS, CL>::Colour(const std::string& str) {

            static_assert(RgbColourSpace<CS>);

            Byte4 bytes = {0,0,0,255};

            size_t i, j, k;
            for (i = 0; i < str.size() && (ascii_ispunct(str[i]) || ascii_isspace(str[i])); ++i) {}
            for (j = i; j < str.size() && ascii_isxdigit(str[j]); ++j) {}
            for (k = j; k < str.size() && (ascii_ispunct(str[k]) || ascii_isspace(str[k])); ++k) {}
            size_t digits = j - i;

            if (k != str.size() || (digits != 6 && digits != 8))
                throw std::invalid_argument("Invalid colour: " + quote(str));

            bytes[0] = to_uint8(str.substr(i, 2), 16);
            bytes[1] = to_uint8(str.substr(i + 2, 2), 16);
            bytes[2] = to_uint8(str.substr(i + 4, 2), 16);

            if (digits == 8)
                bytes[3] = to_uint8(str.substr(i + 6, 2), 16);

            Vector<VT, 4> vts;

            if constexpr (std::is_same_v<VT, uint8_t>) {

                vts = bytes;

            } else if constexpr (std::is_integral_v<VT> && std::is_signed_v<VT>) {

                static constexpr double s = double(scale) / 255;
                for (int x = 0; x < channels; ++x)
                    vts[x] = const_round<VT>(s * double(bytes[x]));

            } else {

                static constexpr VT s = scale / 255;
                for (int x = 0; x < channels; ++x)
                    vts[x] = s * VT(bytes[x]);

            }

            R() = vts[0];
            G() = vts[1];
            B() = vts[2];

            if constexpr (has_alpha)
                alpha() = vts[3];

        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        template <ArithmeticType VT2>
        constexpr VT& Colour<VT, CS, CL>::alpha(std::enable_if<SfinaeTrue<VT2, has_alpha>::value>*) noexcept {
            return vec_[alpha_index];
        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        constexpr const VT& Colour<VT, CS, CL>::alpha() const noexcept {
            if constexpr (has_alpha)
                return vec_[alpha_index];
            else
                return scale;
        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        constexpr void Colour<VT, CS, CL>::clamp() noexcept {

            if constexpr (has_alpha) {

                auto pv = partial_vector();
                clamp_colour<CS>(pv, scale);
                int offset = int(CL == ColourLayout::alpha_forward || CL == ColourLayout::alpha_reverse);
                for (int i = 0; i < colour_space_channels; ++i)
                    vec_[i + offset] = pv[i];
                if (alpha() < 0)
                    alpha() = 0;
                else if (alpha() > scale)
                    alpha() = scale;

            } else {

                clamp_colour<CS>(vec_, scale);

            }

        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        constexpr Colour<VT, CS, CL> Colour<VT, CS, CL>::clamped() const noexcept {
            auto c = *this;
            c.clamp();
            return c;
        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        constexpr bool Colour<VT, CS, CL>::is_clamped() const noexcept {
            if (! is_colour_in_gamut<CS>(partial_vector(), scale))
                return false;
            if constexpr (has_alpha)
                if (alpha() < 0 || alpha() > scale)
                    return false;
            return true;
        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        std::string Colour<VT, CS, CL>::hex() const {
            static_assert(RgbColourSpace<CS>);
            using namespace Literals;
            static const auto form = has_alpha ? "{0:x2}{1:x2}{2:x2}{3:x2}"_fmt : "{0:x2}{1:x2}{2:x2}"_fmt;
            Vector<VT, 4> vts = {R(), G(), B(), alpha()};
            Byte4 bytes;
            if constexpr (std::is_same_v<VT, uint8_t>) {
                bytes = vts;
            } else {
                static constexpr double k = 255 / double(scale);
                for (int i = 0; i < channels; ++i)
                    bytes[i] = const_round<uint8_t>(k * double(vts[i]));
            }
            return form(bytes[0], bytes[1], bytes[2], bytes[3]);
        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        template <ArithmeticType VT2>
        constexpr Colour<VT, CS, CL> Colour<VT, CS, CL>::multiply_alpha(std::enable_if<SfinaeTrue<VT2,
                can_premultiply>::value>*) const noexcept {
            using FT = Detail::FloatingChannelType<VT>;
            auto result = *this;
            FT factor = FT(alpha()) / FT(scale);
            for (auto& x: result.vec_)
                x = Detail::round_channel<VT>(factor * x);
            result.alpha() = alpha();
            return result;
        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        template <ArithmeticType VT2>
        constexpr Colour<VT, CS, CL> Colour<VT, CS, CL>::unmultiply_alpha(std::enable_if<SfinaeTrue<VT2,
                can_premultiply>::value>*) const noexcept {
            using FT = Detail::FloatingChannelType<VT>;
            auto result = *this;
            FT factor = FT(scale) / FT(alpha());
            for (auto& x: result.vec_)
                x = Detail::round_channel<VT>(factor * x);
            result.alpha() = alpha();
            return result;
        }

        template <ArithmeticType VT, ColourSpace CS, ColourLayout CL>
        constexpr int Colour<VT, CS, CL>::space_to_layout_index(int cs) noexcept {
            if constexpr (CL <= ColourLayout::forward_alpha)
                return cs;
            else if constexpr (CL == ColourLayout::alpha_forward)
                return (cs + 1) % channels;
            else if constexpr (CL == ColourLayout::reverse_alpha)
                return (2 * colour_space_channels - cs) % channels;
            else
                return channels - cs - 1;
        }

    template <ColourType CT1, ColourType CT2>
    void convert_colour(CT1 in, CT2& out) noexcept {

        using VT1 = typename CT1::value_type;
        using VT2 = typename CT2::value_type;
        using CS1 = typename CT1::colour_space;
        using CS2 = typename CT2::colour_space;

        static constexpr auto CL1 = CT1::layout;
        static constexpr auto CL2 = CT2::layout;

        if constexpr (std::is_same_v<VT1, VT2> && std::is_same_v<CS1, CS2> && CL1 == CL2) {

            out = in;

        } else if constexpr (std::is_same_v<VT1, VT2> && std::is_same_v<CS1, CS2>) {

            for (int i = 0; i < CS1::count; ++i)
                out.cs(i) = in.cs(i);
            if constexpr (CT2::has_alpha)
                out.alpha() = in.alpha();

        } else {

            using WT = Detail::WorkingChannelType<VT1, VT2>;
            using WC1 = Colour<WT, CS1, ColourLayout::forward_alpha>;
            using WC2 = Colour<WT, CS2, ColourLayout::forward_alpha>;

            // WT is always floating point, so the scale of WC1/2 is always 1

            WC1 wc1;
            for (int i = 0; i < CS1::count; ++i)
                wc1.cs(i) = Detail::channel_to_working_type<WT>(in.cs(i), CT1::scale);
            wc1.alpha() = Detail::channel_to_working_type<WT>(in.alpha(), CT1::scale);

            auto pvec2 = convert_colour_space<CS1, CS2>(wc1.partial_vector());

            WC2 wc2;
            for (int i = 0; i < CS2::count; ++i)
                wc2[i] = pvec2[i];
            wc2.alpha() = wc1.alpha();

            for (int i = 0; i < CT2::channels; ++i)
                out.cs(i) = Detail::working_type_to_channel(wc2.cs(i), CT2::scale);

        }

    }

    template <ColourType CT>
    constexpr CT alpha_blend(CT a, CT b,
            std::enable_if_t<SfinaeTrue<typename CT::value_type, CT::can_premultiply>::value, Pma> flags = {}) noexcept {

        constexpr auto CL = CT::layout;

        using VT = typename CT::value_type;
        using CS = typename CT::colour_space;
        using FT = Detail::FloatingChannelType<VT>;
        using FC = Colour<FT, CS, CL>;

        FC fa, fb, fc;

        convert_colour(a, fa);
        convert_colour(b, fb);

        if (! has_bit(flags, Pma::first))
            fa = fa.multiply_alpha();

        if (! has_bit(flags, Pma::second))
            fb = fb.multiply_alpha();

        for (int i = 0; i < CT::channels; ++i)
            fc[i] = fa[i] + fb[i] * (1 - fa.alpha());

        if (! has_bit(flags, Pma::result))
            fc = fc.unmultiply_alpha();

        CT c;
        convert_colour(fc, c);

        return c;

    }

    template <ColourType CT, std::floating_point FP>
    constexpr CT lerp(const CT& c1, const CT& c2, FP x) noexcept {
        auto v1 = c1.as_vector();
        auto v2 = c2.as_vector();
        auto v3 = lerp(v1, v2, x);
        return CT(v3);
    }

    using Rgb8 = Colour<uint8_t, LinearRGB, ColourLayout::forward>;
    using Rgb16 = Colour<uint16_t, LinearRGB, ColourLayout::forward>;
    using Rgbf = Colour<float, LinearRGB, ColourLayout::forward>;
    using Rgbd = Colour<double, LinearRGB, ColourLayout::forward>;
    using sRgb8 = Colour<uint8_t, sRGB, ColourLayout::forward>;
    using sRgb16 = Colour<uint16_t, sRGB, ColourLayout::forward>;
    using sRgbf = Colour<float, sRGB, ColourLayout::forward>;
    using sRgbd = Colour<double, sRGB, ColourLayout::forward>;
    using Rgba8 = Colour<uint8_t, LinearRGB, ColourLayout::forward_alpha>;
    using Rgba16 = Colour<uint16_t, LinearRGB, ColourLayout::forward_alpha>;
    using Rgbaf = Colour<float, LinearRGB, ColourLayout::forward_alpha>;
    using Rgbad = Colour<double, LinearRGB, ColourLayout::forward_alpha>;
    using sRgba8 = Colour<uint8_t, sRGB, ColourLayout::forward_alpha>;
    using sRgba16 = Colour<uint16_t, sRGB, ColourLayout::forward_alpha>;
    using sRgbaf = Colour<float, sRGB, ColourLayout::forward_alpha>;
    using sRgbad = Colour<double, sRGB, ColourLayout::forward_alpha>;

    static_assert(std::is_standard_layout_v<Rgb8>);
    static_assert(std::is_standard_layout_v<Rgb16>);
    static_assert(std::is_standard_layout_v<Rgbf>);
    static_assert(std::is_standard_layout_v<Rgbd>);
    static_assert(std::is_standard_layout_v<sRgb8>);
    static_assert(std::is_standard_layout_v<sRgb16>);
    static_assert(std::is_standard_layout_v<sRgbf>);
    static_assert(std::is_standard_layout_v<sRgbd>);
    static_assert(std::is_standard_layout_v<Rgba8>);
    static_assert(std::is_standard_layout_v<Rgba16>);
    static_assert(std::is_standard_layout_v<Rgbaf>);
    static_assert(std::is_standard_layout_v<Rgbad>);
    static_assert(std::is_standard_layout_v<sRgba8>);
    static_assert(std::is_standard_layout_v<sRgba16>);
    static_assert(std::is_standard_layout_v<sRgbaf>);
    static_assert(std::is_standard_layout_v<sRgbad>);

    static_assert(sizeof(Rgb8) == 3);
    static_assert(sizeof(Rgb16) == 6);
    static_assert(sizeof(Rgbf) == 12);
    static_assert(sizeof(Rgbd) == 24);
    static_assert(sizeof(sRgb8) == 3);
    static_assert(sizeof(sRgb16) == 6);
    static_assert(sizeof(sRgbf) == 12);
    static_assert(sizeof(sRgbd) == 24);
    static_assert(sizeof(Rgba8) == 4);
    static_assert(sizeof(Rgba16) == 8);
    static_assert(sizeof(Rgbaf) == 16);
    static_assert(sizeof(Rgbad) == 32);
    static_assert(sizeof(sRgba8) == 4);
    static_assert(sizeof(sRgba16) == 8);
    static_assert(sizeof(sRgbaf) == 16);
    static_assert(sizeof(sRgbad) == 32);

    namespace Detail {

        std::optional<sRgba8> get_css_colour(const std::string& str);

        template <ColourType CT>
        struct GetCssColour {
            CT operator()(const std::string& str) const {
                sRgba8 srgb;
                auto opt_srgb = get_css_colour(str);
                if (opt_srgb)
                    srgb = *opt_srgb;
                else
                    srgb = sRgba8(str);
                CT colour;
                convert_colour(srgb, colour);
                return colour;
            }
        };

    }

    template <ColourType CT>
    CT css_colour(const std::string& str) {
        return Detail::GetCssColour<CT>()(str);
    }

}

CROW_STD_HASH_3(Colour, Crow::ArithmeticType, Crow::ColourSpace, Crow::ColourLayout)
