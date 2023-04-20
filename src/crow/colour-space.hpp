#pragma once

#include "crow/binary.hpp"
#include "crow/enum.hpp"
#include "crow/maths.hpp"
#include "crow/matrix.hpp"
#include "crow/transform.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <concepts>
#include <numbers>
#include <type_traits>

namespace Crow {

    namespace Detail {

        template <ArithmeticType T> constexpr Vector<T, 3> D65 = {T(0.950489), T(1), T(1.088840)};

        // Thanks to Randomnetcat and Léo on the #include<C++> Discord for
        // help designing this concept.

        template <typename CS, typename... Seen>
        constexpr bool colour_space_graph_helper() noexcept {
            if constexpr (! requires { typename CS::base; })
                return false;
            else if constexpr (std::is_same_v<CS, typename CS::base>)
                return true;
            else if constexpr ((std::is_same_v<CS, Seen> || ...))
                return false;
            else
                return colour_space_graph_helper<typename CS::base, CS, Seen...>();
        }

        template <typename CS> concept ColourSpaceGraphHelper = colour_space_graph_helper<CS>();

    }

    // Colour space properties

    enum class Csp: int {
        none    = 0,
        linear  = 1,  // Linear colour space
        polar   = 2,  // First channel is polar, not Cartesian
        rgb     = 4,  // Cartesian RGB-based colour space
        unit    = 8,  // Valid colours are restricted to the unit cube
    };

    CROW_BITMASK_OPERATORS(Csp)

    // Colour space concepts

    template <typename CS>
    concept ColourSpace = requires {
        requires Detail::ColourSpaceGraphHelper<CS>;
        { CS::count } -> std::convertible_to<int>;
        { CS::channels } -> std::convertible_to<std::array<char, CS::count>>;
        { CS::properties } -> std::convertible_to<Csp>;
        { CS::from_base(Vector<float, CS::base::count>()) } -> std::convertible_to<Vector<float, CS::count>>;
        { CS::to_base(Vector<float, CS::count>()) } -> std::convertible_to<Vector<float, CS::base::count>>;
    };

    template <typename CS> concept LinearColourSpace = ColourSpace<CS> && has_bit(CS::properties, Csp::linear);
    template <typename CS> concept PolarColourSpace = ColourSpace<CS> && has_bit(CS::properties, Csp::polar);
    template <typename CS> concept RgbColourSpace = ColourSpace<CS> && has_bit(CS::properties, Csp::rgb);
    template <typename CS> concept UnitColourSpace = ColourSpace<CS> && has_bit(CS::properties, Csp::unit);

    // Utility functions

    template <ColourSpace CS, ArithmeticType T>
    constexpr bool is_colour_in_gamut(Vector<T, CS::count> colour, T scale = 1) noexcept {
        if constexpr (UnitColourSpace<CS>) {
            for (auto t: colour)
                if (t < 0 || t > scale)
                    return false;
        } else if constexpr (PolarColourSpace<CS>) {
            if (colour[0] < 0 || colour[0] > scale)
                return false;
        }
        return true;
    }

    template <ColourSpace CS, ArithmeticType T>
    constexpr void clamp_colour(Vector<T, CS::count>& colour, T scale = 1) noexcept {
        if constexpr (PolarColourSpace<CS>)
            colour[0] = euclidean_remainder(colour[0], scale);
        if constexpr (UnitColourSpace<CS>)
            for (int i = int(PolarColourSpace<CS>); i < CS::count; ++i)
                colour[i] = std::clamp(colour[i], T(0), scale);
    }

    // Colour space classes

