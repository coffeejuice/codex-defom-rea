// Isolated, source-qualified SimCtrl.PPV point-list reader contract.
// This helper is not connected to Operation, Project, or the public loader.
#pragma once

#include "m2m/qt4.hpp"
#include <cassert>
#include <cmath>
#include <cstddef>
#include <span>
#include <vector>

namespace m2m::engine::simctrl_ppv {

struct Point {
    int firstIndex = 0;
    int secondIndex = 0;
    double firstValue = 0.0;
    double secondValue = 0.0;
};

// Native 0x140d4b9e0: both integer fields compare exactly; each double delta
// must be strictly less than 1e-6. The original helper searches in QList order.
inline bool samePoint(const Point& left, const Point& right)
{
    return left.firstIndex == right.firstIndex && left.secondIndex == right.secondIndex &&
           std::fabs(left.firstValue - right.firstValue) < 1.0e-6 &&
           std::fabs(left.secondValue - right.secondValue) < 1.0e-6;
}

// Native 0x140d4af00 / 0x140d4bab0 / 0x140d4b8c0: return the first match
// from index zero, or append and return the new final index.
inline int findOrAddPoint(std::vector<Point>& points, const Point& point)
{
    for (std::size_t index = 0; index < points.size(); ++index) {
        if (samePoint(points[index], point)) return static_cast<int>(index);
    }
    points.push_back(point);
    return static_cast<int>(points.size() - 1);
}

namespace detail {

// Qt 4.8 QChar::isSpace table used by QRegExp `\\s`; split uses SkipEmptyParts.
inline bool ppvSpace(char16_t c)
{
    if (c >= 9 && c <= 13) return true;
    if (c < 128) return c == 32;
    switch (c) {
    case 0x85: case 0xa0: case 0x1680: case 0x180e: case 0x2028: case 0x2029:
    case 0x202f: case 0x205f: case 0x3000:
        return true;
    default:
        return c >= 0x2000 && c <= 0x200a;
    }
}

inline std::vector<qt::String> splitPpvRow(const qt::String& row)
{
    std::vector<qt::String> fields;
    const auto& text = row.u16();
    std::size_t start = 0;
    while (start < text.size()) {
        while (start < text.size() && ppvSpace(text[start])) ++start;
        if (start == text.size()) break;
        std::size_t end = start + 1;
        while (end < text.size() && !ppvSpace(text[end])) ++end;
        fields.emplace_back(text.substr(start, end - start));
        start = end;
    }
    return fields;
}

inline qt::String lineAt(std::span<const qt::String> lines, std::size_t index)
{
    return index < lines.size() ? lines[index] : qt::String();
}

inline bool isTrue(const qt::String& text)
{
    return text.size() == 4 && qt::startsWith(text, qt::L("true"), qt::Case::Insensitive);
}

} // namespace detail

// `lines` contains the values returned by successive QTextStream::readLine
// calls, including the first count line. `opened == false` models QFile::open
// failure. On a successful open, only localPoints is cleared. Shared points
// and flags are caller-owned and retained. This is deliberately an isolated
// reader seam; public SimCtrl.PPV loading remains unsupported until OPI/root
// list ownership is connected by the integration owner.
//
// If flags is supplied for a shared row, its size must cover the returned point
// index. The original calls QList::insert(flags,index,value); this helper keeps
// that precondition explicit rather than inventing behavior for inconsistent
// point/flag list state.
inline bool readPpvLines(bool opened, std::span<const qt::String> lines,
                         std::vector<Point>& localPoints,
                         std::vector<Point>* sharedPoints,
                         std::vector<bool>* flags)
{
    if (!opened) return false;

    localPoints.clear(); // 0x140d4b400 clears only param_1 after successful open.
    const int count = qt::toInt(detail::lineAt(lines, 0)); // conversion status ignored.
    if (count <= 0) return false;

    bool ok = true;
    for (int rowIndex = 0; rowIndex < count; ++rowIndex) {
        const auto fields = detail::splitPpvRow(
            detail::lineAt(lines, static_cast<std::size_t>(rowIndex) + 1));
        if (fields.size() < 6) {
            ok = false;
            break;
        }

        (void)qt::toInt(fields[0]); // parsed by the native reader, then overwritten.
        const bool shared = detail::isTrue(fields[1]) && sharedPoints != nullptr;
        Point point{qt::toInt(fields[2]), qt::toInt(fields[3]),
                    qt::toDouble(fields[4]), qt::toDouble(fields[5])};

        bool flag = false;
        if (fields.size() == 7)
            flag = qt::toInt(fields[5]) != 0; // Native index-5 behavior; token 6 is not read.

        if (!shared) {
            (void)findOrAddPoint(localPoints, point);
            continue;
        }

        const int pointIndex = findOrAddPoint(*sharedPoints, point);
        if (flags) {
            assert(pointIndex >= 0 && static_cast<std::size_t>(pointIndex) <= flags->size());
            flags->insert(flags->begin() + pointIndex, flag); // Insert, never replace.
        }
    }
    return ok;
}

} // namespace m2m::engine::simctrl_ppv
