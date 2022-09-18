#include "crow/hexmap.hpp"
#include "crow/iterator.hpp"
#include "crow/string.hpp"
#include "crow/terminal.hpp"
#include "crow/unicode.hpp"
#include <algorithm>
#include <cstdlib>
#include <functional>
#include <iterator>
#include <set>

namespace Crow {

    namespace Detail {

        Int2 rs_to_xy(Int2 hex) noexcept {
            hex = Hexmap::principal(hex);
            int ring = hex[0];
            if (ring == 0)
                return {0, 0};
            int dir = hex[1] / ring;
            int ofs = hex[1] % ring;
            int x = 0;
            int y = 0;
            switch (Hexmap::side(dir)) {
                case Hexmap::side::north:      x = 2 * ofs;           y = 2 * ring - ofs;  break;
                case Hexmap::side::northeast:  x = 2 * ring;          y = ring - 2 * ofs;  break;
                case Hexmap::side::southeast:  x = 2 * (ring - ofs);  y = - (ring + ofs);  break;
                case Hexmap::side::south:      x = -2 * ofs;          y = ofs - 2 * ring;  break;
                case Hexmap::side::southwest:  x = -2 * ring;         y = 2 * ofs - ring;  break;
                case Hexmap::side::northwest:  x = 2 * (ofs - ring);  y = ring + ofs;      break;
                default:                       break;
            }
            return {x, y};
        }

        Int2 xy_to_rs(Int2 xy) noexcept {
            int x = xy.x();
            int y = xy.y();
            x &= ~ 1;
            y = x % 4 ? (y - 1) | 1 : y & ~ 1;
            if (x == 0 && y == 0)
                return {};
            int half_x = std::abs(x) / 2;
            int dir = (y < half_x) + (y < - half_x);
            if (x < 0)
                dir = 5 - dir;
            int ofs = 0;
            int ring = 0;
            switch (Hexmap::side(dir)) {
                case Hexmap::side::north:      ring = (y + half_x) / 2;  ofs = half_x;            break;
                case Hexmap::side::northeast:  ring = half_x;            ofs = (half_x - y) / 2;  break;
                case Hexmap::side::southeast:  ring = (x - 2 * y) / 4;   ofs = ring - half_x;     break;
                case Hexmap::side::south:      ring = (half_x - y) / 2;  ofs = half_x;            break;
                case Hexmap::side::southwest:  ring = half_x;            ofs = (half_x + y) / 2;  break;
                case Hexmap::side::northwest:  ring = (2 * y - x) / 4;   ofs = ring - half_x;     break;
                default:                       break;
            }
            int step = dir * ring + ofs;
            return {ring, step};
        }

    }

    Hexmap::Hexmap(std::initializer_list<init_hex> list) {
        for (auto& item: list) {
            if (item.colour.has_value())
                set(item.where, item.label, *item.colour);
            else
                set(item.where, item.label);
        }
    }

    Hexmap::hexlist Hexmap::all() const {
        hexlist vec;
        for (auto& [k,v]: table_)
            vec.push_back(k);
        return vec;
    }

    Hexmap::hexlist Hexmap::border_in() const {
        hexlist vec;
        for (auto& [k,v]: table_)
            if (! neighbours_unset(k).empty())
                vec.push_back(k);
        return vec;
    }

    Hexmap::hexlist Hexmap::border_out() const {
        std::set<hex> set;
        for (auto& [k,v]: table_) {
            auto nb = neighbours(k);
            std::copy(nb.begin(), nb.end(), append(set));
        }
        hexlist vec;
        for (auto& h: set)
            if (! contains(h))
                vec.push_back(h);
        return vec;
    }

    bool Hexmap::contains(hex h) const noexcept {
        return table_.find(h) != table_.end();
    }

    Hexmap::hexlist Hexmap::neighbours_set(hex h) const {
        hexlist vec;
        for (auto& n: neighbours(h))
            if (contains(n))
                vec.push_back(n);
        return vec;
    }

