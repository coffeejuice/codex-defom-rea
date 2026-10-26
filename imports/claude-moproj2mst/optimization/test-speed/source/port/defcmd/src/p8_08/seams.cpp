// Seam exports of packet p8-08 (defcmd/meshgen/p8_seams.hpp): exchange-file clean-up and two
// std::string helpers used by p8-05 and p8-07 too.
// Decompile: deform-rea-work/out/p8mg/p8-08/all.c.
#include <defcmd/meshgen/p8_seams.hpp>

#include <defdbio/gendb/gendb.hpp>

namespace def::cmd {

namespace {

void rm(const char* name) { (void)def::dbio::gendb::remove_if_exists(name); }  // 1401b85d0

// @src 14013e960  2D mesher (DEF_AMG) exchange files; NBCD.DAT only when flag
void cleanup_2d_mesher(MeshGenBase&, bool flag)
{
    rm("DEF_AMG.INI");
    rm("DSTPAR2.DAT");
    rm("USRWIN.DAT");
    rm("DEF_MGN.ACT");
    rm("DEF_MGN.OUT");
    rm("mesh.out");
    rm("NEWMSH.GEO");
    rm("NBCD.OUT");
    rm("NBCD.LST");
    rm("NEWNOD.DAT");
    if (flag) rm("NBCD.DAT");
}

// @src 14013ea10  3D mesher (DEF_AMG3_GUI, MESH3D_GUI, BRKMSH, CHKVOL) exchange files
void cleanup_3d_mesher(MeshGenBase&, bool flag)
{
    rm("DEF_AMG3.INI");
    rm("DSTPAR.DAT");
    rm("BLOCK3.DAT");
    rm("DEF_MGN.ACT");
    rm("DEF_MGN.OUT");
    rm("MESH3D.INP");
    rm("mesh.out");
    rm("MESH3D.MSG");
    rm("MESH3D.SCR");
    rm("BRKMSH.INI");
    rm("BRKMSH.KEY");
    rm("BRKMSH.MSG");
    rm("SCRATCH.KEY");
    rm("SCRATCH.DAT");
    rm("NEWMSH.GEO");
    rm("NBCD.OUT");
    rm("NBCD.LST");
    rm("CHK_VOL.INI");
    rm("CHK_VOL.MSG");
    rm("crossed.faces.0");
    rm("DEF_AMG.DST");
    rm("NEWNOD.DAT");
    if (flag) rm("NBCD.DAT");
}

} // namespace

// @src 14013e880
void exchange_cleanup(MeshGenBase& mg, bool flag)
{
    cleanup_2d_mesher(mg, flag);
    cleanup_3d_mesher(mg, flag);
    rm("DEF_INT.ACT");
    rm("DEF_REM.INI");
    rm("REM_SCR.SCR");
    rm("NEW.MSH");
    rm("STEP.DAT");
    rm("DEF_FRZ2.OUT");
    rm("DMESH.DAT");
    rm("FRZ1.INI");
    rm("DEF_FRZI.KEY");
    rm("DEF_FRZI.TMP");
    rm("DEF_REM3.INI");
    rm("SCRATCH.KEY");
    rm("NEWMESH.KEY");
    rm("DEF_INT.OUT");
}

// @src 1401b17b0  std::string("\\") (literal 140524d24, through 1400066a0)
std::string fun_1401b17b0()
{
    return std::string("\\");
}

// @src 1401b1520  by-value string: resize to n when 0 < n < (int)size, return it
std::string fun_1401b1520(std::string s, int n)
{
    const int len = static_cast<int>(s.size());  // 140002800, truncated to int
    if (n < len && 0 < n) {
        s.resize(static_cast<std::size_t>(n));
    }
    return s;
}

} // namespace def::cmd
