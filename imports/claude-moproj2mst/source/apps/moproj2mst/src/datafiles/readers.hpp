// P06 internal: the readers on an opened file, without the project and path plumbing (tests use
// these directly).
#pragma once

#include <string>

#include "m2m/data/boolean.hpp"
#include "m2m/data/cogging.hpp"
#include "m2m/data/cycle.hpp"
#include "m2m/data/diestressbm.hpp"
#include "m2m/data/express.hpp"
#include "m2m/datafiles.hpp"

namespace m2m::datafiles {

// Process globals of the Cogging readers that outlive one file (one GUI session = one open here).
using CogGlobals = Project::CogGlobals;

// The cogging process globals of g (created with their .data/.bss values on first use).
data::CogProcess& cogProcess(CogGlobals& g);

// @src 0x1407613b0 with the project's cogging globals (DieMov, MergeL, AxialSub persist per open)
void cogModelDefaults(data::CogModel& m, int unit, CogGlobals& g);

// @src 0x1407617e0 after fopen: the bytes of Cogging.MDT for model m (text mode: LF -> CR LF).
// header = the installation text of line 3 after "* " (mdtHeaderText). Sets the NoDies global as
// the original does. Throws Error(M2M_E_UNSUPPORTED) where the original's output is not defined.
std::string writeMdtBytes(const data::CogModel& m, CogGlobals& g, const std::string& header);

enum class MdtResult { Ok, Corrupt };
// @src 0x140761d90 after fopen: 0x14076a460 sections, then Process Data when the version > 10999.
MdtResult readMdtFile(TextFile& f, data::CogModel& m, CogGlobals& g);

// @src 0x1411e1dd0 loop (token reader 0x1409068f0). False: the original's "ERROR LOADING FILE".
bool readBwzFile(TextFile& f, data::BooleanData& d);
// @src 0x1413f83e0 loop
void readFwzFile(TextFile& f, data::ExpressData& d);
// @src 0x14134ec40 loop
void readCwzFile(TextFile& f, data::CycleData& d);
// @src 0x1402a1df0 after QFile::open: bytes of the file (Qt text mode)
void readWzdBytes(const std::string& bytes, data::DiestressBMData& d);

}  // namespace m2m::datafiles
