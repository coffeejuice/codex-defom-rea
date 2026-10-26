// defmodel/members/furnace.hpp: data of the furnace model (Cls_1403753f0, packet p1-10).
#pragma once

#include <cstdint>

namespace def::model {

// @data 140550ec0  per-curve shape for curves 1..20 used by 140375e90: {curve number, ndim,
// unused, count[0], u40}; copied from DEF_PRE_64.exe (checked by test_members_furnace when the
// executable is present).
inline constexpr std::int32_t kFurnaceCurveShape[20][5] = {
    {1, 1, 0, 2, 6},   {2, 1, 0, 1, 3},   {3, 1, 0, 1, 6},   {4, 1, 0, 1, 5},
    {5, 1, 0, 2, 6},   {6, 1, 0, 2, 3},   {7, 1, 0, 5, 10},  {8, 1, 0, 12, 5},
    {9, 1, 0, 11, 2},  {10, 1, 0, 2, 1},  {11, 1, 1, 1, 1},  {12, 1, 0, 0, 13},
    {13, 1, 0, 0, 12}, {14, 1, 0, 2, 21}, {15, 1, 0, 0, 10}, {16, 1, 0, 0, 1},
    {17, 1, 0, 0, 10}, {18, 1, 0, 0, 2},  {19, 1, 0, 0, 2},  {20, 1, 0, 0, 1},
};

} // namespace def::model
