#pragma once

#include "crow/format-spec.hpp"
#include "crow/format-type.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <string>
#include <type_traits>
#include <vector>

namespace Crow {

    class Table:
    public Formatted {

    public:

        template <typename T> Table& operator<<(const T& t);
        template <typename... Args> Table& add(const Args&... args);
        template <typename T> Table& add_v(const std::vector<T>& args);
        template <typename... Args> Table& format(const Args&... args);
        template <typename T> Table& format_v(const std::vector<T>& args);
        template <typename... Args> Table& header(const Args&... args);
        template <typename T> Table& header_v(const std::vector<T>& args);
        bool has_header() const noexcept { return ! headers_.empty(); }
        size_t columns() const noexcept { return columns_; }
        size_t rows() const noexcept { return cells_.size(); }
        std::string str(const FormatSpec& spec) const override;

    private:

        std::vector<FormatSpec> formats_;
        std::vector<std::string> headers_;
        std::vector<std::vector<std::string>> cells_ = {{}};
        size_t columns_ = 0;

        template <typename T> void add_cell(std::vector<std::string>& row, const T& t);
        void do_add(const std::vector<std::string>& row);
        void do_format(const std::vector<std::string>& row);
        void do_header(const std::vector<std::string>& row);
        void format_header(std::string& out, const std::vector<std::string>& header,
            const std::vector<size_t>& widths, const FormatSpec& spec) const;
        void format_row(std::string& out, const std::vector<std::string>& row,
            const std::vector<size_t>& widths, const FormatSpec& spec, bool is_header) const;
        std::vector<size_t> get_widths(const FormatSpec& spec) const;
        template <typename T, typename... Args>
            void make_row(std::vector<std::string>& row, const T& t, const Args&... args);
        template <typename T> void make_row_v(std::vector<std::string>& row, const std::vector<T>& vec);

    };

        template <typename T>
        Table& Table::operator<<(const T& t) {
            if constexpr (std::is_same_v<T, char>) {
                if (t == '\n') {
                    if (! cells_.back().empty())
                        cells_.push_back({});
                    return *this;
                }
            }
            add_cell(cells_.back(), t);
            columns_ = std::max(columns_, cells_.back().size());
            return *this;
        }

        template <typename... Args>
        Table& Table::add(const Args&... args) {
            std::vector<std::string> row;
            make_row(row, args...);
            do_add(row);
            return *this;
        }

        template <typename T>
        Table& Table::add_v(const std::vector<T>& args) {
            std::vector<std::string> row;
            make_row_v(row, args);
            do_add(row);
            return *this;
        }

        template <typename... Args>
        Table& Table::format(const Args&... args) {
            std::vector<std::string> row;
            make_row(row, args...);
            do_format(row);
            return *this;
        }

        template <typename T>
        Table& Table::format_v(const std::vector<T>& args) {
            std::vector<std::string> row;
            make_row_v(row, args);
            do_format(row);
            return *this;
        }

        template <typename... Args>
        Table& Table::header(const Args&... args) {
            std::vector<std::string> row;
            make_row(row, args...);
            do_header(row);
            return *this;
        }

        template <typename T>
        Table& Table::header_v(const std::vector<T>& args) {
            std::vector<std::string> row;
            make_row_v(row, args);
            do_header(row);
            return *this;
        }

        template <typename T>
        void Table::add_cell(std::vector<std::string>& row, const T& t) {
            static const FormatSpec empty;
            auto& spec = row.size() < formats_.size() ? formats_[row.size()] : empty;
            row.push_back(format_object(t, spec));
        }

        template <typename T, typename... Args>
        void Table::make_row(std::vector<std::string>& row, const T& t, const Args&... args) {
            add_cell(row, t);
            if constexpr (sizeof...(Args) > 0)
                make_row(row, args...);
        }

        template <typename T>
        void Table::make_row_v(std::vector<std::string>& row, const std::vector<T>& vec) {
            for (auto& t: vec)
                add_cell(row, t);
        }

}
