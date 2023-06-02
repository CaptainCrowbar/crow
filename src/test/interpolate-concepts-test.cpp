#include "crow/interpolate.hpp"
#include "crow/colour.hpp"
#include "crow/vector.hpp"
#include "crow/unit-test.hpp"
#include <string>

using namespace Crow;

namespace {

    class Thing {
    public:
        Thing() = default;
        explicit Thing(const std::string& s): str_(s) {}
        std::string str() const { return str_; }
        Thing operator+(const Thing& t) const { return Thing(str_ + t.str_); }
        Thing operator-(const Thing& t) const { return str_.size() > t.str_.size() ? *this : Thing(); }
        Thing operator*(float x) const { auto t = *this; t.str_.resize(size_t(10 * x), '*'); return t; }
        [[maybe_unused]] friend Thing operator*(float x, const Thing& y) { return y * x; }
        bool operator==(const Thing& t) const { return str_ == t.str_; }
    private:
        std::string str_;
    };

}

void test_lerp_concepts() {

    TEST((LerpTypes<float, float, Logs::none>));  TEST((ConstexprLerpTypes<float, float, Logs::none>));
    TEST((LerpTypes<float, float, Logs::X>));     TEST((ConstexprLerpTypes<float, float, Logs::X>));
    TEST((LerpTypes<float, float, Logs::Y>));     TEST((ConstexprLerpTypes<float, float, Logs::Y>));
    TEST((LerpTypes<float, float, Logs::XY>));    TEST((ConstexprLerpTypes<float, float, Logs::XY>));

    TEST((! LerpTypes<int, float, Logs::none>));  TEST((! ConstexprLerpTypes<int, float, Logs::none>));
    TEST((! LerpTypes<int, float, Logs::X>));     TEST((! ConstexprLerpTypes<int, float, Logs::X>));
    TEST((! LerpTypes<int, float, Logs::Y>));     TEST((! ConstexprLerpTypes<int, float, Logs::Y>));
    TEST((! LerpTypes<int, float, Logs::XY>));    TEST((! ConstexprLerpTypes<int, float, Logs::XY>));

    TEST((LerpTypes<float, int, Logs::none>));  TEST((ConstexprLerpTypes<float, int, Logs::none>));
    TEST((LerpTypes<float, int, Logs::X>));     TEST((ConstexprLerpTypes<float, int, Logs::X>));
    TEST((! LerpTypes<float, int, Logs::Y>));   TEST((! ConstexprLerpTypes<float, int, Logs::Y>));
    TEST((! LerpTypes<float, int, Logs::XY>));  TEST((! ConstexprLerpTypes<float, int, Logs::XY>));

    TEST((LerpTypes<float, Float3, Logs::none>));  TEST((ConstexprLerpTypes<float, Float3, Logs::none>));
    TEST((LerpTypes<float, Float3, Logs::X>));     TEST((ConstexprLerpTypes<float, Float3, Logs::X>));
    TEST((! LerpTypes<float, Float3, Logs::Y>));   TEST((! ConstexprLerpTypes<float, Float3, Logs::Y>));
    TEST((! LerpTypes<float, Float3, Logs::XY>));  TEST((! ConstexprLerpTypes<float, Float3, Logs::XY>));

    TEST((LerpTypes<float, Rgbf, Logs::none>));  TEST((ConstexprLerpTypes<float, Rgbf, Logs::none>));
    TEST((LerpTypes<float, Rgbf, Logs::X>));     TEST((ConstexprLerpTypes<float, Rgbf, Logs::X>));
    TEST((! LerpTypes<float, Rgbf, Logs::Y>));   TEST((! ConstexprLerpTypes<float, Rgbf, Logs::Y>));
    TEST((! LerpTypes<float, Rgbf, Logs::XY>));  TEST((! ConstexprLerpTypes<float, Rgbf, Logs::XY>));

    TEST((LerpTypes<float, Thing, Logs::none>));  TEST((! ConstexprLerpTypes<float, Thing, Logs::none>));
    TEST((LerpTypes<float, Thing, Logs::X>));     TEST((! ConstexprLerpTypes<float, Thing, Logs::X>));
    TEST((! LerpTypes<float, Thing, Logs::Y>));   TEST((! ConstexprLerpTypes<float, Thing, Logs::Y>));
    TEST((! LerpTypes<float, Thing, Logs::XY>));  TEST((! ConstexprLerpTypes<float, Thing, Logs::XY>));

}
