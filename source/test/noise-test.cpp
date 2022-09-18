#include "crow/noise.hpp"
#include "crow/colour.hpp"
#include "crow/format.hpp"
#include "crow/thread.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <algorithm>
#include <cmath>
#include <iostream>
#include <random>
#include <string>
#include <vector>

#define STB_IMAGE_WRITE_IMPLEMENTATION
#define STB_IMAGE_WRITE_STATIC
#define STBIW_WINDOWS_UTF8

#ifdef _MSC_VER
    #pragma warning(push, 1)
#else
    #pragma GCC diagnostic push
    #pragma GCC diagnostic ignored "-Wmissing-field-initializers"
    #pragma GCC diagnostic ignored "-Wsign-compare"
    #pragma GCC diagnostic ignored "-Wunused-function"
    #ifndef __clang__
        #pragma GCC diagnostic ignored "-Wunused-but-set-variable"
    #endif
#endif

#include "crow/stb/stb_image_write.h"

#ifdef _MSC_VER
    #pragma warning(pop)
#else
    #pragma GCC diagnostic pop
#endif

using namespace Crow;

void test_crow_noise_result_stability() {

    static constexpr float f_epsilon = 1e-4f;
    static constexpr double d_epsilon = 1e-8;

    static const std::vector<std::vector<double>> test_vector_2d = {

        { -28.5355,  -73.4389,  0.39194867   },
        { 73.3876,   36.4311,   0.58115131   },
        { 35.9149,   -1.5072,   -0.78409477  },
        { -34.3931,  23.2193,   0.39415403   },
        { 81.8252,   45.0107,   -0.03144245  },
        { 81.1025,   -43.8109,  -0.23509402  },
        { -49.2018,  56.1008,   0.49857618   },
        { 4.4534,    8.2275,    -0.50045686  },
        { 72.9411,   -74.5786,  0.01303271   },
        { -31.2419,  -20.8317,  -0.13066982  },
        { -96.6802,  57.2663,   0.08510124   },
        { -81.8596,  12.5733,   0.26993336   },
        { -36.9500,  66.0975,   -0.23117373  },
        { -82.3116,  -87.3939,  0.57250769   },
        { 53.7363,   -61.3454,  0.23131742   },
        { 26.2652,   17.2129,   -0.40218309  },
        { -12.3838,  41.9269,   -0.04623149  },
        { 73.3050,   -82.2935,  0.24485741   },
        { -95.3398,  -87.7491,  0.02589214   },
        { -60.6784,  36.9808,   -0.47103996  },

    };

    static const std::vector<std::vector<double>> test_vector_3d = {

        { 88.0504,   15.7814,   7.2700,    0.00660470   },
        { 31.3617,   -79.3178,  96.5998,   0.24684971   },
        { -57.9663,  61.7691,   -83.9477,  -0.61071236  },
        { 77.3470,   24.6310,   -33.1768,  0.04580475   },
        { 64.0124,   36.0570,   32.9805,   -0.37864779  },
        { 68.5398,   78.4282,   -63.2553,  -0.63772111  },
        { 8.2417,    -69.2370,  31.1080,   -0.58061609  },
        { -59.1477,  66.5690,   66.4941,   0.05583147   },
        { -44.9487,  24.4537,   -39.3939,  -0.20347572  },
        { -64.2558,  -76.5211,  -74.4440,  0.54988045   },
        { 54.3010,   -75.8743,  -8.7747,   -0.44203334  },
        { 47.9434,   18.1871,   -63.3310,  -0.08648390  },
        { 37.1204,   49.4081,   -76.3128,  -0.43328464  },
        { 55.0131,   -60.4002,  15.6761,   0.75791074   },
        { -61.9442,  8.8909,    -20.4062,  -0.38368981  },
        { -65.9503,  54.4693,   -3.1624,   -0.49519853  },
        { 65.8843,   -81.2461,  19.5836,   -0.54544712  },
        { -93.1719,  -67.8707,  -67.9473,  0.46054053   },
        { 79.5371,   83.7737,   39.1863,   -0.22374835  },
        { -81.0517,  53.4438,   -35.7431,  0.61037732   },

    };

    Noise<float, 2> noise2f(42);
    Noise<float, 3> noise3f(42);
    Noise<double, 2> noise2d(42);
    Noise<double, 3> noise3d(42);

    float f_result = 0.0f;
    double d_result = 0.0;

    for (auto& row: test_vector_2d) {
        TRY(f_result = noise2f({float(row[0]), float(row[1])}));
        TEST_NEAR(f_result, float(row[2]), f_epsilon);
    }

    for (auto& row: test_vector_3d) {
        TRY(f_result = noise3f({float(row[0]), float(row[1]), float(row[2])}));
        TEST_NEAR(f_result, float(row[3]), f_epsilon);
    }

    for (auto& row: test_vector_2d) {
        TRY(d_result = noise2d({row[0], row[1]}));
        TEST_NEAR(d_result, row[2], d_epsilon);
    }

    for (auto& row: test_vector_3d) {
        TRY(d_result = noise3d({row[0], row[1], row[2]}));
        TEST_NEAR(d_result, row[3], d_epsilon);
    }

}

