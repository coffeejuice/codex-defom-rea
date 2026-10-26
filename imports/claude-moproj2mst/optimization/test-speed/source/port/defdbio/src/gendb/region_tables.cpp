// Region widths and starts of a GENDB step (packet p4-01): FUN_1401c03f0 and the start tables
// 1401c1980 (3D) / 1401c2d10 (2D). Integer code only. The bodies are a mechanical translation of the
// decompile (index globals g_mar_index / g_a_index of p1-03, DAT_1406e81fc / DAT_1406e82d0); the
// two blocks Ghidra drops in each start table are dead code (XOR EAX,EAX; TEST; JZ around an
// error print and exit(-1)).
#include <defdbio/gendb/gendb.hpp>

#include <cstdint>

#include <defmodel/layout/db_step_data.hpp>
#include <defmodel/step/db_step.hpp>

#include <defdbio/gendb/ext.hpp>

namespace def::dbio::gendb {

using model::field;

#define W(off) field<std::int32_t>(&s, (off))
#define MAR(k) W(0x1b90 + model::g_mar_index[k] * 4)
#define AT(k) W(0x1dc0 + model::g_a_index[k] * 4)

namespace {
inline bool is_2d(const DB_STEP_DATA& m) { return m.dimension == 2; } // 1400015e0 (p1-01, inlined)
inline bool is_3d(const DB_STEP_DATA& m) { return m.dimension == 3; } // 1400015f0
} // namespace

// @src 1401c03f0  region widths
// @src 14019a2f0  (model +0xcc, inlined)
// @src 140199d60  (model +0xd0, inlined)
// @src 140199d80  (model +0xd4, inlined)
void region_sizes(DB_STEP_DATA& s)
{
  bool cVar1;
  int iVar2;
  std::int32_t uVar3;
  int iVar4;
  int iVar5;
  int local_44;
  std::int32_t local_3c;
  std::int32_t local_38;
  std::int32_t local_34;
  int local_30;
  std::int32_t local_2c;
  std::int32_t local_28[1];
  std::int32_t local_24;
  std::int32_t local_20;
  std::int32_t local_1c;
  std::int32_t local_18;
  int local_14;
  local_2c = s.dimension; // 140158e90
  model::select_index_tables(&local_2c); // 1401c7d40
  local_34 = 0;
  local_38 = 0;
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    local_3c = 2;
    local_44 = 4;
  }
  else {
    local_3c = 3;
    local_44 = 6;
  }
  if (0 < W(200)) {
    local_34 = W(0x78);
    local_38 = W(0xbc);
  }
  model::step_clear_address_tables(s); // 1401cb170
  MAR(24) = 0;
  MAR(25) = 0;
  MAR(26) = 0;
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    MAR(27) = 0x14;
  }
  else {
    MAR(27) = 0x32;
  }
  MAR(28) = 0;
  MAR(29) = 0;
  MAR(30) = 100;
  MAR(31) = 0;
  MAR(32) = 0;
  MAR(33) = 3;
  MAR(34) = 0;
  MAR(35) = 0;
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    W(0x1cd8) = 0;
    W(0x1cdc) = 0;
  }
  MAR(36) = 0;
  MAR(37) = 0;
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    W(0x1ce8) = 0;
    W(0x1d90) = 0;
    W(0x1d94) = 0;
    W(0x1d98) = 0;
  }
  MAR(44) = 0;
  MAR(45) = 0;
  MAR(46) = 0;
  MAR(47) = 0;
  MAR(48) = 0;
  MAR(49) = 0;
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    W(0x1db4) = 0;
    W(0x1db8) = 0;
  }
  cVar1 = is_2d(s);
  if (cVar1 != '\0') {
    W(0x1db8) = 0;
  }
  AT(59) = 2;
  AT(60) = 500;
  AT(61) = 0x32;
  AT(62) = 100;
  AT(63) = 0x32;
  AT(64) = 100;
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    AT(65) = 100;
  }
  else {
    AT(65) = 200;
  }
  AT(66) = 0x32;
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    AT(67) = 6;
  }
  else {
    AT(67) = 3;
  }
  AT(68) = 0x14;
  AT(69) = local_3c;
  AT(70) = local_3c;
  AT(71) = local_3c;
  AT(72) = local_3c;
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    W(0x2028) = 0;
  }
  cVar1 = is_2d(s);
  if (cVar1 != '\0') {
    W(0x2028) = 0;
  }
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    AT(73) = local_3c;
    AT(74) = local_3c;
    AT(75) = 0;
    AT(76) = local_3c;
  }
  else {
    AT(73) = local_3c;
    AT(74) = local_3c;
    AT(75) = 3;
    AT(76) = local_3c;
  }
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    W(0x203c) = 3;
  }
  cVar1 = is_2d(s);
  if (cVar1 != '\0') {
    W(0x203c) = 0;
  }
  AT(77) = 1;
  AT(78) = 1;
  AT(79) = 1;
  AT(80) = 3;
  AT(81) = W(0xdc);
  AT(82) = 1;
  AT(83) = 1;
  if (W(0xe4) == 0) {
    AT(84) = 0;
  }
  else {
    AT(84) = 5;
  }
  cVar1 = is_2d(s);
  if (cVar1 == '\0') {
    iVar2 = elmnod_width(s, 2);
    AT(86) = iVar2 * 6;
    uVar3 = elmnod_width(s, 1);
    AT(91) = uVar3;
    iVar2 = elmnod_width(s, 0);
    AT(94) = W(0x16ac) * iVar2;
  }
  else {
    if (W(0x850) == 3) {
      iVar2 = elmnod_width(s, 2);
      AT(86) = iVar2 * 6;
    }
    else {
      iVar2 = elmnod_width(s, 2);
      AT(86) = iVar2 << 2;
    }
    uVar3 = elmnod_width(s, 1);
    AT(91) = uVar3;
    iVar2 = elmnod_width(s, 0);
    AT(94) = W(0x16ac) * iVar2;
  }
  AT(87) = local_44;
  iVar2 = W(0x86c) + W(0x870) + W(0x874) +
          W(0x878) + W(0x87c);
  iVar4 = W(0x880) + W(0x884);
  AT(88) = local_44 * iVar2 + iVar4;
  AT(89) = 1;
  AT(90) = 1;
  AT(92) = 1;
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    W(0x2080) = 2;
  }
  else {
    W(0x2080) = 0;
  }
  AT(93) = 1;
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    W(0x208c) = 1;
  }
  AT(95) = W(0xd8);
  AT(96) = local_34;
  AT(97) = local_38;
  AT(98) = local_34;
  AT(99) = 2;
  AT(100) = local_38;
  local_28[0] = 1;
  local_14 = ext::fun_1401d7160(s, local_28);
  local_14 = local_14 + 9;
  switch(local_14) {
  case 0:
    AT(101) = 0;
    break;
  default:
    AT(101) = 0;
    break;
  case 8:
  case 10:
    local_24 = 1;
    uVar3 = model::step_fun_1401d70c0(s, &local_24);
    AT(101) = uVar3;
    break;
  case 0xb:
    local_20 = 2;
    uVar3 = model::step_fun_1401d70c0(s, &local_20);
    AT(101) = uVar3;
    break;
  case 0xd:
    local_1c = 4;
    uVar3 = model::step_fun_1401d70c0(s, &local_1c);
    AT(101) = uVar3;
    break;
  case 0xe:
    local_18 = 5;
    uVar3 = model::step_fun_1401d70c0(s, &local_18);
    AT(101) = uVar3;
  }
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    AT(102) = 3;
    AT(103) = 1;
  }
  cVar1 = is_2d(s);
  if (cVar1 != '\0') {
    AT(102) = 2;
    AT(103) = 1;
  }
  AT(122) =
       W(0x16d0);
  AT(104) = local_44;
  AT(105) = 2;
  AT(109) = 0;
  AT(110) = 0;
  AT(111) = 0;
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    AT(123) = 2;
  }
  else {
    AT(123) = 3;
  }
  local_30 = 0;
  cVar1 = model::step_has_array(s, 0x37, &local_30);
  if ((cVar1 != '\0') && (local_30 != W(0x16d4))) {
    ext::step_remap_material_state(s, false); // 1401d0fc0 (kw-g4b)
  }
  AT(124) =
       W(0x16d4);
  AT(125) =
       W(0x1680);
  AT(126) =
       W(0x16f4);
  AT(127) =
       W(0x1684);
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    AT(112) = 2;
    AT(113) = 2;
  }
  else {
    AT(112) = 0;
    AT(113) = 0;
  }
  AT(114) = 0;
  AT(115) = local_3c;
  AT(116) = 0;
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    W(0x2200) = 0;
    W(0x2204) = 0;
  }
  AT(117) = 0;
  AT(118) = 0;
  MAR(38) = 1;
  MAR(39) = 1;
  MAR(40) = 1;
  MAR(41) = 1;
  MAR(42) = 1;
  MAR(43) = 1;
  MAR(50) = 1;
  MAR(51) = 1;
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    AT(106) = 0;
    AT(107) = 0;
    AT(108) = 0;
    AT(119) = 2;
    AT(120) = 2;
  }
  else {
    AT(106) = 1;
    AT(107) = 1;
    AT(108) = 1;
    AT(119) = 0;
    AT(120) = 0;
  }
  AT(121) = 0;
  cVar1 = is_2d(s);
  if (cVar1 != '\0') {
    W(0x20d0) = 8;
    W(0x20d4) = 8;
  }
  AT(85) =
       W(0x16b0);
  if (AT(85) == 0) {
    AT(85) = 1;
  }
  cVar1 = is_3d(s);
  if (cVar1 != '\0') {
    iVar5 = W(0xcc) /* 14019a2f0 */;
    if (((iVar5 == 1) && (iVar5 = W(0xd0) /* 140199d60 */, 0 < iVar5)) &&
       (iVar5 = W(0xd4) /* 140199d80 */, 0 < iVar5)) {
      W(0x2028) = 3;
      W(0x1d04) = 4;
    }
    else {
      W(0x2028) = 0;
      W(0x1d04) = 0;
    }
  }
  cVar1 = is_2d(s);
  if ((cVar1 != '\0') && (W(0x850) == 3)) {
    AT(72) = 3;
    AT(73) = 3;
    AT(74) = 3;
    AT(87) = 6;
    AT(88) = iVar2 * 6 + iVar4;
    AT(112) = 3;
    AT(113) = 3;
    AT(119) = 3;
    AT(120) = 3;
    AT(104) = 6;
  }
  cVar1 = is_3d(s);
  if (cVar1 == '\0') {
    region_starts_2d(s);
  }
  else {
    region_starts_3d(s);
  }
  return;
}

