#include "crow/prime.hpp"
#include "crow/unit-test.hpp"

using namespace Crow;

void test_crow_prime_next_prev_prime_int() {

    int n = 0, p = 0, q = 0;

    n = 0;       TRY(p = next_prime(n));  TEST_EQUAL(p, 2);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 0);
    n = 1;       TRY(p = next_prime(n));  TEST_EQUAL(p, 2);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 0);
    n = 2;       TRY(p = next_prime(n));  TEST_EQUAL(p, 2);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 2);
    n = 3;       TRY(p = next_prime(n));  TEST_EQUAL(p, 3);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 3);
    n = 4;       TRY(p = next_prime(n));  TEST_EQUAL(p, 5);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 3);
    n = 5;       TRY(p = next_prime(n));  TEST_EQUAL(p, 5);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 5);
    n = 6;       TRY(p = next_prime(n));  TEST_EQUAL(p, 7);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 5);
    n = 7;       TRY(p = next_prime(n));  TEST_EQUAL(p, 7);       TRY(q = prev_prime(n));  TEST_EQUAL(q, 7);
    n = 8;       TRY(p = next_prime(n));  TEST_EQUAL(p, 11);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 7);
    n = 9;       TRY(p = next_prime(n));  TEST_EQUAL(p, 11);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 7);
    n = 10;      TRY(p = next_prime(n));  TEST_EQUAL(p, 11);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 7);
    n = 11;      TRY(p = next_prime(n));  TEST_EQUAL(p, 11);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 11);
    n = 12;      TRY(p = next_prime(n));  TEST_EQUAL(p, 13);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 11);
    n = 13;      TRY(p = next_prime(n));  TEST_EQUAL(p, 13);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 13);
    n = 14;      TRY(p = next_prime(n));  TEST_EQUAL(p, 17);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 13);
    n = 15;      TRY(p = next_prime(n));  TEST_EQUAL(p, 17);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 13);
    n = 16;      TRY(p = next_prime(n));  TEST_EQUAL(p, 17);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 13);
    n = 17;      TRY(p = next_prime(n));  TEST_EQUAL(p, 17);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 17);
    n = 18;      TRY(p = next_prime(n));  TEST_EQUAL(p, 19);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 17);
    n = 19;      TRY(p = next_prime(n));  TEST_EQUAL(p, 19);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 19);
    n = 20;      TRY(p = next_prime(n));  TEST_EQUAL(p, 23);      TRY(q = prev_prime(n));  TEST_EQUAL(q, 19);
    n = 113;     TRY(p = next_prime(n));  TEST_EQUAL(p, 113);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 114;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 115;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 116;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 117;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 118;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 119;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 120;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 121;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 122;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 123;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 124;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 125;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 126;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 113);
    n = 127;     TRY(p = next_prime(n));  TEST_EQUAL(p, 127);     TRY(q = prev_prime(n));  TEST_EQUAL(q, 127);
    n = 8179;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8179);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8180;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8181;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8182;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8183;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8184;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8185;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8186;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8187;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8188;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8189;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8190;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8179);
    n = 8191;    TRY(p = next_prime(n));  TEST_EQUAL(p, 8191);    TRY(q = prev_prime(n));  TEST_EQUAL(q, 8191);
    n = 131063;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131063);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131064;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131065;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131066;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131067;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131068;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131069;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131070;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131063);
    n = 131071;  TRY(p = next_prime(n));  TEST_EQUAL(p, 131071);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 131071);
    n = 524269;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524269);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524270;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524271;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524272;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524273;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524274;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524275;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524276;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524277;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524278;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524279;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524280;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524281;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524282;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524283;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524284;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524285;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524286;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524269);
    n = 524287;  TRY(p = next_prime(n));  TEST_EQUAL(p, 524287);  TRY(q = prev_prime(n));  TEST_EQUAL(q, 524287);

}
