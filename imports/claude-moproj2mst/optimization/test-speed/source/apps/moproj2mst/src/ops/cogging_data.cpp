// SfOprCogData / CogModel accessors the Cogging writer uses (s1-03 3.3).
// The original prints an error to stdout/stderr and then reads out of range when an index is
// invalid; the writer never asks for one, so here an invalid index is an input error.
#include <cmath>

#include "cogging.hpp"

namespace m2m::ops::cog {

[[noreturn]] static void badIndex(const char* what, int r, int p, int b)
{
    throw Error(M2M_E_INPUT, std::string("Cogging.MDT: ") + what + " invalid (RH " + std::to_string(r) +
                                 ", PS " + std::to_string(p) + ", BT " + std::to_string(b) + ")");
}

static const data::CogReheat& reheat(const CogData& c, int r)
{
    if (!reheatValid(c, r)) badIndex("reheat", r, 0, 0);
    return c.reheats[static_cast<size_t>(r - 1)];
}

int numReheats(const CogModel& m)
{
    return m.cog.nReheats;
}

int numPasses(const CogModel& m, int r)
{
    return reheat(m.cog, r).nPass;
}

int numBites(const CogModel& m, int r, int p)
{
    return pass(m.cog, r, p).noBite;
}

bool reheatValid(const CogData& c, int r)
{
    // The original allocates nReheats records; the vector bound guards a reader that did not.
    return r >= 1 && r <= c.nReheats && static_cast<size_t>(r) <= c.reheats.size();
}

bool passValid(const CogData& c, int r, int p)
{
    if (!reheatValid(c, r) || p < 0) return false;
    const auto& rh = c.reheats[static_cast<size_t>(r - 1)];
    return p <= rh.nPass;
}

bool biteValid(const CogData& c, int r, int p, int b)
{
    return reheatValid(c, r) && passValid(c, r, p) && b >= 0 && b <= pass(c, r, p).noBite;
}

const CogPass& pass(const CogData& c, int r, int p)
{
    if (!passValid(c, r, p) || p < 1) badIndex("pass", r, p, 0);
    const auto& rh = c.reheats[static_cast<size_t>(r - 1)];
    if (static_cast<size_t>(p) > rh.passes.size()) badIndex("pass", r, p, 0);
    return rh.passes[static_cast<size_t>(p - 1)];
}

const CogBite* bite(const CogData& c, int r, int p, int b)
{
    const CogPass& P = pass(c, r, p);
    int k = b;
    if (P.autoBite == 0) {
        if (!biteValid(c, r, p, b)) badIndex("bite", r, p, b);
    } else if (b > 1) {
        k = 1;
    }
    if (k < 1 || static_cast<size_t>(k) > P.bites.size()) badIndex("bite", r, p, b);
    return &P.bites[static_cast<size_t>(k - 1)];
}

int autoBites(const CogData& c, int r, int p)
{
    double L = c.billet.length;
    const CogPass& P = pass(c, r, p);
    if (P.diePosMethod == 1)
        L = std::fabs(P.dieStrtPos - P.dieStopPos);
    else if (P.diePosMethod == 0)
        L = ((1.0 - P.dieStrtPos) - P.dieStopPos) * L;
    else
        L = (L - P.dieStrtPos) - P.dieStopPos;
    if (P.bites.empty()) badIndex("bite", r, p, 1);
    double nom = P.bites[0].nomBite;
    if (nom <= 0.0) return 1;      // original: warning 0x142680380 on stderr only
    double x = L / nom;
    int n = static_cast<int>(x);
    if (n == 0) n = 1;
    return (x - static_cast<double>(n) < 0.5) ? n + 1 : n + 2;
}

double translationX(const CogData& c, int r, int p, int b)
{
    const CogPass& P = pass(c, r, p);
    int d = P.dieMovDir;
    double L = cogSize(c, 1, 0);
    double x;
    if (P.diePosMethod == 1)
        x = ((d == pass(c, r, 1).dieMovDir) ? P.dieStrtPos : P.dieStopPos) + 0.0;
    else if (P.diePosMethod == 0)
        x = static_cast<double>(d) * P.dieStrtPos * L + 0.0;
    else
        x = static_cast<double>(d) * P.dieStrtPos + 0.0;
    if (P.autoBite == 0) {
        for (int i = 0; i < b - 1; i++) {
            if (static_cast<size_t>(i) >= P.bites.size()) badIndex("bite", r, p, i + 1);
            x = x + static_cast<double>(d) * P.bites[static_cast<size_t>(i)].nomBite;
        }
        if (c.useManip && d == 1 && P.diePosMethod != 1) x = x + -1.0 * L;
    } else {
        if (P.bites.empty()) badIndex("bite", r, p, 1);
        if (d == -1)
            x = x + static_cast<double>(-(b - 1)) * P.bites[0].nomBite;
        else if (P.diePosMethod == 1)
            x = x + static_cast<double>(b - 1) * P.bites[0].nomBite;
        else
            x = x + -1.0 * L + static_cast<double>(b - 1) * P.bites[0].nomBite;
    }
    return x;
}

double cogSize(const CogData& c, int a, int b)
{
    if (a == 1) {
        if (b == 0) return c.billet.length;
        switch (c.billet.geoTyp) {
        case 0: return c.billet.dia;
        case 1: return c.billet.width;
        case 2: return c.billet.height;
        default: return c.billet.width;
        }
    }
    if (a == 2) return b == 0 ? c.die.length : 0.0;
    if (a == 4 && b == 0) return c.manip.length;
    return 0.0;
}

int dieCount(const CogModel& m)
{
    return m.cog.die.four == 1 ? 4 : 2;
}

int dieId(int i)
{
    switch (i) {
    case 0: return 2;
    case 1: return 3;
    case 2: return 8;
    case 3: return 9;
    default: return 0;
    }
}

int manipId(int i)
{
    switch (i) {
    case 0: return 4;
    case 1: return 5;
    case 2: return 6;
    case 3: return 7;
    default: return 0;
    }
}

void dieDir(int i, double v[3])
{
    v[0] = 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
    if (i == 0) v[2] = -1.0;
    else if (i == 1) v[2] = 1.0;
    else if (i == 2) v[1] = -1.0;
    else if (i == 3) v[1] = 1.0;
}

void manipDir(int i, double v[3])
{
    v[0] = (i == 0 || i == 1) ? 1.0 : (i == 2 || i == 3) ? -1.0 : 0.0;
    v[1] = 0.0;
    v[2] = 0.0;
}

bool useManip(const CogModel& m)
{
    return m.cog.useManip != 0;
}

bool adaptive(const CogModel& m, int* chkMethod, double* tr0, double* tr1, double* aht)
{
    *chkMethod = m.checkingMethod;
    *tr0 = m.tempRange[0];
    *tr1 = m.tempRange[1];
    *aht = m.adaptiveHeatingTime;
    return m.useAdaptiveReheat != 0;
}

}  // namespace m2m::ops::cog
