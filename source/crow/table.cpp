#include "crow/table.hpp"
#include <utility>

namespace Crow {

    // Class Table

    std::string Table::str(const FormatSpec& spec) const {
        auto widths = get_widths(spec);
        std::string result;
        if (! headers_.empty())
            format_header(result, headers_, widths, spec);
        for (auto& row: cells_)
            format_row(result, row, widths, spec, false);
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

        static const size_t min_width = std::max(utf_width(std::string(body_null)), utf_width(std::string(header_null)));

        std::vector<size_t> widths(columns_, min_width);

        if (spec.mode() == 'm') {

            for (size_t c = 0; c < headers_.size(); ++c) {
                size_t n = std::count(headers_[c].begin(), headers_[c].end(), '\n');
                size_t len = utf_width(headers_[c]) + 3 * n;
                widths[c] = std::max(widths[c], len);
            }

        } else {

            std::vector<std::string> parts;

            for (size_t c = 0; c < headers_.size(); ++c) {
                parts = split(headers_[c], "\n");
                for (auto& part: parts)
                    widths[c] = std::max(widths[c], utf_width(part));
            }

        }

        for (auto& row: cells_)
            for (size_t c = 0; c < row.size(); ++c)
                widths[c] = std::max(widths[c], utf_width(row[c]));

        return widths;
    }

    void Table::format_header(std::string& out, const std::vector<std::string>& header,
            const std::vector<size_t>& widths, const FormatSpec& spec) const {

        std::vector<std::vector<std::string>> headers;

        if (spec.mode() == 'm') {

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
            std::vector<std::string> parts;

            for (auto& field: header) {
                parts = split(field, "\n");
                size_t i = 0;
                for (; i < parts.size(); ++i)
                    headers[i].push_back(parts[i]);
                for (; i < header_lines; ++i)
                    headers[i].push_back({});
            }

        }

        for (auto& row: headers)
            format_row(out, row, widths, spec, true);

        char underline = spec.mode() == 'm' ? '-' : '=';
        std::string line;

        for (auto w: widths) {
            if (spec.mode() == 'm')
                line += "| ";
            line.append(w, underline);
            line += "  ";
        }

        if (spec.mode() == 'm')
            line += '|';
        else
            line = trim_right(line);

        out += line;
        out += '\n';

    }

    void Table::format_row(std::string& out, const std::vector<std::string>& row,
            const std::vector<size_t>& widths, const FormatSpec& spec, bool is_header) const {

        if (row.empty())
            return;

        size_t padding = spec.mode() == 'm' ? 4 : 2;
        std::string line;
        size_t c = 0;

        for (; c < row.size(); ++c) {
            size_t old_len = utf_width(line);
            std::string extra;
            if (spec.mode() == 'm')
                extra = "| ";
            if (! row[c].empty())
                extra += row[c];
            else if (spec.mode() != 'm' && is_header)
                extra += header_null;
            else if (spec.mode() != 'm' && ! is_header)
                extra += body_null;
            line += extra;
            size_t new_len = old_len + widths[c] + padding;
            size_t extra_len = utf_width(extra);
            line.append(new_len - old_len - extra_len, ' ');
        }

        for (; c < widths.size(); ++c) {
            size_t old_len = utf_width(line);
            std::string extra;
            if (spec.mode() == 'm')
                extra = "| ";
            else if (is_header)
                extra = header_null;
            else
                extra = body_null;
            line += extra;
            size_t new_len = old_len + widths[c] + padding;
            size_t extra_len = utf_width(extra);
            line.append(new_len - old_len - extra_len, ' ');
        }

        if (spec.mode() == 'm')
            line += '|';
        else
            line = trim_right(line);

        out += line;
        out += '\n';

    }

}
