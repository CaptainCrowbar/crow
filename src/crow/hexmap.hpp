#pragma once

#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <initializer_list>
#include <map>
#include <optional>
#include <ostream>
#include <string>
#include <utility>
#include <vector>

namespace Crow {

    namespace Detail {

        Int2 rs_to_xy(Int2 hex) noexcept;
        Int2 xy_to_rs(Int2 xy) noexcept;

    }

    class Hexmap {

    public:

        enum class side: int {
            north,
            northeast,
            southeast,
            south,
            southwest,
            northwest,
        };

        enum class xt: int {
            implicit = -1,
            no,
            yes,
        };

        using hex = Int2;
        using hexlist = std::vector<hex>;
        using xcolour = Int3;

        static constexpr xcolour black = {0,0,0};
        static constexpr xcolour white = {5,5,5};

    private:

        struct init_hex {
            hex where;
            std::string label;
            std::optional<xcolour> colour;
            init_hex(hex h, const std::string& s): where(h), label(s), colour() {}
            init_hex(hex h, const std::string& s, xcolour c): where(h), label(s), colour(c) {}
        };

    public:

        Hexmap() = default;
        Hexmap(std::initializer_list<init_hex> list);

        hexlist all() const;
        hexlist border_in() const;
        hexlist border_out() const;
        bool contains(hex h) const noexcept;
        bool empty() const noexcept { return table_.empty(); }
        hexlist neighbours_set(hex h) const;
        hexlist neighbours_unset(hex h) const;
        std::pair<hexlist, hexlist> neighbours_set_unset(hex h) const;
        hexlist reachable(hex h) const;
        int rings() const;
        int size() const noexcept { return int(table_.size()); }

        void clear() noexcept { table_.clear(); }
        void erase(hex h) noexcept { table_.erase(h); }

        xcolour colour(hex h) const noexcept;
        void clear_colour(hex h) noexcept;
        bool set_colour(hex h, xcolour c) noexcept;
        std::string label(hex h) const;
        void set_label(hex h, const std::string& label);
        void set(hex h, const std::string& label);
        void set(hex h, const std::string& label, xcolour c);

        int radius() const noexcept { return radius_; }
        void set_radius(int r = 1) noexcept { radius_ = r; }
        int width() const noexcept { return width_; }
        void set_width(int w = 2) noexcept { width_ = w; }
        xt xterm() const noexcept { return xterm_; }
        void set_xterm(bool b) noexcept { xterm_ = xt(b); }
        void set_xterm(xt x) noexcept { xterm_ = x; }
        std::string str() const { return as_string(false); }

        static int area(int radius) noexcept;
        static int distance(hex h1, hex h2) noexcept;
        static bool is_adjacent(hex h1, hex h2) noexcept;
        static bool is_valid(hex h) noexcept;
        static hex neighbour(hex h, side s) noexcept;
        static hexlist neighbours(hex h);
        static int perimeter(int radius) noexcept;
        static hex principal(hex h) noexcept;
        static int ring_size(int r) noexcept;

        friend std::ostream& operator<<(std::ostream& out, const Hexmap& hm) { return out << hm.as_string(true); }

    private:

        struct hex_info {
            std::string label;
            std::optional<xcolour> colour;
        };

        using hex_table = std::map<hex, hex_info>;
        using xy_pos = Int2; // XY coordinates are 2 units per hex in both directions

        hex_table table_;
        int radius_ = 1;
        int width_ = 2;
        xt xterm_ = xt::implicit;

        std::string as_string(bool io) const;

        static xy_pos rs_to_rc(hex h, int n_rings, int width) noexcept;

    };

}