void test_crow_noise_multiple_sources() {

    NoiseSource<double, 3, 1> source31;
    NoiseSource<double, 2, 2> source22;
    Double3 point3;
    Double2 point2, out2;
    double x = 0, y = 0, z = 0, ysum = 0, zsum = 0;
    int n = 10'000;

    TRY(source31.cell(100));
    TRY(source31.scale(25));
    TRY(source31.octaves(10));
    TRY(source31.seed(86));

    for (int i = 0; i < n; ++i) {
        x = i;
        point3 = {x, x + 1, x + 2};
        TRY(y = source31(point3));
        TEST(y > -50);
        TEST(y < 50);
        ysum += y;
    }

    y = ysum / n;
    TEST_NEAR(y, 0, 5);

    TRY(source22.cell(0.01));
    TRY(source22.scale(1000));
    TRY(source22.octaves(10));
    TRY(source22.seed(99));

    for (int i = 0; i < n; ++i) {
        x = i;
        point2 = {x, 2 * x};
        TRY(out2 = source22(point2));
        y = out2[0];
        z = out2[1];
        TEST(y > -2000);
        TEST(y < 2000);
        TEST(z > -2000);
        TEST(z < 2000);
        ysum += y;
        zsum += z;
    }

    y = ysum / n;
    z = zsum / n;
    TEST_NEAR(y, 0, 100);
    TEST_NEAR(z, 0, 100);

}

void test_crow_noise_statistics() {

    static constexpr int n = 100'000;
    static constexpr int max_oct = 10;
    static constexpr double base_epsilon = 0.03;

    std::minstd_rand rng(42);
    std::uniform_real_distribution<double> coord_dist(-100, 100);

    for (int oct = 1; oct <= max_oct; ++oct) {

        NoiseSource<double, 2, 1> source;
        TRY(source.octaves(oct));
        TRY(source.seed(86));
        Double2 point;
        double epsilon = base_epsilon / oct;
        double x = 0;
        double min = 1e99;
        double max = -1e99;
        double sum = 0;
        double sum2 = 0;
        double sdr = 0;

        for (int i = 0; i < n; ++i) {
            for (auto& p: point)
                p = coord_dist(rng);
            TRY(x = source(point));
            min = std::min(min, x);
            max = std::max(max, x);
            sum += x;
            sum2 += x * x;
        }

        TEST(min > -2);
        TEST(max < 2);
        double mean = sum / n;
        double sd = std::sqrt(sum2 / n - mean * mean);
        TEST_NEAR(mean, 0, epsilon);

        if (oct > 1) {
            TEST(min <= -1);
            TEST(max >= 1);
            sdr = sd / std::sqrt((1 - std::pow(0.25, double(oct))) / 0.75);
            TEST_NEAR(sdr, 0.418, epsilon);
        }

        // std::cout << "... 2D  oct=" << oct << "  min=" << min << "  max=" << max
        //     << "  mean=" << mean << "  sd=" << sd << "  sdr=" << sdr << "\n";

    }

    for (int oct = 1; oct <= max_oct; ++oct) {

        NoiseSource<double, 3, 1> source;
        TRY(source.octaves(oct));
        TRY(source.seed(99));
        Double3 point;
        double epsilon = base_epsilon / oct;
        double x = 0;
        double min = 1e99;
        double max = -1e99;
        double sum = 0;
        double sum2 = 0;
        double sdr = 0;

        for (int i = 0; i < n; ++i) {
            for (auto& p: point)
                p = coord_dist(rng);
            TRY(x = source(point));
            min = std::min(min, x);
            max = std::max(max, x);
            sum += x;
            sum2 += x * x;
        }

        TEST(min > -2);
        TEST(max < 2);
        double mean = sum / n;
        double sd = std::sqrt(sum2 / n - mean * mean);
        TEST_NEAR(mean, 0, epsilon);

        if (oct > 1) {
            TEST(min <= -1);
            TEST(max >= 1);
            sdr = sd / std::sqrt((1 - std::pow(0.25, double(oct))) / 0.75);
            TEST_NEAR(sdr, 0.360, epsilon);
        }

        // std::cout << "... 3D  oct=" << oct << "  min=" << min << "  max=" << max
        //     << "  mean=" << mean << "  sd=" << sd << "  sdr=" << sdr << "\n";

    }

}