    Hexmap::hexlist Hexmap::neighbours_unset(hex h) const {
        hexlist vec;
        for (auto& n: neighbours(h))
            if (! contains(n))
                vec.push_back(n);
        return vec;
    }

    std::pair<Hexmap::hexlist, Hexmap::hexlist> Hexmap::neighbours_set_unset(hex h) const {
        std::pair<Hexmap::hexlist, Hexmap::hexlist> pair;
        for (auto& n: neighbours(h)) {
            if (contains(n))
                pair.first.push_back(n);
            else
                pair.second.push_back(n);
        }
        return pair;
    }

    Hexmap::hexlist Hexmap::reachable(hex h) const {
        std::set<hex> complete;
        std::set<hex> pending{h};
        if (contains(h))
            complete.insert(h);
        while (! pending.empty()) {
            hex p = *pending.begin();
            pending.erase(pending.begin());
            for (auto& n: neighbours_set(p))
                if (complete.insert(n).second)
                    pending.insert(n);
        }
        return hexlist(complete.begin(), complete.end());
    }

    int Hexmap::rings() const {
        return table_.empty() ? 0 : std::prev(table_.end())->first[0] + 1;
    }

    Hexmap::xcolour Hexmap::colour(hex h) const noexcept {
        auto it = table_.find(h);
        if (it != table_.end() && it->second.colour.has_value())
            return *it->second.colour;
        else
            return black;
    }

    void Hexmap::clear_colour(hex h) noexcept {
        auto it = table_.find(h);
        if (it != table_.end())
            it->second.colour.reset();
    }

    bool Hexmap::set_colour(hex h, xcolour c) noexcept {
        auto it = table_.find(h);
        if (it == table_.end())
            return false;
        it->second.colour = c;
        return true;
    }

    std::string Hexmap::label(hex h) const {
        auto it = table_.find(h);
        return it == table_.end() ? std::string() : it->second.label;
    }

    void Hexmap::set_label(hex h, const std::string& label) {
        if (label.empty())
            table_.erase(h);
        else
            table_[h].label = label;
    }

    void Hexmap::set(hex h, const std::string& label) {
        if (label.empty()) {
            table_.erase(h);
        } else {
            auto& info = table_[h];
            info.label = label;
            info.colour.reset();
        }
    }

    void Hexmap::set(hex h, const std::string& label, xcolour c) {
        if (label.empty()) {
            table_.erase(h);
        } else {
            auto& info = table_[h];
            info.label = label;
            info.colour = c;
        }
    }

    int Hexmap::area(int radius) noexcept {
        if (radius < 1)
            return 0;
        else if (radius == 1)
            return 1;
        else
            return 3 * radius * (radius - 1) + 1;
    }

    int Hexmap::distance(hex h1, hex h2) noexcept {
        using namespace Detail;
        auto xy1 = rs_to_xy(h1);
        auto xy2 = rs_to_xy(h2);
        int dx = std::abs(xy1.x() - xy2.x()), dy = std::abs(xy1.y() - xy2.y());
        int d = dx / 2;
        if (dx < 2 * dy)
            d += (dy - d) / 2;
        return d;
    }

    bool Hexmap::is_adjacent(hex h1, hex h2) noexcept {
        using namespace Detail;
        auto xy1 = rs_to_xy(h1);
        auto xy2 = rs_to_xy(h2);
        int dx = std::abs(xy1.x() - xy2.x()), dy = std::abs(xy1.y() - xy2.y());
        if (dx == 0)
            return dy <= 2;
        else if (dx == 2)
            return dy == 1;
        else
            return false;
    }

    bool Hexmap::is_valid(hex h) noexcept {
        if (h[0] < 0 || h[1] < 0)
            return false;
        else if (h[0] == 0)
            return h[1] == 0;
        else
            return h[1] < 6 * h[0];
    }

