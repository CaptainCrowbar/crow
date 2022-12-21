#include "crow/spectrum.hpp"
#include "crow/constants.hpp"
#include "crow/format.hpp"
#include "crow/formula.hpp"
#include "crow/linear-map.hpp"
#include "crow/maths.hpp"
#include "crow/regex.hpp"
#include "crow/string.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <functional>
#include <limits>
#include <stdexcept>
#include <type_traits>
#include <unordered_map>
#include <vector>

namespace Crow {

    namespace {

        using Linmap = LinearMap<double>;

        constexpr double NaN = std::numeric_limits<double>::quiet_NaN();

        constexpr int sp_index(Sp cls, int sub) noexcept {
            return 10 * int(cls) + std::clamp(sub, 0, 9);
        };

        LC default_lc(Sp cls) noexcept {
            if (cls == Sp::none)
                return LC::none;
            else if (cls >= Sp::O && cls <= Sp::M)
                return LC::V;
            else if (cls >= Sp::DA && cls <= Sp::DZ)
                return LC::VII;
            else
                return LC::other;
        }

        bool valid_sp(LC lc, Sp cls, int sub) noexcept {
            if (lc < LC::Ia || lc > LC::other)
                return false;
            else if (cls < Sp::O || cls > Sp::BH)
                return false;
            else if (sub < 0 || sub > 99)
                return false;
            else if (cls <= Sp::M)
                return lc <= LC::VI;
            else if (cls <= Sp::Y)
                return lc == LC::other;
            else if (cls <= Sp::DZ)
                return lc == LC::VII;
            else if (cls <= Sp::S)
                return lc == LC::other;
            else
                return lc == LC::other && sub == 0;
        }

        // Parsing tables

        void parse_hr_type(LC& lc, Sp& cls, int& sub,
                std::string_view sc_str, std::string_view sub_str, std::string_view lc_str) {

            if (lc_str == "Ia")
                lc = LC::Ia;
            else if (lc_str == "Ib")
                lc = LC::Ib;
            else if (lc_str == "II")
                lc = LC::II;
            else if (lc_str == "III")
                lc = LC::III;
            else if (lc_str == "IV")
                lc = LC::IV;
            else if (lc_str == "V")
                lc = LC::V;
            else
                lc = LC::VI;

            switch (sc_str[0]) {
                case 'O':  cls = Sp::O; break;
                case 'B':  cls = Sp::B; break;
                case 'A':  cls = Sp::A; break;
                case 'F':  cls = Sp::F; break;
                case 'G':  cls = Sp::G; break;
                case 'K':  cls = Sp::K; break;
                default:   cls = Sp::M; break;
            }

            sub = to_int(std::string(sub_str));

        }

        void parse_brown_dwarf(LC& lc, Sp& cls, int& sub,
                std::string_view sc_str, std::string_view sub_str) {

            lc = LC::other;

            switch (sc_str[0]) {
                case 'L':  cls = Sp::L; break;
                case 'T':  cls = Sp::T; break;
                default:   cls = Sp::Y; break;
            }

            sub = to_int(std::string(sub_str));

        }

        void parse_white_dwarf(LC& lc, Sp& cls, int& sub,
                std::string_view sc_str, std::string_view sub_str) {

            lc = LC::VII;

            switch (sc_str[0]) {
                case 'A':  cls = Sp::DA; break;
                case 'B':  cls = Sp::DB; break;
                case 'C':  cls = Sp::DC; break;
                case 'O':  cls = Sp::DO; break;
                case 'Q':  cls = Sp::DQ; break;
                case 'X':  cls = Sp::DX; break;
                default:   cls = Sp::DZ; break;
            }

            sub = to_int(std::string(sub_str));

        }

        void parse_wolf_rayet(LC& lc, Sp& cls, int& sub,
                std::string_view sc_str, std::string_view sub_str) {

            lc = LC::other;

            switch (sc_str[0]) {
                case 'C':  cls = Sp::WC; break;
                case 'N':  cls = Sp::WN; break;
                default:   cls = Sp::WO; break;
            }

            sub = to_int(std::string(sub_str));

        }

