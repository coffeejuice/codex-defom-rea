#include "m2m/engine/simcontrol300_io.hpp"
#include "m2m/engine/simctrl_ppv_reader.hpp"
#include "m2m/model.hpp"

#include <algorithm>
#include <cmath>
#include <cstdint>
#include <limits>

namespace m2m::engine::simcontrol300 {

namespace {
simctrl_ppv::Point toPpvPoint(const RecoveryEntry& entry)
{
    return {entry.type, entry.ppvSecondInt, entry.ppvFirstDouble, entry.ppvSecondDouble};
}

RecoveryEntry fromPpvPoint(const simctrl_ppv::Point& point)
{
    // Native PPV construction zero-initializes KEY-visible BC+0x18 and then
    // overwrites only the four PPV key fields.
    return {point.firstIndex, 0.0, point.secondIndex, point.firstValue, point.secondValue};
}

std::vector<simctrl_ppv::Point> toPpvPoints(const std::vector<RecoveryEntry>& entries)
{
    std::vector<simctrl_ppv::Point> points;
    points.reserve(entries.size());
    for (const auto& entry : entries) points.push_back(toPpvPoint(entry));
    return points;
}

bool supportedSpringItems(std::span<const SpringRecord> rows)
{
    // Candidate resource admission, not an original-format maximum. The reader
    // uses the same cap; native allocator/failure behavior remains unrecovered.
    constexpr std::size_t maxItems = 256;
    if (rows.empty() || rows.size() > maxItems) return false;
    return std::all_of(rows.begin(), rows.end(), [](const SpringRecord& row) {
        return std::isfinite(row.firstValue) && std::isfinite(row.secondValue) &&
            std::all_of(row.direction.begin(), row.direction.end(), [](double value) { return std::isfinite(value); });
    });
}

bool containsId(std::span<const int> ids, int id)
{
    return std::find(ids.begin(), ids.end(), id) != ids.end();
}

std::array<qt::String, 5> springVisibleNumbers(const SpringRecord& row)
{
    return {qt::number(row.firstValue, 'g', 6), qt::number(row.secondValue, 'g', 6),
        qt::number(row.direction[0], 'g', 6), qt::number(row.direction[1], 'g', 6),
        qt::number(row.direction[2], 'g', 6)};
}

std::optional<int> springLabelId(const qt::String& label)
{
    const auto& text = label.u16();
    if (text.size() >= 64 || std::any_of(text.begin(), text.end(), [](char16_t c) { return c == 0 || c > 127; }))
        return std::nullopt; // Candidate ASCII/64-byte-including-NUL admission.
    std::size_t i = 0;
    while (i < text.size() && (text[i] == 32 || (text[i] >= 9 && text[i] <= 13))) ++i;
    bool negative = false;
    if (i < text.size() && (text[i] == u'+' || text[i] == u'-')) negative = text[i++] == u'-';
    const auto limit = negative ? std::uint64_t{2147483648} : std::uint64_t{2147483647};
    std::uint64_t value = 0;
    for (; i < text.size() && text[i] >= u'0' && text[i] <= u'9'; ++i) {
        const auto digit = static_cast<unsigned>(text[i] - u'0');
        if (value > (limit - digit) / 10) return std::nullopt;
        value = value * 10 + digit;
    }
    if (negative && value == 2147483648ULL) return (std::numeric_limits<int>::min)();
    return negative ? -static_cast<int>(value) : static_cast<int>(value);
}

SpringComboLabels projectedSpringLabels(std::span<const int> first, std::span<const int> second)
{
    SpringComboLabels labels;
    const auto append = [](std::vector<qt::String>& output, std::span<const int> ids) {
        for (int id : ids) {
            auto prefix = std::to_string(id);
            if (prefix.size() < 3) prefix.append(3 - prefix.size(), ' ');
            output.push_back(qt::String::latin1(prefix + " - "));
        }
    };
    append(labels.first, first); append(labels.second, second);
    return labels; // Synthetic parsed-ID compatibility projection, not native widgets.
}
} // namespace

bool setSpringListCurrentRow(SpringSelectionState& state, int row)
{
    if (row < -1 || row >= static_cast<int>(state.items.size())) return false;
    state.listCurrentRow = row; // Programmatic movement is not an itemClicked callback.
    return true;
}

SpringTransitionReport transitionSpringItem(SpringSelectionState& state,
    std::optional<int> requestedRow, SpringItemEvent event)
{
    SpringTransitionReport report;
    if (event != SpringItemEvent::InitialLoad && event != SpringItemEvent::ItemClicked &&
        event != SpringItemEvent::SaveCurrentItem) return report;
    if (!requestedRow) { report.outcome = ApplyResult::Completed; report.stage = SpringTransitionStage::Complete; return report; }
    if (!supportedSpringItems(state.items) || !state.comboLabels || *requestedRow < 0 ||
        *requestedRow >= static_cast<int>(state.items.size()) || *requestedRow != state.listCurrentRow ||
        state.currentRow < -1 || state.currentRow >= static_cast<int>(state.items.size())) return report;
    auto& labels = *state.comboLabels;
    const auto currentId = [](const std::vector<qt::String>& entries, int index) -> std::optional<int> {
        if (entries.empty() || entries.size() > 4097 || index < 0 || index >= static_cast<int>(entries.size())) return std::nullopt;
        return springLabelId(entries[static_cast<std::size_t>(index)]);
    };
    if (state.currentRow != -1) {
        const auto first = currentId(labels.first, labels.firstCurrentIndex);
        const auto second = currentId(labels.second, labels.secondCurrentIndex);
        auto payload = state.items[static_cast<std::size_t>(state.currentRow)];
        if (!first || !second) return report;
        // Native toDouble(nullptr) also commits +0 for zero-length text.
        // Other failed/nonfinite conversion and validator profiles stay guarded.
        std::array<double, 5> numbers{};
        for (std::size_t i = 0; i < numbers.size(); ++i) {
            bool ok = false; numbers[i] = qt::toDouble(state.visibleNumbers[i], &ok);
            if ((!ok && !state.visibleNumbers[i].isEmpty()) || !std::isfinite(numbers[i])) return report;
        }
        payload.firstObject = *first; payload.secondObject = *second;
        payload.firstValue = numbers[0]; payload.secondValue = numbers[1];
        std::copy(numbers.begin() + 2, numbers.end(), payload.direction.begin());
        payload.flag = state.recordFlagChecked ? 1 : 0;
        state.items[static_cast<std::size_t>(state.currentRow)] = payload;
        state.firstSelectedId = *first; state.secondSelectedId = *second;
        report.oldItemWritten = true; report.stage = SpringTransitionStage::OldItemWritten;
    }
    if (state.currentRow == *requestedRow) {
        report.sameRow = true; report.outcome = ApplyResult::Completed; report.stage = SpringTransitionStage::Complete; return report;
    }
    const auto& payload = state.items[static_cast<std::size_t>(*requestedRow)];
    const auto select = [&](const std::vector<qt::String>& entries, int& index, int target, bool& matched) {
        if (!currentId(entries, index)) return false;
        for (std::size_t i = 0; i < entries.size(); ++i) {
            const auto id = springLabelId(entries[i]);
            if (!id) return false;
            if (*id == target) { index = static_cast<int>(i); matched = true; break; }
        }
        return true; // No match preserves the old current index.
    };
    if (!select(labels.first, labels.firstCurrentIndex, payload.firstObject, report.firstMatch)) return report;
    report.stage = SpringTransitionStage::FirstComboApplied;
    if (!select(labels.second, labels.secondCurrentIndex, payload.secondObject, report.secondMatch)) return report;
    report.stage = SpringTransitionStage::SecondComboApplied;
    state.firstSelectedId = *currentId(labels.first, labels.firstCurrentIndex);
    state.secondSelectedId = *currentId(labels.second, labels.secondCurrentIndex);
    state.visibleNumbers = springVisibleNumbers(payload); state.recordFlagChecked = payload.flag == 1;
    state.currentRow = *requestedRow; report.stage = SpringTransitionStage::VisibleLoaded;
    report.outcome = ApplyResult::Completed; report.stage = SpringTransitionStage::Complete;
    return report;
}

// The model owns the native local/root list slots; Point vectors below are
// call-local adapter snapshots required by the frozen parser interface. Only
// parser additions/clears are committed back to those owners, so there is no
// second persistent PPV list. Public loadSimCtrl still retains its guard.
bool readPpvForOperation(bool opened, std::span<const qt::String> lines,
                         Operation& operation, bool hasPredecessor)
{
    // Native open failure returns before clearing any list.
    if (!opened) return false;
    if (!operation.recoveryB) return false;

    std::vector<RecoveryEntry>* sharedEntries = nullptr;
    std::vector<bool>* flags = nullptr;
    if (hasPredecessor) {
        sharedEntries = recoveryListA(operation);
        if (!sharedEntries) return false;
        flags = operation.recoveryFlags.get(); // Native flag argument may be null.
        // The helper requires QList::insert(index) to be in range. Keep a
        // malformed model state from reaching its asserted precondition.
        if (flags && flags->size() < sharedEntries->size()) return false;
    }

    auto localPoints = toPpvPoints(*operation.recoveryB);
    const std::size_t oldSharedSize = sharedEntries ? sharedEntries->size() : 0;
    std::vector<simctrl_ppv::Point> sharedPoints;
    if (sharedEntries) sharedPoints = toPpvPoints(*sharedEntries);

    const bool result = simctrl_ppv::readPpvLines(
        true, lines, localPoints, sharedEntries ? &sharedPoints : nullptr, flags);

    // A successful QFile open clears local points even when count/row parsing
    // later fails; commit the partial local result in either return case.
    operation.recoveryB->clear();
    operation.recoveryB->reserve(localPoints.size());
    for (const auto& point : localPoints)
        operation.recoveryB->push_back(fromPpvPoint(point));

    // Shared rows only append; a match leaves the existing native BC untouched.
    // Preserve its KEY value field and append newly constructed zero-valued BCs.
    if (sharedEntries) {
        for (std::size_t index = oldSharedSize; index < sharedPoints.size(); ++index)
            sharedEntries->push_back(fromPpvPoint(sharedPoints[index]));
    }
    return result;
}

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

// @src0x140ec8110. Explicit text/index context models private controls;
// no native widget or full Session palette is constructed.
static std::optional<Extra3DPage> fillExtra3DImpl(Extra3DIo& io, NumericLocale locale,
    std::span<const int> firstComboIds, std::span<const int> secondComboIds,
    const std::function<std::optional<std::vector<int>>()>& deferredComboIds,
    std::optional<SpringComboLabels> labels = std::nullopt,
    const std::function<std::optional<SpringComboLabels>()>& deferredLabels = {})
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
    std::optional<std::vector<int>> ownedIds;
    if (deferredComboIds) {
        ownedIds = deferredComboIds();
        if (ownedIds) firstComboIds = secondComboIds = *ownedIds;
    }
    if (deferredLabels) labels = deferredLabels();
    const bool actualLabelContext = labels.has_value();
    std::vector<SpringRecord> springs;
    const bool springOpened = io.readSprings(springs);
    if (!springOpened && !springs.empty()) return std::nullopt;
    if (springOpened && !springs.empty()) {
        if (!supportedSpringItems(springs)) return std::nullopt;
        // Legacy integer inputs are synthetic parsed-ID projections, preserving
        // their prior membership admission. Actual labels retain native fallback.
        if (!actualLabelContext && (!containsId(firstComboIds, springs.front().firstObject) ||
            !containsId(secondComboIds, springs.front().secondObject))) return std::nullopt;
        if (!labels) labels = projectedSpringLabels(firstComboIds, secondComboIds);
        SpringSelectionState selected;
        selected.items = std::move(springs);
        selected.currentRow = -1; selected.listCurrentRow = 0;
        selected.comboLabels = std::move(labels);
        if (transitionSpringItem(selected, 0, SpringItemEvent::InitialLoad).outcome != ApplyResult::Completed)
            return std::nullopt;
        form.spring = std::move(selected);
    }
    return form;
}

