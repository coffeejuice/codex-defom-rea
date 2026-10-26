// SfOprCogging MST writer, private to P11 (s1-03 3.3, 4). The writer reads the cogging model that
// the Cogging.MDT reader (P06, include/m2m/data/cogging.hpp) filled at open.
#pragma once

#include "m2m/data/cogging.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/writer.hpp"

namespace m2m::ops::cog {
using data::CogBite;
using data::CogData;
using data::CogModel;
using data::CogPass;
using qt::String;

// ---- accessors of CogModel / SfOprCogData (s1-03 3.3) ---------------------------------------------
// @src 0x140765740 -> 0x140768d20  number of reheats (cog+0xcc)
int numReheats(const CogModel& m);
// @src 0x140765770 -> 0x140768d40  passes of reheat r
int numPasses(const CogModel& m, int r);
// @src 0x1407657b0 -> 0x140768db0  bites of pass (pass+4)
int numBites(const CogModel& m, int r, int p);
// @src 0x140768c00 / 0x140768c40 / 0x140768ca0  index checks
bool reheatValid(const CogData& c, int r);
bool passValid(const CogData& c, int r, int p);
bool biteValid(const CogData& c, int r, int p, int b);
// @src 0x1407692b0  PASS of (r, p)
const CogPass& pass(const CogData& c, int r, int p);
// @src 0x140767bc0  BITE of (r, p, b); with AutoBite every bite is bite 1
const CogBite* bite(const CogData& c, int r, int p, int b);
// @src 0x140768770  automatic bite count
int autoBites(const CogData& c, int r, int p);
// @src 0x1407682a0  die X translation of bite b
double translationX(const CogData& c, int r, int p, int b);
// @src 0x140768650  billet / die / manipulator size
double cogSize(const CogData& c, int a, int b);
// @src 0x140765b00  die count: 4 when die+0x14a8 == 1, else 2
int dieCount(const CogModel& m);
// @src 0x140765f30  die object id: 2, 3, 8, 9
int dieId(int i);
// @src 0x140765fa0  manipulator object id: 4, 5, 6, 7
int manipId(int i);
// @src 0x140766010  die direction
void dieDir(int i, double v[3]);
// @src 0x140766140  manipulator direction (the writer ignores the result)
void manipDir(int i, double v[3]);
// @src 0x140765960 -> 0x14076b060  UseManip (cog+0xc8)
bool useManip(const CogModel& m);
// @src 0x140765a10  adaptive reheat settings; returns Use_Adaptive_Reheat
bool adaptive(const CogModel& m, int* chkMethod, double* tr0, double* tr1, double* aht);

// ---- writer --------------------------------------------------------------------------------------
// The original passes these as separate arguments to every block writer.
struct Blk {
    mst::W& w;
    Project& prj;
    const String& db;
    int* sim;
    Operation* op;
    const CogModel& m;
    int dbStep;                    // arg7: DBREAD argument
};

// @src 0x140752190  block loop (reheat, pass, bite) with the CHKSTOP sections
void writeBlocks(Blk& k, int only);
// @src 0x1407532b0  PHT (pass heat transfer)
void writePHT(Blk& k, int r, int p, bool* first, bool* firstOp, int state, bool* manipFirst);
// @src 0x140754250  PDWL (pass dwell)
void writePDWL(Blk& k, int r, int p, bool* first, bool* firstOp, int state, bool* manipFirst);
// @src 0x140755b90  BDEF (bite deformation)
void writeBDEF(Blk& k, int r, int p, int b, bool* first, bool* firstOp, int state, bool* manipFirst);
// @src 0x140758fd0  BDCT (decompression)
void writeBDCT(Blk& k, int r, int p, int b, double decomp);
// @src 0x140758620  BDW (dwell)
void writeBDW(Blk& k, int r, int p, int b, double dwell);
// @src 0x140755170  BHT (bite heat transfer)
void writeBHT(Blk& k, int r, int p, int b, int state);
// @src 0x1407595c0  SHT (adaptive reheat)
void writeSHT(Blk& k, int r, int p, double tr1, double aht);

}  // namespace m2m::ops::cog