// @src 1401c1980  region starts, 3D
void region_starts_3d(DB_STEP_DATA& s)
{
  std::int32_t local_28;
  std::int32_t local_24;
  std::int32_t local_20;
  std::int32_t local_1c;
  std::int32_t local_18;
  W(0x1b90) = 1;
  W(0x1b94) = W(0x1b90) + 0x8c;
  W(0x1b98) = W(0x1b94) + 0x118;
  W(0x1b9c) = W(0x1b98) + 2 + W(0x88) * 2;
  W(0x1ba0) =
       W(0x7c) * W(0x1cb4) + W(0x1b9c);
  W(0x1ba4) = W(0x1ba0) + W(0xa0);
  W(0x1ba8) = W(0x1ba4) + W(0x70);
  W(0x1bac) = W(0x1ba8) + W(0x1cc0);
  W(0x1bb0) =
       W(0x1bac) + W(0x6c) * 3 + W(0x7c) * 4;
  W(0x1bb4) =
       W(0x1bb0) + W(0x6c) * 3 + W(0x7c) * 4;
  W(0x1bb8) =
       W(0x6c) * W(0x1ccc) + W(0x1bb4);
  W(0x1bbc) = W(0x1bb8) + W(0x6c);
  W(0x1bc0) = W(0x1bbc) + 500;
  W(0x1bc4) = W(0x1bc0) + W(0x74);
  W(0x1bc8) = W(0x98) * 5 + W(0x1bc4);
  W(0x1bcc) = W(0x1bc8) + W(0x94) * 4;
  W(0x1bd0) = W(0x1bcc) + W(0x94);
  W(0x1bd4) = W(0x1bd0) + W(0x90);
  W(0x1bd8) =
       W(0x94) * W(0x1cec) + W(0x1bd4);
  W(0x1bdc) =
       W(0x94) * W(0x1cf0) + W(0x1bd8);
  W(0x1be0) =
       W(0x94) * W(0x1cf4) + W(0x1bdc);
  W(0x1be4) =
       W(0x94) * W(0x1cf8) + W(0x1be0);
  W(0x1be8) =
       W(0x94) * W(0x1cfc) + W(0x1be4);
  W(0x1bec) =
       W(0x94) * W(0x1d00) + W(0x1be8);
  W(0x1bf0) =
       W(0xd4) * W(0x1d04) + W(0x1bec);
  W(0x1bf4) =
       W(0x94) * W(0x1d08) + W(0x1bf0);
  W(0x1bf8) =
       W(0x94) * W(0x1d0c) + W(0x1bf4);
  for (local_1c = 0x1b; local_1c < 0x3b; local_1c = local_1c + 1) {
    W(0x1b90 + local_1c * 4) = W(0x1bf8);
  }
  W(0x1c7c) = W(0x1c78) + W(0x74);
  W(0x1c80) = W(0x1c7c) + W(0x98) * 4;
  W(0x1c84) = W(0x1c80) + W(0x94) * 4;
  W(0x1c88) = W(0x1c84) + W(0x74);
  W(0x1c8c) = W(0x1c88) + W(0x74) * 4;
  W(0x1c90) = W(0x1c8c) + W(0x74) * 4;
  W(0x1c94) = W(0x1c90) + W(0x74);
  W(0x1c98) = W(0x1c94) + W(0x74);
  W(0x1c9c) = W(0x1c98) + W(0x74);
  W(0x1ca0) = W(0x1c9c) + 1 + W(0x94);
  W(0x1ca4) = W(0x1ca0);
  W(0x1dc0) = 1;
  W(0x1dc4) = W(0x1dc0) + W(0x8c);
  W(0x1dc8) =
       W(0x78) * W(0x1ff4) + W(0x1dc4);
  W(0x1dcc) =
       W(0x1ff8) * W(0x78) * W(0x78) +
       W(0x1dc8);
  W(0x1dd0) = W(0x1dcc) + W(0x1ffc);
  W(0x1dd4) = W(0x1dd0) + W(0x2000);
  W(0x1dd8) =
       W(0x7c) * W(0x2004) + W(0x1dd4);
  W(0x1ddc) =
       W(0x80) * W(0x2008) + W(0x1dd8);
  W(0x1de0) =
       W(0x80) * W(0x80) * W(0x200c) +
       W(0x1ddc);
  W(0x1de4) =
       W(0x9c) * W(0x2010) + W(0x1de0);
  W(0x1de8) =
       W(0x2014) * W(0xc0) + W(0x1de4);
  W(0x1dec) =
       W(0x2018) * W(0xc4) + W(0x1de8);
  W(0x1df0) =
       W(0x6c) * W(0x201c) + W(0x1dec);
  W(0x1df4) =
       W(0x1df0) + W(0x74) * 3 + W(0x7c) * 4;
  W(0x1df8) =
       W(0x1df4) + W(0x74) * 4 + W(0x7c) * 4;
  W(0x1dfc) =
       W(0xd0) * W(0x2028) + W(0x1df8);
  W(0x1e00) =
       W(0x1dfc) + W(0x74) * 3 + W(0x7c) * 4;
  W(0x1e04) =
       W(0x1e00) + W(0x74) * 3 + W(0x7c) * 4;
  W(0x1e08) = W(0x74) * 3 + W(0x1e04);
  W(0x1e0c) = W(0x74) * 3 + W(0x1e08);
  W(0x1e10) =
       W(0x74) * W(0x203c) + W(0x1e0c);
  W(0x1e14) =
       W(0x6c) * W(0x2040) + W(0x1e10);
  W(0x1e18) =
       W(0x6c) * W(0x2044) + W(0x1e14);
  W(0x1e1c) =
       W(0x6c) * W(0x2048) + W(0x1e18);
  W(0x1e20) =
       W(0x6c) * W(0x204c) + W(0x1e1c);
  W(0x1e24) =
       W(0x1e20) + W(0x6c) * W(0x2050) +
       W(0x2050) * 8 * W(0x7c);
  W(0x1e28) =
       W(0x6c) * W(0x2054) + W(0x1e24);
  W(0x1e2c) =
       W(0x6c) * W(0x2058) + W(0x1e28);
  W(0x1e30) =
       W(0x6c) * W(0x205c) + W(0x1e2c);
  W(0x1e34) =
       W(0x70) * W(0x2060) + W(0x1e30);
  local_28 = 0;
  if (W(0x890) == 0) {
    local_28 = W(0x70);
  }
  else if (W(0x890) == 2) {
    local_28 = W(0x70) + W(0x6c);
  }
  else if (W(0x890) == 3) {
    local_28 = W(0x70);
  }
  W(0x1e38) = local_28 * W(0x2064) + W(0x1e34);
  W(0x1e3c) =
       W(0x70) * W(0x2068) + W(0x1e38);
  W(0x1e40) =
       W(0x70) * W(0x206c) + W(0x1e3c);
  W(0x1e44) =
       W(0x70) * W(0x2070) + W(0x1e40);
  W(0x1e48) =
       W(0x70) * W(0x2074) + W(0x1e44);
  if (W(0x88c) == 0) {
    local_20 = W(0x70);
  }
  else if (W(0x88c) == 2) {
    local_20 = W(0x70) + W(0x6c);
  }
  else if (W(0x88c) == 3) {
    local_20 = W(0x70);
  }
  else {
    local_20 = W(0x70);
  }
  W(0x1e4c) = local_20 * W(0x2078) + W(0x1e48);
  W(0x1e50) =
       W(0x70) * W(0x207c) + W(0x1e4c);
  W(0x1e54) =
       W(0x70) * W(0x2080) + W(0x1e50);
  W(0x1e58) =
       W(0x6c) * W(0x2084) + W(0x1e54);
  local_24 = 0;
  if (W(0x888) == 0) {
    local_24 = W(0x70);
  }
  else if (W(0x888) == 2) {
    local_24 = W(0x70) + W(0x6c);
  }
  else if (W(0x888) == 3) {
    local_24 = W(0x70);
  }
  W(0x1e5c) = local_24 * W(0x2088) + W(0x1e58);
  W(0x1e60) =
       W(0x70) * W(0x208c) + W(0x1e5c);
  W(0x1e64) =
       W(0x1e60) + W(0x70) * W(0x2090) +
       W(0x2090) * 8 * W(0x7c);
  W(0x1e68) =
       W(0x70) * W(0x2094) + W(0x1e64);
  W(0x1e6c) =
       W(0x70) * W(0x2098) + W(0x1e68);
  W(0x1e70) =
       W(0x70) * W(0x209c) + W(0x1e6c);
  W(0x1e74) =
       W(0x70) * W(0x20a0) + W(0x1e70);
  W(0x1e78) =
       W(0x70) * W(0x20a4) + W(0x1e74);
  if (W(0x898) == 1) {
    W(0x1e7c) =
         W(0x6c) * W(0x20a8) + W(0x1e78);
  }
  else {
    W(0x1e7c) =
         W(0x70) * W(0x20a8) + W(0x1e78);
  }
  W(0x1e80) =
       W(0x70) * W(0x20ac) + W(0x1e7c);
  W(0x1e84) =
       W(0x94) * W(0x20b0) + W(0x1e80);
  W(0x1e88) =
       W(0x70) * W(0x20b4) + W(0x1e84);
  W(0x1e8c) =
       W(0x70) * W(0x20b8) + W(0x1e88);
  W(0x1e90) =
       W(0x94) * W(0x20bc) + W(0x1e8c);
  W(0x1e94) =
       W(0x94) * W(0x20c0) + W(0x1e90);
  W(0x1e98) =
       W(0x94) * W(0x20c4) + W(0x1e94);
  W(0x1e9c) = W(0x1e98) + 500;
  W(0x1ea0) = W(0x1e9c) + W(0x6c);
  W(0x1ea4) =
       W(0x70) * W(0x20d0) + W(0x1ea0);
  W(0x1ea8) =
       W(0x70) * W(0x20d4) + W(0x1ea4);
  W(0x1eac) =
       W(0x6c) * W(0x20d8) + W(0x1ea8);
  W(0x1eb0) =
       W(0x70) * W(0x20dc) + W(0x1eac);
  W(0x1eb4) =
       W(0x70) * W(0x20e0) + W(0x1eb0);
  W(0x1eb8) =
       W(0x6c) * W(0x20e4) + W(0x1eb4);
  for (local_18 = 0x3f; local_18 < 0x7f; local_18 = local_18 + 1) {
    W(0x1dc0 + local_18 * 4) = W(0x1eb8);
  }
  W(0x1fbc) = W(0x1fb8) + W(0x6c);
  W(0x1fc0) = W(0x74) * 3 + W(0x1fbc);
  W(0x1fc4) = W(0x74) * 3 + W(0x1fc0);
  W(0x1fc8) = W(0x1fc4) + W(0x6c);
  W(0x1fcc) = W(0x74) * 3 + W(0x1fc8);
  W(0x1fd0) = W(0x1fcc) + W(0x6c);
  W(0x1fd4) = W(0x1fd0) + W(0x74) * 2;
  W(0x1fd8) = W(0x74) * 3 + W(0x1fd4);
  W(0x1fdc) = W(0x1fd8) + W(0x74);
  W(0x1fe0) = W(0x90) * 9 + W(0x1fdc);
  W(0x1fe4) = W(0x1fe0) + W(0x74) * 4;
  W(0x1fe8) = W(0x1fe4) + W(0x74) * 4;
  W(0x1fec) = W(0x1fe8);
  return;
}