std::optional<Extra3DPage> fillExtra3D(Extra3DIo& io, NumericLocale locale,
    std::span<const int> firstComboIds, std::span<const int> secondComboIds)
{
    return fillExtra3DImpl(io, locale, firstComboIds, secondComboIds, {});
}

std::optional<Extra3DPage> fillExtra3D(Extra3DIo& io, NumericLocale locale,
    std::function<std::optional<std::vector<int>>()> comboIdsAfterScalarReads)
{
    return fillExtra3DImpl(io, locale, {}, {}, comboIdsAfterScalarReads);
}

std::optional<Extra3DPage> fillExtra3D(Extra3DIo& io, NumericLocale locale)
{
    return fillExtra3DImpl(io, locale, {}, {}, {});
}

std::optional<Extra3DPage> fillExtra3D(Extra3DIo& io, NumericLocale locale, SpringComboLabels labels)
{
    return fillExtra3DImpl(io, locale, {}, {}, {}, std::move(labels));
}

std::optional<Extra3DPage> fillExtra3D(Extra3DIo& io, NumericLocale locale,
    std::function<std::optional<SpringComboLabels>()> labelsAfterScalarReads)
{
    return fillExtra3DImpl(io, locale, {}, {}, {}, std::nullopt, labelsAfterScalarReads);
}

