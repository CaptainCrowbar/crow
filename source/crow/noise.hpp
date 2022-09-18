#pragma once

#include "crow/maths.hpp"
#include "crow/types.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <array>
#include <cmath>
#include <cstdlib>
#include <type_traits>

namespace Crow {

    namespace Detail {

        template <typename T>
        constexpr int fast_floor(T x) noexcept {
            int i = int(x);
            return i - int(x < T(i));
        }

        constexpr uint64_t lcg64_1(uint64_t x) noexcept {
            return x * 6'364'136'223'846'793'005ull + 1'442'695'040'888'963'407ull;
        }

        constexpr uint64_t lcg64_2(uint64_t x) noexcept {
            return x * 3'935'559'000'370'003'845ull + 8'831'144'850'135'198'739ull;
        }

    }

    // Noise class template

    template <typename T, int N> class Noise;

    // 2D noise

    template <typename T>
    class Noise<T, 2> {

    public:

        static_assert(std::is_floating_point_v<T>);

        using scalar_type = T;
        using seed_type = uint64_t;
        using vector_type = Vector<T, 2>;

        static constexpr int dim = 2;

        Noise() = default;
        explicit Noise(uint64_t s) noexcept { seed(s); }

        T operator()(const vector_type& point) const noexcept;
        void seed(uint64_t s) noexcept;

    private:

