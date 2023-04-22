#include "crow/table.hpp"
#include <cstring>
#include <map>
#include <stdexcept>
#include <string_view>
#include <utility>

namespace Crow {

    namespace {

        constexpr std::string_view body_null = "--";
        constexpr std::string_view header_null = "..";

        char get_mode(const FormatSpec& spec) noexcept {
            char m = spec.mode();
            if (m == 0)
                m = 'T';
            return m;
        }

        struct RuleSpec {
            const char* left;
            const char* lpad;
            const char* cell;
            const char* rpad;
            const char* cross;
            const char* right;
        };

        constexpr RuleSpec b_top = { "╭", "─", "─", "─", "┬", "╮" };
        constexpr RuleSpec b_mid = { "╞", "═", "═", "═", "╪", "╡" };
        constexpr RuleSpec b_bot = { "╰", "─", "─", "─", "┴", "╯" };
        constexpr RuleSpec m_mid = { "|", " ", "-", "  ", "|", "|" };
        constexpr RuleSpec t_mid = { "", "", "=", "  ", "", "" };

        std::string make_rule(const std::vector<size_t>& widths, const RuleSpec& spec) {
            std::string rule = spec.left;
            for (auto w: widths) {
                rule += spec.lpad;
                rule += repeat(spec.cell, w);
                rule += spec.rpad;
                rule += spec.cross;
            }
            rule.resize(rule.size() - std::strlen(spec.cross));
            rule += spec.right;
            rule = trim_right(rule) + '\n';
            return rule;
        }

    }

    // Class Table

    std::string Table::str(const FormatSpec& spec) const {

        char mode = get_mode(spec);

        if (mode != 'B' && mode != 'M' && mode != 'T')
            throw std::invalid_argument("Invalid table format: " + quote(spec.str()));

        auto widths = get_widths(spec);
        std::string result;

        if (mode == 'B')
            result += make_rule(widths, b_top);

        if (! headers_.empty()) {
            format_header(result, headers_, widths, spec);
            switch (mode) {
                case 'B':  result += make_rule(widths, b_mid); break;
                case 'M':  result += make_rule(widths, m_mid); break;
                default:   result += make_rule(widths, t_mid); break;
            }
        }

        for (auto& row: cells_)
            format_row(result, row, widths, spec, false);

        if (mode == 'B')
            result += make_rule(widths, b_bot);

        return result;

    }

    void Table::do_add(const std::vector<std::string>& row) {
        if (cells_.back().empty()) {
            cells_.back() = std::move(row);
        } else {
            cells_.push_back(row);
            cells_.push_back({});
        }
        columns_ = std::max(columns_, row.size());
    }

    void Table::do_format(const std::vector<std::string>& row) {
        formats_.assign(row.size(), {});
        std::copy(row.begin(), row.end(), formats_.begin());
    }

    void Table::do_header(const std::vector<std::string>& row) {
        headers_ = row;
        columns_ = std::max(columns_, row.size());
    }

    std::vector<size_t> Table::get_widths(const FormatSpec& spec) const {

        static const size_t min_width = std::max(utf_size(body_null, Usize::columns),
            utf_size(header_null, Usize::columns));

        char mode = get_mode(spec);
        std::vector<size_t> widths(columns_, min_width);

        if (mode == 'M') {

            for (size_t c = 0; c < headers_.size(); ++c) {
                size_t n = std::count(headers_[c].begin(), headers_[c].end(), '\n');
                size_t len = utf_size(headers_[c], Usize::columns) + 4 * n;
                widths[c] = std::max(widths[c], len);
            }

        } else {

            std::vector<std::string_view> parts;

            for (size_t c = 0; c < headers_.size(); ++c) {
                parts = split_v(headers_[c], "\n");
                for (auto& part: parts)
                    widths[c] = std::max(widths[c], utf_size(part, Usize::columns));
            }

        }

        for (auto& row: cells_)
            for (size_t c = 0; c < row.size(); ++c)
                widths[c] = std::max(widths[c], utf_size(row[c], Usize::columns));

        return widths;
    }

    void Table::format_header(std::string& out, const std::vector<std::string>& header,
            const std::vector<size_t>& widths, const FormatSpec& spec) const {

        char mode = get_mode(spec);
        std::vector<std::vector<std::string>> headers;

        if (mode == 'M') {

            headers.push_back(header);

            for (auto& field: headers[0])
                field = replace(field, "\n", "<br>");

        } else {

            size_t header_lines = 1;

            for (auto& field: header) {
                size_t n = std::count(field.begin(), field.end(), '\n') + 1;
                header_lines = std::max(header_lines, n);
            }

            headers.resize(header_lines);
            std::vector<std::string_view> parts;

            for (auto& field: header) {
                parts = split_v(field, "\n");
                size_t i = 0;
                for (; i < parts.size(); ++i)
                    headers[i].push_back(std::string(parts[i]));
                for (; i < header_lines; ++i)
                    headers[i].push_back({});
            }

        }

        for (auto& row: headers)
            format_row(out, row, widths, spec, true);

    }

    void Table::format_row(std::string& out, const std::vector<std::string>& row,
            const std::vector<size_t>& widths, const FormatSpec& spec, bool is_header) const {

        if (row.empty())
            return;

        char mode = get_mode(spec);
        std::string delimiter, line;
        size_t c = 0, padding = 0;

        switch (mode) {
            case 'B':  delimiter = "│ "; padding = 3; break;
            case 'M':  delimiter = "| "; padding = 4; break;
            default:   delimiter = ""; padding = 2; break;
        }

        for (; c < row.size(); ++c) {
            size_t old_len = utf_size(line, Usize::columns);
            std::string extra = delimiter;
            if (! row[c].empty())
                extra += row[c];
            else if (mode == 'T')
                extra += is_header ? header_null : body_null;
            line += extra;
            size_t new_len = old_len + widths[c] + padding;
            size_t extra_len = utf_size(extra, Usize::columns);
            line.append(new_len - old_len - extra_len, ' ');
        }

        for (; c < widths.size(); ++c) {
            size_t old_len = utf_size(line, Usize::columns);
            std::string extra = delimiter;
            if (mode == 'T') {
                if (is_header)
                    extra = header_null;
                else
                    extra = body_null;
            }
            line += extra;
            size_t new_len = old_len + widths[c] + padding;
            size_t extra_len = utf_size(extra, Usize::columns);
            line.append(new_len - old_len - extra_len, ' ');
        }

        line += delimiter;
        out += trim_right(line);
        out += '\n';

    }

}