void constructorRefreshExtra3D(Extra3DPage& form)
{
    form.nbc = {qt::number(75.0, 'g', 6), qt::number(105.0, 'g', 6)};
}

// @src0x140ec9040. Every failed write/delete continues at the original next
// control; only NBC deletion reports a warning. No rollback or batch effects.
ApplyResult applyExtra3D(Extra3DIo& io, const Extra3DPage& form)
{
    std::optional<std::vector<SpringRecord>> springSnapshot;
    // Native unchecked/count-zero gate skips the current-item callback and
    // every payload/control read. Both readable-deletes still follow scalars.
    if (form.spring && form.spring->pageChecked && !form.spring->items.empty()) {
        // Reconstruction admission precedes sidecar writes. Excluded positive
        // profiles do not claim native already-performed scalar effects.
        auto selected = *form.spring;
        if (!supportedSpringItems(selected.items))
            return ApplyResult::UnsupportedSnapshot;
        const auto current = selected.listCurrentRow == -1 ? std::optional<int>{} :
            std::optional<int>{selected.listCurrentRow};
        if (transitionSpringItem(selected, current, SpringItemEvent::SaveCurrentItem).outcome != ApplyResult::Completed)
            return ApplyResult::UnsupportedSnapshot;
        // The callback writes old visible controls before loading a different
        // current item; Save then snapshots all item payloads in list order.
        springSnapshot = std::move(selected.items);
    }
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
    if (springSnapshot) {
        // FUN_1410db030 builds a QStringList; FUN_1410da9e0 appends a tab
        // after every non-LF item and a final CRLF. A literal LF follows count,
        // then first object/direction[0]/direction[2] of each row (1 + 3*N LFs).
        std::vector<qt::String> output;
        output.reserve(2 + springSnapshot->size() * 10);
        output.push_back(qt::number(static_cast<int>(springSnapshot->size())));
        output.push_back(qt::L("\n"));
        for (const auto& row : *springSnapshot) {
            output.insert(output.end(), {qt::number(row.firstObject), qt::L("\n"),
                qt::number(row.firstValue, 'g', 6), qt::number(row.secondValue, 'g', 6),
                qt::number(row.secondObject), qt::number(row.direction[0], 'g', 6), qt::L("\n"),
                qt::number(row.direction[1], 'g', 6), qt::number(row.direction[2], 'g', 6), qt::L("\n")});
        }
        const bool primaryOpened = io.writeStringList(qt::L("SPRING.INI"), output);
        const bool flagged = std::any_of(springSnapshot->begin(), springSnapshot->end(),
            [](const SpringRecord& row) { return row.flag != 0; });
        if (!flagged) io.removeIfReadable(qt::L("SPRING2.INI")); // Even if primary failed.
        else if (primaryOpened) {
            std::vector<qt::String> flags;
            flags.reserve(springSnapshot->size());
            for (const auto& row : *springSnapshot) flags.push_back(qt::number(row.flag));
            io.writeStringList(qt::L("SPRING2.INI"), flags);
        }
    } else {
        io.removeIfReadable(qt::L("SPRING.INI"));
        io.removeIfReadable(qt::L("SPRING2.INI"));
    }
    return ApplyResult::Completed;
}
} // namespace m2m::engine::simcontrol300