// @src 1401c2d10  region starts, 2D
void region_starts_2d(DB_STEP_DATA& s)
{
  int iVar1;
  std::int32_t local_28;
  std::int32_t local_24;
  std::int32_t local_20;
  std::int32_t local_18;
  std::int32_t local_14;
  W(0x1b90) = 1;
  W(0x1b94) = W(0x1b90) + 0x8c;
  W(0x1b98) = W(0x1b94) + 0x118;
  W(0x1b9c) = W(0x1b98) + 2 + W(0x88) * 2;
  W(0x1ba0) =
       W(0x7c) * W(0x1cb4) + W(0x1b9c);
  W(0x1ba4) =
       W(0x1ba0) + W(0x84) + W(0x80);
  W(0x1ba8) =
       W(0x1ba4) + W(0x84) + W(0x80);
  W(0x1bac) = W(0x1ba8) + W(0x70) * 4;
  W(0x1bb0) = W(0x1bac) + W(0x70);
  W(0x1bb4) = W(0x1bb0) + W(0x1cc8);
  W(0x1bb8) =
       W(0x1bb4) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1bbc) =
       W(0x1bb8) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1bc0) =
       W(0x6c) * W(0x1cd4) + W(0x1bbc);
  W(0x1bc4) = W(0x1bc0) + W(0x6c);
  iVar1 = W(0x84) + W(0x80);
  W(0x1bc8) = W(0x1bc4) + 500;
  W(0x1bcc) = W(0x1bc8) + iVar1;
  W(0x1bd0) = W(0x1bcc) + iVar1;
  W(0x1bd4) = W(0x1bd0) + iVar1;
  W(0x1bd8) = W(0x1bd4) + iVar1;
  W(0x1bdc) = W(0x1bd8) + iVar1;
  W(0x1be0) = W(0x1bdc) + iVar1;
  W(0x1be4) = W(0x1be0) + iVar1;
  W(0x1be8) = W(0x1be4) + iVar1;
  for (local_18 = 0x17; local_18 < 0x3f; local_18 = local_18 + 1) {
    W(0x1b90 + local_18 * 4) = W(0x1be8);
  }
  W(0x1c8c) =
       W(0x1c88) + W(0x6c) + W(0x80);
  W(0x1c90) =
       W(0x1c8c) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1c94) =
       W(0x1c90) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1c98) =
       W(0x1c94) + W(0x6c) + W(0x80);
  W(0x1c9c) =
       W(0x1c98) + W(0x6c) + W(0x80);
  W(0x1ca0) =
       W(0x1c9c) + W(0x6c) + W(0x80);
  W(0x1ca4) = W(0x6c) * 5 + W(0x1ca0);
  W(0x1dc0) = 1;
  W(0x1dc4) = W(0x1dc0) + W(0x8c);
  W(0x1dc8) =
       W(0x78) * W(0x1ff4) + W(0x1dc4);
  W(0x1dcc) =
       W(0x1ff8) * W(0x78) * W(0x78) +
       W(0x1dc8);
  W(0x1dd0) = W(0x1dcc) + W(0x1ffc);
  W(0x1dd4) = W(0x1dd0) + W(0x2000);
  W(0x1dd8) =
       W(0x7c) * W(0x2004) + W(0x1dd4);
  W(0x1ddc) =
       W(0x80) * W(0x2008) + W(0x1dd8);
  W(0x1de0) =
       W(0x80) * W(0x80) * W(0x200c) +
       W(0x1ddc);
  W(0x1de4) =
       W(0xb8) * W(0x2010) + W(0x1de0);
  W(0x1de8) =
       W(0x2014) * W(0xc0) + W(0x1de4);
  W(0x1dec) =
       W(0x2018) * W(0xc4) + W(0x1de8);
  W(0x1df0) =
       W(0x1dec) + W(0x6c) * W(0x201c) +
       W(0x80) * (W(0x201c) + 1);
  W(0x1df4) =
       W(0x1df0) + W(0x6c) * W(0x2020) +
       W(0x80) * (W(0x2020) + 1);
  W(0x1df8) =
       W(0x1df4) + W(0x6c) * W(0x2024) +
       W(0x80) * (W(0x2024) + 1);
  W(0x1dfc) =
       W(0x1df8) + W(0x6c) + W(0x80);
  W(0x1e00) =
       W(0x1dfc) + W(0x6c) * W(0x202c) +
       W(0x80) * (W(0x202c) + 1);
  W(0x1e04) =
       W(0x1e00) + W(0x6c) * W(0x2030) +
       W(0x80) * (W(0x2030) + 1);
  W(0x1e08) =
       W(0x1e04) + W(0x6c) * W(0x2034) +
       W(0x80) * (W(0x2034) + 1);
  W(0x1e0c) =
       W(0x1e08) + W(0x6c) * W(0x2038) +
       W(0x80) * (W(0x2038) + 1);
  W(0x1e10) =
       W(0x1e0c) + W(0x6c) + W(0x80);
  W(0x1e14) =
       W(0x6c) * W(0x2040) + W(0x1e10);
  W(0x1e18) =
       W(0x6c) * W(0x2044) + W(0x1e14);
  W(0x1e1c) =
       W(0x6c) * W(0x2048) + W(0x1e18);
  W(0x1e20) =
       W(0x6c) * W(0x204c) + W(0x1e1c);
  W(0x1e24) =
       W(0x6c) * W(0x2050) + W(0x1e20);
  W(0x1e28) =
       W(0x6c) * W(0x2054) + W(0x1e24);
  W(0x1e2c) =
       W(0x6c) * W(0x2058) + W(0x1e28);
  W(0x1e30) =
       W(0x6c) * W(0x205c) + W(0x1e2c);
  W(0x1e34) =
       W(0x70) * W(0x2060) + W(0x1e30);
  local_28 = 0;
  if (W(0x890) == 0) {
    local_28 = W(0x70);
  }
  else if (W(0x890) == 2) {
    local_28 = W(0x70) + W(0x6c);
  }
  else if (W(0x890) == 3) {
    local_28 = W(0x70);
  }
  W(0x1e38) = local_28 * W(0x2064) + W(0x1e34);
  W(0x1e3c) =
       W(0x70) * W(0x2068) + W(0x1e38);
  W(0x1e40) =
       W(0x70) * W(0x206c) + W(0x1e3c);
  W(0x1e44) =
       W(0x70) * W(0x2070) + W(0x1e40);
  W(0x1e48) =
       W(0x70) * W(0x2074) + W(0x1e44);
  if (W(0x88c) == 0) {
    local_20 = W(0x70);
  }
  else if (W(0x88c) == 2) {
    local_20 = W(0x70) + W(0x6c);
  }
  else if (W(0x88c) == 3) {
    local_20 = W(0x70);
  }
  else {
    local_20 = W(0x70);
  }
  W(0x1e4c) = local_20 * W(0x2078) + W(0x1e48);
  W(0x1e50) =
       W(0x70) * W(0x207c) + W(0x1e4c);
  W(0x1e54) =
       W(0x70) * W(0x2080) + W(0x1e50);
  W(0x1e58) =
       W(0x6c) * W(0x2084) + W(0x1e54);
  local_24 = 0;
  if (W(0x888) == 0) {
    local_24 = W(0x70);
  }
  else if (W(0x888) == 2) {
    local_24 = W(0x70) + W(0x6c);
  }
  else if (W(0x888) == 3) {
    local_24 = W(0x70);
  }
  W(0x1e5c) = local_24 * W(0x2088) + W(0x1e58);
  W(0x1e60) =
       W(0x70) * W(0x208c) + W(0x1e5c);
  W(0x1e64) =
       W(0x70) * W(0x2090) + W(0x1e60);
  W(0x1e68) =
       W(0x70) * W(0x2094) + W(0x1e64);
  W(0x1e6c) =
       W(0x70) * W(0x2098) + W(0x1e68);
  W(0x1e70) =
       W(0x70) * W(0x209c) + W(0x1e6c);
  W(0x1e74) =
       W(0x70) * W(0x20a0) + W(0x1e70);
  W(0x1e78) =
       W(0x70) * W(0x20a4) + W(0x1e74);
  if (W(0x898) == 1) {
    W(0x1e7c) =
         W(0x6c) * W(0x20a8) + W(0x1e78);
  }
  else {
    W(0x1e7c) =
         W(0x70) * W(0x20a8) + W(0x1e78);
  }
  W(0x1e80) =
       W(0x70) * W(0x20ac) + W(0x1e7c);
  W(0x1e84) = iVar1 * W(0x20b0) + W(0x1e80);
  W(0x1e88) =
       W(0x6c) * W(0x20b4) + W(0x1e84);
  W(0x1e8c) =
       W(0x70) * W(0x20b8) + W(0x1e88);
  W(0x1e90) =
       W(0x70) * W(0x20bc) + W(0x1e8c);
  W(0x1e94) = W(0x1e90) + 500;
  W(0x1e98) = W(0x1e94) + iVar1;
  W(0x1e9c) = W(0x1e98) + iVar1;
  W(0x1ea0) = W(0x1e9c) + iVar1;
  W(0x1ea4) =
       W(0x208c) * W(0x20d0) + W(0x1ea0);
  W(0x1ea8) =
       W(0x2050) * W(0x20d4) + W(0x1ea4);
  W(0x1eac) =
       W(0x1ea8) + W(0x6c) * 2 + W(0x80) * 3;
  W(0x1eb0) =
       W(0x70) * W(0x20dc) + W(0x1eac);
  W(0x1eb4) =
       W(0x70) * W(0x20e0) + W(0x1eb0);
  W(0x1eb8) =
       W(0x6c) * W(0x20e4) + W(0x1eb4);
  W(0x1ebc) =
       W(0x70) * W(0x20e8) + W(0x1eb8);
  W(0x1ec0) =
       W(0x6c) * W(0x20ec) + W(0x1ebc);
  for (local_14 = 0x41; local_14 < 0x82; local_14 = local_14 + 1) {
    W(0x1dc0 + local_14 * 4) = W(0x1ec0);
  }
  W(0x1fc8) = W(0x1fc4) + W(0x6c);
  W(0x1fcc) =
       W(0x1fc8) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1fd0) =
       W(0x1fcc) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1fd4) = W(0x1fd0) + W(0x6c);
  W(0x1fd8) =
       W(0x1fd4) + W(0x6c) * 2 + W(0x80) * 3;
  W(0x1fdc) =
       W(0x1fd8) + W(0x6c) * 2 + W(0x80) * 3;
  W(0x1fe0) =
       W(0x1fdc) + W(0x6c) * 2 + W(0x80) * 3;
  W(0x1fe4) =
       W(0x1fe0) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1fe8) =
       W(0x1fe4) + W(0x6c) * W(0x104) +
       W(0x80) * (W(0x104) + 1);
  W(0x1fec) = W(0x1fe8) + W(0x70) * 4;
  return;
}

#undef AT
#undef MAR
#undef W

} // namespace def::dbio::gendb
