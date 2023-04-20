#include "crow/dice.hpp"
#include "crow/maths.hpp"
#include "crow/regex.hpp"
#include <algorithm>
#include <cmath>
#include <cstdlib>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace Crow {

    Dice::Dice(const std::string& str) {

        static const auto parse_integer = [] (const std::string& str, int def) noexcept {
            return str.empty() ? def : to_integer<int>(str);
        };

        static const Regex pattern(R"(
            ( [+-] )                # [1] sign
            (?:
                (?: (\d+) [*x] ) ?  # [2] left multiplier
                (\d*)               # [3] number of dice
                d (\d*)             # [4] number of faces
                (?: [*x] (\d+) ) ?  # [5] right multiplier
            |
                (\d+)               # [6] fixed modifier
            )
            (?: / (\d+) ) ?         # [7] divisor
        )", Regex::anchor | Regex::extended | Regex::icase);

        std::string text(str);
        std::erase_if(text, ascii_isspace);

        if (text.empty())
            return;

        if (text[0] != '+' && text[0] != '-')
            text.insert(0, 1, '+');

        size_t pos = 0;

        while (pos < text.size()) {

            auto match = pattern(text, pos);
            auto sign = *match.begin() == '-' ? -1 : 1;
            auto divisor = parse_integer(std::string(match[7]), 1);

            if (match.matched(6)) {

                auto factor = parse_integer(std::string(match[6]), 1);
                add_ += Rational(sign * factor, divisor);

            } else {

                auto factor1 = parse_integer(std::string(match[2]), 1);
                auto n_dice = parse_integer(std::string(match[3]), 1);
                auto n_faces = parse_integer(std::string(match[4]), 6);
                auto factor2 = parse_integer(std::string(match[5]), 1);
                insert(n_dice, n_faces, Rational(sign * factor1 * factor2, divisor));

            }

            pos = match.endpos();

        }

        modified();

    }

    Dice Dice::operator-() const {
        Dice d = *this;
        for (auto& g: d.groups_)
            g.factor = - g.factor;
        d.add_ = - d.add_;
        d.modified();
        return d;
    }

    Dice& Dice::operator+=(const Dice& rhs) {
        Dice d = *this;
        for (auto& g: rhs.groups_)
            d.insert(g.number, g.one_dice.max(), g.factor);
        d.add_ += rhs.add_;
        d.modified();
        *this = std::move(d);
        return *this;
    }

    Dice& Dice::operator+=(const Rational& b) {
        add_ += b;
        modified();
        return *this;
    }

    Dice& Dice::operator+=(int b) {
        add_ += b;
        modified();
        return *this;
    }

    Dice& Dice::operator-=(const Dice& rhs) {
        Dice d = *this;
        for (auto& g: rhs.groups_)
            d.insert(g.number, g.one_dice.max(), - g.factor);
        d.add_ -= rhs.add_;
        d.modified();
        *this = std::move(d);
        return *this;
    }

    Dice& Dice::operator-=(const Rational& b) {
        add_ -= b;
        modified();
        return *this;
    }

    Dice& Dice::operator-=(int b) {
        add_ -= b;
        modified();
        return *this;
    }

    Dice& Dice::operator*=(const Rational& rhs) {
        if (rhs) {
            for (auto& g: groups_)
                g.factor *= rhs;
            add_ *= rhs;
        } else {
            groups_.clear();
            add_ = 0;
        }
        modified();
        return *this;
    }

    Rational Dice::mean() const noexcept {
        Rational sum = add_;
        for (auto& g: groups_)
            sum += Rational(g.number * (g.one_dice.max() + 1)) * g.factor / Rational(2);
        return sum;
    }

    Rational Dice::variance() const noexcept {
        Rational sum;
        for (auto& g: groups_)
            sum += Rational(g.number * (g.one_dice.max() * g.one_dice.max() - 1)) * g.factor * g.factor / Rational(12);
        return sum;
    }

    double Dice::sd() const noexcept {
        using std::sqrt;
        return sqrt(double(variance()));
    }

    Rational Dice::pdf(const Rational& x) const {
        if (! check_table())
            return 0;
        auto it = info_->table.find(x);
        if (it == info_->table.end())
            return 0;
        else
            return it->second.pdf;
    }

    Rational Dice::cdf(const Rational& x) const {
        if (! check_table())
            return 0;
        auto it = info_->table.lower_bound(x);
        if (it == info_->table.end())
            return 1;
        else if (it->first == x)
            return it->second.cdf;
        else if (it == info_->table.begin())
            return 0;
        else
            return std::prev(it)->second.cdf;
    }

    Rational Dice::ccdf(const Rational& x) const {
        if (! check_table())
            return 0;
        auto it = info_->table.lower_bound(x);
        if (it == info_->table.end())
            return 0;
        else
            return it->second.ccdf;
    }

    Rational Dice::interval(const Rational& x, const Rational& y) const {
        return cdf(y) - cdf(x - 1);
    }

    std::string Dice::str() const {

        std::string text;

        for (auto& g: groups_) {
            text += g.factor.sign() == -1 ? '-' : '+';
            if (g.number > 1)
                text += std::to_string(g.number);
            text += 'd' + std::to_string(g.one_dice.max());
            auto n = std::abs(g.factor.num());
            if (n > 1)
                text += '*' + std::to_string(n);
            auto d = g.factor.den();
            if (d > 1)
                text += '/' + std::to_string(d);
        }

        if (add_ > 0)
            text += '+';
        if (add_)
            text += add_.str();

        if (text[0] == '+')
            text.erase(0, 1);
        if (text.empty())
            text = "0";

        return text;

    }

    bool Dice::check_table() const {

        if (! info_)
            return false;

        auto lock = std::unique_lock(info_->mutex);

        if (! info_->table.empty())
            return true;

        int n = int(groups_.size());
        std::vector<pdf_table> subtables(n);
        std::transform(groups_.begin(), groups_.end(), subtables.begin(), make_table);
        std::vector<pdf_table::const_iterator> iterators(n);
        std::transform(subtables.begin(), subtables.end(), iterators.begin(), [] (auto& sub) { return sub.begin(); });

        for (;;) {

            auto x = add_;
            Rational p = 1;

            for (auto i: iterators) {
                x += i->first;
                p *= i->second;
            }

            info_->table[x].pdf += p;

            int i = n - 1;
            for (; i >= 0; --i) {
                ++iterators[i];
                if (iterators[i] != subtables[i].end())
                    break;
                iterators[i] = subtables[i].begin();
            }
            if (i == -1)
                break;

        }

        Rational cdf = 0;
        auto j = info_->table.end();

        for (auto& [x,ps]: info_->table)
            (--j)->second.ccdf = ps.cdf = cdf = cdf + ps.pdf;

        info_->table.begin()->second.ccdf = std::prev(info_->table.end())->second.cdf = 1;

        return true;

    }

    void Dice::insert(int n, int faces, const Rational& factor) {

        static const auto match_terms = [] (const dice_group& g1, const dice_group& g2) noexcept {
            return g1.one_dice.max() == g2.one_dice.max() && g1.factor == g2.factor;
        };

        static const auto sort_terms = [] (const dice_group& g1, const dice_group& g2) noexcept {
            return g1.one_dice.max() == g2.one_dice.max() ? g1.factor < g2.factor : g1.one_dice.max() > g2.one_dice.max();
        };

        if (n < 0 || faces < 0)
            throw std::invalid_argument("Invalid dice");

        if (n > 0 && faces > 0 && factor != 0) {
            dice_group g;
            g.number = n;
            g.one_dice = distribution_type(1, faces);
            g.factor = factor;
            auto it = std::lower_bound(groups_.begin(), groups_.end(), g, sort_terms);
            if (it != groups_.end() && match_terms(*it, g))
                it->number += g.number;
            else
                groups_.insert(it, g);
        }

    }

    void Dice::modified() {

        info_ = std::make_shared<table_info>();
        min_ = max_ = add_;

        for (auto& g: groups_) {
            if (g.factor > 0) {
                min_ += Rational(g.number) * g.factor;
                max_ += Rational(g.number * g.one_dice.max()) * g.factor;
            } else {
                min_ += Rational(g.number * g.one_dice.max()) * g.factor;
                max_ += Rational(g.number) * g.factor;
            }
        }

    }

    Dice::pdf_table Dice::make_table(const dice_group& group) {

        pdf_table table;
        int n = group.number;
        int f = group.one_dice.max();
        int den = integer_power(f, n);
        int max = f * n;
        double b = n - 1;

        for (int i = n; i <= max; ++i) {

            int num = 0;
            int sign = 1;
            double a = i - 1;

            for (int j = 0; j < n; ++j, a -= f, sign = - sign)
                num += sign * int(std::lround(binomial(a, b) * binomial(double(n), double(j))));

            Rational x = i * group.factor;
            Rational p(num, den);
            table.insert({x, p});

        }

        return table;

    }

}
