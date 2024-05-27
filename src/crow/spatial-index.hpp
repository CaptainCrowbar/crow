#pragma once

#include "crow/algorithm.hpp"
#include "crow/hash.hpp"
#include "crow/iterator.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <cmath>
#include <compare>
#include <concepts>
#include <limits>
#include <numeric>
#include <optional>
#include <stdexcept>
#include <unordered_map>
#include <utility>
#include <vector>

namespace Crow {

    template <RegularHashable T, std::floating_point S, int N>
    requires (N > 0)
    class SpatialIndex {

    public:

        using object_type = T;
        using scalar_type = S;
        using vector_type = Vector<S, N>;

    private:

        static_assert(N > 0);

        using index_type = int64_t;
        using cell_index_vector = Vector<index_type, N>;
        using object_position_map_type = std::unordered_map<T, vector_type>;
        using object_position_map_iterator = typename object_position_map_type::iterator;
        using cell_object_map_type = std::unordered_map<cell_index_vector, object_position_map_type>;
        using cell_object_map_iterator = typename cell_object_map_type::iterator;

        struct object_iterator_pair {
            cell_object_map_iterator cell_obj_it;
            object_position_map_iterator obj_pos_it;
        };

        using object_cell_map_type = std::unordered_map<T, object_iterator_pair>;
        using object_cell_map_const_iterator = typename object_cell_map_type::const_iterator;

    public:

        using value_type = typename object_position_map_type::value_type;

        class iterator:
        public ForwardIterator<iterator, const value_type> {
        public:
            iterator() = default;
            const value_type& operator*() const noexcept { return *obj_cell_iter_->second.obj_pos_it; }
            iterator& operator++() { ++obj_cell_iter_; return *this; }
            bool operator==(const iterator& it) const noexcept { return obj_cell_iter_ == it.obj_cell_iter_; }
        private:
            friend class SpatialIndex;
            const SpatialIndex* space_;
            object_cell_map_const_iterator obj_cell_iter_;
            iterator(const SpatialIndex& s, object_cell_map_const_iterator obj_cell_it): space_(&s), obj_cell_iter_(obj_cell_it) {}
        };

        static constexpr int dimension = N;

        SpatialIndex(): SpatialIndex(1) {}
        explicit SpatialIndex(S scale);

        bool contains(const T& t) const { return obj_cell_map_.contains(t); }
        bool empty() const noexcept { return obj_cell_map_.empty(); }
        size_t size() const noexcept { return obj_cell_map_.size(); }
        S scale() const noexcept { return scale_; }
        S outer_limit() const noexcept { return outer_limit_; }

        iterator begin() const { return {*this, obj_cell_map_.begin()}; }
        iterator end() const { return {*this, obj_cell_map_.end()}; }
        iterator find(const T& t) const { return {*this, obj_cell_map_.find(t)}; }
        std::optional<vector_type> position_of(const T& t) const;

        std::pair<iterator, bool> insert(const T& t, vector_type pos);
        std::pair<iterator, bool> update(const T& t, vector_type pos);
        bool erase(const T& t) noexcept;
        void erase(iterator it) noexcept;
        void clear() noexcept;

        std::vector<iterator> search(vector_type origin, S radius) const;
        std::vector<iterator> search(const T& origin, S radius) const;
        std::vector<iterator> search(iterator origin, S radius) const;

    private:

        cell_object_map_type cell_obj_map_;
        object_cell_map_type obj_cell_map_;
        S scale_;
        S outer_limit_;
        S r2_limit_;