        static constexpr int psize = 2048;
        static constexpr int pmask = psize - 1;
        static constexpr T scale1 = T(0.366'025'403'8);
        static constexpr T scale2 = T(0.211'324'865'4);

        struct grad { T dx, dy; };

        struct grad_table {
            std::array<grad, psize> grads;
            grad_table() noexcept;
        };

        struct lattice_point {

            int xsv, ysv;
            T dx, dy;

            lattice_point() = default;

            lattice_point(int x, int y) noexcept {
                xsv = x;
                ysv = y;
                T ssv = - T(x + y) * scale2;
                dx = - T(x) - ssv;
                dy = - T(y) - ssv;
            }

        };

        struct lattice_table {
            std::array<lattice_point, 32> points;
            lattice_table() noexcept;
        };

        std::array<int, psize> perm_;
        std::array<grad, psize> grads_;

    };

        template <typename T>
        void Noise<T, 2>::seed(uint64_t s) noexcept {

            using namespace Detail;

            static const grad_table gtable;

            int source[psize];

            for (int i = 0; i < psize; i++)
                source[i] = i;

            for (int i = psize - 1; i >= 0; i--) {
                s = lcg64_1(s);
                int r = int((s + 31) % (i + 1));
                perm_[i] = source[r];
                grads_[i] = gtable.grads[perm_[i]];
                source[r] = source[i];
            }

            (*this)(vector_type(T(0)));

        }

        template <typename T>
        T Noise<T, 2>::operator()(const vector_type& point) const noexcept {

            using namespace Detail;

            static const lattice_table lut;

            T s = scale1 * (point.x() + point.y());
            T xs = point.x() + s;
            T ys = point.y() + s;
            T value = T(0);

            int xsb = fast_floor(xs);
            int ysb = fast_floor(ys);
            T xsi = xs - T(xsb);
            T ysi = ys - T(ysb);
            int a = int(xsi + ysi);

            int index = (a << 2)
                | (int(xsi - ysi / 2 + 1 - T(a) / 2) << 3)
                | (int(ysi - xsi / 2 + 1 - T(a) / 2) << 4);

            T ssi = - (xsi + ysi) * scale2;
            T xi = xsi + ssi, yi = ysi + ssi;

            for (int i = 0; i < 4; i++) {

                auto& c = lut.points[index + i];

                T dx = xi + c.dx;
                T dy = yi + c.dy;
                T attn = T(2) / T(3) - dx * dx - dy * dy;

                if (attn > 0) {

                    int pxm = (xsb + c.xsv) & pmask;
                    int pym = (ysb + c.ysv) & pmask;

                    grad g = grads_[perm_[pxm] ^ pym];
                    T extrapolation = g.dx * dx + g.dy * dy;
                    attn *= attn;
                    value += attn * attn * extrapolation;

                }

            }

            return value;

        }

        template <typename T>
        Noise<T, 2>::grad_table::grad_table() noexcept {

            static constexpr int n = 24;
            static constexpr T g1 = T(2.381'053'830);
            static constexpr T g2 = T(6.980'896'609);
            static constexpr T g3 = T(11.105'002'818);
            static constexpr T g4 = T(14.472'321'439);
            static constexpr T g5 = T(16.853'375'269);
            static constexpr T g6 = T(18.085'899'427);

            grad table[n] = {
                { + g1, + g6 }, { + g2, + g5 }, { + g3, + g4 }, { + g4, + g3 }, { + g5, + g2 }, { + g6, + g1 },
                { + g6, - g1 }, { + g5, - g2 }, { + g4, - g3 }, { + g3, - g4 }, { + g2, - g5 }, { + g1, - g6 },
                { - g1, - g6 }, { - g2, - g5 }, { - g3, - g4 }, { - g4, - g3 }, { - g5, - g2 }, { - g6, - g1 },
                { - g6, + g1 }, { - g5, + g2 }, { - g4, + g3 }, { - g3, + g4 }, { - g2, + g5 }, { - g1, + g6 },
            };

            for (int i = 0; i < psize; i++)
                grads[i] = table[i % n];

        }

        template <typename T>
        Noise<T, 2>::lattice_table::lattice_table() noexcept {

            int i1, j1, i2, j2;

            for (int i = 0; i < 8; i++) {

                if ((i & 1) == 0) {
                    if ((i & 2) == 0) {
                        i1 = -1;
                        j1 = 0;
                    } else {
                        i1 = 1;
                        j1 = 0;
                    }
                    if ((i & 4) == 0) {
                        i2 = 0;
                        j2 = -1;
                    } else {
                        i2 = 0;
                        j2 = 1;
                    }
                } else {
                    if ((i & 2) != 0) {
                        i1 = 2;
                        j1 = 1;
                    } else {
                        i1 = 0;
                        j1 = 1;
                    }
                    if ((i & 4) != 0) {
                        i2 = 1;
                        j2 = 2;
                    } else {
                        i2 = 1;
                        j2 = 0;
                    }
                }

                points[4 * i] = {0, 0};
                points[4 * i + 1] = {1, 1};
                points[4 * i + 2] = {i1, j1};
                points[4 * i + 3] = {i2, j2};

            }

        }

    // 3D noise

    template <typename T>
    class Noise<T, 3> {

    public:

        static_assert(std::is_floating_point_v<T>);

        using scalar_type = T;
        using seed_type = uint64_t;
        using vector_type = Vector<T, 3>;

        static constexpr int dim = 3;

        Noise() = default;
        explicit Noise(uint64_t s) noexcept { seed(s); }

        T operator()(const vector_type& point) const noexcept;
        void seed(uint64_t s) noexcept;

    private:

        static constexpr int psize = 2048;
        static constexpr int pmask = psize - 1;

        struct grad { T dx, dy, dz; };

        struct lattice_point {

            T dxr, dyr, dzr;
            int xrv, yrv, zrv;
            int fail, succ;

            lattice_point() = default;

            lattice_point(int x, int y, int z, int lattice) noexcept {
                dxr = - T(x) + T(lattice) / T(2);
                dyr = - T(y) + T(lattice) / T(2);
                dzr = - T(z) + T(lattice) / T(2);
                xrv = x + lattice * 1024;
                yrv = y + lattice * 1024;
                zrv = z + lattice * 1024;
            }

        };

        struct grad_table {
            std::array<grad, psize> grads;
            grad_table() noexcept;
        };

        struct lattice_table {
            std::array<lattice_point, 112> points;
            lattice_table() noexcept;
        };

        std::array<int, psize> perm_;
        std::array<grad, psize> grads_;

    };

        template <typename T>
        void Noise<T, 3>::seed(uint64_t s) noexcept {

            using namespace Detail;

            static const grad_table gtable;

            int source[psize];

            for (int i = 0; i < psize; i++)
                source[i] = i;

            for (int i = psize - 1; i >= 0; i--) {
                s = lcg64_1(s);
                int r = int((s + 31) % (i + 1));
                perm_[i] = source[r];
                grads_[i] = gtable.grads[perm_[i]];
                source[r] = source[i];
            }

            (*this)(vector_type(T(0)));

        }

        template <typename T>
        T Noise<T, 3>::operator()(const vector_type& point) const noexcept {

            using namespace Detail;

            static const lattice_table lut;

            T r = T(2) / T(3) * (point.x() + point.y() + point.z());
            T xr = r - point.x();
            T yr = r - point.y();
            T zr = r - point.z();

            int xrb = fast_floor(xr);
            int yrb = fast_floor(yr);
            int zrb = fast_floor(zr);

            T xri = xr - T(xrb);
            T yri = yr - T(yrb);
            T zri = zr - T(zrb);

            int xht = int(xri + T(0.5));
            int yht = int(yri + T(0.5));
            int zht = int(zri + T(0.5));

            int index = xht | (yht << 1) | (zht << 2);
            T value = T(0);
            int ci = 14 * index;

            while (ci != -1) {

                auto& c = lut.points[ci];

                T dxr = xri + c.dxr;
                T dyr = yri + c.dyr;
                T dzr = zri + c.dzr;
                T attn = T(0.75) - dxr * dxr - dyr * dyr - dzr * dzr;

                if (attn < T(0)) {

                    ci = c.fail;

                } else {

                    int pxm = (xrb + c.xrv) & pmask;
                    int pym = (yrb + c.yrv) & pmask;
                    int pzm = (zrb + c.zrv) & pmask;

                    grad g = grads_[perm_[perm_[pxm] ^ pym] ^ pzm];
                    T extrapolation = g.dx * dxr + g.dy * dyr + g.dz * dzr;
                    attn *= attn;
                    value += attn * attn * extrapolation;
                    ci = c.succ;

                }

            }

            return value;

        }

        template <typename T>
        Noise<T, 3>::grad_table::grad_table() noexcept {

            static constexpr int n = 48;
            static constexpr T g1 = T(3.594'631'769);
            static constexpr T g2 = T(4.213'452'452);
            static constexpr T g3 = T(7.997'138'591);
            static constexpr T g4 = T(11.093'991'497);

            static constexpr grad table[n] = {
                { - g3, - g3, - g1 }, { - g3, - g3, + g1 }, { - g4, - g2, T(0) }, { - g2, - g4, T(0) },
                { - g3, - g1, - g3 }, { - g3, + g1, - g3 }, { - g2, T(0), - g4 }, { - g4, T(0), - g2 },
                { - g3, - g1, + g3 }, { - g3, + g1, + g3 }, { - g4, T(0), + g2 }, { - g2, T(0), + g4 },
                { - g3, + g3, - g1 }, { - g3, + g3, + g1 }, { - g2, + g4, T(0) }, { - g4, + g2, T(0) },
                { - g1, - g3, - g3 }, { + g1, - g3, - g3 }, { T(0), - g4, - g2 }, { T(0), - g2, - g4 },
                { - g1, - g3, + g3 }, { + g1, - g3, + g3 }, { T(0), - g2, + g4 }, { T(0), - g4, + g2 },
                { - g1, + g3, - g3 }, { + g1, + g3, - g3 }, { T(0), + g2, - g4 }, { T(0), + g4, - g2 },
                { - g1, + g3, + g3 }, { + g1, + g3, + g3 }, { T(0), + g4, + g2 }, { T(0), + g2, + g4 },
                { + g3, - g3, - g1 }, { + g3, - g3, + g1 }, { + g2, - g4, T(0) }, { + g4, - g2, T(0) },
                { + g3, - g1, - g3 }, { + g3, + g1, - g3 }, { + g4, T(0), - g2 }, { + g2, T(0), - g4 },
                { + g3, - g1, + g3 }, { + g3, + g1, + g3 }, { + g2, T(0), + g4 }, { + g4, T(0), + g2 },
                { + g3, + g3, - g1 }, { + g3, + g3, + g1 }, { + g4, + g2, T(0) }, { + g2, + g4, T(0) },
            };

            for (int i = 0; i < psize; i++)
                grads[i] = table[i % n];

        }

        template <typename T>
        Noise<T, 3>::lattice_table::lattice_table() noexcept {

            for (int i = 0; i < 8; i++) {

                int i1 = (i >> 0) & 1;
                int i2 = i1 ^ 1;
                int j1 = (i >> 1) & 1;
                int j2 = j1 ^ 1;
                int k1 = (i >> 2) & 1;
                int k2 = k1 ^ 1;

                int ci = 14 * i;
                auto c = points.begin() + ci;

                c[0] = {i1, j1, k1, 0};
                c[1] = {i1 + i2, j1 + j2, k1 + k2, 1};
                c[2] = {i1 ^ 1, j1, k1, 0};
                c[3] = {i1, j1 ^ 1, k1 ^ 1, 0};
                c[4] = {i1 + (i2 ^ 1), j1 + j2, k1 + k2, 1};
                c[5] = {i1 + i2, j1 + (j2 ^ 1), k1 + (k2 ^ 1), 1};
                c[6] = {i1, j1 ^ 1, k1, 0};
                c[7] = {i1 ^ 1, j1, k1 ^ 1, 0};
                c[8] = {i1 + i2, j1 + (j2 ^ 1), k1 + k2, 1};
                c[9] = {i1 + (i2 ^ 1), j1 + j2, k1 + (k2 ^ 1), 1};
                c[10] = {i1, j1, k1 ^ 1, 0};
                c[11] = {i1 ^ 1, j1 ^ 1, k1, 0};
                c[12] = {i1 + i2, j1 + j2, k1 + (k2 ^ 1), 1};
                c[13] = {i1 + (i2 ^ 1), j1 + (j2 ^ 1), k1 + k2, 1};

                c[0].fail = c[0].succ = ci + 1;
                c[1].fail = c[1].succ = ci + 2;
                c[2].fail = ci + 3;
                c[2].succ = ci + 5;
                c[3].fail = ci + 4;
                c[3].succ = ci + 4;
                c[4].fail = ci + 5;
                c[4].succ = ci + 6;
                c[5].fail = c[5].succ = ci + 6;
                c[6].fail = ci + 7;
                c[6].succ = ci + 9;
                c[7].fail = ci + 8;
                c[7].succ = ci + 8;
                c[8].fail = ci + 9;
                c[8].succ = ci + 10;
                c[9].fail = c[9].succ = ci + 10;
                c[10].fail = ci + 11;
                c[10].succ = ci + 13;
                c[11].fail = ci + 12;
                c[11].succ = ci + 12;
                c[12].fail = ci + 13;
                c[12].succ = c[13].fail = c[13].succ = -1;

            }

        }

    // Generalised noise source

    template <typename T, int DimIn, int DimOut>
    class NoiseSource {

    public:

        static_assert(std::is_floating_point_v<T>);
        static_assert(DimIn >= 1);
        static_assert(DimOut >= 1);

        using scalar_type = T;
        using seed_type = uint64_t;
        using domain_type = std::conditional_t<DimIn == 1, T, Vector<T, DimIn>>;
        using result_type = std::conditional_t<DimOut == 1, T, Vector<T, DimOut>>;

        static constexpr int dim_in = DimIn;
        static constexpr int dim_out = DimOut;

        NoiseSource() = default;
        NoiseSource(T cell, T scale, int octaves, uint64_t seed) noexcept;

        result_type operator()(domain_type point) const noexcept;

        T cell() const noexcept { return cell_; }
        void cell(T size) noexcept { cell_ = std::abs(size); }
        int octaves() const noexcept { return octaves_; }
        void octaves(int n) noexcept { octaves_ = n; }
        T scale() const noexcept { return scale_; }
        void scale(T factor) noexcept { scale_ = std::abs(factor); }
        void seed(uint64_t s) noexcept;

    private:

        using noise_type = Noise<T, std::max(DimIn, 2)>;
        using input_vector = typename noise_type::vector_type;

        std::array<noise_type, DimOut> generators_;
        T cell_ = 1;
        T scale_ = 1;
        int octaves_ = 1;

    };

        template <typename T, int DimIn, int DimOut>
        NoiseSource<T, DimIn, DimOut>::NoiseSource(T cell, T scale, int octaves, uint64_t s) noexcept:
        generators_(), cell_(std::abs(cell)), scale_(std::abs(scale)), octaves_(octaves) {
            seed(s);
        }

        template <typename T, int DimIn, int DimOut>
        typename NoiseSource<T, DimIn, DimOut>::result_type NoiseSource<T, DimIn, DimOut>::operator()(domain_type point) const noexcept {

            point /= cell_;

            input_vector in;
            if constexpr (DimIn == 1)
                in = {point, T(0)};
            else
                in = point;

            result_type out(T(0));
            T s = scale_;

            for (int i = 0; i < octaves_; ++i, in *= 2, s /= 2) {
                if constexpr (DimOut == 1) {
                    out += s * generators_[0](in);
                } else {
                    for (int j = 0; j < DimOut; ++j)
                        out[j] += s * generators_[j](in);
                }
            }

            return out;

        }

        template <typename T, int DimIn, int DimOut>
        void NoiseSource<T, DimIn, DimOut>::seed(uint64_t s) noexcept {
            using namespace Detail;
            for (auto& gen: generators_) {
                gen.seed(s);
                s = lcg64_2(s);
            }
        }

}
