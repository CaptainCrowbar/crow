#pragma once

#include "crow/constants.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include <array>
#include <cmath>
#include <concepts>
#include <numbers>

namespace Crow {

    namespace Detail {

        template <std::floating_point T> constexpr T period_factor = CC::G_v<T> / (4 * SN::pi_v<T> * SN::pi_v<T>);
        template <std::floating_point T> constexpr T luminosity_factor = 4 * SN::pi_v<T> * CC::sigma_v<T>;
        template <std::floating_point T> constexpr T sphere_volume_factor = SN::pi_v<T> * 4 / 3;

    }

    // Surface area and volume of a sphere

    template <std::floating_point T>
    constexpr T sphere_area_from_radius(T radius) noexcept {
        using std::numbers::pi_v;
        return 4 * pi_v<T> * radius * radius;
    }

    template <std::floating_point T>
    T sphere_radius_from_area(T area) noexcept {
        using std::numbers::pi_v;
        return std::sqrt(area / (4 * pi_v<T>));
    }

    template <std::floating_point T>
    T sphere_volume_from_radius(T radius) noexcept {
        return Detail::sphere_volume_factor<T> * std::pow(radius, T(3));
    }

    template <std::floating_point T>
    T sphere_radius_from_volume(T volume) noexcept {
        return std::cbrt(volume / Detail::sphere_volume_factor<T>);
    }

    // Mass and density

    template <std::floating_point T>
    T mass_from_radius_density(T radius, T density) noexcept {
        return density * sphere_volume_from_radius(radius);
    }

    template <std::floating_point T>
    T radius_from_mass_density(T mass, T density) noexcept {
        return sphere_radius_from_volume(mass / density);
    }

    template <std::floating_point T>
    T density_from_mass_radius(T mass, T radius) noexcept {
        return mass / sphere_volume_from_radius(radius);
    }

    // Surface gravity

    template <std::floating_point T>
    constexpr T gravity_from_mass_radius(T mass, T radius) noexcept {
        using namespace Constants;
        return G_v<T> * mass / (radius * radius);
    }

    // Orbital period

    template <std::floating_point T>
    T distance_from_mass_period(T mass, T period) noexcept {
        return std::cbrt(Detail::period_factor<T> * period * period * mass);
    }

    template <std::floating_point T>
    T period_from_mass_distance(T mass, T distance) noexcept {
        return std::sqrt(std::pow(distance, T(3)) / (Detail::period_factor<T> * mass));
    }

    // Hill sphere

    template <std::floating_point T>
    T hill_radius(T primary_mass, T secondary_mass, T distance) noexcept {
        return distance * std::cbrt(secondary_mass / (3 * primary_mass));
    }

    // Stellar luminosity

    template <std::floating_point T>
    T luminosity_from_radius_temperature(T radius, T temperature) noexcept {
        return radius * radius * std::pow(temperature, T(4)) * Detail::luminosity_factor<T>;
    }

    template <std::floating_point T>
    T radius_from_luminosity_temperature(T luminosity, T temperature) noexcept {
        return std::sqrt(luminosity / (Detail::luminosity_factor<T> * std::pow(temperature, T(4))));
    }

    template <std::floating_point T>
    T temperature_from_luminosity_radius(T luminosity, T radius) noexcept {
        return std::pow(luminosity / (radius * radius * Detail::luminosity_factor<T>), T(0.25));
    }

    // Main sequence lifetime

    template <std::floating_point T>
    constexpr T main_sequence_lifetime(T mass, T luminosity) noexcept {
        using namespace Constants;
        return (T(1e10) * jy_v<T> * L_sun_v<T> / M_sun_v<T>) * (mass / luminosity);
    }

    // Schwarzschild radius

    template <std::floating_point T>
    constexpr T schwarzschild_radius(T mass) noexcept {
        using namespace Constants;
        return 2 * G_v<T> * mass / (c_v<T> * c_v<T>);
    }

    // Magnitude and luminosity

    template <std::floating_point T>
    T delta_magnitude_to_ratio(T delta_mag) noexcept {
        return std::pow(T(10), - T(0.4) * delta_mag);
    }

    template <std::floating_point T>
    T ratio_to_delta_magnitude(T ratio) noexcept {
        return - T(2.5) * std::log10(ratio);
    }

    template <std::floating_point T>
    T bolometric_magitude_to_luminosity(T mag) noexcept {
        using namespace Constants;
        return L_sun_v<T> * std::pow(T(10), T(0.4) * (Mbol_sun_v<T> - mag));
    }

    template <std::floating_point T>
    T bolometric_luminosity_to_magnitude(T luminosity) noexcept {
        using namespace Constants;
        return Mbol_sun_v<T> - T(2.5) * std::log10(luminosity / L_sun_v<T>);
    }

    // Bolometric correction

    template <std::floating_point T>
    T temperature_to_bc(T temperature) noexcept {

        static constexpr std::array<T, 4> coeffs1 {
            T(-19'053.729'149'645'6),
            T(+15'514.486'676'441'2),
             T(-4'212.788'193'017'17),
               T(+381.476'328'422'343),
        };

        static constexpr std::array<T, 5> coeffs2 {
            T(-37'051.020'380'901'5),
            T(+38'567.262'996'580'4),
            T(-15'065.148'631'602'5),
             T(+2'617.246'371'194'16),
               T(-170.623'810'323'864),
        };

        static constexpr std::array<T, 6> coeffs3 {
            T(-118'115.450'538'963),
            T(+137'145.973'583'929),
             T(-63'623.381'210'022'5),
             T(+14'741.292'356'264'6),
              T(-1'705.872'784'068'72),
                 T(+78.873'172'180'499'0),
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

    template <std::floating_point T>
    T planet_distance_to_temperature(T luminosity, T distance) noexcept {
        return std::pow(luminosity / (distance * distance * Detail::luminosity_factor<T> * 4), T(0.25));
    }

    template <std::floating_point T>
    T planet_temperature_to_distance(T luminosity, T temperature) noexcept {
        return std::sqrt(luminosity / (4 * Detail::luminosity_factor<T> * std::pow(temperature, T(4))));
    }

    template <std::floating_point T>
    T planet_distance_to_illumination(T luminosity, T distance) noexcept {
        return luminosity / sphere_area_from_radius(distance);
    }

    template <std::floating_point T>
    T planet_illumination_to_temperature(T illumination) noexcept {
        using namespace Constants;
        return std::pow(illumination / (4 * sigma_v<T>), T(0.25));
    }

}
