// SfObjectSetupBool's filled selections/apply and Boolean's MST side files.
// Static original contracts: 0x1411e25b0, 0x1411e2870, 0x1411e1820.
#include <cstring>
#include <filesystem>
#include "engine_internal.hpp"
#include "m2m/data/boolean.hpp"
#include "m2m/data/distortion.hpp"
#include "m2m/mst/loader_l5.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/mst/boolean_side_file_result.hpp"
#include "m2m/mst/writer.hpp"
#include "m2m/props.hpp"
#include "m2m/qt4path.hpp"

namespace m2m::mst {
using engine::at;
using engine::FileEffects;
using qt::L;

// @src 0x1411e25b0. A matching workpiece excludes the cutter match,
// even if the two stored numbers agree. The constructor calls this fill.
bool booleanEditorFill(BooleanObjectEditor& ed, Project& p, Operation& op)
{
    ed = BooleanObjectEditor{};
    auto* props = dynamic_cast<BooleanProps*>(op.props.get());
    if (!engine::opModel(p, op) || !props || !props->data) return false;
    ed.shown = true;
    ed.workpieceText = ed.cutterText = L("Add New Object");
    const auto& d = *props->data;
    for (int s = 0; s < static_cast<int>(op.objs.size()); ++s) {
        auto* o = engine::slotObject(p, op, s);            // 0x140057690
        if (!o || *at<std::int32_t>(o, 0xc) == 7) continue; // 0x140834fa0
        const int no = *at<std::int32_t>(o, 8);
        const String text = L("(") + qt::number(no) + L(") ") + String::latin1(at<char>(o, 0x828));
        if (no == d.workpiece) ed.workpieceText = text;
        else if (no == d.cutter) ed.cutterText = text;
    }
    return true;
}

namespace {
int selectionNumber(const String& text)
{
    // QString::section('(',1,1).section(')',0,0).toInt(nullptr,10).
    const auto& s = text.u16();
    const auto begin = s.find(u'(');
    if (begin == std::u16string::npos) return 0;
    const auto end = s.find(u'(', begin + 1);
    const String section(s.substr(begin + 1, end == std::u16string::npos ? end : end - begin - 1));
    return qt::toInt(String(section.u16().substr(0, section.u16().find(u')'))));
}
} // namespace

// @src 0x1411e2870: workpiece first, then cutter. The selection is the
// editor's filled snapshot, rather than the current properties' number.
bool booleanEditorApply(BooleanObjectEditor& ed, Project& p, Operation& op, FileEffects& fx)
{
    if (!ed.shown) return false;
    auto* props = dynamic_cast<BooleanProps*>(op.props.get());
    if (!props || !props->data) return true;
    auto apply = [&](const String& text, int& no, const char* name) {
        if (text != L("Add New Object")) { no = selectionNumber(text); return; }
        int slot = 0;                                    // 0x140060530, before add/trim
        for (; slot < static_cast<int>(op.objIds.size()); ++slot)
            if (op.objIds[slot] == -1 || op.objIds[slot] == -99) break;
        auto* o = engine::addObject(p, op, fx);           // 0x140057ba0
        if (!o) throw Error(M2M_E_UNSUPPORTED, "Boolean editor add-object returned no object (0x1411e2870)");
        no = *at<std::int32_t>(o, 8);
        std::strcpy(at<char>(o, 0x828), name);            // 0x14146fec0, literal format
        renameSlot(p, op, slot, String::latin1(at<char>(o, 0x828)), fx); // 0x1400585e0
    };
    apply(ed.workpieceText, props->data->workpiece, "Workpiece");
    apply(ed.cutterText, props->data->cutter, "Cutter");
    return true;
}

namespace {
// @src 0x1411638a0: default FreeDistBcc(0x1411631e0), +0x91=1
// (0x1411e2450). Same recovered field order as save_l6.cpp's helper.
void freeDistBcc(Stream& ts)
{
    const data::FreeDistBcc fd{};
    ts << "BEGIN_FREE_DIST_BCC_INFO" << endl << "BCC_INIT    \t" << 1 << endl;
    ts << "FIXEDNODE    \t" << fd.fixedNode << endl;
    for (const char* label : {"FIXEDCOORD    ", "FIXED_Z_COORD    ", "FIXED_X_COORD    ", "FIXED_Y_COORD    "}) {
        ts << label;
        for (int k = 0; k < 3; ++k) ts << "\t" << 0.0;
        ts << endl;
    }
    ts << "AUTO    \t" << static_cast<int>(fd.autoFix) << endl;
    ts << "FIXEDZNODE    \t" << fd.fixedZNode << endl << "FIXEDZANGLE    \t" << fd.fixedZAngle << endl;
    ts << "FIXEDXNODE    \t" << fd.fixedXNode << endl << "FIXEDXANGLE    \t" << fd.fixedXAngle << endl;
    ts << "FIXEDYNODE    \t" << fd.fixedYNode << endl << "FIXEDYANGLE    \t" << fd.fixedYAngle << endl;
    ts << "END_FREE_DIST_BCC_INFO" << endl;
}

void writeBooleanBwz(Stream& ts, const data::BooleanData& d)
{
    ts << "WZD_MACHDIST_WORKPIECE_OBJECT_NO" << endl << "  * OBJECT # FOR WORKPIECE " << endl
       << d.workpiece << endl << endl;
    ts << "WZD_BOOLEAN_CUTTER_OBJECT_NO" << endl << "  * OBJECT # FOR CUTTER " << endl
       << d.cutter << endl << endl;
    ts << "WZD_BOOLEAN_REMESH" << endl << "  * PERFORM REMESH AFTER BOOLEAN" << endl
       << static_cast<int>(d.remesh) << endl << endl << "DONE" << endl << endl;
}

void writeBooleanIni(Stream& ts, Operation& op, const data::BooleanData& d)
{
    const String step = qt::cleanPath(op.taskDir + qt::kSep + L("Step.KEY"));
    ts << " * Keyword file which contains the workpiece " << endl << "KFREAD" << endl
       << qt::toAscii(step).c_str() << endl << endl;
    ts << " * Workpiece object #" << endl << "WZD_MACHDIST_WORKPIECE_OBJECT_NO" << "\t"
       << d.workpiece << endl << endl;
    ts << " * Pass keyword file name" << endl << "WZD_MACHDIST_PASS_OBJNO" << endl
       << d.cutter << endl << endl;
    ts << " * Boolean operation" << endl << "WZD_MACHDIST_OPERATION_BOOLEAN" << endl << endl;
    if (auto* c = opBase(op); c && c->dim() == 3) freeDistBcc(ts); // 0x14005f1c0
    ts << " * Output file name " << endl << "WZD_BOOLEAN_OUTPUT_FILENAME" << endl
       << qt::cleanPath(op.taskDir + qt::kSep + L("DEF_MACHDIST_OUT.KEY")) << endl << endl;
}
} // namespace

int booleanSideFilesNativeResult(Project& project, Operation& op, const data::BooleanData& d, Ctx& ctx)
{
    if (!ctx.nativeMstSideFiles)
        throw Error(M2M_E_UNSUPPORTED, "Boolean native side-file result requires actual native Save mode");
    auto emit = [&](const char* name, const auto& write) {
        const String path = qt::cleanPath(taskDirAbs(op) + qt::kSep + L(name));
        FileDevice file(path);
        if (!file.opened()) return BooleanFileAttemptResult::OpenFailed;
        Stream ts(ctx.codepage, &file);
        write(ts);
        file.close(); // explicit close precedes Stream destruction
        // 1970's opened path returns1 regardless of sticky stream/close status.
        return BooleanFileAttemptResult::OpenedPath;
    };
    return nativeBooleanSideFileSequence(project, op, d, {
        [&] { return emit("Boolean.BWZ", [&](Stream& ts) { writeBooleanBwz(ts, d); }); },
        [&] { return emit("DEF_MACHDIST_CTRL2.INI", [&](Stream& ts) { writeBooleanIni(ts, op, d); }); }
    });
}

// @src 0x1411e1820 -> 0x1411e1970 then 0x1411e0e40. Missing parent
// directory skips a file; no directory is created here. The void L5 adapter
// discards the native helper result; recording mode remains a separate profile.
void booleanSideFiles(Project& project, Operation& op, const data::BooleanData& d, Ctx& ctx, FileEffects& fx)
{
    if (ctx.nativeMstSideFiles) {
        (void)booleanSideFilesNativeResult(project, op, d, ctx);
        return;
    }
    if (!op.project || !op.props) return;
    auto emit = [&](const char* name, const auto& write) {
        const String path = qt::cleanPath(taskDirAbs(op) + qt::kSep + L(name));
        const String dir = qt::cleanPath(qt::absolutePath(path));
        std::error_code ec;
        bool exists = std::filesystem::is_directory(std::filesystem::path(dir.u16()), ec);
        for (const auto& pending : ctx.pendingDirectories)
            if (qt::cleanPath(String(pending)) == dir) exists = true;
        if (!exists) return;
        Stream ts(ctx.codepage);
        write(ts);
        fx.writes.push_back({path, ts.bytes()});
        ctx.pendingWrites.push_back(path.u16());
    };
    emit("Boolean.BWZ", [&](Stream& ts) { writeBooleanBwz(ts, d); });
    emit("DEF_MACHDIST_CTRL2.INI", [&](Stream& ts) { writeBooleanIni(ts, op, d); });
}
} // namespace m2m::mst
