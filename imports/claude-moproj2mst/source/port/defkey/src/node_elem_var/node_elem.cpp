// defkey/node_elem_var: the per-node and per-element value readers (group2-helpers.md "Per-node
// and per-element value readers"). The edge/face readers 14041a090 / 14041a440 are in
// edge_face.cpp. Decompiles: deform-rea-work/out/g2/D_lw.c (1404199a0, 140419b90, 140416c90),
// out/g2/B_w6.c (140419d40), out/g2/parser2.c (140419ea0).
#include <defkey/node_elem_var.hpp>

#include <cstdlib>

#include <defkey/crt_scan.hpp>

namespace def::key {

// @src 1404199a0  count records "%i" index + m x "%Flf", stored when 1 <= index <= n; final fgets
int read_value_records(std::FILE* f, double* dst, std::int32_t count, std::int32_t n, std::int32_t m)
{
    if (count < 1) return 0;
    // calloc(m, 8): the record buffer keeps the previous record's values when a token does not
    // convert. The index is a stack int the binary never initialises; 0 here (dropped).
    double* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(m > 0 ? m : 0) + 1, 8));
    std::int32_t idx = 0;
    for (std::int32_t r = 0; r < count; ++r) {
        if (crt::fscanf(f, "%i", &idx) == -1) {
            std::free(buf);  // the binary leaks the buffer here
            return 1;
        }
        for (std::int32_t j = 0; j < m; ++j) {
            if (crt::fscanf(f, "%Flf", &buf[j]) == -1) {
                std::free(buf);
                return 1;
            }
        }
        if (idx <= n && 0 < idx) {
            for (std::int32_t j = 0; j < m; ++j) dst[(idx - 1) * m + j] = buf[j];
        }
    }
    std::free(buf);
    char line[0x400];
    return std::fgets(line, 0x400, f) == nullptr ? 1 : 0;
}

// @src 140419b90  as 1404199a0 with k values per record (first min(m, k) stored), no final fgets
int read_value_records_k(std::FILE* f, double* dst, std::int32_t count, std::int32_t n, std::int32_t m,
                         std::int32_t k)
{
    if (k == 0) return read_value_records(f, dst, count, n, m);
    double* buf = static_cast<double*>(std::calloc(static_cast<std::size_t>(k > 0 ? k : 0) + 1, 8));
    std::int32_t idx = 0;
    for (std::int32_t r = 0; r < count; ++r) {
        if (crt::fscanf(f, "%i", &idx) == -1) {
            std::free(buf);
            return 1;
        }
        for (std::int32_t j = 0; j < k; ++j) {
            if (crt::fscanf(f, "%Flf", &buf[j]) == -1) {
                std::free(buf);
                return 1;
            }
        }
        if (idx <= n && 0 < idx) {
            for (std::int32_t j = 0; j < (m < k ? m : k); ++j) dst[(idx - 1) * m + j] = buf[j];
        }
    }
    std::free(buf);
    return 0;
}

// @src 140416c90  header line by mode, default fill, then the records
int read_node_values(std::FILE* f, double* dst, std::int32_t n, std::int32_t* ncomp, std::int32_t mode)
{
    int nconv = 0;
    std::int32_t count = 0;
    std::int32_t k = 0;
    double d[10] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0};
    int nd = 0;
    if (mode == 6 && 10 < *ncomp) return 1;
    if (mode == 2 && 10 < *ncomp) return 1;
    char line[0x400];
    if (std::fgets(line, 0x400, f) == nullptr) return 1;
    if (mode == 0) {
        nconv = crt::sscanf(line, "%i %Flf", &count, &d[0]);
        nd = nconv - 1;
    } else if (mode == 6) {
        nconv = crt::sscanf(line, "%i %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf", &count, &d[0],
                            &d[1], &d[2], &d[3], &d[4], &d[5], &d[6], &d[7], &d[8], &d[9]);
        nd = nconv - 1;
    } else if (mode == 1) {
        nconv = crt::sscanf(line, "%i %i %Flf", &count, &k, &d[0]);
        nd = nconv - 2;
    } else if (mode == 2) {
        nconv = crt::sscanf(line, "%i %i %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf %Flf", &count, &k,
                            &d[0], &d[1], &d[2], &d[3], &d[4], &d[5], &d[6], &d[7], &d[8], &d[9]);
        nd = nconv - 2;
        *ncomp = k;
    } else {
        if (mode != 3) return 1;
        nconv = crt::sscanf(line, "%i %Flf %i", &count, &d[0], &k);
        nd = nconv - 2;
    }
    if (nconv < 1) return 1;
    if (0 < nd) {
        if (nd == *ncomp) {
            for (std::int32_t i = 0; i < n; ++i)
                for (std::int32_t j = 0; j < *ncomp; ++j) dst[i * *ncomp + j] = d[j];
        } else {
            for (std::int32_t i = 0; i < n; ++i)
                for (std::int32_t j = 0; j < *ncomp; ++j) dst[i * *ncomp + j] = d[0];
        }
    }
    if (mode == 0) return read_value_records(f, dst, count, n, *ncomp);
    return read_value_records_k(f, dst, count, n, *ncomp, k);
}

// @src 140419ea0  int version of 1404199a0: count records "%i" index + m x "%i"; final fgets
int read_int_records(std::FILE* f, std::int32_t* dst, std::int32_t count, std::int32_t n, std::int32_t m)
{
    if (count < 1) return 0;
    std::int32_t* buf =
        static_cast<std::int32_t*>(std::calloc(static_cast<std::size_t>(m > 0 ? m : 0) + 1, 4));
    std::int32_t idx = 0;
    for (std::int32_t r = 0; r < count; ++r) {
        if (crt::fscanf(f, "%i", &idx) == -1) {
            std::free(buf);
            return 1;
        }
        for (std::int32_t j = 0; j < m; ++j) {
            if (crt::fscanf(f, "%i", &buf[j]) == -1) {
                std::free(buf);
                return 1;
            }
        }
        if (idx <= n && 0 < idx) {
            for (std::int32_t j = 0; j < m; ++j) dst[(idx - 1) * m + j] = buf[j];
        }
    }
    std::free(buf);
    char line[0x400];
    return std::fgets(line, 0x400, f) == nullptr ? 1 : 0;
}

// @src 140419d40  int arrays: header "%i %i" (count, default), default fill, then 140419ea0
int read_int_values(std::FILE* f, std::int32_t* dst, std::int32_t n, std::int32_t m)
{
    char line[0x400];
    if (std::fgets(line, 0x400, f) == nullptr) return 1;
    // count is an uninitialised stack int in the binary when the first token does not convert;
    // 0 here (group2-helpers.md).
    std::int32_t count = 0;
    std::int32_t def = 0;
    int nconv = crt::sscanf(line, "%i %i", &count, &def);
    if (nconv == -1) return 1;
    if (1 < nconv) {
        for (std::int32_t i = 0; i < n; ++i)
            for (std::int32_t j = 0; j < m; ++j) dst[i * m + j] = def;
    }
    return read_int_records(f, dst, count, n, m);
}

}  // namespace def::key
