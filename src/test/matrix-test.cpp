#include "crow/matrix.hpp"
#include "crow/format.hpp"
#include "crow/unit-test.hpp"
#include "crow/vector.hpp"
#include <cmath>
#include <cstdlib>
#include <string>
#include <vector>

using namespace Crow;

namespace {

    template <typename R>
    void fuzz(R& r) {
        for (auto& x: r)
            if (std::abs(x) < 1e-6)
                x = 0;
    }

}

void test_crow_matrix_basics() {

    Double3 v1, v2;
    Double3x3r rm1(2, 1), rm2, rm3;
    Double3x3c cm1(2, 1), cm2, cm3;
    std::vector<double> v;

    TEST_EQUAL(sizeof(Double3x3r), 9 * sizeof(double));
    TEST_EQUAL(sizeof(Double3x3c), 9 * sizeof(double));

    TEST_EQUAL(rm1.str(),
        "[[2,1,1],"
        "[1,2,1],"
        "[1,1,2]]"
    );
    TEST_EQUAL(cm1.str(),
        "[[2,1,1],"
        "[1,2,1],"
        "[1,1,2]]"
    );
    TEST_EQUAL(rm1.str("e3"),
        "[[2.00e0,1.00e0,1.00e0],"
        "[1.00e0,2.00e0,1.00e0],"
        "[1.00e0,1.00e0,2.00e0]]"
    );
    TEST_EQUAL(rm1.str("f3"),
        "[[2.000,1.000,1.000],"
        "[1.000,2.000,1.000],"
        "[1.000,1.000,2.000]]"
    );

    TRY((rm1 = Double3x3r{1,2,3,4,5,6,7,8,9}));
    TRY((cm1 = Double3x3c{1,2,3,4,5,6,7,8,9}));
    TEST_EQUAL(format_range(rm1), "[1,2,3,4,5,6,7,8,9]");
    TEST_EQUAL(format_range(cm1), "[1,2,3,4,5,6,7,8,9]");

    TRY((rm1 = {2,3,5,7,11,13,17,19,23}));
    TRY((cm1 = {2,3,5,7,11,13,17,19,23}));
    TEST_EQUAL(format_range(rm1), "[2,3,5,7,11,13,17,19,23]");
    TEST_EQUAL(format_range(cm1), "[2,3,5,7,11,13,17,19,23]");

    TEST_EQUAL(rm1.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TEST_EQUAL(cm1.str(),
        "[[2,7,17],"
        "[3,11,19],"
        "[5,13,23]]"
    );

    TRY(rm2 = Double3x3r(cm1));
    TEST_EQUAL(rm2.str(),
        "[[2,7,17],"
        "[3,11,19],"
        "[5,13,23]]"
    );
    TRY(cm2 = Double3x3c(rm1));
    TEST_EQUAL(cm2.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );

    TRY(rm2 = cm1);
    TEST_EQUAL(rm2.str(),
        "[[2,7,17],"
        "[3,11,19],"
        "[5,13,23]]"
    );
    TRY(cm2 = rm1);
    TEST_EQUAL(cm2.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );

    TEST_EQUAL(rm1(0, 0), 2);
    TEST_EQUAL(rm1(0, 1), 3);
    TEST_EQUAL(rm1(0, 2), 5);
    TEST_EQUAL(rm1(1, 0), 7);
    TEST_EQUAL(rm1(1, 1), 11);
    TEST_EQUAL(rm1(1, 2), 13);
    TEST_EQUAL(rm1(2, 0), 17);
    TEST_EQUAL(rm1(2, 1), 19);
    TEST_EQUAL(rm1(2, 2), 23);
    TEST_EQUAL(cm1(0, 0), 2);
    TEST_EQUAL(cm1(0, 1), 7);
    TEST_EQUAL(cm1(0, 2), 17);
    TEST_EQUAL(cm1(1, 0), 3);
    TEST_EQUAL(cm1(1, 1), 11);
    TEST_EQUAL(cm1(1, 2), 19);
    TEST_EQUAL(cm1(2, 0), 5);
    TEST_EQUAL(cm1(2, 1), 13);
    TEST_EQUAL(cm1(2, 2), 23);

    TRY((rm2 = {29,31,37,41,43,47,53,59,61}));
    TRY(cm1 = rm1);
    TRY(cm2 = rm2);

    TEST_EQUAL(rm1.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TEST_EQUAL(cm1.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TEST_EQUAL(rm2.str(),
        "[[29,31,37],"
        "[41,43,47],"
        "[53,59,61]]"
    );
    TEST_EQUAL(cm2.str(),
        "[[29,31,37],"
        "[41,43,47],"
        "[53,59,61]]"
    );

    TEST(rm1 == rm1);
    TEST(rm1 != rm2);
    TEST(cm1 == cm1);
    TEST(cm1 != cm2);

    TRY(rm3 = rm1 * 10);
    TEST_EQUAL(rm3.str(),
        "[[20,30,50],"
        "[70,110,130],"
        "[170,190,230]]"
    );
    TRY(cm3 = cm1 * 10);
    TEST_EQUAL(cm3.str(),
        "[[20,30,50],"
        "[70,110,130],"
        "[170,190,230]]"
    );

    TRY(rm3 = 10 * rm1);
    TEST_EQUAL(rm3.str(),
        "[[20,30,50],"
        "[70,110,130],"
        "[170,190,230]]"
    );
    TRY(cm3 = 10 * cm1);
    TEST_EQUAL(cm3.str(),
        "[[20,30,50],"
        "[70,110,130],"
        "[170,190,230]]"
    );

    TRY(rm3 = rm1 / 10);
    TEST_EQUAL(rm3.str(),
        "[[0.2,0.3,0.5],"
        "[0.7,1.1,1.3],"
        "[1.7,1.9,2.3]]"
    );
    TRY(cm3 = cm1 / 10);
    TEST_EQUAL(cm3.str(),
        "[[0.2,0.3,0.5],"
        "[0.7,1.1,1.3],"
        "[1.7,1.9,2.3]]"
    );

    TRY(rm3 = rm1 + rm2);
    TEST_EQUAL(rm3.str(),
        "[[31,34,42],"
        "[48,54,60],"
        "[70,78,84]]"
    );
    TRY(cm3 = cm1 + cm2);
    TEST_EQUAL(cm3.str(),
        "[[31,34,42],"
        "[48,54,60],"
        "[70,78,84]]"
    );

    TRY(rm3 = rm1 - rm2);
    TEST_EQUAL(rm3.str(),
        "[[-27,-28,-32],"
        "[-34,-32,-34],"
        "[-36,-40,-38]]"
    );
    TRY(cm3 = cm1 - cm2);
    TEST_EQUAL(cm3.str(),
        "[[-27,-28,-32],"
        "[-34,-32,-34],"
        "[-36,-40,-38]]"
    );

    TRY(rm3 = rm1 * rm2);
    TEST_EQUAL(rm3.str(),
        "[[446,486,520],"
        "[1343,1457,1569],"
        "[2491,2701,2925]]"
    );
    TRY(cm3 = cm1 * cm2);
    TEST_EQUAL(cm3.str(),
        "[[446,486,520],"
        "[1343,1457,1569],"
        "[2491,2701,2925]]"
    );

    TRY((v1 = {67,71,73}));

    TRY(v2 = rm1 * v1);  TEST_EQUAL(v2.str(), "[712,2199,4167]");
    TRY(v2 = v1 * rm1);  TEST_EQUAL(v2.str(), "[1872,2369,2937]");
    TRY(v2 = cm1 * v1);  TEST_EQUAL(v2.str(), "[712,2199,4167]");
    TRY(v2 = v1 * cm1);  TEST_EQUAL(v2.str(), "[1872,2369,2937]");

    TRY(rm3 = Double3x3r::identity());
    TEST_EQUAL(rm3.str(),
        "[[1,0,0],"
        "[0,1,0],"
        "[0,0,1]]"
    );
    TRY(cm3 = Double3x3r::identity());
    TEST_EQUAL(cm3.str(),
        "[[1,0,0],"
        "[0,1,0],"
        "[0,0,1]]"
    );

    TRY(rm3 = rm1);
    TRY(v1 = rm3.column(0));  TEST_EQUAL(v1.str(), "[2,7,17]");
    TRY(v1 = rm3.column(1));  TEST_EQUAL(v1.str(), "[3,11,19]");
    TRY(v1 = rm3.column(2));  TEST_EQUAL(v1.str(), "[5,13,23]");
    TRY(v1 = rm3.row(0));     TEST_EQUAL(v1.str(), "[2,3,5]");
    TRY(v1 = rm3.row(1));     TEST_EQUAL(v1.str(), "[7,11,13]");
    TRY(v1 = rm3.row(2));     TEST_EQUAL(v1.str(), "[17,19,23]");

    TRY(cm3 = cm1);
    TRY(v1 = cm3.column(0));  TEST_EQUAL(v1.str(), "[2,7,17]");
    TRY(v1 = cm3.column(1));  TEST_EQUAL(v1.str(), "[3,11,19]");
    TRY(v1 = cm3.column(2));  TEST_EQUAL(v1.str(), "[5,13,23]");
    TRY(v1 = cm3.row(0));     TEST_EQUAL(v1.str(), "[2,3,5]");
    TRY(v1 = cm3.row(1));     TEST_EQUAL(v1.str(), "[7,11,13]");
    TRY(v1 = cm3.row(2));     TEST_EQUAL(v1.str(), "[17,19,23]");

    TRY((v1 = {1,2,3}));
    TRY(rm3 = rm1);
    TRY(rm3.set_column(0, v1));
    TEST_EQUAL(rm3.str(),
        "[[1,3,5],"
        "[2,11,13],"
        "[3,19,23]]"
    );
    TRY(rm3.set_column(1, v1));
    TEST_EQUAL(rm3.str(),
        "[[1,1,5],"
        "[2,2,13],"
        "[3,3,23]]"
    );
    TRY(rm3.set_column(2, v1));
    TEST_EQUAL(rm3.str(),
        "[[1,1,1],"
        "[2,2,2],"
        "[3,3,3]]"
    );
    TRY(rm3 = rm1);
    TRY(rm3.set_row(0, v1));
    TEST_EQUAL(rm3.str(),
        "[[1,2,3],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TRY(rm3.set_row(1, v1));
    TEST_EQUAL(rm3.str(),
        "[[1,2,3],"
        "[1,2,3],"
        "[17,19,23]]"
    );
    TRY(rm3.set_row(2, v1));
    TEST_EQUAL(rm3.str(),
        "[[1,2,3],"
        "[1,2,3],"
        "[1,2,3]]"
    );

    TRY(cm3 = cm1);
    TRY(cm3.set_column(0, v1));
    TEST_EQUAL(cm3.str(),
        "[[1,3,5],"
        "[2,11,13],"
        "[3,19,23]]"
    );
    TRY(cm3.set_column(1, v1));
    TEST_EQUAL(cm3.str(),
        "[[1,1,5],"
        "[2,2,13],"
        "[3,3,23]]"
    );
    TRY(cm3.set_column(2, v1));
    TEST_EQUAL(cm3.str(),
        "[[1,1,1],"
        "[2,2,2],"
        "[3,3,3]]"
    );
    TRY(cm3 = cm1);
    TRY(cm3.set_row(0, v1));
    TEST_EQUAL(cm3.str(),
        "[[1,2,3],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TRY(cm3.set_row(1, v1));
    TEST_EQUAL(cm3.str(),
        "[[1,2,3],"
        "[1,2,3],"
        "[17,19,23]]"
    );
    TRY(cm3.set_row(2, v1));
    TEST_EQUAL(cm3.str(),
        "[[1,2,3],"
        "[1,2,3],"
        "[1,2,3]]"
    );

    TRY(rm3 = rm1.swap_rows(0, 1));
    TEST_EQUAL(rm3.str(),
        "[[7,11,13],"
        "[2,3,5],"
        "[17,19,23]]"
    );
    TRY(rm3 = rm3.swap_rows(1, 2));
    TEST_EQUAL(rm3.str(),
        "[[7,11,13],"
        "[17,19,23],"
        "[2,3,5]]"
    );
    TRY(rm3 = rm3.swap_columns(0, 1));
    TEST_EQUAL(rm3.str(),
        "[[11,7,13],"
        "[19,17,23],"
        "[3,2,5]]"
    );
    TRY(rm3 = rm3.swap_columns(1, 2));
    TEST_EQUAL(rm3.str(),
        "[[11,13,7],"
        "[19,23,17],"
        "[3,5,2]]"
    );

    TRY(cm3 = rm1.swap_rows(0, 1));
    TEST_EQUAL(cm3.str(),
        "[[7,11,13],"
        "[2,3,5],"
        "[17,19,23]]"
    );
    TRY(cm3 = cm3.swap_rows(1, 2));
    TEST_EQUAL(cm3.str(),
        "[[7,11,13],"
        "[17,19,23],"
        "[2,3,5]]"
    );
    TRY(cm3 = cm3.swap_columns(0, 1));
    TEST_EQUAL(cm3.str(),
        "[[11,7,13],"
        "[19,17,23],"
        "[3,2,5]]"
    );
    TRY(cm3 = cm3.swap_columns(1, 2));
    TEST_EQUAL(cm3.str(),
        "[[11,13,7],"
        "[19,23,17],"
        "[3,5,2]]"
    );

    TRY(rm3 = rm1.transposed());
    TEST_EQUAL(rm3.str(),
        "[[2,7,17],"
        "[3,11,19],"
        "[5,13,23]]"
    );
    TRY(rm3 = rm3.transposed());
    TEST_EQUAL(rm3.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TRY(cm3 = cm1.transposed());
    TEST_EQUAL(cm3.str(),
        "[[2,7,17],"
        "[3,11,19],"
        "[5,13,23]]"
    );
    TRY(cm3 = cm3.transposed());
    TEST_EQUAL(cm3.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );

}

void test_crow_matrix_inversion() {

    double x = 0;

    Double2x2r r2, s2, t2;
    Double3x3r r3, s3, t3;
    Double4x4r r4, s4, t4;
    Double2x2c c2, d2, e2;
    Double3x3c c3, d3, e3;
    Double4x4c c4, d4, e4;

    TRY((r2 = {2,3,5,7}));
    TEST_EQUAL(r2.str(),
        "[[2,3],"
        "[5,7]]"
    );
    TRY(c2 = r2);
    TEST_EQUAL(c2.str(),
        "[[2,3],"
        "[5,7]]"
    );
    TRY((r3 = {2,3,5,7,11,13,17,19,23}));
    TEST_EQUAL(r3.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TRY(c3 = r3);
    TEST_EQUAL(c3.str(),
        "[[2,3,5],"
        "[7,11,13],"
        "[17,19,23]]"
    );
    TRY((r4 = {2,3,5,7,11,13,17,19,23,29,31,37,41,43,47,53}));
    TEST_EQUAL(r4.str(),
        "[[2,3,5,7],"
        "[11,13,17,19],"
        "[23,29,31,37],"
        "[41,43,47,53]]"
    );
    TRY(c4 = r4);
    TEST_EQUAL(c4.str(),
        "[[2,3,5,7],"
        "[11,13,17,19],"
        "[23,29,31,37],"
        "[41,43,47,53]]"
    );

    TRY(x = r2.det());  TEST_EQUAL(x, -1);
    TRY(x = c2.det());  TEST_EQUAL(x, -1);
    TRY(x = r3.det());  TEST_EQUAL(x, -78);
    TRY(x = c3.det());  TEST_EQUAL(x, -78);
    TRY(x = r4.det());  TEST_EQUAL(x, 880);
    TRY(x = c4.det());  TEST_EQUAL(x, 880);

    std::string id_str_2 = Double2x2r::identity().str();
    std::string id_str_3 = Double3x3r::identity().str();
    std::string id_str_4 = Double4x4r::identity().str();
    std::string inv_str_2 =
        "[[-7,3],"
        "[5,-2]]";
    std::string inv_str_3 =
        "[[-0.0769231,-0.333333,0.205128],"
        "[-0.769231,0.5,-0.115385],"
        "[0.692308,-0.166667,-0.0128205]]";
    std::string inv_str_4 =
        "[[0.272727,-0.218182,-0.2,0.181818],"
        "[-0.454545,-0.0363636,0.3,-0.136364],"
        "[-0.590909,0.697727,-0.1,-0.102273],"
        "[0.681818,-0.420455,0,0.0795455]]";

    TRY(s2 = r2.inverse());  TEST_EQUAL(s2.str(), inv_str_2);  TRY(t2 = r2 * s2);  TRY(fuzz(t2));  TEST_EQUAL(t2.str(), id_str_2);
    TRY(d2 = c2.inverse());  TEST_EQUAL(d2.str(), inv_str_2);  TRY(e2 = c2 * d2);  TRY(fuzz(e2));  TEST_EQUAL(e2.str(), id_str_2);
    TRY(s3 = r3.inverse());  TEST_EQUAL(s3.str(), inv_str_3);  TRY(t3 = r3 * s3);  TRY(fuzz(t3));  TEST_EQUAL(t3.str(), id_str_3);
    TRY(d3 = c3.inverse());  TEST_EQUAL(d3.str(), inv_str_3);  TRY(e3 = c3 * d3);  TRY(fuzz(e3));  TEST_EQUAL(e3.str(), id_str_3);
    TRY(s4 = r4.inverse());  TEST_EQUAL(s4.str(), inv_str_4);  TRY(t4 = r4 * s4);  TRY(fuzz(t4));  TEST_EQUAL(t4.str(), id_str_4);
    TRY(d4 = c4.inverse());  TEST_EQUAL(d4.str(), inv_str_4);  TRY(e4 = c4 * d4);  TRY(fuzz(e4));  TEST_EQUAL(e4.str(), id_str_4);

}
