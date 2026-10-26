// The conversion driver (owner: G0). Mirrors the GUI sequence File > Open (0x14001ca30) then
// File > Save (0x1400980d0 -> 0x1400a20d0):
//   project ctor (0x140089880) for the DB name changeSuffix(file, ".DB"), class registry
//   (0x14006dba0), OpenProject (0x14008a9b0), activation of sub-project 0 (0x1400a7250), then the
//   full Save opens the MST before its writer dispatch and performs Boolean writes in place.
//   Other adapter modes retain their existing in-memory/atomic output behavior.
#define WIN32_LEAN_AND_MEAN
#include <windows.h>

#include <atomic>
#include <mutex>
#include <filesystem>

#include "m2m/convert.hpp"
#include "m2m/datafiles.hpp"
#include "m2m/data/distortion.hpp"
#include "m2m/doe_io.hpp"
#include "m2m/engine.hpp"
#include "m2m/engine/conversion_session.hpp"
#include "m2m/longpath.hpp"
#include "m2m/model.hpp"
#include "m2m/p17.hpp"
#include "m2m/moproj_write.hpp"
#include "m2m/mst/emit.hpp"
#include "m2m/mst/file_device.hpp"
#include "m2m/qt4path.hpp"

namespace m2m {
namespace {

std::wstring wide(const std::u16string& s)
{
    return std::wstring(s.begin(), s.end());
}

bool sameFile(const std::u16string& a, const std::u16string& b)
{
    const std::wstring fa = fullPathName(wide(a)), fb = fullPathName(wide(b));
    if (fa.empty() || fb.empty()) return false;
    return CompareStringOrdinal(fa.c_str(), -1, fb.c_str(), -1, TRUE) == CSTR_EQUAL;
}

std::string lastErrorText(DWORD e)
{
    wchar_t* buf = nullptr;
    FormatMessageW(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,
                   nullptr, e, 0, reinterpret_cast<wchar_t*>(&buf), 0, nullptr);
    std::u16string u;
    if (buf) {
        for (wchar_t* p = buf; *p; ++p) u += static_cast<char16_t>(*p);
        LocalFree(buf);
    }
    while (!u.empty() && (u.back() == u'\n' || u.back() == u'\r' || u.back() == u' ')) u.pop_back();
    return qt::String(u).utf8() + " (" + std::to_string(e) + ")";
}

// Read access check only: the project is opened with read sharing and closed again.
void checkReadable(const std::u16string& path)
{
    HANDLE h = CreateFileW(win32Path(path).c_str(), GENERIC_READ, FILE_SHARE_READ | FILE_SHARE_WRITE, nullptr,
                           OPEN_EXISTING, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE)
        throw Error(M2M_E_OPEN, "cannot read " + qt::String(path).utf8() + ": " + lastErrorText(GetLastError()));
    CloseHandle(h);
}

void writeAtomically(const std::u16string& out, const std::string& bytes)
{
    static std::atomic<unsigned> seq{0};
    std::wstring target = wide(out);
    std::wstring tmp = target + L".m2m" + std::to_wstring(GetCurrentProcessId()) + L"_" +
                       std::to_wstring(GetCurrentThreadId()) + L"_" + std::to_wstring(++seq) + L".tmp";
    // The OS gets the long-path form (m2m/longpath.hpp); messages keep the names as given.
    const std::wstring osTmp = win32Path(tmp), osTarget = win32Path(target);
    HANDLE h = CreateFileW(osTmp.c_str(), GENERIC_WRITE, 0, nullptr, CREATE_NEW, FILE_ATTRIBUTE_NORMAL, nullptr);
    if (h == INVALID_HANDLE_VALUE)
        throw Error(M2M_E_WRITE, "cannot create " + qt::String(std::u16string(tmp.begin(), tmp.end())).utf8() +
                                     ": " + lastErrorText(GetLastError()));
    size_t done = 0;
    bool ok = true;
    DWORD err = 0;
    while (done < bytes.size()) {
        DWORD chunk = static_cast<DWORD>(std::min<size_t>(bytes.size() - done, 1u << 20)), wrote = 0;
        if (!WriteFile(h, bytes.data() + done, chunk, &wrote, nullptr) || wrote != chunk) {
            ok = false;
            err = GetLastError();
            break;
        }
        done += wrote;
    }
    CloseHandle(h);
    if (ok && !MoveFileExW(osTmp.c_str(), osTarget.c_str(), MOVEFILE_REPLACE_EXISTING | MOVEFILE_WRITE_THROUGH)) {
        ok = false;
        err = GetLastError();
    }
    if (!ok) {
        DeleteFileW(osTmp.c_str());
        throw Error(M2M_E_WRITE, "cannot write " + qt::String(out).utf8() + ": " + lastErrorText(err));
    }
}

// The files one step wrote, made and deleted (engine::FileEffects): folders first, then the
// writes in order, then the deletes, which win over a write of the same file as they do for the
// readers (moproj::readProjectFile).
void applyEffects(const engine::FileEffects& fx)
{
    for (const String& d : fx.mkdirs) {
        std::error_code ec;
        std::filesystem::create_directories(std::filesystem::path(win32Path(d.u16())), ec);
        if (ec) throw Error(M2M_E_WRITE, "cannot create the folder " + d.utf8() + ": " + ec.message());
    }
    for (const auto& w : fx.writes) {
        std::error_code ec;
        std::filesystem::create_directories(std::filesystem::path(win32Path(w.path.u16())).parent_path(), ec);
        writeAtomically(w.path.u16(), w.bytes);
    }
    for (const auto& r : fx.renames)   // QFile::rename: fails (silently) when the target exists
        MoveFileW(win32Path(r.from.u16()).c_str(), win32Path(r.to.u16()).c_str());
    for (const String& d : fx.deletes)
        if (!DeleteFileW(win32Path(d.u16()).c_str()) && GetLastError() != ERROR_FILE_NOT_FOUND)
            throw Error(M2M_E_WRITE, "cannot delete " + d.utf8() + ": " + lastErrorText(GetLastError()));
}

// GUI start-up 0x1407fb240: the version text of the KEY banners, a process global of the engine.
// Rewritten only when the DEFORM folder changes, so concurrent conversions with the same folder
// never write it while another reads it.
void engineVersionFor(const std::u16string& deformDir)
{
    static std::mutex lock;
    static bool set = false;
    static std::u16string last;
    std::lock_guard<std::mutex> g(lock);
    if (set && last == deformDir) return;
    engine::setEngineVersion(deformDir);
    last = deformDir;
    set = true;
}

std::string joinWarnings(const Ctx& ctx)
{
    std::string s;
    for (const auto& w : ctx.warnings) {
        if (!s.empty()) s += '\n';
        s += w;
    }
    return s;
}

Result run(const std::u16string& moproj, const std::u16string& outMst, unsigned flags, unsigned codepage,
           int guiUnit, const std::u16string& deformDir, std::string& mst, std::u16string& outPath,
           SideBytes* side, bool writeFile, const OriginalSettings& settings)
{
    // The engine (the DEF_PRE port: the project DB reader's Fortran units, KEY reader and model
    // globals) is process-wide state, as in the original's single process. Conversions therefore
    // run one at a time; calls from several threads wait here.
    static std::mutex engineMutex;
    std::lock_guard<std::mutex> engineLock(engineMutex);
    Result r;
    Ctx ctx;
    ctx.originalSettings = settings;
    ctx.flags = flags;
    ctx.codepage = codepage;
    ctx.guiUnitOverride = guiUnit;
    try {
        GuiStartupUnitScope startupUnit(guiUnit); // before project, registry and any GUI DB ctor
        qt::CurrentDirScope keepDir(qt::currentPath());
        if (moproj.empty()) throw Error(M2M_E_ARG, "no project path");
        if ((flags & ~(M2M_FLAG_DOE_TEMP | M2M_FLAG_STRICT | M2M_FLAG_WRITE_MDT | M2M_FLAG_WRITE_SAVE)) != 0) throw Error(M2M_E_ARG, "unknown flags");
        if (flags & M2M_FLAG_DOE_TEMP) throw Error(M2M_E_UNSUPPORTED, "the DOE .MST.TEMP form is not supported yet");
        // Full lifecycle starts with an already selected path. Native01ca30
        // constructs/registers the model before08a9b0 attempts the read. Even a
        // missing or sharing-denied .moproj reaches its current-dir/remesh tail
        // and later openedOk Save decision. Do not insert a second early open.
        // The narrower legacy conversion ABI retains its E_OPEN preflight.
        if (!(flags & M2M_FLAG_WRITE_SAVE)) checkReadable(moproj);

        // the GUI's file dialog always gives an absolute path; a relative one is taken against the
        // directory the conversion starts in (the open later changes the thread's directory, and
        // the .moproj writer's item paths are relative to the project's absolute folder)
        const qt::String given{moproj};
        const qt::String file = qt::isRelativePath(given) ? qt::cleanPath(qt::currentPath() + qt::L("/") + given) : given;
        engine::DiagramEffectDriver diagramEffects;
        engine::ConversionSession session(diagramEffects);
        Project& p = session.project(); // owns ProjectScope and retains Display through model teardown
        // Full Save recovers original global Open/Save effects. The legacy
        // MST-only/MDT-only adapters retain their narrower conversion profile.
        ctx.session = (flags & M2M_FLAG_WRITE_SAVE) ? &session : nullptr;
        // 0x14001ca30: data root for changeSuffix(file, ".DB"), then the project ctor with its dir/base.
        const qt::String db = app::changeSuffix(file, qt::L(".DB"));
        if (ctx.session) session.prepareProjectModel(db);
        initProject(p, qt::absolutePath(db), qt::completeBaseName(db));
        if (ctx.session) session.commitProjectModelConstruction();
        p.doeIo = writeFile && (flags & M2M_FLAG_WRITE_SAVE)
                      ? doe::makeNativeStudyIo() : doe::makeRecordingStudyIo();
        p.doeIo->bindSessionNotificationState(session.sessionNotificationStateOwner());
        p.doeIo->bindSliceConstructorRuntime(session.sliceConstructorRuntimeOwner());
        p.doeIo->bindTaskAllocationStore(session.taskAllocationStoreOwner());
        p.doeIo->bindTaskFactoryRuntime(session.taskFactoryRuntimeOwner());
        if (ctx.session) p.doeIo->bindModelManager(session.modelManagerOwner());
        p.doeIo->setQtWarningSink([&ctx](const std::string& text) {
            ctx.originalDiagnostics.push_back(text);
        });
        p.originalSettings = ctx.originalSettings;
        ctx.nativeOperationBlobs = writeFile && (flags & M2M_FLAG_WRITE_SAVE);
        ctx.nativeItemBlobs = ctx.nativeOperationBlobs;
        ctx.beforeNativeOperationBlob = [&](const std::u16string& target) {
            if (!outMst.empty() && sameFile(target, outMst))
                throw Error(M2M_E_ARG, "the output path is an embedded operation KEY");
        };
        ctx.beforeNativeItemBlob = [&](const std::u16string& target) {
            if (!outMst.empty() && sameFile(target, outMst))
                throw Error(M2M_E_ARG, "the output path is an embedded item KEY");
        };
        makeRegistry(p);
        const bool acceptedProject = openProject(p, file, ctx);
        // Native01ca30 tests the accepted-suffix result, not Project+55.
        // Full Save must retain/select a Project whose XML read failed and
        // reach saveCurrent's later openedOk decision. Narrow legacy adapters
        // retain their existing early parse-status mapping.
        if (!acceptedProject || (!ctx.session && !p.openedOk))
            throw Error(M2M_E_PARSE, "Error on reading project file!");
        if (ctx.session) session.commitProjectModelSelection();
        activateSubProject(p, 0, ctx);

        mst::State st;
        engine::FileEffects step5;
        outPath = outMst.empty() ? mst::mstPath(p, true).u16() : outMst;
        if (sameFile(outPath, moproj)) throw Error(M2M_E_ARG, "the output path is the project file");
        // 0x1400980d0 as File > Save after File > Open: the open loads the active operation
        // (0x14003a3c0), Save checks and saves it (step 2, 0x140052ec0: its model effects reach
        // the MST), writes the .moproj (step 3, 0x1400983c0, with the GUI's views), then
        // properties vt+0xb8 per operation (step 4, the Cogging.MDT writer) and the MST (step 5),
        // which sees the globals step 4 left.
        // M2M_FLAG_WRITE_SAVE: the files the open, step 2 and step 3 wrote or deleted are applied
        // too. Without it step 3 is not run: nothing reads what it changes, and it writes only the
        // .moproj.
        const bool withSave = (flags & M2M_FLAG_WRITE_SAVE) != 0;
        const bool withMdt = withSave || (flags & M2M_FLAG_WRITE_MDT) != 0;
        const bool nativeSave = writeFile && withSave;
        std::vector<datafiles::SideFile> mdts;
        engine::FileEffects opened;
        engine::SaveEffects saved;
        engineVersionFor(deformDir);
        const auto rejectNativeKeyAlias = [&](const String& target) {
            if (sameFile(target.u16(), outPath))
                throw Error(M2M_E_ARG, "the output path is a task KEY file");
        };
        if (nativeSave) {
            opened.nativeTaskKeys = true;
            opened.nativeRemovals = true;
            opened.nativePathCodepage = codepage;
            opened.beforeNativeKeyWrite = rejectNativeKeyAlias;
            opened.beforeNativeRemoval = rejectNativeKeyAlias;
        }
        Operation* openedOperation = engine::openActiveOperation(p, ctx, opened);
        engine::FileEffects blobs;
        for (const auto& [path, bytes] : p.overlay) blobs.writes.push_back({path, bytes});
        const auto checkOutputAlias = [&](const engine::FileEffects& effects) {
            for (const auto& f : effects.writes)
                if (sameFile(f.path.u16(), outPath)) throw Error(M2M_E_ARG, "the output path is a file Save writes");
        };
        if (nativeSave) {
            if (sameFile((qt::currentPath() + qt::kSep + qt::L("LOCAL_REMESH.DAT")).u16(), outPath))
                throw Error(M2M_E_ARG, "the output path is the session remesh file");
            if (openedOperation && sameFile(simCtrlPath(*openedOperation).u16(), outPath))
                throw Error(M2M_E_ARG, "the output path is the active SimCtrl.KEY");
            // Adapter aliases are rejected before their corresponding mutations.
            for (const auto& operation : p.operations)
                if (operation && dynamic_cast<BooleanProps*>(operation->props.get()))
                    for (const char* name : {"Boolean.BWZ", "DEF_MACHDIST_CTRL2.INI"})
                        if (sameFile((taskDirAbs(*operation) + qt::kSep + qt::L(name)).u16(), outPath))
                            throw Error(M2M_E_ARG, "the output path is a Boolean side file");
            for (const auto& operation : p.operations)
                if (operation && dynamic_cast<ExpressProps*>(operation->props.get()) &&
                    sameFile((taskDirAbs(*operation) + qt::kSep + qt::L("Forming.FWZ")).u16(), outPath))
                    throw Error(M2M_E_ARG, "the output path is a Forming.FWZ");
            for (const auto& operation : p.operations)
                if (operation && dynamic_cast<CoggingProps*>(operation->props.get()) &&
                    sameFile((taskDirAbs(*operation) + qt::kSep + qt::L("Cogging.MDT")).u16(), outPath))
                    throw Error(M2M_E_ARG, "the output path is a Cogging.MDT");
            for (const auto& operation : p.operations) {
                if (!operation) continue;
                const auto* property = operation->props.get();
                const char* name = dynamic_cast<const DiestressProps*>(property) ? "DieStress.Wzd" :
                    dynamic_cast<const DiestressBMProps*>(property) ? "DieStressBmWizard.Wzd" :
                    dynamic_cast<const HeatTreatProps*>(property) ? "HeatTreatWizard.HTWZ" : nullptr;
                if (name && sameFile((taskDirAbs(*operation) + qt::kSep + qt::L(name)).u16(), outPath))
                    throw Error(M2M_E_ARG, "the output path is an operation wizard");
            }
            for (const auto& operation : p.operations) {
                const auto* props = operation ? dynamic_cast<DistortionProps*>(operation->props.get()) : nullptr;
                if (!props || !props->data || !props->owner) continue;
                const auto dir = taskDirAbs(*props->owner) + qt::kSep;
                if (sameFile((dir + qt::L("MachDistortion.MDWZ")).u16(), outPath))
                    throw Error(M2M_E_ARG, "the output path is a MachDistortion.MDWZ");
                const auto name = qt::fileName(String(outPath)).u16();
                const auto separator = name.rfind(u'_');
                bool validIndex = false;
                const int index = separator == std::u16string::npos ? -1 :
                    qt::toInt(String(name.substr(separator + 1)), &validIndex);
                if (validIndex && index >= 0 && static_cast<std::int64_t>(index) <=
                        static_cast<std::int64_t>(props->data->passCount) + 1 &&
                    sameFile((dir + qt::L("DEF_MACHDIST_CTRL2.INI_") + qt::number(index)).u16(), outPath))
                    throw Error(M2M_E_ARG, "the output path is a Distortion INI");
            }
            checkOutputAlias(blobs);
            checkOutputAlias(opened);
            applyEffects(blobs);
            applyEffects(opened);
        }
        if (withSave) {
            const auto views = moproj::guiViews(p);
            if (nativeSave) {
                saved.step2.nativeDirectories = true;
                saved.step2.nativeSimCtrl = true;
                saved.step2.nativeTaskKeys = true;
                saved.step2.nativeRemovals = true;
                saved.step2.nativePathCodepage = codepage;
                saved.step2.beforeNativeKeyWrite = rejectNativeKeyAlias;
                saved.step2.beforeNativeRemoval = rejectNativeKeyAlias;
                if (!engine::saveCurrent(p, ctx, saved)) throw Error(M2M_E_PARSE, "Error on reading project file!");
                checkOutputAlias(saved.step2);
                applyEffects(saved.step2);
                // Original0x1400980d0 ignores this false until after properties
                // and MST. Keep DOM mutation behind the actual project-file open.
                saved.call.projectWriteAttempted = true;
                saved.call.projectWriteSucceeded = moproj::saveProjectFileNative(p, false, *views, {&opened, &saved.step2});
                saved.call.projectWriteReturned = true;
            } else if (!engine::saveProject(p, ctx, *views, opened, saved)) {
                throw Error(M2M_E_PARSE, "Error on reading project file!");
            }
        } else if (!engine::saveCurrent(p, ctx, saved)) {
            throw Error(M2M_E_PARSE, "Error on reading project file!");
        }
        const auto mdtHeader = datafiles::mdtHeaderText(deformDir);
        if (!nativeSave) datafiles::saveCoggingMdts(p, mdtHeader, withMdt, &mdts);
        // step 4: props vt+0xb8 in operation order, including Cogging.MDT and L6 (DieStress.Wzd,
        // DieStressBmWizard.Wzd, MachDistortion.MDWZ + DEF_MACHDIST_CTRL2.INI_<i>,
        // HeatTreatWizard.HTWZ), per operation in the same order; only with the Save files
        engine::FileEffects step4;
        ctx.nativePropertyFiles = nativeSave;
        if (withSave) {
            for (auto& opPtr : p.operations) {
                if (!opPtr || !opPtr->props) continue;
                engine::FileEffects current;
                auto& effects = nativeSave ? current : step4;
                if (nativeSave && datafiles::saveCoggingMdt(p, *opPtr, mdtHeader, true, true, nullptr))
                    continue;
                if (!engine::l6PropsSave(p, *opPtr, effects, ctx))
                    engine::expressPropsSave(p, *opPtr, effects, ctx); // Forming.FWZ (L3)
                if (nativeSave) {
                    checkOutputAlias(current);
                    applyEffects(current);                         // remaining writer-specific gaps
                }
            }
        }
        if (withSave)
            for (const engine::FileEffects* fx : {&blobs, &opened, &saved.step2, &saved.project, &step4}) {
                for (const auto& f : fx->writes) ctx.pendingWrites.push_back(f.path.u16());
                for (const auto& dir : fx->mkdirs) ctx.pendingDirectories.push_back(dir.u16());
            }
        if (nativeSave) {
            // Each earlier property row has already attempted its writes. Native
            // writers retain their own failure/continuation contract; the remaining
            // recorded writers still have explicit applyEffects failure gaps.
            ctx.pendingWrites.clear();
            ctx.pendingDirectories.clear();
            if (p.subs.empty()) throw Error(M2M_E_NO_OPERATIONS, "the project has no sub-project to write");
            // 0x1401313c0: failed open skips mask reset, generation and all its
            // Boolean effects. Internal false maps to the existing adapter WRITE
            // code; the GUI's separate success wrapper is not an m2m ABI status.
            saved.call.mstWriteAttempted = true;
            mst::FileDevice target{String(outPath)};
            std::string mstFailure;
            if (!target.opened()) {
                saved.call.mstWriteSucceeded = false;
                saved.call.mstWriteReturned = true;
                mstFailure = "cannot open MST " + String(outPath).utf8();
            } else {
                mst::Stream output(codepage, &target);
                mst::W writer{output, st, ctx, &step5};
                ctx.nativeMstSideFiles = true;
                (void)mst::writeMainMst(writer, p);
                target.close(); // QFile close retries pending buffer before handle release
                mst = output.bytes();
                saved.call.mstWriteSucceeded = !output.writeFailed();
                saved.call.mstWriteReturned = true;
                if (!saved.call.mstWriteSucceeded) mstFailure = "cannot write MST " + String(outPath).utf8();
            }
            // 09823e..: remesh singleton Save/remove runs even after either
            // project/MST writer returned false. Adapter error reporting follows.
            const auto remesh = engine::simcontrol300::saveRemeshAtProjectSave(
                session.remeshState(), qt::currentPath().utf8(), engine::simcontrol300::NumericLocale::KnownC);
            if (remesh.outcome != engine::simcontrol300::ApplyResult::Completed)
                throw Error(M2M_E_UNSUPPORTED, "Save remesh reaches an unrecovered writer or invalid-parameter boundary");
            if (!mstFailure.empty()) throw Error(M2M_E_WRITE, mstFailure);
            if (!saved.call.projectWriteSucceeded) throw Error(M2M_E_WRITE, "cannot write project " + p.path.utf8());
        } else {
            mst::Stream output(codepage);
            mst::W writer{output, st, ctx, withSave ? &step5 : nullptr};
            if (!mst::writeMainMst(writer, p)) throw Error(M2M_E_NO_OPERATIONS, "the project has no sub-project to write");
            mst = output.bytes();
        }
        if (ctx.strict() && !ctx.warnings.empty()) throw Error(M2M_E_INPUT, joinWarnings(ctx));
        for (const auto& f : mdts) {
            if (sameFile(f.path.u16(), outPath)) throw Error(M2M_E_ARG, "the output path is a Cogging.MDT");
            if (side) side->emplace_back(f.path.u16(), f.bytes);
        }
        const engine::FileEffects* const effects[] = {&blobs, &opened, &saved.step2, &saved.project, &step4, &step5};   // GUI order: open, Save steps 2..5
        if (withSave)
            for (const engine::FileEffects* fx : effects)
                for (const auto& f : fx->writes) {
                    if (sameFile(f.path.u16(), outPath)) throw Error(M2M_E_ARG, "the output path is a file Save writes");
                    if (side) side->emplace_back(f.path.u16(), f.bytes);
                }
        if (writeFile && !nativeSave) {
            if (withSave)
                for (const engine::FileEffects* fx : effects) applyEffects(*fx);
            for (const auto& f : mdts) writeAtomically(f.path.u16(), f.bytes);
            writeAtomically(outPath, mst);
        }
        r.message = joinWarnings(ctx);
    } catch (const Error& e) {
        r.code = e.code;
        r.message = e.what();
    } catch (const std::bad_alloc&) {
        r.code = M2M_E_INPUT;
        r.message = "out of memory";
    } catch (const std::exception& e) {
        r.code = M2M_E_INPUT;
        r.message = std::string("internal error: ") + e.what();
    }
    return r;
}

}  // namespace

Result convert(const std::u16string& moproj, const std::u16string& outMst, unsigned flags, unsigned codepage,
               int guiUnit, const std::u16string& deformDir)
{
    return convert(moproj, outMst, flags, codepage, guiUnit, deformDir, OriginalSettings{});
}

Result convert(const std::u16string& moproj, const std::u16string& outMst, unsigned flags, unsigned codepage,
               int guiUnit, const std::u16string& deformDir, const OriginalSettings& settings)
{
    std::string mst;
    std::u16string outPath;
    return run(moproj, outMst, flags, codepage, guiUnit, deformDir, mst, outPath, nullptr, true, settings);
}

Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, int guiUnit)
{
    return run(moproj, outMst, flags, codepage, guiUnit, {}, mst, outPath, nullptr, false, OriginalSettings{});
}

Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, SideBytes& side,
                      int guiUnit, const std::u16string& deformDir)
{
    return convertToBytes(moproj, outMst, flags, codepage, mst, outPath, side, guiUnit, deformDir,
                          OriginalSettings{});
}

Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, SideBytes& side,
                      int guiUnit, const std::u16string& deformDir, const OriginalSettings& settings)
{
    side.clear();
    return run(moproj, outMst, flags, codepage, guiUnit, deformDir, mst, outPath, &side, false, settings);
}

}  // namespace m2m
