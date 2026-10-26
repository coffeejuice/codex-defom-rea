// defcmd/drop/contact.hpp: contact results of the OBJPOS drop solver (packet p67-3): extraction
// of the engine's per-node contacts (140373650 with the local-coordinate kernels 140374252 /
// 140374954), the signed gap and separation release 140370a26, the contact forces 140371339.
// Spec: docs/re/commands/OBJPOS_drop_solver.md §5.5.2, §5.5.3, §5.8.
// Fortran calling style: every argument by pointer, the binary's argument order, names
// fun_<address>. Arrays: see state.hpp (BC(3,n), FID(3,f), NRM(3,3,f), XE(2,3,f), 1-based).
#pragma once

#include <defcmd/drop/state.hpp>

namespace def::cmd::drop {

// @src 140373650  for nodes n = *first..*last with flags(n) > 0: NI4 of (obj, inst, n - *off);
// per negative code (slot 3, 2, 1): FID, NRM = -ER1 of the polygon, XE = local coordinates of
// X(:,n) on it (140374252 quad / 140374954 triangle); a code without polygon prints the CNTINF
// fatal-error lines. Arrays FID/NRM/XE/SW are indexed by k = flags(n), BC and X by n.
// insttab(obj) = instance of object obj (the caller passes &dat_140778684[1]).
void fun_140373650(i32* obj, i32* first, i32* last, i32* inst, i32* bc, i32* sw, i32* flags,
                   double* nrm, i32* fid, double* xe, double* x, i32* insttab, i32* off);

// @src 140374252  local coordinates (xi, eta) of point p on quad xp(3,4); *bad = 1 when p is
// outside (beyond 1e-4 / b84720) or off the plane (beyond 0.01 / b84728); mode 1..6 zeroes
// components of the normal first (the drop passes 0)
void fun_140374252(double* xp, double* p, double* xi, double* eta, i32* bad, i32* mode);
// @src 140374954  the same for triangle xp(3,3) (barycentric xi, eta)
void fun_140374954(double* xp, double* p, double* xi, double* eta, i32* bad, i32* mode);

// @src 140370a26  for every node of the free bodies: contact point Q on the fixed face of the last
// negative slot, signed gap GAP(n) = ±|Q - X(:,n)|, release BC(s,n) = 0 when separated
void fun_140370a26(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   double* gap, i32* nb, i32* nb2, i32* nodes);

// @src 140371339  contact forces: constant normal force per contact node, moments about the
// centre, total rescaled so that its component along d equals *w; added to f6(6)
void fun_140371339(double* rt, double* pt, double* x, double* xu, i32* it, i32* f, i32* fu,
                   i32* bc, i32* sw, i32* fid, double* nrm, double* xe, i32* flags, double* vn,
                   i32* nb, i32* nb2, i32* nodes, double* f6, double* w, double* d, double* kc,
                   double* damp, double* p15, double* fn);

} // namespace def::cmd::drop
