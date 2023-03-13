#include "crow/probability.hpp"
#include "crow/unit-test.hpp"
#include <limits>

using namespace Crow;

using PD = Probability<double>;

void test_crow_probability_powers() {

    PD x, z;
    double y = 0;

    TRY(x = 0);      y = 0.25;   TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0.75;   TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 1;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 1.25;   TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 2;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 3;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  y = -1;     TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.125);  y = -0.25;  TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.125);  y = 0;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.125);  y = 0.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.5946035575, 1e-10);
    TRY(x = 0.125);  y = 0.75;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.2102241038, 1e-10);
    TRY(x = 0.125);  y = 1;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.125);  y = 1.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.0743254447, 1e-10);
    TRY(x = 0.125);  y = 2;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.015625);
    TRY(x = 0.125);  y = 3;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.001953125);
    TRY(x = 0.375);  y = -1;     TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.375);  y = -0.25;  TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.375);  y = 0;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.375);  y = 0.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.7825422900, 1e-10);
    TRY(x = 0.375);  y = 0.75;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.4792073282, 1e-10);
    TRY(x = 0.375);  y = 1;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.375);
    TRY(x = 0.375);  y = 1.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.2934533588, 1e-10);
    TRY(x = 0.375);  y = 2;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.140625);
    TRY(x = 0.375);  y = 3;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.052734375);
    TRY(x = 0.5);    y = -1;     TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.5);    y = -0.25;  TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.5);    y = 0;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.5);    y = 0.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.8408964153, 1e-10);
    TRY(x = 0.5);    y = 0.75;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.5946035575, 1e-10);
    TRY(x = 0.5);    y = 1;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 0.5);    y = 1.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.4204482076, 1e-10);
    TRY(x = 0.5);    y = 2;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.25);
    TRY(x = 0.5);    y = 3;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.625);  y = -1;     TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.625);  y = -0.25;  TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.625);  y = 0;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.625);  y = 0.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.8891397050, 1e-10);
    TRY(x = 0.625);  y = 0.75;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.7029266565, 1e-10);
    TRY(x = 0.625);  y = 1;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.625);
    TRY(x = 0.625);  y = 1.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.5557123156, 1e-10);
    TRY(x = 0.625);  y = 2;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.390625);
    TRY(x = 0.625);  y = 3;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.244140625);
    TRY(x = 0.875);  y = -1;     TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = -0.25;  TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = 0;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 0.875);  y = 0.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.9671682101, 1e-10);
    TRY(x = 0.875);  y = 0.75;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.9047030194, 1e-10);
    TRY(x = 0.875);  y = 1;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.875);
    TRY(x = 0.875);  y = 1.25;   TRY(z = pow(x, y));  TEST_NEAR(z.value(), 0.8462721839, 1e-10);
    TRY(x = 0.875);  y = 2;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.765625);
    TRY(x = 0.875);  y = 3;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 0.669921875);
    TRY(x = 1);      y = -1;     TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = -0.25;  TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0.25;   TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0.75;   TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 1;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 1.25;   TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 2;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 3;      TRY(z = pow(x, y));  TEST_EQUAL(z.value(), 1);

    TRY(x = 0);      y = -1;     TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = -0.25;  TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0.25;   TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 0.75;   TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 1;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 1.25;   TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 2;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0);      y = 3;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  y = -1;     TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  y = -0.25;  TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  y = 0;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.125);  y = 0.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.0328317899, 1e-10);
    TRY(x = 0.125);  y = 0.75;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.0952969806, 1e-10);
    TRY(x = 0.125);  y = 1;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.125);
    TRY(x = 0.125);  y = 1.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.1537278161, 1e-10);
    TRY(x = 0.125);  y = 2;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.234375);
    TRY(x = 0.125);  y = 3;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.330078125);
    TRY(x = 0.375);  y = -1;     TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  y = -0.25;  TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  y = 0;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.375);  y = 0.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.1108602950, 1e-10);
    TRY(x = 0.375);  y = 0.75;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.2970733435, 1e-10);
    TRY(x = 0.375);  y = 1;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.375);
    TRY(x = 0.375);  y = 1.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.4442876844, 1e-10);
    TRY(x = 0.375);  y = 2;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.609375);
    TRY(x = 0.375);  y = 3;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.755859375);
    TRY(x = 0.5);    y = -1;     TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.5);    y = -0.25;  TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.5);    y = 0;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.5);    y = 0.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.1591035847, 1e-10);
    TRY(x = 0.5);    y = 0.75;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.4053964425, 1e-10);
    TRY(x = 0.5);    y = 1;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.5);
    TRY(x = 0.5);    y = 1.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.5795517924, 1e-10);
    TRY(x = 0.5);    y = 2;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.75);
    TRY(x = 0.5);    y = 3;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.875);
    TRY(x = 0.625);  y = -1;     TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.625);  y = -0.25;  TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.625);  y = 0;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.625);  y = 0.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.2174577100, 1e-10);
    TRY(x = 0.625);  y = 0.75;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.5207926718, 1e-10);
    TRY(x = 0.625);  y = 1;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.625);
    TRY(x = 0.625);  y = 1.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.7065466412, 1e-10);
    TRY(x = 0.625);  y = 2;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.859375);
    TRY(x = 0.625);  y = 3;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.947265625);
    TRY(x = 0.875);  y = -1;     TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.875);  y = -0.25;  TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.875);  y = 0;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0);
    TRY(x = 0.875);  y = 0.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.4053964425, 1e-10);
    TRY(x = 0.875);  y = 0.75;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.7897758962, 1e-10);
    TRY(x = 0.875);  y = 1;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.875);
    TRY(x = 0.875);  y = 1.25;   TRY(z = compl_pow(x, y));  TEST_NEAR(z.value(), 0.9256745553, 1e-10);
    TRY(x = 0.875);  y = 2;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.984375);
    TRY(x = 0.875);  y = 3;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 0.998046875);
    TRY(x = 1);      y = 0.25;   TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 0.75;   TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 1;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 1.25;   TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 2;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 1);
    TRY(x = 1);      y = 3;      TRY(z = compl_pow(x, y));  TEST_EQUAL(z.value(), 1);

}

