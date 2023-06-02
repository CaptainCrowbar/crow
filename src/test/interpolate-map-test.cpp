#include "crow/interpolate.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_lerp_interpolated_map() {

    LerpMap<double> map;
    double y = 0;

    TRY(map.insert(10, 100));
    TRY(map.insert(20, 150));
    TRY(map.insert(30, 50));
    TRY(map.insert(60, 110));

    TRY(y = map[0]);   TEST_EQUAL(y, 50);
    TRY(y = map[5]);   TEST_EQUAL(y, 75);
    TRY(y = map[10]);  TEST_EQUAL(y, 100);
    TRY(y = map[15]);  TEST_EQUAL(y, 125);
    TRY(y = map[20]);  TEST_EQUAL(y, 150);
    TRY(y = map[25]);  TEST_EQUAL(y, 100);
    TRY(y = map[30]);  TEST_EQUAL(y, 50);
    TRY(y = map[35]);  TEST_EQUAL(y, 60);
    TRY(y = map[40]);  TEST_EQUAL(y, 70);
    TRY(y = map[45]);  TEST_EQUAL(y, 80);
    TRY(y = map[50]);  TEST_EQUAL(y, 90);
    TRY(y = map[55]);  TEST_EQUAL(y, 100);
    TRY(y = map[60]);  TEST_EQUAL(y, 110);
    TRY(y = map[65]);  TEST_EQUAL(y, 120);
    TRY(y = map[70]);  TEST_EQUAL(y, 130);

    TRY((map = {
        { 100, 1000 },
        { 200, 1500 },
        { 300, 500 },
        { 600, 1100 },
    }));

    TRY(y = map[0]);    TEST_EQUAL(y, 500);
    TRY(y = map[50]);   TEST_EQUAL(y, 750);
    TRY(y = map[100]);  TEST_EQUAL(y, 1000);
    TRY(y = map[150]);  TEST_EQUAL(y, 1250);
    TRY(y = map[200]);  TEST_EQUAL(y, 1500);
    TRY(y = map[250]);  TEST_EQUAL(y, 1000);
    TRY(y = map[300]);  TEST_EQUAL(y, 500);
    TRY(y = map[350]);  TEST_EQUAL(y, 600);
    TRY(y = map[400]);  TEST_EQUAL(y, 700);
    TRY(y = map[450]);  TEST_EQUAL(y, 800);
    TRY(y = map[500]);  TEST_EQUAL(y, 900);
    TRY(y = map[550]);  TEST_EQUAL(y, 1000);
    TRY(y = map[600]);  TEST_EQUAL(y, 1100);
    TRY(y = map[650]);  TEST_EQUAL(y, 1200);
    TRY(y = map[700]);  TEST_EQUAL(y, 1300);

}

void test_lerp_interpolated_map_lin_log() {

    LerpMap<double, double, Logs::Y> map;
    double y = 0;

    TRY((map = {
        { 10, 1e20 },
        { 20, 1e30 },
        { 30, 1e10 },
        { 60, 1e22 },
    }));

    TRY(y = map[0]);   TEST_NEAR(y, 1e10, 1);
    TRY(y = map[5]);   TEST_NEAR(y, 1e15, 1e5);
    TRY(y = map[10]);  TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map[15]);  TEST_NEAR(y, 1e25, 1e15);
    TRY(y = map[20]);  TEST_NEAR(y, 1e30, 1e20);
    TRY(y = map[25]);  TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map[30]);  TEST_NEAR(y, 1e10, 1);
    TRY(y = map[35]);  TEST_NEAR(y, 1e12, 1e2);
    TRY(y = map[40]);  TEST_NEAR(y, 1e14, 1e4);
    TRY(y = map[45]);  TEST_NEAR(y, 1e16, 1e6);
    TRY(y = map[50]);  TEST_NEAR(y, 1e18, 1e8);
    TRY(y = map[55]);  TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map[60]);  TEST_NEAR(y, 1e22, 1e12);
    TRY(y = map[65]);  TEST_NEAR(y, 1e24, 1e14);
    TRY(y = map[70]);  TEST_NEAR(y, 1e26, 1e16);

}

void test_lerp_interpolated_map_log_log() {

    LerpMap<double, double, Logs::XY> map;
    double y = 0;

    TRY(map.insert(4, 1e20));
    TRY(map.insert(16, 1e30));
    TRY(map.insert(64, 1e10));
    TRY(map.insert(4096, 1e22));

    TRY(y = map[1]);      TEST_NEAR(y, 1e10, 1);
    TRY(y = map[2]);      TEST_NEAR(y, 1e15, 1e5);
    TRY(y = map[4]);      TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map[8]);      TEST_NEAR(y, 1e25, 1e15);
    TRY(y = map[16]);     TEST_NEAR(y, 1e30, 1e20);
    TRY(y = map[32]);     TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map[64]);     TEST_NEAR(y, 1e10, 1);
    TRY(y = map[128]);    TEST_NEAR(y, 1e12, 1e2);
    TRY(y = map[256]);    TEST_NEAR(y, 1e14, 1e4);
    TRY(y = map[512]);    TEST_NEAR(y, 1e16, 1e6);
    TRY(y = map[1024]);   TEST_NEAR(y, 1e18, 1e8);
    TRY(y = map[2048]);   TEST_NEAR(y, 1e20, 1e10);
    TRY(y = map[4096]);   TEST_NEAR(y, 1e22, 1e12);
    TRY(y = map[8192]);   TEST_NEAR(y, 1e24, 1e14);
    TRY(y = map[16384]);  TEST_NEAR(y, 1e26, 1e16);

}
