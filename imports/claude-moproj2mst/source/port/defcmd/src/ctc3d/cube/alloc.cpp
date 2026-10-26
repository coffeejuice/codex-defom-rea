// Allocation wrappers and the squared distance of the cube library (packet p67-7, group "cube").
#include <defcmd/ctc3d/cube.hpp>

#include <defdbio/dbread/w3.hpp>

#include <cstdlib>

namespace def::cmd::cube {

// @src 140256a60  calloc(n, 1) ("DEF_BYTE"), 0 for n == 0
void* fun_140256a60(i32 n)
{
    if (n == 0) {
        return nullptr;
    }
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 1);
    if (p == nullptr) {
        fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "DEF_BYTE", n); // str 1405e4c48
    }
    return p;
}

// @src 140256ac0  realloc(p, n) ("DEF_BYTE"), 0 for n == 0 (p is then left as it is)
void* fun_140256ac0(void* p, i32 n)
{
    if (n == 0) {
        return nullptr;
    }
    void* q = std::realloc(p, static_cast<std::size_t>(static_cast<std::int64_t>(n)));
    if (q == nullptr) {
        fun_14039d3a0("%s Error when re-allocating %s memory of size %d\n", "ERROR    ", "DEF_BYTE", n); // str 1405e4c98
    }
    return q;
}

// @src 140256b30  calloc(n, 1) ("CHAR")
char* fun_140256b30(i32 n)
{
    if (n == 0) {
        return nullptr;
    }
    void* p = std::calloc(static_cast<std::size_t>(static_cast<std::int64_t>(n)), 1);
    if (p == nullptr) {
        fun_14039d3a0("%s Error when allocating %s memory of size %d\n", "ERROR    ", "CHAR", n); // str 1405e4ce8
    }
    return static_cast<char*>(p);
}

// 1402571b0: DBREAD's port owns it (defdbio/src/dbread/w3_arrays.cpp realloc_doubles)
double* fun_1402571b0(void* p, i32 n) { return def::dbio::dbread::realloc_doubles(p, n); }

// @src 140406540  ((dx*dx + dy*dy) + dz*dz) (listing, GENCTC_3d_search.md sec. 1)
double fun_140406540(const double* a, const double* b)
{
    const double dx = a[0] - b[0];
    const double dy = a[1] - b[1];
    const double dz = a[2] - b[2];
    return (dx * dx + dy * dy) + dz * dz;
}

} // namespace def::cmd::cube