void test_crow_probability_normal_variates() {

    static constexpr auto inf = std::numeric_limits<double>::infinity();

    PD p;
    double z1 = 0, z2 = 0;

    TRY(z1 = - inf);  TRY(p = PD::from_z(z1));  TEST_EQUAL(p.value(), 0);                   TRY(z2 = p.z());  TEST_EQUAL(z2, - inf);
    TRY(z1 = -5.00);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0000002867, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -4.75);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0000010171, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -4.50);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0000033977, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -4.25);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0000106885, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -4.00);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0000316712, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -3.75);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0000884173, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -3.50);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0002326291, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -3.25);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0005770250, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -3.00);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0013498980, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -2.75);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0029797632, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -2.50);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0062096653, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -2.25);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0122244727, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -2.00);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0227501319, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -1.75);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0400591569, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -1.50);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.0668072013, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -1.25);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.1056497737, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -1.00);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.1586552539, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -0.75);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.2266273524, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -0.50);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.3085375387, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = -0.25);  TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.4012936743, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 0.00);   TRY(p = PD::from_z(z1));  TEST_EQUAL(p.value(), 0.5);                 TRY(z2 = p.z());  TEST_EQUAL(z2, 0);
    TRY(z1 = 0.25);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.5987063257, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 0.50);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.6914624613, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 0.75);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.7733726476, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 1.00);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.8413447461, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 1.25);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.8943502263, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 1.50);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9331927987, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 1.75);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9599408431, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 2.00);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9772498681, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 2.25);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9877755273, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 2.50);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9937903347, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 2.75);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9970202368, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 3.00);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9986501020, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 3.25);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9994229750, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 3.50);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9997673709, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 3.75);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9999115827, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 4.00);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9999683288, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 4.25);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9999893115, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 4.50);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9999966023, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 4.75);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9999989829, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = 5.00);   TRY(p = PD::from_z(z1));  TEST_NEAR(p.value(), 0.9999997133, 1e-10);  TRY(z2 = p.z());  TEST_NEAR(z2, z1, 1e-10);
    TRY(z1 = inf);    TRY(p = PD::from_z(z1));  TEST_EQUAL(p.value(), 1);                   TRY(z2 = p.z());  TEST_EQUAL(z2, inf);

}