void test_crow_noise_sample_renders() {

    static constexpr int width_cells = 4;
    static constexpr int width_pixels = 200;
    static constexpr int max_octaves = 8;
    static constexpr uint64_t seed = 42;

    static const std::string prefix = "../docs/images/";

    static const auto image_to_noise_coord = [] (int x) {
        return ((double(x) + 0.5) / double(width_pixels) - 0.5) * width_cells;
    };

    static const auto make_sample = [] (const std::string& name, auto noise) {

        std::vector<Rgba8> pixels(width_pixels * width_pixels);
        std::string sample_file = prefix + name + ".png";
        auto p = pixels.begin();

        for (int j = 0; j < width_pixels; ++j) {
            double y = image_to_noise_coord(j);
            for (int i = 0; i < width_pixels; ++i) {
                double x = image_to_noise_coord(i);
                double z = noise(Double2(x, y));
                z = std::clamp(z, -1.0, 1.0);
                int z8 = int(std::round((z + 1) * 127.5));
                *p++ = Rgba8(z8, 255);
            }
        }

        stbi_write_png(sample_file.data(), width_pixels, width_pixels, 4, pixels.data(), 0);
        printp("...", sample_file);

    };

    NoiseSource<double, 2, 1> source2a(1, 1, 1, seed);
    NoiseSource<double, 2, 1> source2b(1, 0.5, max_octaves, seed);
    NoiseSource<double, 3, 1> source3a(1, 1, 1, seed);
    NoiseSource<double, 3, 1> source3b(1, 0.5, max_octaves, seed);

    std::vector<Thread> threads;

    threads.emplace_back([&] { make_sample("supersimplex-2d-1oct", source2a); });
    threads.emplace_back([&] { make_sample("supersimplex-3d-xy-1oct", [&] (auto p) { return source3a(Double3(p.x(), p.y(), 0.0)); }); });
    threads.emplace_back([&] { make_sample("supersimplex-3d-xz-1oct", [&] (auto p) { return source3a(Double3(p.x(), 0.0, p.y())); }); });
    threads.emplace_back([&] { make_sample("supersimplex-3d-yz-1oct", [&] (auto p) { return source3a(Double3(0.0, p.x(), p.y())); }); });
    threads.emplace_back([&] { make_sample("supersimplex-2d-8oct", source2b); });
    threads.emplace_back([&] { make_sample("supersimplex-3d-xy-8oct", [&] (auto p) { return source3b(Double3(p.x(), p.y(), 0.0)); }); });
    threads.emplace_back([&] { make_sample("supersimplex-3d-xz-8oct", [&] (auto p) { return source3b(Double3(p.x(), 0.0, p.y())); }); });
    threads.emplace_back([&] { make_sample("supersimplex-3d-yz-8oct", [&] (auto p) { return source3b(Double3(0.0, p.x(), p.y())); }); });

}