    class CIEXYZ {
    public:
        using base = CIEXYZ;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'X', 'Y', 'Z'};
        static constexpr Csp properties = Csp::linear | Csp::unit;
        template <ArithmeticType T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept { return colour; }
        template <ArithmeticType T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept { return colour; }
    };

    class CIExyY {
    public:
        using base = CIEXYZ;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'x', 'y', 'Y'};
        static constexpr Csp properties = Csp::unit;
        template <ArithmeticType T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <ArithmeticType T>
        constexpr Vector<T, 3> CIExyY::from_base(Vector<T, 3> colour) noexcept {
            auto out = Vector<T, 3>::null();
            T sum = colour.x() + colour.y() + colour.z();
            if (sum != 0) {
                out.x() = colour.x() / sum;
                out.y() = colour.y() / sum;
                out.z() = colour.y();
            }
            return out;
        }

        template <ArithmeticType T>
        constexpr Vector<T, 3> CIExyY::to_base(Vector<T, 3> colour) noexcept {
            Vector<T, 3> out;
            T scale = colour.z() / colour.y();
            out.x() = scale * colour.x();
            out.y() = colour.z();
            out.z() = scale * (1 - colour.x() - colour.y());
            return out;
        }

    class CIELab {
    public:
        using base = CIEXYZ;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'L', 'a', 'b'};
        static constexpr Csp properties = {};
        template <ArithmeticType T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    private:
        template <ArithmeticType T> static constexpr T delta = T(6) / T(29);
        template <ArithmeticType T> static constexpr T c1 = delta<T> * delta<T> * delta<T>;
        template <ArithmeticType T> static constexpr T c2 = 3 * delta<T> * delta<T>;
        template <ArithmeticType T> static constexpr T c3 = T(4) / T(29);
        template <ArithmeticType T> static T f(T t) noexcept;
        template <ArithmeticType T> static T inverse_f(T t) noexcept;
    };

        template <ArithmeticType T>
        Vector<T, 3> CIELab::from_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            Vector<T, 3> out;
            colour /= D65<T>;
            out[0] = 116 * f(colour.y()) - 16;
            out[1] = 500 * (f(colour.x()) - f(colour.y()));
            out[2] = 200 * (f(colour.y()) - f(colour.z()));
            return out;
        }

        template <ArithmeticType T>
        Vector<T, 3> CIELab::to_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            Vector<T, 3> out;
            T lx = (colour[0] + 16) / 116;
            out.x() = inverse_f(lx + colour[1] / 500);
            out.y() = inverse_f(lx);
            out.z() = inverse_f(lx - colour[2] / 200);
            return out * D65<T>;
        }

        template <ArithmeticType T>
        T CIELab::f(T t) noexcept {
            if (t <= c1<T>)
                return t / c2<T> + c3<T>;
            else
                return std::cbrt(t);
        }

        template <ArithmeticType T>
        T CIELab::inverse_f(T t) noexcept {
            if (t <= delta<T>)
                return c2<T> * (t - c3<T>);
            else
                return t * t * t;
        }

    class CIELuv {
    public:
        using base = CIEXYZ;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'L', 'u', 'v'};
        static constexpr Csp properties = {};
        template <ArithmeticType T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    private:
        template <ArithmeticType T> static constexpr T delta = T(6) / T(29);
        template <ArithmeticType T> static constexpr T c1 = delta<T> * delta<T> * delta<T>;
        template <ArithmeticType T> static constexpr T c2 = T(29) / T(3);
        template <ArithmeticType T> static constexpr T c3 = c2<T> * c2<T> * c2<T>;
        template <ArithmeticType T> static constexpr T c4 = 1 / c3<T>;
        template <ArithmeticType T> static constexpr T u_prime(Vector<T, 3> xyz) noexcept;
        template <ArithmeticType T> static constexpr T v_prime(Vector<T, 3> xyz) noexcept;
        template <ArithmeticType T> static constexpr T u_prime_n = u_prime(Detail::D65<T>);
        template <ArithmeticType T> static constexpr T v_prime_n = v_prime(Detail::D65<T>);
    };

        template <ArithmeticType T>
        Vector<T, 3> CIELuv::from_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            Vector<T, 3> out;
            T y = colour.y() / D65<T>.y();
            if (y <= c1<T>)
                out[0] = c3<T> * y;
            else
                out[0] = 116 * std::cbrt(y) - 16;
            out[1] = 13 * out[0] * (u_prime(colour) - u_prime_n<T>);
            out[2] = 13 * out[0] * (v_prime(colour) - v_prime_n<T>);
            return out;
        }

        template <ArithmeticType T>
        Vector<T, 3> CIELuv::to_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            if (colour[0] == 0)
                return {0,0,0};
            Vector<T, 3> out;
            T u = colour[1] / (13 * colour[0]) + u_prime_n<T>;
            T v = colour[2] / (13 * colour[0]) + v_prime_n<T>;
            if (colour[0] <= 8)
                out[1] = c4<T> * colour[0];
            else
                out[1] = std::pow((colour[0] + 16) / 116, T(3));
            out[1] *= D65<T>.y();
            out[0] = out[1] * 9 * u / (4 * v);
            out[2] = out[1] * (12 - 3 * u - 20 * v) / (4 * v);
            return out;
        }

        template <ArithmeticType T>
        constexpr T CIELuv::u_prime(Vector<T, 3> xyz) noexcept {
            return 4 * xyz.x() / (xyz.x() + 15 * xyz.y() + 3 * xyz.z());
        }

        template <ArithmeticType T>
        constexpr T CIELuv::v_prime(Vector<T, 3> xyz) noexcept {
            return 9 * xyz.y() / (xyz.x() + 15 * xyz.y() + 3 * xyz.z());
        }

    template <typename Base>
    class HCLSpace {
    public:
        using base = Base;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'H', 'C', 'L'};
        static constexpr Csp properties = Csp::polar;
        template <ArithmeticType T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename Base>
        template <ArithmeticType T>
        Vector<T, 3> HCLSpace<Base>::from_base(Vector<T, 3> colour) noexcept {
            using std::numbers::pi_v;
            Vector<T, 3> abl = {colour[1], colour[2], colour[0]};
            auto chl = cartesian_to_cylindrical(abl);
            auto h = fraction(chl[1] / (2 * pi_v<T>));
            return {h, chl[0], chl[2]};
        }

        template <typename Base>
        template <ArithmeticType T>
        Vector<T, 3> HCLSpace<Base>::to_base(Vector<T, 3> colour) noexcept {
            using std::numbers::pi_v;
            Vector<T, 3> chl = {colour[1], 2 * pi_v<T> * colour[0], colour[2]};
            auto abl = cylindrical_to_cartesian(chl);
            return {abl[2], abl[0], abl[1]};
        }

    using HCLab = HCLSpace<CIELab>;
    using HCLuv = HCLSpace<CIELuv>;

    template <int64_t M00, int64_t M01, int64_t M02,
        int64_t M10, int64_t M11, int64_t M12,
        int64_t M20, int64_t M21, int64_t M22,
        int64_t Divisor>
    class WorkingSpace {
    public:
        using base = CIEXYZ;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'R', 'G', 'B'};
        static constexpr Csp properties = Csp::linear | Csp::rgb | Csp::unit;
        template <ArithmeticType T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept { return xyz_to_rgb_matrix<T> * colour; }
        template <ArithmeticType T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept { return rgb_to_xyz_matrix<T> * colour; }
    private:
        template <ArithmeticType T> static constexpr auto rgb_to_xyz_matrix = Matrix<T, 3, MatrixLayout::row>
            (T(M00), T(M01), T(M02), T(M10), T(M11), T(M12), T(M20), T(M21), T(M22)) / T(Divisor);
        template <ArithmeticType T> static constexpr auto xyz_to_rgb_matrix = rgb_to_xyz_matrix<T>.inverse();
    };

    template <typename WorkingSpace, int64_t GammaNumerator, int64_t GammaDenominator>
    class NonlinearSpace {
    public:
        using base = WorkingSpace;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'R', 'G', 'B'};
        static constexpr Csp properties = Csp::rgb | Csp::unit;
        template <ArithmeticType T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <typename WS, int64_t GN, int64_t GD>
        template <ArithmeticType T>
        Vector<T, 3> NonlinearSpace<WS, GN, GD>::from_base(Vector<T, 3> colour) noexcept {
            static constexpr T inverse_gamma = T(GD) / T(GN);
            for (auto& c: colour)
                c = std::pow(std::max(c, T(0)), inverse_gamma);
            return colour;
        }

        template <typename WS, int64_t GN, int64_t GD>
        template <ArithmeticType T>
        Vector<T, 3> NonlinearSpace<WS, GN, GD>::to_base(Vector<T, 3> colour) noexcept {
            static constexpr T gamma = T(GN) / T(GD);
            for (auto& c: colour)
                c = std::pow(std::max(c, T(0)), gamma);
            return colour;
        }

    using LinearAdobeRGB = WorkingSpace<
         5'767'309,  1'855'540,  1'881'852,
         2'973'769,  6'273'491,    752'741,
           270'343,    706'872,  9'911'085,
        10'000'000
    >;

    using AdobeRGB = NonlinearSpace<LinearAdobeRGB, 22, 10>;

    using LinearWideGamut = WorkingSpace<
         7'161'046,  1'009'296,  1'471'858,
         2'581'874,  7'249'378,    168'748,
                 0,    517'813,  7'734'287,
        10'000'000
    >;

    using WideGamut = NonlinearSpace<LinearWideGamut, 563, 256>;

    using LinearRGB = WorkingSpace<
         4'124'564,  3'575'761,  1'804'375,
         2'126'729,  7'151'522,    721'750,
           193'339,  1'191'920,  9'503'041,
        10'000'000
    >;

    namespace Detail {

        template <ArithmeticType T>
        T sRGB_function(T t) noexcept {
            static constexpr T a = T(0.003'130'8);
            static constexpr T b = T(12.92);
            static constexpr T c = T(0.055);
            static constexpr T d = c + 1;
            static constexpr T ig = 1 / T(2.4);
            if (t < a)
                return t * b;
            else
                return d * std::pow(t, ig) - c;
        }

        template <ArithmeticType T>
        T sRGB_inverse(T t) noexcept {
            static constexpr T a = T(0.040'45);
            static constexpr T b = 1 / T(12.92);
            static constexpr T c = T(0.055);
            static constexpr T d = 1 / (c + 1);
            static constexpr T g = T(2.4);
            if (t < a)
                return t * b;
            else
                return std::pow((t + c) * d, g);
        }

    }

    class sRGB {
    public:
        using base = LinearRGB;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'R', 'G', 'B'};
        static constexpr Csp properties = Csp::rgb | Csp::unit;
        template <ArithmeticType T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <ArithmeticType T>
        Vector<T, 3> sRGB::from_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            for (auto& x: colour)
                x = sRGB_function(x);
            return colour;
        }

        template <ArithmeticType T>
        Vector<T, 3> sRGB::to_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            for (auto& x: colour)
                x = sRGB_inverse(x);
            return colour;
        }

    using LinearProPhoto = WorkingSpace<
         7'976'749,  1'351'917,    313'534,
         2'880'402,  7'118'741,        857,
                 0,          0,  8'252'100,
        10'000'000
    >;

    class ProPhoto {
    public:
        using base = LinearProPhoto;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'R', 'G', 'B'};
        static constexpr Csp properties = Csp::rgb | Csp::unit;
        template <ArithmeticType T> static Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <ArithmeticType T>
        Vector<T, 3> ProPhoto::from_base(Vector<T, 3> colour) noexcept {
            static constexpr T et = 1 / T(512);
            static constexpr T ig = 1 / T(1.8);
            for (auto& x: colour) {
                if (x < et)
                    x *= 16;
                else
                    x = std::pow(x, ig);
            }
            return colour;
        }

        template <ArithmeticType T>
        Vector<T, 3> ProPhoto::to_base(Vector<T, 3> colour) noexcept {
            static constexpr T etx = 1 / T(32);
            static constexpr T g = T(1.8);
            for (auto& x: colour) {
                if (x < etx)
                    x /= T(16);
                else
                    x = std::pow(x, g);
            }
            return colour;
        }

    namespace Detail {

        template <ArithmeticType T>
        constexpr void rgb_to_hcv(T r, T g, T b, T& h, T& c, T& v) noexcept {
            v = std::max({r, g, b});
            c = v - std::min({r, g, b});
            if (c == 0)
                h = 0;
            else if (v == r)
                h = (g - b) / c;
            else if (v == g)
                h = (b - r) / c + 2;
            else
                h = (r - g) / c + 4;
            h = euclidean_remainder(h, T(6)) / 6;
        }

        template <ArithmeticType T>
        constexpr void hcm_to_rgb(T h, T c, T m, T& r, T& g, T& b) noexcept {
            h *= 6;
            T x = c * (1 - const_abs(euclidean_remainder(h, T(2)) - 1));
            r = g = b = m;
            switch (int(h)) {
                case 0:   r += c; g += x; break;
                case 1:   r += x; g += c; break;
                case 2:   g += c; b += x; break;
                case 3:   g += x; b += c; break;
                case 4:   r += x; b += c; break;
                default:  r += c; b += x; break;
            }
        }

    }

    class HSL {
    public:
        using base = LinearRGB;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'H', 'S', 'L'};
        static constexpr Csp properties = Csp::polar | Csp::unit;
        template <ArithmeticType T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <ArithmeticType T>
        constexpr Vector<T, 3> HSL::from_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            Vector<T, 3> out;
            T c, v;
            rgb_to_hcv(colour[0], colour[1], colour[2], out[0], c, v);
            out[2] = (T(2) * v - c) / T(2);
            if (c != 0)
                out[1] = c / (T(1) - const_abs(T(2) * out[2] - T(1)));
            return out;
        }

        template <ArithmeticType T>
        constexpr Vector<T, 3> HSL::to_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            Vector<T, 3> out;
            T c = (T(1) - const_abs(T(2) * colour[2] - T(1))) * colour[1];
            T m = colour[2] - c / T(2);
            hcm_to_rgb(colour[0], c, m, out[0], out[1], out[2]);
            return out;
        }

    class HSV {
    public:
        using base = LinearRGB;
        static constexpr int count = 3;
        static constexpr std::array<char, count> channels = {'H', 'S', 'V'};
        static constexpr Csp properties = Csp::polar | Csp::unit;
        template <ArithmeticType T> static constexpr Vector<T, 3> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static constexpr Vector<T, 3> to_base(Vector<T, 3> colour) noexcept;
    };

        template <ArithmeticType T>
        constexpr Vector<T, 3> HSV::from_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            Vector<T, 3> out;
            T c;
            rgb_to_hcv(colour[0], colour[1], colour[2], out[0], c, out[2]);
            if (c != 0)
                out[1] = c / out[2];
            return out;
        }

        template <ArithmeticType T>
        constexpr Vector<T, 3> HSV::to_base(Vector<T, 3> colour) noexcept {
            using namespace Detail;
            Vector<T, 3> out;
            T c = colour[2] * colour[1];
            T m = colour[2] - c;
            hcm_to_rgb(colour[0], c, m, out[0], out[1], out[2]);
            return out;
        }

    class Greyscale {
    public:
        using base = CIEXYZ;
        static constexpr int count = 1;
        static constexpr std::array<char, count> channels = {'Y'};
        static constexpr Csp properties = Csp::linear | Csp::unit;
        template <ArithmeticType T> static constexpr Vector<T, 1> from_base(Vector<T, 3> colour) noexcept;
        template <ArithmeticType T> static constexpr Vector<T, 3> to_base(Vector<T, 1> colour) noexcept;
    };

        template <ArithmeticType T>
        constexpr Vector<T, 1> Greyscale::from_base(Vector<T, 3> colour) noexcept {
            return Vector<T, 1>(colour.y());
        }

        template <ArithmeticType T>
        constexpr Vector<T, 3> Greyscale::to_base(Vector<T, 1> colour) noexcept {
            using namespace Detail;
            return colour[0] * D65<T>;
        }

    class sGreyscale {
    public:
        using base = Greyscale;
        static constexpr int count = 1;
        static constexpr std::array<char, count> channels = {'Y'};
        static constexpr Csp properties = Csp::unit;
        template <ArithmeticType T> static constexpr Vector<T, 1> from_base(Vector<T, 1> colour) noexcept;
        template <ArithmeticType T> static constexpr Vector<T, 1> to_base(Vector<T, 1> colour) noexcept;
    };

        template <ArithmeticType T>
        constexpr Vector<T, 1> sGreyscale::from_base(Vector<T, 1> colour) noexcept {
            using namespace Detail;
            colour[0] = sRGB_function(colour[0]);
            return colour;
        }

        template <ArithmeticType T>
        constexpr Vector<T, 1> sGreyscale::to_base(Vector<T, 1> colour) noexcept {
            using namespace Detail;
            colour[0] = sRGB_inverse(colour[0]);
            return colour;
        }

    // Conversion functions

    template <ColourSpace CS1, ColourSpace CS2, ArithmeticType T>
    Vector<T, CS2::channels.size()> convert_colour_space(Vector<T, CS1::count> colour) {
        using Base1 = typename CS1::base;
        using Base2 = typename CS2::base;
        if constexpr (std::is_same_v<CS1, CS2>)
            return colour;
        else if constexpr (std::is_same_v<CS1, CIEXYZ> && std::is_same_v<CS2, LinearRGB>)
            return CS2::from_base(colour);
        else if constexpr (std::is_same_v<CS1, LinearRGB> && std::is_same_v<CS2, CIEXYZ>)
            return CS1::to_base(colour);
        else if constexpr (! std::is_same_v<CS1, Base1>)
            return convert_colour_space<Base1, CS2>(CS1::to_base(colour));
        else
            return CS2::from_base(convert_colour_space<CS1, Base2>(colour));
    }

}
