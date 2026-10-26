// m2m/convert.hpp: the conversion as a C++ call (owner: G0). The C ABI and the CLI wrap it.
#pragma once

#include <string>
#include <utility>
#include <vector>

#include "m2m/common.hpp"

namespace m2m {

struct Result {
    int code = M2M_OK;
    std::string message;           // UTF-8: error text, or warnings joined by '\n'
};

// Convert moproj -> MST. outMst empty: next to the project (the original's 0x1400a50a0 rule).
// Without WRITE_SAVE, creates/replaces the output file (temporary file + rename), and with
// M2M_FLAG_WRITE_MDT the cogging operations' Cogging.MDT (before the MST, as File > Save).
// The Save sequence of 0x1400980d0: the active operation is opened and saved (step 2; its effects
// on the engine model reach the MST, its KEY files are written only with M2M_FLAG_WRITE_SAVE), the
// .moproj is written (step 3, only with M2M_FLAG_WRITE_SAVE: it overwrites the project file as the
// GUI's Save does), then step 4 (Cogging.MDT) and step 5 (the MST).
// Native WRITE_SAVE opens MOPROJ before DOM mutation, samples QFile error after Close,
// and continues later property/MST steps even if that project write fails. It opens
// the MST directly before generation. Boolean side files open/write
// independently at their dispatch point; their failed opens do not cancel the MST. The adapter
// retains M2M_E_WRITE for a failed main MST open/stream, independently of GUI Save status.
// Forming.FWZ opens and flushes each original line during property dispatch; a failed
// open leaves it untouched and does not cancel later writers. Other Open/Save-current
// and property writers still have individually tracked deferred/atomic failure-order gaps.
// deformDir: m2m_set_deform_dir_w (empty = the v11.0.2 version text).
Result convert(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
               unsigned codepage, int guiUnit = 0, const std::u16string& deformDir = {});

// Explicit original settings for this call; old overloads above/below retain mode 0.
// Nonzero mode selects editor 300 for active Forming; its Save is explicitly unsupported
// until its full widget apply is recovered. No environment is read by this C++ entry point.
Result convert(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
               unsigned codepage, int guiUnit, const std::u16string& deformDir,
               const OriginalSettings& settings);

// A file besides the MST that the Save writes (M2M_FLAG_WRITE_MDT): absolute path, bytes.
using SideBytes = std::vector<std::pair<std::u16string, std::string>>;

// Same, but return the MST bytes instead of writing a file (tests). outPath receives the path
// the file would have been written to.
Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, int guiUnit = 0);
// Same, with the side files of M2M_FLAG_WRITE_MDT returned instead of written.
Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, SideBytes& side,
                      int guiUnit = 0, const std::u16string& deformDir = {});

Result convertToBytes(const std::u16string& moproj, const std::u16string& outMst, unsigned flags,
                      unsigned codepage, std::string& mst, std::u16string& outPath, SideBytes& side,
                      int guiUnit, const std::u16string& deformDir, const OriginalSettings& settings);

}  // namespace m2m
