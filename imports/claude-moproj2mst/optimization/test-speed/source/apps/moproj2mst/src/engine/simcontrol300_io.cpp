#include "m2m/engine/simcontrol300_io.hpp"

#include <cmath>

namespace m2m::engine::simcontrol300 {

bool Extra3DIo::writeStringList(const qt::String& path, const std::vector<qt::String>& items)
{
    auto file = openWrite(path);
    if (!file || !file->opened()) return false;
    mst::Stream stream(codepage_, file.get());
    for (const auto& item : items) {
        stream << item;
        if (!(item == qt::L("\n"))) stream << "\t";
    }
    stream << "\r\n";
    file->close();
    return true;
}

// @src0x140ec8110, restricted to the initial scalar controls and no spring
// records. Reader return values are retained independently of parsed zeros.
std::optional<Extra3DPage> fillExtra3D(Extra3DIo& io, NumericLocale locale)
{
    if (locale != NumericLocale::KnownC) return std::nullopt;
    Extra3DPage form;
    std::array<double, 2> nbc{};
    form.nbcEnabled = io.readDoubles(qt::L("NBC.DAT"), nbc);
    if (form.nbcEnabled && (!std::isfinite(nbc[0]) || !std::isfinite(nbc[1])))
        return std::nullopt;
    form.nbc = form.nbcEnabled
        ? std::array{qt::number(nbc[0], 'g', 6), qt::number(nbc[1], 'g', 6)}
        : std::array{qt::L("75.0"), qt::L("105.0")};
    std::array<int, 1> integer{};
    form.switchToSparseEnabled = io.readInts(qt::L("SW2SP.DAT"), integer);
    form.switchToSparse = form.switchToSparseEnabled ? qt::number(integer[0]) : qt::L("140000");
    integer[0] = 0;
    form.stretchEnabled = io.readInts(qt::L("STRETCH.DAT"), integer);
    form.stretchKind = form.stretchEnabled ? qt::number(integer[0]) : qt::L("1");
    std::array<double, 3> stretch{};
    const bool stretchValues = form.stretchEnabled && io.readDoubles(qt::L("STRETCH.DAT"), stretch);
    if (stretchValues && (!std::isfinite(stretch[1]) || !std::isfinite(stretch[2])))
        return std::nullopt;
    form.stretch = stretchValues
        ? std::array{qt::number(stretch[1], 'g', 6), qt::number(stretch[2], 'g', 6)}
        : std::array{qt::L("0"), qt::L("0")};
    std::array<double, 1> weight{};
    form.weightEnabled = io.readDoubles(qt::L("B_WEIGHT.DAT"), weight);
    if (form.weightEnabled && !std::isfinite(weight[0])) return std::nullopt;
    form.weight = form.weightEnabled ? qt::number(weight[0], 'g', 6) : qt::L("0");
    form.strainDistributionEnabled = io.readDoubles(qt::L("STRAIN_DST.DAT"), {});
    std::vector<SpringRecord> springs;
    if (io.readSprings(springs) && !springs.empty()) return std::nullopt;
    return form;
}

void constructorRefreshExtra3D(Extra3DPage& form)
{
    form.nbc = {qt::number(75.0, 'g', 6), qt::number(105.0, 'g', 6)};
}

// @src0x140ec9040. Every failed write/delete continues at the original next
// control; only NBC deletion reports a warning. No rollback or batch effects.
ApplyResult applyExtra3D(Extra3DIo& io, const Extra3DPage& form)
{
    if (form.nbcEnabled)
        io.writeStringList(qt::L("NBC.DAT"), {form.nbc[0], qt::L("\t"), form.nbc[1]});
    else if (!io.removeIfReadable(qt::L("NBC.DAT"))) io.nbcRemoveWarning();
    if (form.switchToSparseEnabled)
        io.writeStringList(qt::L("SW2SP.DAT"), {form.switchToSparse});
    else io.removeIfReadable(qt::L("SW2SP.DAT"));
    if (form.stretchEnabled)
        io.writeStringList(qt::L("STRETCH.DAT"),
            {form.stretchKind, qt::L("\t"), form.stretch[0], qt::L("\t"), form.stretch[1]});
    else io.removeIfReadable(qt::L("STRETCH.DAT"));
    if (form.weightEnabled) io.writeStringList(qt::L("B_WEIGHT.DAT"), {form.weight});
    else io.removeIfReadable(qt::L("B_WEIGHT.DAT"));
    if (form.strainDistributionEnabled) io.writeStringList(qt::L("STRAIN_DST.DAT"), {});
    else io.removeIfReadable(qt::L("STRAIN_DST.DAT"));
    io.removeIfReadable(qt::L("SPRING.INI"));
    io.removeIfReadable(qt::L("SPRING2.INI"));
    return ApplyResult::Completed;
}
} // namespace m2m::engine::simcontrol300
