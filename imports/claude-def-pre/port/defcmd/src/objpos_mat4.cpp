// OBJPOS 4x4 transform helpers (packet p67-3). Listing 0x140240fc0..0x140241260.
#include <defcmd/objpos_mat4.hpp>

namespace def::cmd::objpos {

// @src 140240fc0  identity
void fun_140240fc0(double* m)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) m[i * 4 + j] = (i == j) ? 1.0 : 0.0;
}

// @src 140241070  copy
void fun_140241070(double* dst, const double* src)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j) dst[i * 4 + j] = src[i * 4 + j];
}

// @src 140241110  M := X * M (1402411f0: sum = sum + (X[c][k] * M[k][a]))
void fun_140241110(double* m, const double* x)
{
    double out[16];
    for (int a = 0; a < 4; ++a) {
        for (int c = 0; c < 4; ++c) {
            double s = 0.0;
            for (int k = 0; k < 4; ++k) s = s + (x[c * 4 + k] * m[k * 4 + a]);
            out[c * 4 + a] = s;
        }
    }
    fun_140241070(m, out);
}

} // namespace def::cmd::objpos