        void parse_carbon_star(LC& lc, Sp& cls, int& sub,
                std::string_view sc_str, std::string_view sub_str) {

            lc = LC::other;

            switch (sc_str[0]) {
                case 'H':  cls = sc_str[1] == 'd' ? Sp::CHd : Sp::CH; break;
                case 'J':  cls = Sp::CJ; break;
                case 'N':  cls = Sp::CN; break;
                default:   cls = Sp::CR; break;
            }

            sub = to_int(std::string(sub_str));

        }

        void parse_s_star(LC& lc, Sp& cls, int& sub,
                std::string_view sub_str) {
            lc = LC::other;
            cls = Sp::S;
            sub = to_int(std::string(sub_str));
        }

        void parse_stellar_remnant(LC& lc, Sp& cls, int& sub,
                std::string_view sc_str) {
            lc = LC::other;
            cls = sc_str == "BH" ? Sp::BH : Sp::NS;
            sub = 0;
        }

        // Temperature tables

        const auto& hr_star_log_t_table() {
            static const Linmap table = {
                { sp_index(Sp::O, 3), 4.70 },
                { sp_index(Sp::O, 5), 4.62 },
                { sp_index(Sp::B, 0), 4.48 },
                { sp_index(Sp::B, 5), 4.18 },
                { sp_index(Sp::A, 0), 3.99 },
                { sp_index(Sp::A, 5), 3.91 },
                { sp_index(Sp::F, 0), 3.86 },
                { sp_index(Sp::F, 5), 3.82 },
                { sp_index(Sp::G, 0), 3.775 },
                { sp_index(Sp::G, 5), 3.74 },
                { sp_index(Sp::K, 0), 3.71 },
                { sp_index(Sp::K, 5), 3.64 },
                { sp_index(Sp::M, 0), 3.58 },
                { sp_index(Sp::M, 5), 3.51 },
                { sp_index(Sp::M, 9), 3.48 },
            };
            return table;
        }

        const auto& brown_dwarf_log_t_table() {
            static const Linmap table = {
                { sp_index(Sp::L, 0), 3.45 },
                { sp_index(Sp::T, 0), 3.10 },
                { sp_index(Sp::Y, 0), 2.70 },
                { sp_index(Sp::Y, 2), 2.60 },
            };
            return table;
        }

        const auto& wc_star_log_t_table() {
            static const Linmap table = {
                { sp_index(Sp::WC, 4), 5.07 },
                { sp_index(Sp::WC, 5), 4.92 },
                { sp_index(Sp::WC, 9), 4.64 },
            };
            return table;
        }

        const auto& wn_star_log_t_table() {
            static const Linmap table = {
                { sp_index(Sp::WN, 2), 5.15 },
                { sp_index(Sp::WN, 5), 4.78 },
                { sp_index(Sp::WN, 9), 4.54 },
            };
            return table;
        }

        const auto& wo_star_log_t_table() {
            static const Linmap table = {
                { sp_index(Sp::WO, 1), 5.35 },
                { sp_index(Sp::WO, 4), 5.20 },
            };
            return table;
        }

        const auto& c_star_log_t_table() {
            static const Linmap table = {
                { sp_index(Sp::none, 0), 3.70 },
                { sp_index(Sp::none, 5), 3.50 },
                { sp_index(Sp::none, 9), 3.42 },
            };
            return table;
        }

        const auto& s_star_log_t_table() {
            static const Linmap table = {
                { sp_index(Sp::S, 1), 3.60 },
                { sp_index(Sp::S, 5), 3.50 },
                { sp_index(Sp::S, 9), 3.40 },
            };
            return table;
        }

        // Mass and luminosity tables

        struct HrInfo {
            int sp_index;
            std::array<double, 7> log_mass;
            std::array<double, 7> log_luminosity;
        };

