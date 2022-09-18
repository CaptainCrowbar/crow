#pragma once

// This header is not part of the public interface

#include "crow/colour.hpp"
#include "crow/image.hpp"
#include "crow/types.hpp"
#include <algorithm>
#include <cstring>
#include <limits>
#include <memory>
#include <type_traits>

namespace Crow::Detail {

    template <typename T>
    class ImageMask {

    public:

        static_assert(std::is_arithmetic_v<T>);

        using value_type = T;

        static constexpr T scale = std::is_floating_point_v<T> ? 1 : std::numeric_limits<T>::max();

        ImageMask() = default;
        explicit ImageMask(Point shape) noexcept;
        template <typename Del> ImageMask(Point shape, T* ptr, Del del) noexcept: shape_(shape), ptr_(ptr, del) {}

        T& operator[](Point p) noexcept { return ptr_.get()[point_to_index(p)]; }
        const T& operator[](Point p) const noexcept { return ptr_.get()[point_to_index(p)]; }

        T* begin() noexcept { return ptr_.get(); }
        const T* begin() const noexcept { return ptr_.get(); }
        T* end() noexcept { return begin() + area(); }
        const T* end() const noexcept { return begin() + area(); }
        size_t area() const noexcept { return size_t(shape_.x()) * size_t(shape_.y()); }
        bool empty() const noexcept { return ! ptr_ || shape_.x() <= 0 || shape_.y() <= 0; }
        Point shape() const noexcept { return shape_; }

        template <typename C, ImageFlags F> void make_image(Image<C, F>& image, C foreground, C background) const;
        template <typename C, ImageFlags F> void onto_image(Image<C, F>& image, Point offset, C colour) const;

    private:

        Point shape_;
        std::shared_ptr<T[]> ptr_;

        size_t point_to_index(Point p) const noexcept { return size_t(shape_.x()) * size_t(p.y()) + size_t(p.x()); }

        template <typename C> static constexpr C blend(C fg, C bg, T alpha, Pma pma) noexcept;

    };

        using ByteMask = ImageMask<unsigned char>;
        using HdrMask = ImageMask<float>;

        template <typename T>
        ImageMask<T>::ImageMask(Point shape) noexcept:
        shape_(shape),
        ptr_(new T[area()]) {
            std::memset(ptr_.get(), 0, area() * sizeof(T));
        }

        template <typename T>
        template <typename C, ImageFlags F>
        void ImageMask<T>::make_image(Image<C, F>& image, C foreground, C background) const {

            static_assert(C::is_linear);

            static constexpr Pma pma = Image<C, F>::is_premultiplied ? Pma::result : Pma::none;

            image.reset(shape());
            auto out = image.begin();

            for (T alpha: *this)
                *out++ = blend(foreground, background, alpha, pma);

        }

        template <typename T>
        template <typename C, ImageFlags F>
        void ImageMask<T>::onto_image(Image<C, F>& image, Point offset, C colour) const {

            static_assert(C::is_linear);

            static constexpr Pma pma = Image<C, F>::is_premultiplied ? Pma::second | Pma::result : Pma::none;

            int mask_x1 = std::max(0, - offset.x());
            int mask_y1 = std::max(0, - offset.y());
            int mask_x2 = std::min(shape().x(), image.width() - offset.x());
            int mask_y2 = std::min(shape().y(), image.height() - offset.y());

            if (mask_x1 >= mask_x2 || mask_y1 >= mask_y2)
                return;

            int image_x1 = mask_x1 + offset.x();
            int image_y1 = mask_y1 + offset.y();
            Point p; // point on mask
            Point q; // point on image

            for (p.y() = mask_y1, q.y() = image_y1; p.y() < mask_y2; ++p.y(), ++q.y())
                for (p.x() = mask_x1, q.x() = image_x1; p.x() < mask_x2; ++p.x(), ++q.x())
                    image[q] = blend(colour, image[q], (*this)[p], pma);

        }

        template <typename T>
        template <typename C>
        constexpr C ImageMask<T>::blend(C fg, C bg, T alpha, Pma pma) noexcept {

            using value_type = typename C::value_type;
            using alpha_type = std::conditional_t<(sizeof(value_type) < sizeof(double)), float, double>;

            alpha_type a = alpha_type(alpha) / alpha_type(scale);

            if constexpr (C::has_alpha) {

                alpha_type fga1 = a * alpha_type(fg.alpha());
                value_type fga2;

                if constexpr (std::is_floating_point_v<value_type>)
                    fga2 = value_type(fga1);
                else
                    fga2 = value_type(alpha_type(C::scale) * fga1 + alpha_type(0.5));

                C modified_fg = fg;
                modified_fg.alpha() = fga2;
                C result = alpha_blend(modified_fg, bg);

                if (!! pma)
                    result.multiply_alpha();

                return result;

            } else {

                return lerp(bg, fg, a);

            }

        }

}
