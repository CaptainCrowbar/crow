#pragma once

#include "crow/constants.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <array>
#include <cmath>
#include <numbers>
#include <type_traits>

namespace Crow {

    namespace Detail {

        template <typename T> constexpr T period_factor = CC::G_v<T> / (4 * SN::pi_v<T> * SN::pi_v<T>);
        template <typename T> constexpr T luminosity_factor = 4 * SN::pi_v<T> * CC::sigma_v<T>;
        template <typename T> constexpr T sphere_volume_factor = SN::pi_v<T> * 4 / 3;

    }

    // Surface area and volume of a sphere

    template <typename T>
    constexpr T sphere_area_from_radius(T radius) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using std::numbers::pi_v;
        return 4 * pi_v<T> * radius * radius;
    }

    template <typename T>
    T sphere_radius_from_area(T area) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using std::numbers::pi_v;
        return std::sqrt(area / (4 * pi_v<T>));
    }

    template <typename T>
    T sphere_volume_from_radius(T radius) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return Detail::sphere_volume_factor<T> * std::pow(radius, T(3));
    }

    template <typename T>
    T sphere_radius_from_volume(T volume) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::cbrt(volume / Detail::sphere_volume_factor<T>);
    }

    // Mass and density

    template <typename T>
    T mass_from_radius_density(T radius, T density) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return density * sphere_volume_from_radius(radius);
    }

    template <typename T>
    T radius_from_mass_density(T mass, T density) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return sphere_radius_from_volume(mass / density);
    }

    template <typename T>
    T density_from_mass_radius(T mass, T radius) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return mass / sphere_volume_from_radius(radius);
    }

    // Surface gravity

    template <typename T>
    constexpr T gravity_from_mass_radius(T mass, T radius) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using namespace Constants;
        return G_v<T> * mass / (radius * radius);
    }

    // Orbital period

    template <typename T>
    T distance_from_mass_period(T mass, T period) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::cbrt(Detail::period_factor<T> * period * period * mass);
    }

    template <typename T>
    T period_from_mass_distance(T mass, T distance) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::sqrt(std::pow(distance, T(3)) / (Detail::period_factor<T> * mass));
    }

    // Hill sphere

    template <typename T>
    T hill_radius(T primary_mass, T secondary_mass, T distance) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return distance * std::cbrt(secondary_mass / (3 * primary_mass));
    }

    // Stellar luminosity

    template <typename T>
    T luminosity_from_radius_temperature(T radius, T temperature) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return radius * radius * std::pow(temperature, T(4)) * Detail::luminosity_factor<T>;
    }

    template <typename T>
    T radius_from_luminosity_temperature(T luminosity, T temperature) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::sqrt(luminosity / (Detail::luminosity_factor<T> * std::pow(temperature, T(4))));
    }

    template <typename T>
    T temperature_from_luminosity_radius(T luminosity, T radius) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::pow(luminosity / (radius * radius * Detail::luminosity_factor<T>), T(0.25));
    }

    // Main sequence lifetime

    template <typename T>
    constexpr T main_sequence_lifetime(T mass, T luminosity) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using namespace Constants;
        return (T(1e10) * jy_v<T> * L_sun_v<T> / M_sun_v<T>) * (mass / luminosity);
    }

    // Schwarzschild radius

    template <typename T>
    constexpr T schwarzschild_radius(T mass) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using namespace Constants;
        return 2 * G_v<T> * mass / (c_v<T> * c_v<T>);
    }

    // Magnitude and luminosity

    template <typename T>
    T delta_magnitude_to_ratio(T delta_mag) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::pow(T(10), - T(0.4) * delta_mag);
    }

    template <typename T>
    T ratio_to_delta_magnitude(T ratio) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return - T(2.5) * std::log10(ratio);
    }

    template <typename T>
    T bolometric_magitude_to_luminosity(T mag) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using namespace Constants;
        return L_sun_v<T> * std::pow(T(10), T(0.4) * (Mbol_sun_v<T> - mag));
    }

    template <typename T>
    T bolometric_luminosity_to_magnitude(T luminosity) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using namespace Constants;
        return Mbol_sun_v<T> - T(2.5) * std::log10(luminosity / L_sun_v<T>);
    }

    // Bolometric correction

    template <typename T>
    T temperature_to_bc(T temperature) noexcept {

        static_assert(std::is_floating_point_v<T>);

        static constexpr std::array coeffs1 {
            -19'053.729'149'645'6,
            +15'514.486'676'441'2,
             -4'212.788'193'017'17,
               +381.476'328'422'343,
        };

        static constexpr std::array coeffs2 {
            -37'051.020'380'901'5,
            +38'567.262'996'580'4,
            -15'065.148'631'602'5,
             +2'617.246'371'194'16,
               -170.623'810'323'864,
        };

        static constexpr std::array coeffs3 {
            -118'115.450'538'963,
            +137'145.973'583'929,
             -63'623.381'210'022'5,
             +14'741.292'356'264'6,
              -1'705.872'784'068'72,
                 +78.873'172'180'499'0,
        };

        T log_t = std::log10(temperature);
        Irange<const T*> range;

        if (log_t <= 3.7)
            range = {coeffs1.data(), coeffs1.data() + coeffs1.size()};
        else if (log_t <= 3.9)
            range = {coeffs2.data(), coeffs2.data() + coeffs2.size()};
        else
            range = {coeffs3.data(), coeffs3.data() + coeffs3.size()};

        T bc = 0;
        T x = 1;

        for (T t: range) {
            bc += t * x;
            x *= log_t;
        }

        return bc;

    }

    // Planet temperature and illumination

    template <typename T>
    T planet_distance_to_temperature(T luminosity, T distance) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::pow(luminosity / (distance * distance * Detail::luminosity_factor<T> * 4), T(0.25));
    }

    template <typename T>
    T planet_temperature_to_distance(T luminosity, T temperature) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return std::sqrt(luminosity / (4 * Detail::luminosity_factor<T> * std::pow(temperature, T(4))));
    }

    template <typename T>
    T planet_distance_to_illumination(T luminosity, T distance) noexcept {
        static_assert(std::is_floating_point_v<T>);
        return luminosity / sphere_area_from_radius(distance);
    }

    template <typename T>
    T planet_illumination_to_temperature(T illumination) noexcept {
        static_assert(std::is_floating_point_v<T>);
        using namespace Constants;
        return std::pow(illumination / (4 * sigma_v<T>), T(0.25));
    }

}