        cell_index_vector pos_to_cell_index(vector_type pos) const;

    };

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        SpatialIndex<T, S, N>::SpatialIndex(S scale):
        scale_(scale),
        outer_limit_(scale * S(std::numeric_limits<index_type>::max())),
        r2_limit_(outer_limit_ * outer_limit_) {}

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        std::optional<typename SpatialIndex<T, S, N>::vector_type> SpatialIndex<T, S, N>::position_of(const T& t) const {
            auto obj_cell_it = obj_cell_map_.find(t);
            if (obj_cell_it == obj_cell_map_.end())
                return {};
            else
                return obj_cell_it->second.obj_pos_it->second;
        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        std::pair<typename SpatialIndex<T, S, N>::iterator, bool> SpatialIndex<T, S, N>::insert(const T& t, vector_type pos) {

            if (pos.r2() > r2_limit_)
                throw std::out_of_range("Spatial index entry is out of range");

            auto obj_cell_it = obj_cell_map_.find(t);

            if (obj_cell_it != obj_cell_map_.end())
                return {{*this, obj_cell_it}, false};

            auto cell_index = pos_to_cell_index(pos);
            auto cell_obj_it = cell_obj_map_.find(cell_index);

            if (cell_obj_it == cell_obj_map_.end())
                cell_obj_it = cell_obj_map_.insert({cell_index, {}}).first;

            auto obj_pos_it = cell_obj_it->second.insert({t, pos}).first;
            object_iterator_pair obj_its = {cell_obj_it, obj_pos_it};
            obj_cell_it = obj_cell_map_.insert({t, obj_its}).first;

            return {{*this, obj_cell_it}, true};

        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        std::pair<typename SpatialIndex<T, S, N>::iterator, bool> SpatialIndex<T, S, N>::update(const T& t, vector_type pos) {

            if (pos.r2() > r2_limit_)
                throw std::out_of_range("Spatial index entry is out of range");

            auto new_index = pos_to_cell_index(pos);
            auto obj_cell_it = obj_cell_map_.find(t);

            // If this is a new object
            if (obj_cell_it == obj_cell_map_.end()) {
                auto cell_obj_it = cell_obj_map_.find(new_index);
                if (cell_obj_it == cell_obj_map_.end())
                    cell_obj_it = cell_obj_map_.insert({new_index, {}}).first;
                auto obj_pos_it = cell_obj_it->second.insert({t, pos}).first;
                object_iterator_pair obj_its = {cell_obj_it, obj_pos_it};
                obj_cell_it = obj_cell_map_.insert({t, obj_its}).first;
                return {{*this, obj_cell_it}, true};
            }

            auto old_obj_pos_it = obj_cell_it->second.obj_pos_it;
            auto old_pos = old_obj_pos_it->second;

            // If position has not changed
            if (old_pos == pos)
                return {{*this, obj_cell_it}, false};

            auto old_cell_obj_it = obj_cell_it->second.cell_obj_it;
            auto old_index = old_cell_obj_it->first;

            // If position has changed but still within the same cell
            if (old_index == new_index) {
                old_obj_pos_it->second = pos;
                return {{*this, obj_cell_it}, false};
            }

            // Object has moved to a different cell
            auto& old_obj_pos_map = old_cell_obj_it->second;
            auto new_cell_obj_it = cell_obj_map_.find(new_index);
            if (new_cell_obj_it == cell_obj_map_.end())
                new_cell_obj_it = cell_obj_map_.insert({new_index, {}}).first;
            auto& new_obj_pos_map = new_cell_obj_it->second;
            auto new_obj_pos_it = new_obj_pos_map.insert({t, pos}).first;
            old_obj_pos_map.erase(t);
            if (old_obj_pos_map.empty())
                cell_obj_map_.erase(old_cell_obj_it);
            obj_cell_it->second = {new_cell_obj_it, new_obj_pos_it};
            return {{*this, obj_cell_it}, false};

        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        bool SpatialIndex<T, S, N>::erase(const T& t) noexcept {
            auto it = find(t);
            if (it == end())
                return false;
            erase(it);
            return true;
        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        void SpatialIndex<T, S, N>::erase(iterator it) noexcept {
            auto obj_its = it.obj_cell_iter_->second;
            auto& obj_pos_map = obj_its.cell_obj_it->second;
            obj_pos_map.erase(obj_its.obj_pos_it);
            if (obj_pos_map.empty())
                cell_obj_map_.erase(obj_its.cell_obj_it);
            obj_cell_map_.erase(it.obj_cell_iter_);
        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        void SpatialIndex<T, S, N>::clear() noexcept {
            cell_obj_map_.clear();
            obj_cell_map_.clear();
        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        std::vector<typename SpatialIndex<T, S, N>::iterator>
        SpatialIndex<T, S, N>::search(vector_type origin, S radius) const {

            auto origin_r2 = origin.r2();
            auto max_r2 = radius * radius;
            std::vector<iterator> results;

            auto try_entry = [this,&results,max_r2,origin] (const T& obj, vector_type vec) {
                if ((vec - origin).r2() <= max_r2) {
                    auto obj_cell_it = obj_cell_map_.find(obj);
                    iterator it(*this, obj_cell_it);
                    results.push_back(it);
                }
            };

            if (origin_r2 <= r2_limit_ && radius <= outer_limit_) {

                auto origin_cell = pos_to_cell_index(origin);
                auto max_offset_scalar = std::ceil(radius / scale_);
                auto max_offset = index_type(max_offset_scalar);
                auto max_cell_r2 = max_offset_scalar * max_offset_scalar;
                std::vector<index_type> linear_offsets(size_t(2 * max_offset + 1));
                std::iota(linear_offsets.begin(), linear_offsets.end(), - max_offset);
                auto cell_offsets = cartesian_power(linear_offsets, N);

                for (auto& cell_ofs: cell_offsets) {

                    S cell_r2 = 0;

                    for (auto n: cell_ofs) {
                        auto s = S(n);
                        cell_r2 += s * s;
                    }

                    if (cell_r2 > max_cell_r2)
                        continue;

                    cell_index_vector cell_index;

                    for (int i = 0; i < N; ++i)
                        cell_index[i] = origin_cell[i] + cell_ofs[i];

                    auto cell_obj_it = cell_obj_map_.find(cell_index);

                    if (cell_obj_it == cell_obj_map_.end())
                        continue;

                    for (auto& [obj,vec]: cell_obj_it->second)
                        try_entry(obj, vec);

                }

            } else {

                for (auto& [cell,obj_pos_map]: cell_obj_map_)
                    for (auto& [obj,vec]: obj_pos_map)
                        try_entry(obj, vec);

            }

            return results;

        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        std::vector<typename SpatialIndex<T, S, N>::iterator>
        SpatialIndex<T, S, N>::search(const T& origin, S radius) const {
            auto obj_cell_it = obj_cell_map_.find(origin);
            if (obj_cell_it == obj_cell_map_.end())
                return {};
            vector_type pos = obj_cell_it->second.obj_pos_it->second;
            return search(pos, radius);
        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        std::vector<typename SpatialIndex<T, S, N>::iterator>
        SpatialIndex<T, S, N>::search(iterator origin, S radius) const {
            return search(origin->second, radius);
        }

        template <RegularHashable T, std::floating_point S, int N>
        requires (N > 0)
        typename SpatialIndex<T, S, N>::cell_index_vector
        SpatialIndex<T, S, N>::pos_to_cell_index(vector_type pos) const {
            cell_index_vector cell_index;
            for (int i = 0; i < N; ++i)
                cell_index[i] = index_type(std::floor(pos[i] / scale_));
            return cell_index;
        }

}
