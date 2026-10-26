// SfOprDistortionProperties post-load (vt+0xc0): reads Operations/TaskNNNNN/MachDistortion.MDWZ
// into the DistortionPassData (owner P13). Decompiles: deform-rea-work gui_mo/s1/p13/d1..d4.c.
//
// The trailing 0x14005f770(owner) builds the operation's engine model when it has none, and its
// 0x14005f820 makes the operation its class's current item (0x1405ef280, not restored): the sim
// list's 0x141363fc0 of a Distortion operation that was never active reads that item (MDT L5S,
// oracle B 66_B_DIS3 / 67_B_DIS2). Not ported: the message box of a read error, which becomes a
// warning.
#include <cstring>
#include <string>

#include "m2m/data/distortion.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/model.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

namespace m2m {
namespace {

using datafiles::TextFile;
using datafiles::TokenReader;

// strncmp(token, lit, 0x50) == 0 (0x141470380)
bool is(const std::string& tok, const char* lit)
{
    return std::strncmp(tok.c_str(), lit, 0x50) == 0;
}

// @src 0x1413667b0  data file name
String mdwzName()
{
    return String(u"MachDistortion.MDWZ");
}

// @src 0x141163cc0  free-distortion BCC block, after BEGIN_FREE_DIST_BCC_INFO
void readFreeDistBcc(data::FreeDistBcc& fd, TextFile& f)
{
    TokenReader rdr;                                     // 0x140906740
    std::string tok;
    do {
        // The original loops until END_FREE_DIST_BCC_INFO; at the end of the file the reader keeps
        // returning "DONE" and the original never leaves the loop.
        if (rdr.read(f, TokenReader::Upper, &tok, TokenReader::kNewLine) == -2)
            throw Error(M2M_E_INPUT, "MachDistortion.MDWZ: END_FREE_DIST_BCC_INFO missing");
        if (is(tok, "FIXEDNODE")) {
            rdr.read(f, TokenReader::Int, &fd.fixedNode, 0);
        } else if (is(tok, "FIXEDCOORD")) {
            for (int k = 0; k < 3; ++k) rdr.read(f, TokenReader::Double, &fd.fixedCoord[k], 0);
        } else if (is(tok, "FIXED_Z_COORD")) {
            for (int k = 0; k < 3; ++k) rdr.read(f, TokenReader::Double, &fd.fixedZ[k], 0);
        } else if (is(tok, "FIXED_X_COORD")) {
            for (int k = 0; k < 3; ++k) rdr.read(f, TokenReader::Double, &fd.fixedX[k], 0);
        } else if (is(tok, "FIXED_Y_COORD")) {
            for (int k = 0; k < 3; ++k) rdr.read(f, TokenReader::Double, &fd.fixedY[k], 0);
        } else if (is(tok, "AUTO")) {
            rdr.read(f, TokenReader::Bool, &fd.autoFix, 0);
        } else if (is(tok, "FIXEDZNODE")) {
            rdr.read(f, TokenReader::Int, &fd.fixedZNode, 0);
        } else if (is(tok, "FIXEDZANGLE")) {
            rdr.read(f, TokenReader::Double, &fd.fixedZAngle, 0);
        } else if (is(tok, "FIXEDXNODE")) {
            rdr.read(f, TokenReader::Int, &fd.fixedXNode, 0);
        } else if (is(tok, "FIXEDXANGLE")) {
            rdr.read(f, TokenReader::Double, &fd.fixedXAngle, 0);
        } else if (is(tok, "FIXEDYNODE")) {
            rdr.read(f, TokenReader::Int, &fd.fixedYNode, 0);
        } else if (is(tok, "FIXEDYANGLE")) {
            rdr.read(f, TokenReader::Double, &fd.fixedYAngle, 0);
        }
    } while (!is(tok, "END_FREE_DIST_BCC_INFO"));
}

// The original writes count entries into a 100-entry array without a bound.
void readList(TokenReader& rdr, TextFile& f, std::int32_t& count, std::array<std::int32_t, 100>& a)
{
    rdr.read(f, TokenReader::Int, &count, 0);
    if (count > static_cast<int>(a.size()))
        throw Error(M2M_E_INPUT, "MachDistortion.MDWZ: more than 100 entries");
    for (int i = 0; i < count; ++i) rdr.read(f, TokenReader::Int, &a[static_cast<size_t>(i)], 0);
}

// @src 0x1413680e0  read the MDWZ of task folder `dir`; -1 when it cannot be opened or read
int readMdwz(data::DistortionData& D, Project& prj, const String& dir, Operation& owner, Ctx& ctx)
{
    String name;
    TokenReader rdr;                                     // 0x140906740
    D.resetCounts();                                     // 0x141366710
    name = mdwzName();
    String path = qt::convertSeparators(dir + qt::kSep + name);
    TextFile f;
    if (!TextFile::open(prj, path, f)) return -1;        // fopen(latin1(path), "rt")
    std::string tok;
    do {
        int rc = rdr.read(f, TokenReader::Upper, &tok, TokenReader::kNewLine);
        if (rc == -1) {
            // QMessageBox "ERROR LOADING FILE": "Error occurred when file <b> %1 </b> was being
            // read. <br>.Please check the format of the file." with %1 = the file name.
            ctx.warn("ERROR LOADING FILE: Error occurred when file " + name.utf8() +
                     " was being read. Please check the format of the file.");
            return -1;
        }
        if (is(tok, "WZD_MACHDIST_WORKPIECE_OBJECT_NO")) {
            rdr.read(f, TokenReader::Int, &D.workpiece, 0);
        } else if (is(tok, "WZD_MACHDIST_NUM_FIXTURES")) {
            readList(rdr, f, D.fixtureCount, D.fixtures);
        } else if (is(tok, "WZD_MACHDIST_NUM_PASSES")) {
            readList(rdr, f, D.passCount, D.passObjects);
        } else if (is(tok, "BEGIN_FREE_DIST_BCC_INFO")) {
            readFreeDistBcc(D.freeDist, f);
        } else if (is(tok, "INIT_VEL_BCC")) {
            std::uint8_t v = 0;
            rdr.read(f, TokenReader::Bool, &v, 0);
            D.initVelBcc = v;                            // 0x140f8a540
        } else if (is(tok, "$END")) {
            break;
        }
    } while (!is(tok, "DONE"));
    // fclose; then 0x14005f770(owner +0x5d8 -> 0x1405f8d90): 1 with an operation model, else -1
    if (!mst::g_loaderSlots.opModel) mst::loaderSlotsMissing();
    return mst::g_loaderSlots.opModel(prj, owner) ? 1 : -1;
}

}  // namespace

// @src 0x141365c60 -> 0x141367fe0
void DistortionProps::postLoad(Project& p, Ctx& ctx)
{
    if (!data || !owner) return;                         // 0x1405f8d90(+0x5d8 owner props)
    String dir = taskDirAbs(*owner);                     // 0x140050df0
    (void)readMdwz(*data, p, dir, *owner, ctx);
}

}  // namespace m2m
