// ctest-labels: fp
// Checks that the build flags in cmake/DefPort.cmake give the original's floating-point model:
// every operation rounded to double, no fused multiply-add, no excess precision.
#include <testkit/check.hpp>

#include <cfloat>

static volatile double g_one_plus = 1.0 + 0x1p-27; // volatile: keep the compiler from folding

TEST_CASE(no_excess_precision) { CHECK_EQ(FLT_EVAL_METHOD, 0); }

TEST_CASE(multiply_add_is_not_fused)
{
    // a*a = 1 + 2^-26 + 2^-54 exactly; rounded to double the 2^-54 term is lost. If the compiler
    // contracts a*a - p into fma(a, a, -p), the result is 2^-54 instead of 0.
    double a = g_one_plus;
    double p = a * a;
    double r = a * a - p;
    CHECK_BITS(r, 0.0);
}

TEST_CASE(sum_is_not_reassociated)
{
    volatile double big = 0x1p53, one = 1.0;
    double b = big, o = one;
    double r = (b + o) + o; // each add rounds to even: 2^53 + 1 -> 2^53
    CHECK_BITS(r, 0x1p53);
}

int main() { return testkit::run_all(); }