    Hexmap::hex Hexmap::neighbour(hex h, side s) noexcept {
        using namespace Detail;
        auto xy = rs_to_xy(h);
        int x = xy.x(), y = xy.y();
        hex result;
        switch (s) {
            case side::north:      result = xy_to_rs({x, y + 2}); break;
            case side::northeast:  result = xy_to_rs({x + 2, y + 1}); break;
            case side::southeast:  result = xy_to_rs({x + 2, y - 1}); break;
            case side::south:      result = xy_to_rs({x, y - 2}); break;
            case side::southwest:  result = xy_to_rs({x - 2, y - 1}); break;
            case side::northwest:  result = xy_to_rs({x - 2, y + 1}); break;
            default:               break;
        }
        return result;
    }

    Hexmap::hexlist Hexmap::neighbours(hex h) {
        using namespace Detail;
        auto xy = rs_to_xy(h);
        int x = xy.x(), y = xy.y();
        return {
            xy_to_rs({x, y + 2}),
            xy_to_rs({x + 2, y + 1}),
            xy_to_rs({x + 2, y - 1}),
            xy_to_rs({x, y - 2}),
            xy_to_rs({x - 2, y - 1}),
            xy_to_rs({x - 2, y + 1}),
        };
    }

    int Hexmap::perimeter(int radius) noexcept {
        if (radius < 1)
            return 0;
        else if (radius == 1)
            return 1;
        else
            return 6 * (radius - 1);
    }

    Hexmap::hex Hexmap::principal(hex h) noexcept {
        if (h[0] == 0)
            return {0, 0};
        if (h[0] < 0) {
            h[0] = - h[0];
            h[1] += 3 * h[0];
        }
        int n = 6 * h[0];
        h[1] %= n;
        if (h[1] < 0)
            h[1] += n;
        return h;
    }

    int Hexmap::ring_size(int r) noexcept {
        return perimeter(r + 1);
    }

    std::string Hexmap::as_string(bool io) const {

        xt xtio = xterm_;
        if (xtio == xt::implicit && io == false)
            xtio = xt::no;
        auto term = xtio == xt::implicit ? Xterm() : Xterm(xtio == xt::yes);

        int n_rings = std::max(rings(), radius_);
        if (n_rings <= 0 && table_.empty())
            return {};

        int max_width = width_;
        std::map<hex, int> widths;

        for (auto& [k,v]: table_) {
            int width = int(utf_width(v.label));
            max_width = std::max(max_width, width);
        }

        int hex_cols = max_width + 1;
        int n_rows = 4 * n_rings - 1;
        int n_columns = hex_cols * (2 * n_rings - 1) + 1;
        std::vector<std::string> page(n_rows, std::string(n_columns, ' '));

        for (int r = 0; r < n_rings; ++r) {
            int perimeter = r == 0 ? 1 : 6 * r;
            for (int s = 0; s < perimeter; ++s) {
                xy_pos rc = rs_to_rc({r, s}, n_rings, hex_cols);
                int row = rc[0];
                int col = rc[1];
                page[row][col] = page[row + 1][col + hex_cols] = '/';
                page[row + 1][col] = page[row][col + hex_cols] = '\\';
                for (int c = 1; c < hex_cols; ++c)
                    page[row - 1][col + c] = page[row + 1][col + c] = '_';
            }
        }

        std::map<xy_pos, const hex_info*, std::greater<xy_pos>> labels;

        for (auto& [k,v]: table_) {
            xy_pos rc = rs_to_rc(k, n_rings, hex_cols);
            labels[rc] = &v;
        }

        for (auto& [k,v]: labels) {
            int row = k[0];
            int col = k[1];
            std::string hex_label = v->label;
            size_t width = utf_width(hex_label);
            if (v->colour.has_value()) {
                auto rgb = term.rgb(v->colour->x(), v->colour->y(), v->colour->z());
                hex_label = term.bold() + rgb + hex_label + term.reset();
            }
            page[row].replace(col + 1, width, hex_label);
        }

        return join(page, "\n") + "\n";

    }

    Hexmap::xy_pos Hexmap::rs_to_rc(Hexmap::hex h, int n_rings, int width) noexcept {
        using namespace Detail;
        auto xy = rs_to_xy(h);
        int row = 2 * n_rings - xy.y() - 1;
        int col = (xy.x() / 2 + n_rings - 1) * width;
        return {row, col};
    }

}