        const auto& hr_mass_luminosity() {
            static const std::vector<HrInfo>& table = {
                // Sp                  M: Ia     Ib     II       III     IV      V       VI        L: Ia    Ib     II      III    IV      V       VI
                { sp_index(Sp::O, 3),  { 2.225,  2.20,  2.175,   2.15,   2.125,  2.10,   0.10 },   { 6.75,  6.65,  6.55,   6.45,  6.35,   6.25,   6.25 }},
                { sp_index(Sp::O, 5),  { 1.875,  1.85,  1.825,   1.80,   1.775,  1.75,   0.75 },   { 6.50,  6.40,  6.30,   6.20,  6.10,   6.00,   6.00 }},
                { sp_index(Sp::B, 0),  { 1.45,   1.40,  1.35,    1.30,   1.275,  1.25,   0.25 },   { 5.75,  5.50,  5.25,   5.00,  4.875,  4.75,   4.75 }},
                { sp_index(Sp::B, 5),  { 1.35,   1.30,  1.075,   0.85,   0.80,   0.75,   0.75 },   { 5.50,  5.00,  4.50,   4.00,  3.50,   3.00,   3.00 }},
                { sp_index(Sp::A, 0),  { 1.25,   1.20,  0.90,    0.60,   0.525,  0.45,   0.45 },   { 5.10,  4.60,  4.05,   3.50,  2.65,   1.80,   1.80 }},
                { sp_index(Sp::A, 5),  { 1.15,   1.10,  0.775,   0.45,   0.375,  0.30,   0.30 },   { 5.00,  4.50,  3.75,   3.00,  2.10,   1.20,   1.20 }},
                { sp_index(Sp::F, 0),  { 1.10,   1.05,  0.70,    0.35,   0.275,  0.20,   0.20 },   { 5.00,  4.50,  3.50,   2.50,  2.00,   0.80,   0.80 }},
                { sp_index(Sp::F, 5),  { 1.05,   1.00,  0.625,   0.25,   0.20,   0.15,   0.15 },   { 5.00,  4.50,  3.35,   2.20,  2.00,   0.60,   0.60 }},
                { sp_index(Sp::G, 0),  { 1.05,   1.00,  0.525,   0.05,   0.025,  0.02,   0.02 },   { 5.00,  4.50,  3.15,   1.80,  1.60,   0.08,   -0.32 }},
                { sp_index(Sp::G, 5),  { 1.10,   1.05,  0.55,    0.05,   0.025,  -0.03,  -0.03 },  { 5.00,  4.50,  3.10,   1.70,  1.50,   -0.12,  -0.52 }},
                { sp_index(Sp::K, 0),  { 1.15,   1.10,  0.575,   0.05,   0.025,  -0.08,  -0.08 },  { 5.00,  4.50,  3.15,   1.80,  1.60,   -0.32,  -0.72 }},
                { sp_index(Sp::K, 5),  { 1.15,   1.10,  0.5875,  0.075,  0.05,   -0.18,  -0.18 },  { 5.00,  4.50,  3.35,   2.20,  2.00,   -0.72,  -1.12 }},
                { sp_index(Sp::M, 0),  { 1.15,   1.10,  0.60,    0.10,   0.075,  -0.30,  -0.30 },  { 5.25,  4.75,  3.675,  2.60,  2.40,   -1.08,  -1.48 }},
                { sp_index(Sp::M, 5),  { 1.15,   1.10,  0.6125,  0.125,  0.10,   -0.68,  -0.68 },  { 5.50,  5.00,  4.05,   3.10,  2.90,   -2.22,  -2.62 }},
                { sp_index(Sp::M, 9),  { 1.15,   1.10,  0.6125,  0.125,  0.10,   -1.15,  -1.15 },  { 5.70,  5.20,  4.35,   3.50,  3.30,   -3.63,  -4.03 }},
            };
            return table;
        }

        const auto& hr_log_mass() {
            static const auto map = [] {
                std::unordered_map<LC, Linmap> map;
                for (auto& [spx,logm,logl]: hr_mass_luminosity())
                    for (int lci = 0; lci < 7; ++lci)
                        map[LC(lci + int(LC::Ia))].insert(spx, logm[lci]);
                return map;
            }();
            return map;
        }

        const auto& hr_log_luminosity() {
            static const auto map = [] {
                std::unordered_map<LC, Linmap> map;
                for (auto& [spx,logm,logl]: hr_mass_luminosity())
                    for (int lci = 0; lci < 7; ++lci)
                        map[LC(lci + int(LC::Ia))].insert(spx, logl[lci]);
                return map;
            }();
            return map;
        }

