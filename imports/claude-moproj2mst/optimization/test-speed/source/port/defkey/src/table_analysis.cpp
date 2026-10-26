// FUN_1402f5360 (packet kw-g2mg). Spec: docs/re/keywords/DYNARY.md "KEY writer".
#include <defkey/table_analysis.hpp>

namespace def::key {

// @src 1402f5360  uniform / default / record-count analysis of a double array
void analyze_table(const double* values, const std::int32_t* rows, const std::int32_t* ncomp,
                   const double* reference, const std::int32_t* mode, char* uniform,
                   std::int32_t* nrec, double* defaults)
{
    if (*mode == 2) {
        for (std::int32_t r = 0; r < *rows && *uniform == 1; r = r + 1) {
            for (std::int32_t k = 0; k < *ncomp && *uniform == 1; k = k + 1) {
                if (values[r * *ncomp + k] != values[k]) {
                    *uniform = 0;
                }
            }
        }
    }
    else {
        for (std::int32_t i = 0; i < *rows * *ncomp && *uniform == 1; i = i + 1) {
            if (values[i] != *values) {
                *uniform = 0;
            }
        }
    }
    if (*uniform == 0) {
        for (std::int32_t k = 0; k < *ncomp; k = k + 1) {
            defaults[k] = *reference;
        }
        for (std::int32_t r = 0; r < *rows; r = r + 1) {
            for (std::int32_t k = 0; k < *ncomp; k = k + 1) {
                if (values[r * *ncomp + k] != *reference) {
                    *nrec = *nrec + 1;
                    break;
                }
            }
        }
    }
    else {
        for (std::int32_t k = 0; k < *ncomp; k = k + 1) {
            defaults[k] = values[k];
        }
    }
}

} // namespace def::key
