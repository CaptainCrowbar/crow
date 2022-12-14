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

    template <ArithmeticType T>
    class ImageMask {

    public:

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

        template <ColourType C, ImageFlags F> requires C::is_linear
            void make_image(Image<C, F>& image, C foreground, C background) const;
        template <ColourType C, ImageFlags F> requires C::is_linear
            void onto_image(Image<C, F>& image, Point offset, C colour) const;

    private:

        Point shape_;
        std::shared_ptr<T[]> ptr_;

        size_t point_to_index(Point p) const noexcept { return size_t(shape_.x()) * size_t(p.y()) + size_t(p.x()); }

        template <ColourType C> static constexpr C blend(C fg, C bg, T alpha, Pma pma) noexcept;

    };

        using ByteMask = ImageMask<unsigned char>;
        using HdrMask = ImageMask<float>;

        template <ArithmeticType T>
        ImageMask<T>::ImageMask(Point shape) noexcept:
        shape_(shape),
        ptr_(new T[area()]) {
            std::memset(ptr_.get(), 0, area() * sizeof(T));
        }

        template <ArithmeticType T>
        template <ColourType C, ImageFlags F>
        requires C::is_linear
        void ImageMask<T>::make_image(Image<C, F>& image, C foreground, C background) const {

            static constexpr Pma pma = Image<C, F>::is_premultiplied ? Pma::result : Pma::none;

            image.reset(shape());
            auto out = image.begin();

            for (T alpha: *this)
                *out++ = blend(foreground, background, alpha, pma);

        }

        template <ArithmeticType T>
        template <ColourType C, ImageFlags F>
        requires C::is_linear
        void ImageMask<T>::onto_image(Image<C, F>& image, Point offset, C colour) const {

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

        template <ArithmeticType T>
        template <ColourType C>
        constexpr C ImageMask<T>::blend(C fg, C bg, T alpha, Pma pma) noexcept {

            using V = typename C::value_type;
            using A = std::conditional_t<(sizeof(V) < sizeof(double)), float, double>;

            A a = A(alpha) / A(scale);

            if constexpr (C::has_alpha) {

                A fga1 = a * A(fg.alpha());
                V fga2;

                if constexpr (std::is_floating_point_v<V>)
                    fga2 = V(fga1);
                else
                    fga2 = V(A(C::scale) * fga1 + A(0.5));

                C modified_fg = fg;
                modified_fg.alpha() = fga2;
                C result = alpha_blend(modified_fg, bg);

                if (pma != Pma::none)
                    result.multiply_alpha();

                return result;

            } else {

                return lerp(bg, fg, a);

            }

        }

}