        const Linmap& brown_dwarf_log_mass() {
            static const Linmap map = {
                { sp_index(Sp::L, 0), -1.25 },
                { sp_index(Sp::T, 0), -1.50 },
                { sp_index(Sp::Y, 0), -1.75 },
                { sp_index(Sp::Y, 2), -1.80 },
            };
            return map;
        }

        struct OffSequenceInfo {
            double log_mass;
            double log_luminosity;
        };

        const auto& off_sequence_mass_luminosity() {
            static const std::unordered_map<Sp, OffSequenceInfo> map = {
                { Sp::WC,   OffSequenceInfo{1.15, 5.50} },
                { Sp::WN,   OffSequenceInfo{1.40, 5.80} },
                { Sp::WO,   OffSequenceInfo{1.40, 5.80} },
                { Sp::CH,   OffSequenceInfo{0.50, 3.10} },
                { Sp::CHd,  OffSequenceInfo{0.50, 3.80} },
                { Sp::CJ,   OffSequenceInfo{0.70, 3.80} },
                { Sp::CN,   OffSequenceInfo{0.60, 3.20} },
                { Sp::CR,   OffSequenceInfo{0.10, 2.30} },
                { Sp::S,    OffSequenceInfo{0.45, 3.85} },
            };
            return map;
        }

    }

    // Class Spectrum

    Spectrum::Spectrum(Sp cls, int sub, LC lc) {
        if (lc == LC::none && cls == Sp::none && sub == 0)
            return;
        if (lc == LC::none)
            lc = default_lc(cls);
        if (! valid_sp(lc, cls, sub))
            throw std::invalid_argument(fmt("Invalid spectral class: LC={0}, class={1}, subclass={2}", lc, cls, sub));
        lc_ = lc;
        cls_ = cls;
        sub_ = uint8_t(sub);
    }

    Spectrum::Spectrum(std::string_view str) {

        if (str.empty())
            return;

        static const Regex pattern(R"(
            ([OBAFGKM]) (\d+) [ ]? (I[ab] | III? | IV | VI?)  # [1] [2] [3]
            | ([LTY]) (\d+)                                   # [4] [5]
            | D ([ABCOQXZ]) (\d+) (?: [ ]? VII) ?             # [6] [7]
            | W ([CNO]) (\d+)                                 # [8] [9]
            | C (\d+) - (Hd? | [JNR])                         # [10] [11]
            | S (\d+)                                         # [12]
            | (NS | PSR | BH)                                 # [13]
            )", Regex::extended | Regex::full);

        auto match = pattern(str);

        if (! match)
            throw std::invalid_argument("Invalid spectral type: " + quote(str));

        LC lc = LC::none;
        Sp cls = Sp::none;
        int sub = 0;

        if (match.matched(1))
            parse_hr_type(lc, cls, sub, match[1], match[2], match[3]);
        else if (match.matched(4))
            parse_brown_dwarf(lc, cls, sub, match[4], match[5]);
        else if (match.matched(6))
            parse_white_dwarf(lc, cls, sub, match[6], match[7]);
        else if (match.matched(8))
            parse_wolf_rayet(lc, cls, sub, match[8], match[9]);
        else if (match.matched(10))
            parse_carbon_star(lc, cls, sub, match[11], match[10]);
        else if (match.matched(12))
            parse_s_star(lc, cls, sub, match[12]);
        else
            parse_stellar_remnant(lc, cls, sub, match[13]);

        *this = Spectrum(cls, sub, lc);

    }

    double Spectrum::bc() const {
        if (cls_ == Sp::none || is_stellar_remnant())
            return NaN;
        else
            return temperature_to_bc(temperature());
    }

    double Spectrum::b_v() const {
        if (cls_ == Sp::none || is_stellar_remnant())
            return NaN;
        auto t = temperature();
        auto t2 = t * t;
        return 0.0217391 * (std::sqrt(729 * t2 + 5.29e10) - 58 * t + 2.3e5) / t;
    }

    double Spectrum::luminosity() const {
        if (cls_ == Sp::none)
            return NaN;
        else if (is_brown_dwarf() || is_white_dwarf())
            return luminosity_from_radius_temperature(radius(), temperature());
        else if (is_stellar_remnant())
            return 0;
        else
            return std::pow(10.0, log_l()) * solar_luminosity;
    }

