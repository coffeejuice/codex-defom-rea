// m2m/mst/simcount.hpp: the vt+0x298 simulation lists of every class (owner: P14).
// s1-04 7.4, s1-05 5.6. The base body is OpClass::simList (0x1405f5880); a class whose +0x298 is
// not the base overrides OpClass::simList with a one-line call to its function below.
//
// The bodies read class data through the accessors in "data seams" at the end. Those belong to
// the packet that owns the data; until it merges they are stubs in src/stubs/p14.cpp (the owner
// implements them in its own files and deletes the stub lines).
#pragma once

#include <vector>

#include "m2m/model.hpp"
#include "m2m/props.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m::mst {

// MultiBlow block names (src/ops/multiblow.cpp): "BL<j>-BL", "-DW", "-TI", "-HT", "-TO"; with
// bare false prefixed by the operation name and "-"
String nameBL(const Operation* op, int i, int j, bool bare);
String nameDW(const Operation* op, int i, int j, bool bare);
String nameTI(const Operation* op, int i, int j, bool bare);
String nameHT(const Operation* op, int i, int j, bool bare);
String nameTO(const Operation* op, int i, int j, bool bare);

// @src 0x140068210  entry position: Project::doeSimPos when doeSimPosSet, else pos
void setSimPos(SimEntry& e, int pos, const Operation* op);

// One entry for the operation, numbered ++*counter (bodies 0x1405f5880 and 0x1402b7380 differ
// only in which DB they read step data from, which no entry field the port keeps depends on).
void simListOne(int pos, int* counter, Operation* op, std::vector<SimEntry>& out);

// @src 0x14029a4b0  DiestressBM2D/3D: calls the base (0x1405f5880)
void simListDiestressBM(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
// @src 0x1402b7380  Diestress: one entry
void simListDiestress(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
// @src 0x140e32990 DoeOutput, 0x1412b05c0 Report, 0x14013fbb0 ShapeOptim: empty
void simListNone(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
// @src 0x14134e280  Cycle: CycleData.count x (members' own +0x298, at the cycle's position)
void simListCycle(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
// @src 0x1413642d0  Distortion2D/3D: n + 2 entries (ignores the first-pass field, s1-04 O-8)
void simListDistortion(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
// @src 0x140759d50  Cogging: PHT, PDWL, BDEF/BDCT/BDWL/BHT per bite, SHT per pass
void simListCogging(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
// @src 0x141423390  FormingExpress(2D): base unless express (D+0x110 only, unlike the writer);
// else BL, DW if gap > 0, TO if drop && TO time > 0, per (blow, step)
void simListFormingExpress(OpClass& self, int pos, int* counter, Operation* op,
                           std::vector<SimEntry>& out);
// @src 0x1411b2790 HeatTreat, 0x1411b0170 HeatTreat2D (same body): one entry per schedule step
void simListHeatTreat(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);
// @src 0x140606c10  MultiBlow(2D): BL, DW, TI, HT, TO per (heat, blow), same conditions as the writer
void simListMultiBlow(OpClass& self, int pos, int* counter, Operation* op, std::vector<SimEntry>& out);

// =================================================================================================
// Data seams the bodies above need (owners in brackets)
// =================================================================================================
namespace simdata {
// [P06] @src 0x14134eeb0  CycleData.count (Cycle.CWZ, default 5)
int cycleCount(const CycleProps& p);
// [P13] @src 0x1401cc6b0 -> 0x140cbc690  DistortionPassData pass count (+8)
int distortionPasses(const DistortionProps& p);

// [P11] Cogging model accessors (s1-03 3.3); r, p, b are 1-based
int cogTempType(const CoggingProps& p);                // @src 0x14142ff20  model+0x4
bool cogAdaptive(const CoggingProps& p);               // @src 0x140765a10  return byte model+0x4cc
bool cogHeatTransfer(const CoggingProps& p);           // @src 0x14075dc20  model+0x504
bool cogRemainingDwell(const CoggingProps& p);         // @src 0x14075dc40  model+0x505
double cogDwellTime(const CoggingProps& p);            // @src 0x14075dc60  model+0x510
double cogDecompressionTime(const CoggingProps& p);    // @src 0x14075dc80  model+0x518
int cogReheats(const CoggingProps& p);                 // @src 0x140765740  cog+0xcc
int cogPasses(const CoggingProps& p, int r);           // @src 0x140765770
int cogNoBite(const CoggingProps& p, int r, int pass); // @src 0x1407657b0  PASS+4
int cogAutoBite(const CoggingProps& p, int r, int pass);        // @src 0x1407692b0  PASS+0
double cogPassHeatTmax(const CoggingProps& p, int r, int pass);  // PASS+0x38 (pass HEAT TMAX)
double cogPassDwell(const CoggingProps& p, int r, int pass);     // PASS+0x100 DwellTime
int cogAutoBites(const CoggingProps& p, int r, int pass);       // @src 0x140768770
bool cogHasBite(const CoggingProps& p, int r, int pass, int b); // @src 0x140767bc0 != null

// [P15] FormingExpress data (s1-05 5.1); b, s 1-based
bool feExpress(const ExpressProps& p);                 // @src 0x14142bf50  D+0x110
int feBlows(const ExpressProps& p);                    // @src 0x14142c080  D+0x120
int feSteps(const ExpressProps& p, int b);             // @src 0x14142c110  D+0x128[b-1]
double feGap(const ExpressProps& p, int b, int s);     // @src 0x14142c490  D+0x160
bool feDrop(const ExpressProps& p, int b, int s);      // @src 0x14142c5f0  D+0x168
double feToTime(const ExpressProps& p);                // @src 0x14142bd00  D+0xd0

// [P16] @src 0x141681de0 -> 0x1411b0a70  schedule step count; false when there is no schedule
bool heatTreatSteps(const HeatTreatProps& p, int* n);
}  // namespace simdata

}  // namespace m2m::mst
