#pragma once

#include "crow/enum.hpp"
#include "crow/hash.hpp"
#include "crow/types.hpp"
#include <compare>
#include <ostream>
#include <string>
#include <string_view>

namespace Crow {

    CROW_ENUM_CLASS(LC, uint8_t, 0,
        none,
        Ia,   // Bright supergiants
        Ib,   // Supergiants
        II,   // Bright giants
        III,  // Giants
        IV,   // Subgiants
        V,    // Main sequence
        VI,   // Subdwarfs
        VII,  // White dwarfs
        other
    )

    CROW_ENUM_CLASS(Sp, uint8_t, 0,
        none,
        O, B, A, F, G, K, M,         // Main sequence and giants (HR diagram types)
        L, T, Y,                     // Brown dwarfs
        DA, DB, DC, DO, DQ, DX, DZ,  // White dwarfs
        WC, WN, WO,                  // Wolf-Rayet stars
        CH, CHd, CJ, CN, CR, S,      // Carbon stars
        NS, BH                       // Stellar remnants
    )

    class Spectrum {

    public:

        constexpr Spectrum() noexcept {}
        explicit Spectrum(Sp cls, int sub = 0, LC lc = LC::none);
        explicit Spectrum(std::string_view str);

        constexpr LC lc() const noexcept { return lc_; }
        constexpr Sp cls() const noexcept { return cls_; }
        constexpr int sub() const noexcept { return sub_; }

        constexpr bool is_giant() const noexcept { return lc_ >= LC::Ia && cls_ >= Sp::O && cls_ <= Sp::M
            && (lc_ <= LC::III || (lc_ == LC::IV && cls_ >= Sp::G)); }
        constexpr bool is_main_sequence() const noexcept { return lc_ <= LC::VI && cls_ >= Sp::O && cls_ <= Sp::M
            && (lc_ >= LC::V || (lc_ == LC::IV && cls_ <= Sp::F)); }
        constexpr bool is_hr_type() const noexcept { return cls_ >= Sp::O && cls_ <= Sp::M; }
        constexpr bool is_brown_dwarf() const noexcept { return cls_ >= Sp::L && cls_ <= Sp::Y; }
        constexpr bool is_white_dwarf() const noexcept { return cls_ >= Sp::DA && cls_ <= Sp::DZ; }
        constexpr bool is_wolf_rayet_star() const noexcept { return cls_ >= Sp::WC && cls_ <= Sp::WO; }
        constexpr bool is_carbon_star() const noexcept { return cls_ >= Sp::CH && cls_ <= Sp::S; }
        constexpr bool is_stellar_remnant() const noexcept { return cls_ >= Sp::NS && cls_ <= Sp::BH; }

        double bc() const;
        double b_v() const;
        double luminosity() const;
        double mass() const;
        double radius() const;
        double temperature() const;
        double log_l() const;
        double log_m() const;
        double log_r() const;
        double log_t() const;
        double M_bol() const;
        double M_v() const;

        size_t hash() const noexcept;
        std::string str() const;

        friend constexpr bool operator==(Spectrum a, Spectrum b) noexcept = default;
        friend constexpr std::strong_ordering operator<=>(Spectrum a, Spectrum b) noexcept = default;

    private:

        LC lc_ = LC::none;
        Sp cls_ = Sp::none;
        uint8_t sub_ = 0;

    };

    inline std::ostream& operator<<(std::ostream& out, const Spectrum& sp) {
        return out << sp.str();
    }

    namespace Literals {

        inline Spectrum operator""_sp(const char* ptr, size_t len) {
            return Spectrum(std::string(ptr, len));
        }

    }

}

CROW_STD_HASH_0(Spectrum)