    double Spectrum::mass() const {
        if (cls_ == Sp::none)
            return NaN;
        else if (is_white_dwarf())
            return 0.5 * solar_mass;
        else if (cls_ == Sp::NS)
            return 1.5 * solar_mass;
        else if (cls_ == Sp::BH)
            return 10 * solar_mass;
        else
            return std::pow(10.0, log_m()) * solar_mass;
    }

    double Spectrum::radius() const {
        if (cls_ == Sp::NS)
            return 1e4;
        else if (cls_ == Sp::BH)
            return schwarzschild_radius(mass());
        else if (is_brown_dwarf() || is_white_dwarf())
            return solar_radius * std::pow(10.0, log_r());
        else
            return radius_from_luminosity_temperature(luminosity(), temperature());
    }

    double Spectrum::temperature() const {
        static constexpr double theta = 50'400;
        if (cls_ == Sp::none)
            return NaN;
        else if (is_white_dwarf())
            return sub_ > 0 ? theta / sub_ : 1.5 * theta;
        else if (is_stellar_remnant())
            return 0;
        else
            return std::pow(10.0, log_t());
    }

    double Spectrum::log_l() const {
        if (cls_ == Sp::none || is_stellar_remnant())
            return NaN;
        else if (is_brown_dwarf() || is_white_dwarf())
            return std::log10(luminosity() / solar_luminosity);
        else if (is_hr_type())
            return hr_log_luminosity().find(lc_)->second[sp_index(cls_, sub_)];
        else
            return off_sequence_mass_luminosity().find(cls_)->second.log_luminosity;
    }

    double Spectrum::log_m() const {
        if (cls_ == Sp::none)
            return NaN;
        else if (is_hr_type())
            return hr_log_mass().find(lc_)->second[sp_index(cls_, sub_)];
        else if (is_brown_dwarf())
            return brown_dwarf_log_mass()[sp_index(cls_, sub_)];
        else if (is_white_dwarf() || is_stellar_remnant())
            return std::log10(mass() / solar_mass);
        else
            return off_sequence_mass_luminosity().find(cls_)->second.log_mass;
    }

    double Spectrum::log_r() const {
        if (cls_ == Sp::none)
            return NaN;
        else if (is_brown_dwarf())
            return -1;
        else if (is_white_dwarf())
            return - log_m() / 3 - 2;
        else
            return std::log10(radius() / solar_radius);
    }

    double Spectrum::log_t() const {

        using Linmap = LinearMap<double>;

        if (cls_ == Sp::none || is_stellar_remnant())
            return NaN;
        else if (is_white_dwarf())
            return std::log10(temperature());

        const Linmap* table = nullptr;

        if (is_hr_type())
            table = &hr_star_log_t_table();
        else if (is_brown_dwarf())
            table = &brown_dwarf_log_t_table();
        else if (cls_ == Sp::WC)
            table = &wc_star_log_t_table();
        else if (cls_ == Sp::WN)
            table = &wn_star_log_t_table();
        else if (cls_ == Sp::WO)
            table = &wo_star_log_t_table();
        else if (cls_ == Sp::S)
            table = &s_star_log_t_table();
        else
            table = &c_star_log_t_table();

        auto use_class = cls_;

        if (cls_ >= Sp::CH && cls_ <= Sp::CR)
            use_class = Sp::none;

        auto lt = (*table)[sp_index(use_class, sub_)];

        return lt;

    }

    size_t Spectrum::hash() const noexcept {
        using UL = std::underlying_type_t<LC>;
        using US = std::underlying_type_t<Sp>;
        size_t h1 = std::hash<UL>()(UL(lc_));
        size_t h2 = std::hash<US>()(US(cls_));
        size_t h3 = std::hash<int>()(int(sub_));
        return hash_mix(h1, h2, h3);
    }

    std::string Spectrum::str() const {
        if (lc_ == LC::none)
            return {};
        bool c = cls_ >= Sp::CH && cls_ <= Sp::CR;
        std::string s;
        if (c)
            s = "C";
        else
            s = to_string(cls_);
        if (cls_ <= Sp::S)
            s += std::to_string(int(sub_));
        if (c)
            s += '-' + to_string(cls_).substr(1);
        if (cls_ <= Sp::M)
            s += ' ' + to_string(lc_);
        return s;
    }

}
