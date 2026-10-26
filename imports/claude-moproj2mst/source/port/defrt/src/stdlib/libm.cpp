// The elementary functions of DEF_PRE_64, transcribed from its listing instruction by
// instruction (operation order, comparison direction and NaN behaviour of each COMISD/UCOMISD
// kept). Packet p1-01; see defrt/stdlib/libm.hpp. The tables below are copied from the binary.
#include <defrt/stdlib/libm.hpp>

#include <bit>
#include <cstdint>
#include <emmintrin.h>

namespace def::rt::libm {

namespace {

constexpr double D(std::uint64_t b) { return std::bit_cast<double>(b); }
constexpr float F(std::uint32_t b) { return std::bit_cast<float>(b); }
std::uint64_t bits(double x) { return std::bit_cast<std::uint64_t>(x); }
std::uint32_t bitsf(float x) { return std::bit_cast<std::uint32_t>(x); }
std::uint32_t hi32(double x) { return static_cast<std::uint32_t>(bits(x) >> 32); }
double with_hi(double x, std::uint32_t hi)
{
    return D((std::uint64_t{hi} << 32) | (bits(x) & 0xffffffffu));
}

// CVTTSD2SI / CVTTSS2SI (truncation; 0x80000000 for NaN and out of range)
std::int32_t cvtt(double x) { return _mm_cvttsd_si32(_mm_set_sd(x)); }
std::int32_t cvttf(float x) { return _mm_cvttss_si32(_mm_set_ss(x)); }

// ---- constants of the Fortran-runtime library (address: value) ----
constexpr double kOne = D(0x3ff0000000000000);       // 140509c90
constexpr double kTwo = D(0x4000000000000000);       // 140509fb8
constexpr double kPi = D(0x400921fb54442d18);        // 14051f230
constexpr double kPiBy2 = D(0x3ff921fb54442d18);     // 14051f238
constexpr double kHalf = D(0x3fe0000000000000);      // 14051f240
constexpr double kQuarter = D(0x3fd0000000000000);   // 140522fc0
constexpr double kDegToRad = D(0x3f91df46a2529d39);  // 140522fd0
constexpr double kRadToDeg = D(0x404ca5dc1a63c1f8);  // 140522fd8
constexpr double kSixteen = D(0x4030000000000000);   // 14053a7b0
constexpr double k1By256 = D(0x3f70000000000000);    // 14056ef50
constexpr double k1By16 = D(0x3fb0000000000000);     // 14056ef58
constexpr double kNegZero = D(0x8000000000000000);   // 14056ef60
constexpr double kPiBy4 = D(0x3fe921fb54442d18);     // 14056efd8
constexpr double kNegPiBy2 = D(0xbff921fb54442d18);  // 14056f0a8
constexpr double kInf = D(0x7ff0000000000000);       // 1405738b0
constexpr double kNaN = D(0x7ff8000000000000);       // 1405738b8
constexpr double kEps = D(0x3e46a09e667f3bc8);       // 140572480 (2^-26 * sqrt 2)

constexpr float kOneF = F(0x3f800000);      // 14053176c
constexpr float kTwoF = F(0x40000000);      // 140530be0
constexpr float kHalfF = F(0x3f000000);     // 14052c5a8
constexpr float kNegZeroF = F(0x80000000);  // 140573894
constexpr float kInfF = F(0x7f800000);      // 1405738a0
constexpr float kNaNF = F(0x7fc00000);      // 1405738a4

// @data 140572310  pow: A1 = 2^(-i/16) rounded, index 1..17 (17 entries)
constexpr std::uint64_t kPowA1[17] = {
    0x3ff0000000000000ull, 0x3feea4afa2a490daull, 0x3fed5818dcfba487ull, 0x3fec199bdd85529cull,
    0x3feae89f995ad3adull, 0x3fe9c49182a3f090ull, 0x3fe8ace5422aa0dbull, 0x3fe7a11473eb0187ull,
    0x3fe6a09e667f3bcdull, 0x3fe5ab07dd485429ull, 0x3fe4bfdad5362a27ull, 0x3fe3dea64c123422ull,
    0x3fe306fe0a31b715ull, 0x3fe2387a6e756238ull, 0x3fe172b83c7d517bull, 0x3fe0b5586cf9890full,
    0x3fe0000000000000ull,
};

// @data 1405723a0  pow: A2 = A1 correction for odd entries (8 entries)
constexpr std::uint64_t kPowA2[8] = {
    0xbc8ea00000000000ull, 0x3c71100000000000ull, 0x3c6c800000000000ull, 0xbc74200000000000ull,
    0x3c86300000000000ull, 0x3c7ad00000000000ull, 0x3c89b00000000000ull, 0x3c88a80000000000ull,
};

// @data 1405725c0  log: high part of log(1 + j/128) (129 entries)
constexpr std::uint64_t kLogT5c0[129] = {
    0x0000000000000000ull, 0x3f7fe02a6b200000ull, 0x3f8fc0a8b1000000ull, 0x3f97b91b07d80000ull,
    0x3f9f829b0e780000ull, 0x3fa39e87ba000000ull, 0x3fa77458f6340000ull, 0x3fab42dd71180000ull,
    0x3faf0a30c0100000ull, 0x3fb16536eea40000ull, 0x3fb341d7961c0000ull, 0x3fb51b073f060000ull,
    0x3fb6f0d28ae60000ull, 0x3fb8c345d6320000ull, 0x3fba926d3a4a0000ull, 0x3fbc5e548f5c0000ull,
    0x3fbe27076e2a0000ull, 0x3fbfec9131dc0000ull, 0x3fc0d77e7cd10000ull, 0x3fc1b72ad52f0000ull,
    0x3fc29552f8200000ull, 0x3fc371fc201f0000ull, 0x3fc44d2b6ccb0000ull, 0x3fc526e5e3a20000ull,
    0x3fc5ff3070a80000ull, 0x3fc6d60fe71a0000ull, 0x3fc7ab8902110000ull, 0x3fc87fa065210000ull,
    0x3fc9525a9cf40000ull, 0x3fca23bc1fe30000ull, 0x3fcaf3c94e810000ull, 0x3fcbc286742e0000ull,
    0x3fcc8ff7c79b0000ull, 0x3fcd5c216b500000ull, 0x3fce27076e2b0000ull, 0x3fcef0adcbdc0000ull,
    0x3fcfb9186d5e0000ull, 0x3fd0402594b50000ull, 0x3fd0a324e2738000ull, 0x3fd1058bf9ae8000ull,
    0x3fd1675cabab8000ull, 0x3fd1c898c1698000ull, 0x3fd22941fbcf8000ull, 0x3fd2895a13de8000ull,
    0x3fd2e8e2bae10000ull, 0x3fd347dd9a988000ull, 0x3fd3a64c55698000ull, 0x3fd4043086868000ull,
    0x3fd4618bc21c8000ull, 0x3fd4be5f95778000ull, 0x3fd51aad872e0000ull, 0x3fd5767717458000ull,
    0x3fd5d1bdbf580000ull, 0x3fd62c82f2ba0000ull, 0x3fd686c81e9b0000ull, 0x3fd6e08eaa2b8000ull,
    0x3fd739d7f6bc0000ull, 0x3fd792a55fdd8000ull, 0x3fd7eaf83b828000ull, 0x3fd842d1da1e8000ull,
    0x3fd89a3386c18000ull, 0x3fd8f11e87368000ull, 0x3fd947941c210000ull, 0x3fd99d9581180000ull,
    0x3fd9f323ecbf8000ull, 0x3fda484090e58000ull, 0x3fda9cec9a9a0000ull, 0x3fdaf12932478000ull,
    0x3fdb44f77bcc8000ull, 0x3fdb985896930000ull, 0x3fdbeb4d9da70000ull, 0x3fdc3dd7a7cd8000ull,
    0x3fdc8ff7c79a8000ull, 0x3fdce1af0b860000ull, 0x3fdd32fe7e010000ull, 0x3fdd83e7258a0000ull,
    0x3fddd46a04c20000ull, 0x3fde24881a7c8000ull, 0x3fde744261d68000ull, 0x3fdec399d2468000ull,
    0x3fdf128f5faf0000ull, 0x3fdf6123fa700000ull, 0x3fdfaf588f790000ull, 0x3fdffd2e08580000ull,
    0x3fe02552a5a5c000ull, 0x3fe04bdf9da94000ull, 0x3fe0723e5c1cc000ull, 0x3fe0986f4f574000ull,
    0x3fe0be72e4254000ull, 0x3fe0e44985d1c000ull, 0x3fe109f39e2d4000ull, 0x3fe12f7195940000ull,
    0x3fe154c3d2f4c000ull, 0x3fe179eabbd88000ull, 0x3fe19ee6b467c000ull, 0x3fe1c3b81f714000ull,
    0x3fe1e85f5e704000ull, 0x3fe20cdcd192c000ull, 0x3fe23130d7bec000ull, 0x3fe2555bce990000ull,
    0x3fe2795e1289c000ull, 0x3fe29d37fec2c000ull, 0x3fe2c0e9ed448000ull, 0x3fe2e47436e40000ull,
    0x3fe307d7334f0000ull, 0x3fe32b1339120000ull, 0x3fe34e289d9d0000ull, 0x3fe37117b5474000ull,
    0x3fe393e0d3564000ull, 0x3fe3b6844a000000ull, 0x3fe3d9026a714000ull, 0x3fe3fb5b84d18000ull,
    0x3fe41d8fe8468000ull, 0x3fe43f9fe2f9c000ull, 0x3fe4618bc21c4000ull, 0x3fe48353d1ea8000ull,
    0x3fe4a4f85db04000ull, 0x3fe4c679afcd0000ull, 0x3fe4e7d811b74000ull, 0x3fe50913cc018000ull,
    0x3fe52a2d265bc000ull, 0x3fe54b2467998000ull, 0x3fe56bf9d5b40000ull, 0x3fe58cadb5cd8000ull,
    0x3fe5ad404c358000ull, 0x3fe5cdb1dc6c0000ull, 0x3fe5ee02a9240000ull, 0x3fe60e32f4478000ull,
    0x3fe62e42fefa4000ull,
};

// @data 1405729d0  log: low part of log(1 + j/128) (129 entries)
constexpr std::uint64_t kLogT9d0[129] = {
    0x0000000000000000ull, 0xbd6f30ee07912df9ull, 0xbd5fe0e183092c59ull, 0xbd62772ab6c0559cull,
    0x3d2980267c7e09e4ull, 0xbd642a056fea4dfdull, 0xbd62303b9cb0d5e1ull, 0x3d671bec28d14c7eull,
    0x3d662a6617cc9717ull, 0xbd60a3e2f3b47d18ull, 0xbd4717b6b33e44f8ull, 0x3d383f69278e686aull,
    0xbd62968c836cc8c2ull, 0xbd5937c294d2f567ull, 0x3d6aac6ca17a4554ull, 0xbd4c5e7514f4083full,
    0x3d6e5cbd3d50fffcull, 0xbd354555d1ae6607ull, 0xbd6c69a65a23a170ull, 0x3d69e80a41811a39ull,
    0xbd35b967f4471dfcull, 0xbd6c22f10c9a4ea8ull, 0x3d6f4799f4f6543eull, 0xbd62f21746ff8a47ull,
    0xbd6b0b0de3077d7eull, 0xbd56f1b955c4d1daull, 0xbd537b720e4a694bull, 0xbd5b77b7effb7f41ull,
    0x3d65ad1d904c1d4eull, 0xbd62a739b23b93e1ull, 0xbd600349cc67f9b2ull, 0xbd6cca75818c5dbcull,
    0xbd697794f689f843ull, 0xbd611ba91bbca682ull, 0xbd3a342c2af0003cull, 0x3d664d948637950eull,
    0x3d5f1546aaa3361cull, 0xbd67df928ec217a5ull, 0x3d50e35f73f7a018ull, 0xbd6a9573b02faa5aull,
    0x3d630701ce63eab9ull, 0x3d59fafbc68e7540ull, 0xbd3a6976f5eb0963ull, 0x3d3a8d7ad24c13f0ull,
    0x3d5d309c2cc91a85ull, 0xbd25594dd4c58092ull, 0xbd6d0b1c68651946ull, 0x3d63f1de86093efaull,
    0xbd609ec17a426426ull, 0xbd3d7c92cd9ad824ull, 0xbd3f4bd8db0a7cc1ull, 0xbd62c9d5b2a49af9ull,
    0x3d4394a11b1c1ee4ull, 0xbd6c356848506eadull, 0x3d54aec442be1015ull, 0x3d60f1c609c98c6cull,
    0xbd67fcb18ed9d603ull, 0xbd6c2ec1f512dc03ull, 0x3d67e1b259d2f3daull, 0x3d462e927628cbc2ull,
    0xbd6ed2a52c73bf78ull, 0xbd5d3881e8962a96ull, 0x3d56faba4cdd147dull, 0xbd5f753456d113b8ull,
    0x3d584bf2b68d766full, 0x3d6d8515fe535b87ull, 0x3d40931a909fea5eull, 0xbd3e53bb31eed7a9ull,
    0x3d4ec5197ddb55d3ull, 0x3d50fb598fb14f89ull, 0x3d5b7bf7861d37acull, 0x3d66a6b9d9e0a5bdull,
    0x3d5a21ac25db1ef3ull, 0xbd48290905a86aa6ull, 0xbd542a9e21373414ull, 0x3d679f2828add176ull,
    0xbd6dafa08cecadb1ull, 0xbd53d9e34270ba6bull, 0x3d3e1f8bf68dbcf3ull, 0x3d49802eb9dca7e7ull,
    0x3d3bb2cd720ec44cull, 0x3d645639a2b61e5bull, 0xbd49c24ca098362bull, 0xbd46cf54d05f9367ull,
    0x3d60fec69c695d7full, 0xbd692d9a033eff75ull, 0x3d6f404e57963891ull, 0xbd55be8dc04ad601ull,
    0xbd657d49676844ccull, 0x3d5917edd5cbbd2dull, 0x3d592dfbc7d93617ull, 0xbd6043acfedce638ull,
    0x3d65e9a98f33a396ull, 0x3d69a0bfc60e6fa0ull, 0x3d52dd98b97baef0ull, 0xbd3eda1b58389902ull,
    0x3d1a07bd8b34be7cull, 0xbd64926cafc2f08aull, 0xbd17afa4392f1ba7ull, 0xbd506987f78a4a5eull,
    0xbd5dca290f81848dull, 0xbd5eea6f465268b4ull, 0x3d5d1772f5386374ull, 0x3d334202a10c3491ull,
    0x3d60be1fb590a1f5ull, 0x3d6d71320556b67bull, 0xbd6e2ce9146d277aull, 0x3d4ed71774092113ull,
    0xbd65e6563bbd9fc9ull, 0xbd3eea838909f3d3ull, 0x3d66faa404263d0bull, 0xbd60bda4b162afa3ull,
    0xbd5aa33736867a17ull, 0x3d5ccef4e4f736c2ull, 0x3d6ec27d0b7b37b3ull, 0x3d51bee7abd17660ull,
    0xbd244fdd840b8591ull, 0xbd61c64e971322ceull, 0x3d6bb09cb0985646ull, 0xbd6794b434c5a4f5ull,
    0x3d46abb9df22bc57ull, 0x3d6497a915428b44ull, 0xbd58cd7dc73bd194ull, 0xbd49db3db43689b4ull,
    0x3d6f2cfb29aaa5f0ull, 0x3d67648cf6e3c5d7ull, 0x3d667570d6095fd2ull, 0x3d51b194f912b417ull,
    0xbd48432a1b0e2634ull,
};

// @data 140572de0  log: 1/(1 + j/128) (129 entries)
constexpr std::uint64_t kLogTde0[129] = {
    0x3ff0000000000000ull, 0x3fefc07f01fc07f0ull, 0x3fef81f81f81f820ull, 0x3fef44659e4a4271ull,
    0x3fef07c1f07c1f08ull, 0x3feecc07b301ecc0ull, 0x3fee9131abf0b767ull, 0x3fee573ac901e574ull,
    0x3fee1e1e1e1e1e1eull, 0x3fede5d6e3f8868aull, 0x3fedae6076b981dbull, 0x3fed77b654b82c34ull,
    0x3fed41d41d41d41dull, 0x3fed0cb58f6ec074ull, 0x3fecd85689039b0bull, 0x3feca4b3055ee191ull,
    0x3fec71c71c71c71cull, 0x3fec3f8f01c3f8f0ull, 0x3fec0e070381c0e0ull, 0x3febdd2b899406f7ull,
    0x3febacf914c1bad0ull, 0x3feb7d6c3dda338bull, 0x3feb4e81b4e81b4full, 0x3feb2036406c80d9ull,
    0x3feaf286bca1af28ull, 0x3feac5701ac5701bull, 0x3fea98ef606a63beull, 0x3fea6d01a6d01a6dull,
    0x3fea41a41a41a41aull, 0x3fea16d3f97a4b02ull, 0x3fe9ec8e951033d9ull, 0x3fe9c2d14ee4a102ull,
    0x3fe999999999999aull, 0x3fe970e4f80cb872ull, 0x3fe948b0fcd6e9e0ull, 0x3fe920fb49d0e229ull,
    0x3fe8f9c18f9c18faull, 0x3fe8d3018d3018d3ull, 0x3fe8acb90f6bf3aaull, 0x3fe886e5f0abb04aull,
    0x3fe8618618618618ull, 0x3fe83c977ab2beddull, 0x3fe8181818181818ull, 0x3fe7f405fd017f40ull,
    0x3fe7d05f417d05f4ull, 0x3fe7ad2208e0ecc3ull, 0x3fe78a4c8178a4c8ull, 0x3fe767dce434a9b1ull,
    0x3fe745d1745d1746ull, 0x3fe724287f46debcull, 0x3fe702e05c0b8170ull, 0x3fe6e1f76b4337c7ull,
    0x3fe6c16c16c16c17ull, 0x3fe6a13cd1537290ull, 0x3fe6816816816817ull, 0x3fe661ec6a5122f9ull,
    0x3fe642c8590b2164ull, 0x3fe623fa77016240ull, 0x3fe6058160581606ull, 0x3fe5e75bb8d015e7ull,
    0x3fe5c9882b931057ull, 0x3fe5ac056b015ac0ull, 0x3fe58ed2308158edull, 0x3fe571ed3c506b3aull,
    0x3fe5555555555555ull, 0x3fe5390948f40febull, 0x3fe51d07eae2f815ull, 0x3fe5015015015015ull,
    0x3fe4e5e0a72f0539ull, 0x3fe4cab88725af6eull, 0x3fe4afd6a052bf5bull, 0x3fe49539e3b2d067ull,
    0x3fe47ae147ae147bull, 0x3fe460cbc7f5cf9aull, 0x3fe446f86562d9fbull, 0x3fe42d6625d51f87ull,
    0x3fe4141414141414ull, 0x3fe3fb013fb013fbull, 0x3fe3e22cbce4a902ull, 0x3fe3c995a47babe7ull,
    0x3fe3b13b13b13b14ull, 0x3fe3991c2c187f63ull, 0x3fe3813813813814ull, 0x3fe3698df3de0748ull,
    0x3fe3521cfb2b78c1ull, 0x3fe33ae45b57bcb2ull, 0x3fe323e34a2b10bfull, 0x3fe30d190130d190ull,
    0x3fe2f684bda12f68ull, 0x3fe2e025c04b8097ull, 0x3fe2c9fb4d812ca0ull, 0x3fe2b404ad012b40ull,
    0x3fe29e4129e4129eull, 0x3fe288b01288b013ull, 0x3fe27350b8812735ull, 0x3fe25e22708092f1ull,
    0x3fe2492492492492ull, 0x3fe23456789abcdfull, 0x3fe21fb78121fb78ull, 0x3fe20b470c67c0d9ull,
    0x3fe1f7047dc11f70ull, 0x3fe1e2ef3b3fb874ull, 0x3fe1cf06ada2811dull, 0x3fe1bb4a4046ed29ull,
    0x3fe1a7b9611a7b96ull, 0x3fe19453808ca29cull, 0x3fe1811811811812ull, 0x3fe16e0689427379ull,
    0x3fe15b1e5f75270dull, 0x3fe1485f0e0acd3bull, 0x3fe135c81135c811ull, 0x3fe12358e75d3033ull,
    0x3fe1111111111111ull, 0x3fe0fef010fef011ull, 0x3fe0ecf56be69c90ull, 0x3fe0db20a88f4696ull,
    0x3fe0c9714fbcda3bull, 0x3fe0b7e6ec259dc8ull, 0x3fe0a6810a6810a7ull, 0x3fe0953f39010954ull,
    0x3fe0842108421084ull, 0x3fe073260a47f7c6ull, 0x3fe0624dd2f1a9fcull, 0x3fe05197f7d73404ull,
    0x3fe0410410410410ull, 0x3fe03091b51f5e1aull, 0x3fe0204081020408ull, 0x3fe0101010101010ull,
    0x3fe0000000000000ull,
};

// @data 140573250  logf: high part of log(1 + j/128) (129 entries)
constexpr std::uint32_t kLogfT250[129] = {
    0x00000000u, 0x3bff0000u, 0x3c7e0000u, 0x3cbdc000u, 0x3cfc2000u, 0x3d1cf000u,
    0x3d3ba000u, 0x3d5a1000u, 0x3d785000u, 0x3d8b2800u, 0x3d9a1000u, 0x3da8d800u,
    0x3db78800u, 0x3dc61800u, 0x3dd49000u, 0x3de2f000u, 0x3df13800u, 0x3dff6800u,
    0x3e06bc00u, 0x3e0db800u, 0x3e14ac00u, 0x3e1b9000u, 0x3e226800u, 0x3e293800u,
    0x3e2ff800u, 0x3e36b000u, 0x3e3d5c00u, 0x3e43fc00u, 0x3e4a9400u, 0x3e511c00u,
    0x3e57a000u, 0x3e5e1400u, 0x3e648000u, 0x3e6ae000u, 0x3e713800u, 0x3e778400u,
    0x3e7dc800u, 0x3e820200u, 0x3e851a00u, 0x3e882c00u, 0x3e8b3a00u, 0x3e8e4400u,
    0x3e914a00u, 0x3e944a00u, 0x3e974800u, 0x3e9a3e00u, 0x3e9d3200u, 0x3ea02200u,
    0x3ea30c00u, 0x3ea5f200u, 0x3ea8d600u, 0x3eabb400u, 0x3eae8e00u, 0x3eb16400u,
    0x3eb43600u, 0x3eb70400u, 0x3eb9ce00u, 0x3ebc9600u, 0x3ebf5800u, 0x3ec21600u,
    0x3ec4d200u, 0x3ec78800u, 0x3eca3c00u, 0x3eccec00u, 0x3ecf9a00u, 0x3ed24200u,
    0x3ed4e800u, 0x3ed78a00u, 0x3eda2800u, 0x3edcc200u, 0x3edf5a00u, 0x3ee1ee00u,
    0x3ee48000u, 0x3ee70e00u, 0x3ee99800u, 0x3eec2000u, 0x3eeea400u, 0x3ef12400u,
    0x3ef3a200u, 0x3ef61c00u, 0x3ef89400u, 0x3efb0a00u, 0x3efd7a00u, 0x3effea00u,
    0x3f012b00u, 0x3f025f00u, 0x3f039200u, 0x3f04c300u, 0x3f05f400u, 0x3f072200u,
    0x3f085000u, 0x3f097c00u, 0x3f0aa600u, 0x3f0bcf00u, 0x3f0cf700u, 0x3f0e1e00u,
    0x3f0f4300u, 0x3f106700u, 0x3f118a00u, 0x3f12ab00u, 0x3f13cb00u, 0x3f14ea00u,
    0x3f160700u, 0x3f172400u, 0x3f183f00u, 0x3f195900u, 0x3f1a7100u, 0x3f1b8900u,
    0x3f1c9f00u, 0x3f1db400u, 0x3f1ec800u, 0x3f1fdb00u, 0x3f20ec00u, 0x3f21fd00u,
    0x3f230c00u, 0x3f241b00u, 0x3f252800u, 0x3f263400u, 0x3f273f00u, 0x3f284900u,
    0x3f295100u, 0x3f2a5900u, 0x3f2b6000u, 0x3f2c6500u, 0x3f2d6a00u, 0x3f2e6e00u,
    0x3f2f7000u, 0x3f307200u, 0x3f317200u,
};

// @data 140573460  logf: low part (129 entries)
constexpr std::uint32_t kLogfT460[129] = {
    0x00000000u, 0x3429ac42u, 0x35a8b0fcu, 0x368d83ebu, 0xb6b278c4u, 0x3687b9ffu,
    0x3631ec66u, 0x36dd7119u, 0x35c30046u, 0x365bba8eu, 0xb621a791u, 0x34e7e0c3u,
    0xb635d46au, 0x368bac63u, 0x36da7496u, 0x36a91eb8u, 0x34edc55eu, 0xb6dd9c48u,
    0xb44197b9u, 0x36ab54beu, 0xb6b41f80u, 0xb4f7f85cu, 0x36adb32eu, 0xb650e2f2u,
    0x36c1c29eu, 0x35fe719du, 0x3590210eu, 0x36819483u, 0xb6958c2fu, 0x36f07f8bu,
    0xb6dac5fdu, 0x354e85b2u, 0xb5838656u, 0x3685ad3fu, 0x356dc55eu, 0x36b72f71u,
    0x36436af2u, 0xb6d35a59u, 0xb6d8ec63u, 0x363f9ae5u, 0x36e55d5du, 0x36c60b4du,
    0x34fde7bdu, 0x36d09ef4u, 0xb6ea28f7u, 0x36ecd4c4u, 0x36455694u, 0xb6779796u,
    0x363c21c6u, 0x36fcabbcu, 0xb693c690u, 0xb60e8baau, 0xb51029feu, 0x353cae72u,
    0x3601e9b1u, 0x366aa2bau, 0x36bfb5dfu, 0xb6d50116u, 0xb5f88faau, 0x368ed0f4u,
    0xb64793ecu, 0x36f439b3u, 0x36a0e109u, 0x36ac08bfu, 0xb6e09a03u, 0x3410e5bbu,
    0xb69b2b30u, 0xb6b66dc4u, 0xb6084337u, 0x36c4b499u, 0x3659da72u, 0x36bd3e6du,
    0xb6038656u, 0xb687a3d0u, 0xb4c0ff8au, 0xb6c6d3afu, 0xb6afd9f2u, 0x3601a7c7u,
    0x351875a2u, 0x36ce9234u, 0x3675faf0u, 0xb6e02c7fu, 0x36c47bc8u, 0xb68fbd40u,
    0xb6d5a5a3u, 0xb444adb2u, 0xb551f190u, 0x36f4f573u, 0xb6d1bdadu, 0x36985d1du,
    0xb6c61d2bu, 0xb6e6a6c1u, 0x35f4bd35u, 0x36abbd8au, 0x36568cf9u, 0xb67c11d8u,
    0xb4a18fbfu, 0xb5cb9b55u, 0xb6f28414u, 0xb6062ce1u, 0xb576bb27u, 0xb68013d5u,
    0x369ed449u, 0xb6bc91c0u, 0xb68ccb0fu, 0xb6cc6edeu, 0x3689d9ceu, 0xb684ab8cu,
    0x34d3562au, 0x36094000u, 0x359a9c56u, 0xb60f65d2u, 0x36fe8467u, 0xb368318du,
    0x36bc21c6u, 0xb6c2e157u, 0xb67449f8u, 0xb64a0662u, 0xb67dc915u, 0xb6c33fe9u,
    0x36d265bcu, 0x360cf333u, 0xb6454982u, 0x36db5cd8u, 0x34186b3eu, 0xb6e2393fu,
    0x35aa4906u, 0xb6d0bb87u, 0x35bfbe8eu,
};

// @data 140573670  logf: 1/(1 + j/128) (129 entries)
constexpr std::uint32_t kLogfT670[129] = {
    0x3f800000u, 0x3f7e03f8u, 0x3f7c0fc1u, 0x3f7a232du, 0x3f783e10u, 0x3f76603eu,
    0x3f74898du, 0x3f72b9d6u, 0x3f70f0f1u, 0x3f6f2eb7u, 0x3f6d7304u, 0x3f6bbdb3u,
    0x3f6a0ea1u, 0x3f6865acu, 0x3f66c2b4u, 0x3f652598u, 0x3f638e39u, 0x3f61fc78u,
    0x3f607038u, 0x3f5ee95cu, 0x3f5d67c9u, 0x3f5beb62u, 0x3f5a740eu, 0x3f5901b2u,
    0x3f579436u, 0x3f562b81u, 0x3f54c77bu, 0x3f53680du, 0x3f520d21u, 0x3f50b6a0u,
    0x3f4f6475u, 0x3f4e168au, 0x3f4ccccdu, 0x3f4b8728u, 0x3f4a4588u, 0x3f4907dau,
    0x3f47ce0cu, 0x3f46980cu, 0x3f4565c8u, 0x3f443730u, 0x3f430c31u, 0x3f41e4bcu,
    0x3f40c0c1u, 0x3f3fa030u, 0x3f3e82fau, 0x3f3d6910u, 0x3f3c5264u, 0x3f3b3ee7u,
    0x3f3a2e8cu, 0x3f392144u, 0x3f381703u, 0x3f370fbbu, 0x3f360b61u, 0x3f3509e7u,
    0x3f340b41u, 0x3f330f63u, 0x3f321643u, 0x3f311fd4u, 0x3f302c0bu, 0x3f2f3adeu,
    0x3f2e4c41u, 0x3f2d602bu, 0x3f2c7692u, 0x3f2b8f6au, 0x3f2aaaabu, 0x3f29c84au,
    0x3f28e83fu, 0x3f280a81u, 0x3f272f05u, 0x3f2655c4u, 0x3f257eb5u, 0x3f24a9cfu,
    0x3f23d70au, 0x3f23065eu, 0x3f2237c3u, 0x3f216b31u, 0x3f20a0a1u, 0x3f1fd80au,
    0x3f1f1166u, 0x3f1e4cadu, 0x3f1d89d9u, 0x3f1cc8e1u, 0x3f1c09c1u, 0x3f1b4c70u,
    0x3f1a90e8u, 0x3f19d723u, 0x3f191f1au, 0x3f1868c8u, 0x3f17b426u, 0x3f17012eu,
    0x3f164fdau, 0x3f15a025u, 0x3f14f209u, 0x3f144581u, 0x3f139a86u, 0x3f12f114u,
    0x3f124925u, 0x3f11a2b4u, 0x3f10fdbcu, 0x3f105a38u, 0x3f0fb824u, 0x3f0f177au,
    0x3f0e7835u, 0x3f0dda52u, 0x3f0d3dcbu, 0x3f0ca29cu, 0x3f0c08c1u, 0x3f0b7034u,
    0x3f0ad8f3u, 0x3f0a42f8u, 0x3f09ae41u, 0x3f091ac7u, 0x3f088889u, 0x3f07f781u,
    0x3f0767abu, 0x3f06d905u, 0x3f064b8au, 0x3f05bf37u, 0x3f053408u, 0x3f04a9fau,
    0x3f042108u, 0x3f039930u, 0x3f03126fu, 0x3f028cc0u, 0x3f020821u, 0x3f01848eu,
    0x3f010204u, 0x3f008081u, 0x3f000000u,
};

// @data 14063bbd0  __remainder_piby2: bits of 2/pi in 10-bit chunks (126 entries)
constexpr std::uint64_t kPiBits[126] = {
    0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull, 0x0000000000000000ull,
    0x0000000000000000ull, 0x0000000000000000ull, 0x00000000000000a2ull, 0x00000000000003e6ull,
    0x0000000000000036ull, 0x0000000000000393ull, 0x0000000000000244ull, 0x0000000000000054ull,
    0x000000000000029full, 0x0000000000000309ull, 0x0000000000000357ull, 0x0000000000000347ull,
    0x0000000000000353ull, 0x0000000000000137ull, 0x00000000000001c0ull, 0x000000000000036dull,
    0x0000000000000229ull, 0x0000000000000166ull, 0x000000000000013cull, 0x000000000000010eull,
    0x0000000000000104ull, 0x000000000000007full, 0x0000000000000251ull, 0x000000000000018eull,
    0x00000000000002bdull, 0x00000000000003aeull, 0x00000000000003c5ull, 0x0000000000000186ull,
    0x0000000000000372ull, 0x000000000000011bull, 0x000000000000023aull, 0x0000000000000109ull,
    0x00000000000000ddull, 0x00000000000000b8ull, 0x0000000000000006ull, 0x0000000000000124ull,
    0x00000000000002eeull, 0x0000000000000282ull, 0x00000000000001d1ull, 0x0000000000000248ull,
    0x00000000000001cfull, 0x0000000000000387ull, 0x00000000000001ebull, 0x0000000000000072ull,
    0x0000000000000312ull, 0x0000000000000269ull, 0x000000000000033eull, 0x00000000000003a2ull,
    0x0000000000000023ull, 0x000000000000017dull, 0x000000000000012eull, 0x00000000000002edull,
    0x0000000000000048ull, 0x000000000000013aull, 0x000000000000019cull, 0x00000000000001c0ull,
    0x000000000000026bull, 0x0000000000000117ull, 0x000000000000037eull, 0x0000000000000104ull,
    0x0000000000000399ull, 0x0000000000000075ull, 0x0000000000000239ull, 0x000000000000020dull,
    0x0000000000000133ull, 0x000000000000027dull, 0x000000000000009cull, 0x0000000000000211ull,
    0x00000000000001f8ull, 0x00000000000002efull, 0x00000000000001f9ull, 0x00000000000000a0ull,
    0x00000000000003b1ull, 0x00000000000003feull, 0x0000000000000097ull, 0x00000000000003ffull,
    0x00000000000001e0ull, 0x0000000000000166ull, 0x000000000000000full, 0x00000000000003bcull,
    0x00000000000002f1ull, 0x0000000000000062ull, 0x000000000000035aull, 0x0000000000000029ull,
    0x00000000000002d1ull, 0x00000000000003dbull, 0x0000000000000136ull, 0x00000000000001fbull,
    0x00000000000000f2ull, 0x00000000000001f2ull, 0x0000000000000309ull, 0x00000000000002ddull,
    0x00000000000000f4ull, 0x000000000000018full, 0x0000000000000366ull, 0x0000000000000279ull,
    0x00000000000001feull, 0x000000000000028bull, 0x0000000000000175ull, 0x000000000000009eull,
    0x00000000000003acull, 0x00000000000001faull, 0x00000000000003e5ull, 0x00000000000003c5ull,
    0x00000000000003b3ull, 0x0000000000000341ull, 0x0000000000000339ull, 0x00000000000003deull,
    0x00000000000000a5ull, 0x00000000000000a4ull, 0x00000000000002eaull, 0x00000000000001afull,
    0x00000000000003b5ull, 0x00000000000003ecull, 0x000000000000011full, 0x0000000000000235ull,
    0x00000000000001d0ull, 0x0000000000000215ull, 0x0000000000000203ull, 0x00000000000000c1ull,
    0x000000000000006full, 0x000000000000031eull,
};


// sin/cos core: 1404b3960(x, cosine) and its float twin 1404b491c
double sincos_core(double x, int cosine)
{
    bool negate = false;
    const double y = ps_fabs(x);
    double xmm1 = y;
    if (cosine != 0) {
        xmm1 = y + kPiBy2;
    } else if (0.0 > x) {
        negate = true;
    }
    if (xmm1 > D(0x41a921fb54000000)) { // 1405724a0
        return 0.0;
    }
    const std::int32_t n = cvtt(xmm1 * D(0x3fd45f306dc9c883) + kHalf); // 140572498 (1/pi)
    double xn = static_cast<double>(n);
    if (n & 1) {
        negate = !negate;
    }
    if (cosine != 0) {
        xn = xn + (0.0 - kHalf);
    }
    double f = (y - xn * D(0x4009220000000000)) - xn * D(0xbee2aeef4b9ee59e); // 140572488/490
    if (ps_fabs(f) >= kEps) {
        const double g = f * f;
        double r = g * D(0x3ce880ff6993df95);     // 140572478
        r = r - D(0x3d6ae420dc08499c);            // 140572470
        r = r * g;
        r = r + D(0x3de6123c686ad430);            // 140572468
        r = r * g;
        r = r - D(0x3e5ae6454b5dc0ab);            // 140572460
        r = r * g;
        r = r + D(0x3ec71de3a524f063);            // 140572458
        r = r * g;
        r = r - D(0x3f2a01a01a013e1a);            // 140572450
        r = r * g;
        r = r + D(0x3f811111111110b0);            // 140572448
        r = r * g;
        r = r - D(0x3fc5555555555555);            // 140572440
        r = r * f;
        r = r * g;
        f = f + r;
    }
    if (negate) {
        return kNegZero - f;
    }
    return f;
}

float sincos_core_f(float x, int cosine)
{
    bool negate = false;
    const float y = ps_fabsf(x);
    float xmm1 = y;
    if (cosine != 0) {
        xmm1 = y + F(0x3fc90fdb); // 1405738ec
    } else if (0.0f > x) {
        negate = true;
    }
    if (xmm1 > F(0x46490c00)) { // 1405738e8: 12867, go through the double routine
        return static_cast<float>(sincos_core(static_cast<double>(x), cosine));
    }
    const std::int32_t n = cvttf(xmm1 * F(0x3ea2f983) + kHalfF); // 1405738e4
    float xn = static_cast<float>(n);
    if (n & 1) {
        negate = !negate;
    }
    if (cosine != 0) {
        xn = xn + (0.0f - kHalfF);
    }
    float f = (y - xn * F(0x40490000)) - xn * F(0x3a7daa23); // 1405738dc/8e0
    if (ps_fabsf(f) >= F(0x39800000)) {                       // 1405738d8
        const float g = f * f;
        float r = g * F(0x362e9c5b); // 1405738d4
        r = r - F(0x394fb222);       // 1405738d0
        r = r * g;
        r = r + F(0x3c08873e);       // 1405738cc
        r = r * g;
        r = r - F(0x3e2aaaa4);       // 1405738c8
        r = r * g;
        r = r * f;
        f = f + r;
    }
    if (negate) {
        return kNegZeroF - f;
    }
    return f;
}

// asin/acos core: 1404b3ba4(x, k), k = 0 asin, 1 acos
double asincos_core(double x, int k)
{
    double y;
    if (0.0 > x) {
        k += 2;
        y = kNegZero - x;
    } else {
        y = x;
    }
    if (y > kEps) {
        double g;
        if (kHalf < y) {
            if (kOne < y) {
                return kNaN;
            }
            k += 4;
            g = ((kHalf - y) + kHalf) * kHalf;
            y = ps_sqrt(g);
            y = y + y;
        } else {
            g = y * y;
        }
        double num = D(0x40244e1764ec3927) - g * D(0x3fe64bbdb5e61e65); // 1405724f0, 1405724e8
        double den = g - D(0x4037d2e86ef9861f);                          // 1405724e0
        num = num * g;
        den = den * g;
        num = num - D(0x4043d82ca9a6da9f);                               // 1405724d8
        den = den + D(0x4062de7c96591c70);                               // 1405724d0
        num = num * g;
        num = num + D(0x404c9aa7360ad48a);                               // 1405724c8
        den = den * g;
        den = den - D(0x4077ddcefc56a848);                               // 1405724c0
        num = num * g;
        num = num - D(0x403b5e55a83a0a62);                               // 1405724b8
        den = den * g;
        den = den + D(0x407a124f101eb843);                               // 1405724b0
        num = num * g;
        den = den * g;
        den = den - D(0x406486c03e2b87cc);                               // 1405724a8
        num = num / den;
        num = num * y;
        y = y + num;
    }
    switch (k) {
    case 0:
    case 5:
        return y;
    case 1:
    case 4:
        return (kPiBy4 - y) + kPiBy4;
    case 2:
        return kNegZero - y;
    case 3:
        return (y + kPiBy4) + kPiBy4;
    case 6:
        return (y - kPiBy4) - kPiBy4;
    default: // 7
        return (kPiBy2 - y) + kPiBy2;
    }
}

} // namespace

// @src 1404b4898  fabs (clears the sign bit)
double ps_fabs(double x) { return D(bits(x) & 0x7fffffffffffffffull); }
// @src 1404b48b8  fabs, by reference
double ps_fabs_r(const double* x) { return ps_fabs(*x); }
// @src 1404b4d98  fabsf
float ps_fabsf(float x) { return F(bitsf(x) & 0x7fffffffu); }
// @src 1404b4dac  fabsf, by reference
float ps_fabsf_r(const float* x) { return ps_fabsf(*x); }

// @src 1404b3960  (sin/cos core, shared by the entries below)
// @src 1404b3ab0  sin
double ps_sin(double x) { return sincos_core(x, 0); }
// @src 1404b3ab8  cos
double ps_cos(double x) { return sincos_core(x, 1); }
// @src 1404b3ac4  sind
double ps_sind(double x) { return sincos_core(x * kDegToRad, 0); }
// @src 1404b3ad4  cosd
double ps_cosd(double x) { return sincos_core(x * kDegToRad, 1); }
// @src 1404b3ae8  sin, by reference
double ps_sin_r(const double* x) { return sincos_core(*x, 0); }
// @src 1404b3af4  cos, by reference
double ps_cos_r(const double* x) { return sincos_core(*x, 1); }
// @src 1404b3b04  sind, by reference
double ps_sind_r(const double* x) { return sincos_core(*x * kDegToRad, 0); }
// @src 1404b3b18  cosd, by reference
double ps_cosd_r(const double* x) { return sincos_core(*x * kDegToRad, 1); }

// @src 1404b3b30  sincos: *s = sin(x), *c = cos(x)
void ps_sincos(double x, double* s, double* c)
{
    *s = ps_sin(x);
    *c = ps_cos(x);
}

// @src 1404b491c  (float sin/cos core)
// @src 1404b4a40  sinf
float ps_sinf(float x) { return sincos_core_f(x, 0); }
// @src 1404b4a48  cosf
float ps_cosf(float x) { return sincos_core_f(x, 1); }
// @src 1404b4a54  sindf
float ps_sindf(float x) { return sincos_core_f(x * F(0x3c8efa35), 0); } // 1405738f0
// @src 1404b4a64  cosdf
float ps_cosdf(float x) { return sincos_core_f(x * F(0x3c8efa35), 1); }
// @src 1404b4a78  sinf, by reference
float ps_sinf_r(const float* x) { return sincos_core_f(*x, 0); }
// @src 1404b4a84  cosf, by reference
float ps_cosf_r(const float* x) { return sincos_core_f(*x, 1); }
// @src 1404b4a94  sindf, by reference
float ps_sindf_r(const float* x) { return sincos_core_f(*x * F(0x3c8efa35), 0); }
// @src 1404b4aa8  cosdf, by reference
float ps_cosdf_r(const float* x) { return sincos_core_f(*x * F(0x3c8efa35), 1); }

// @src 1404b3b6c  sincosf
void ps_sincosf(float x, float* s, float* c)
{
    *s = ps_sinf(x);
    *c = ps_cosf(x);
}

// @src 1404b3df0  tan
double ps_tan(double x)
{
    const double ay = ps_fabs(x);
    if (ay > D(0x41a1c5832329691a)) { // 140572548
        return 0.0;
    }
    std::int32_t n = cvtt(ay * D(0x3fe45f306dc9c883) + kHalf); // 140572540 (2/pi)
    if (0.0 > x) {
        n = static_cast<std::int32_t>(0u - static_cast<std::uint32_t>(n));
    }
    const double xn = static_cast<double>(n);
    double f = (x - xn * D(0x3ff9220000000000)) - xn * D(0xbed2aeef4b9ee59e); // 140572530/538
    double xnum;
    double xden;
    if (kEps > ps_fabs(f)) {
        xnum = f;
        xden = kOne;
    } else {
        const double g = f * f;
        double a = D(0x3f6c0e82a63baadf) - g * D(0x3ef2bab72ea2c724); // 140572528, 140572520
        double b = g * D(0x3ea0b774f07678e9);                          // 140572518
        a = a * g;
        b = b - D(0x3f346f6499094841);                                 // 140572510
        a = a - D(0x3fc112b5e54d0900);                                 // 140572508
        b = b * g;
        b = b + D(0x3f9a479ea17e2159);                                 // 140572500
        a = a * g;
        a = a * f;
        b = b * g;
        a = a + f;
        b = b - D(0x3fdddeb047fbd9d5);                                 // 1405724f8
        xnum = a;
        b = b * g;
        xden = b + kOne;
    }
    if (n & 1) {
        return kNegZero - xden / xnum;
    }
    return xnum / xden;
}

// @src 1404b3f18  tand
double ps_tand(double x) { return ps_tan(x * kDegToRad); }
// @src 1404b3f28  tan, by reference
double ps_tan_r(const double* x) { return ps_tan(*x); }
// @src 1404b3f34  tand, by reference
double ps_tand_r(const double* x) { return ps_tan(*x * kDegToRad); }

// @src 1404b3f48  atan
double ps_atan(double x)
{
    int n = 0;
    bool negative = false;
    double y = x;
    if (0.0 > x) {
        y = kNegZero - x;
        negative = true;
    }
    if (y > kOne) {
        y = kOne / y;
        n = 2;
    }
    if (y > D(0x3fd126145e9ecd56)) { // 1405725b0 (2 - sqrt 3)
        n = n + 1;
        double a = y * D(0x3fe76cf5d0b09955); // 1405725a8 (sqrt 3 - 1)
        a = a - kHalf;
        a = a - kHalf;
        const double b = y + D(0x3ffbb67ae8584caa); // 1405725a0 (sqrt 3)
        a = a + y;
        y = a / b;
    }
    if (ps_fabs(y) > kEps) {
        const double g = y * y;
        double p = D(0xc020fd3f5c8d6a63) - g * D(0x3feacd7ad9b187bd); // 140572598, 140572590
        double q = g + D(0x402e0c49e14ac710);                          // 140572588
        p = p * g;
        p = p - D(0x4034817fb9e2bccb);                                 // 140572580
        q = q * g;
        q = q + D(0x404dca0a320da3d7);                                 // 140572578
        p = p * g;
        p = p - D(0x402b60a651061ce2);                                 // 140572570
        q = q * g;
        q = q + D(0x40558a12040b6da5);                                 // 140572568
        p = p * g;
        q = q * g;
        q = q + D(0x4044887cbcc495a9);                                 // 140572560
        p = p / q;
        p = p * y;
        y = y + p;
    }
    if (n > 1) {
        y = kNegZero - y;
    }
    if (n == 1) {
        y = y + D(0x3fe0c152382d7366); // 140572550 (pi/6)
    } else if (n == 2) {
        y = y + kPiBy2;
    } else if (n == 3) {
        y = y + D(0x3ff0c152382d7366); // 140572558 (pi/3)
    }
    if (negative) {
        return kNegZero - y;
    }
    return y;
}

// @src 1404b40c8  atand
double ps_atand(double x) { return ps_atan(x) * kRadToDeg; }
// @src 1404b40e0  atan, by reference
double ps_atan_r(const double* x) { return ps_atan(*x); }
// @src 1404b40ec  atand, by reference
double ps_atand_r(const double* x) { return ps_atan(*x) * kRadToDeg; }

// @src 1404b3878  atan2(y, x)
double ps_atan2(double y, double x)
{
    if (x == 0.0) {
        if (y == 0.0) {
            return 0.0;
        }
        if (0.0 > y) {
            return kNegPiBy2;
        }
        return kPiBy2;
    }
    const double t = ps_atan(ps_fabs(y / x));
    double r = t;
    if (0.0 > x) {
        r = kPi - t;
    }
    if (bits(y) >> 63) { // 1404b38ea: sign bit of y
        return kNegZero - r;
    }
    return r;
}

// @src 1404b3918  atan2d
double ps_atan2d(double y, double x) { return ps_atan2(y, x) * kRadToDeg; }
// @src 1404b3930  atan2, by reference
double ps_atan2_r(const double* y, const double* x) { return ps_atan2(*y, *x); }
// @src 1404b3940  atan2d, by reference
double ps_atan2d_r(const double* y, const double* x) { return ps_atan2(*y, *x) * kRadToDeg; }

// @src 1404b3ba4  (asin/acos core)
// @src 1404b3d50  asin
double ps_asin(double x) { return asincos_core(x, 0); }
// @src 1404b3d58  acos
double ps_acos(double x) { return asincos_core(x, 1); }
// @src 1404b3d64  asind
double ps_asind(double x) { return asincos_core(x, 0) * kRadToDeg; }
// @src 1404b3d7c  acosd
double ps_acosd(double x) { return asincos_core(x, 1) * kRadToDeg; }
// @src 1404b3d98  asin, by reference
double ps_asin_r(const double* x) { return asincos_core(*x, 0); }
// @src 1404b3da4  acos, by reference
double ps_acos_r(const double* x) { return asincos_core(*x, 1); }
// @src 1404b3db4  asind, by reference
double ps_asind_r(const double* x) { return asincos_core(*x, 0) * kRadToDeg; }
// @src 1404b3dd0  acosd, by reference
double ps_acosd_r(const double* x) { return asincos_core(*x, 1) * kRadToDeg; }

// @src 1404b4ac0  sqrt: an exponent-halving guess and two double Newton steps
double ps_sqrt(double x)
{
    if (x == 0.0) {
        return x;
    }
    if (0.0 > x) {
        return kNaN;
    }
    const std::uint32_t hi = (hi32(x) >> 1) + 0x1ff80000u;
    double s = D(std::uint64_t{hi} << 32);
    s = s + x / s;
    double t = x / s + s * kQuarter;
    t = t + x / t;
    return x / t + t * kQuarter;
}

// @src 1404b4b48  sqrt, by reference
double ps_sqrt_r(const double* x) { return ps_sqrt(*x); }

// @src 1404b4264  log
double ps_log(double x)
{
    constexpr double ln2_hi = D(0x3fe62e42fefa4000); // stored on the stack at 1404b4277
    constexpr double ln2_lo = D(0xbd48432a1b0e2634); // 1404b428f
    if (0.0 >= x) { // COMISD 0,x; JC taken for 0 < x and for NaN
        if (0.0 > x) {
            return kNaN;
        }
        return kNegZero - kInf;
    }
    if (!(x >= D(0x3ff1082b577d34f0)) && !(D(0x3fee0fabfbc702a6) >= x)) { // 140573238/230
        // near 1: log(x) = 2 atanh(f / (f + 2)), split for accuracy
        const double f = x - kOne;
        const double u = kOne / (f + kTwo);
        double v = u * f;
        v = v + v;
        const double v_hi = D(bits(v) & 0xffffffffe0000000ull);
        const double f_hi = D(bits(f) & 0xffffffffe0000000ull);
        const double w = v * v;
        double p = w * D(0x3f3c8034c85dfff8); // 140573228
        p = p + D(0x3f62492307f151a3);        // 140573220
        p = p * w;
        p = p + D(0x3f89999999bac6f0);        // 140573218
        p = p * w;
        const double w3 = w * v;
        p = p + D(0x3fb55555555554e7);        // 140573210
        const double vf = v_hi * f_hi;
        p = p * w3;
        double a = (f - v_hi) * kTwo;
        const double b = (f - f_hi) * v_hi;
        a = a - vf;
        a = a - b;
        a = a * u;
        a = a + p;
        return a + v_hi;
    }
    const std::uint64_t xb = bits(x);
    const std::uint32_t hi = static_cast<std::uint32_t>(xb >> 32);
    const std::int32_t e = static_cast<std::int32_t>((hi >> 20) & 0x7ff) - 0x3ff;
    const std::uint32_t mhi = (hi & ~(1u << 30)) | 0x3ff00000u;
    if (e == 0x400) {
        return kInf;
    }
    double z = D((std::uint64_t{mhi} << 32) | (xb & 0xffffffffu));
    const double de = static_cast<double>(e);
    std::int32_t j = static_cast<std::int32_t>((mhi >> 13) & 0x7f);
    const double c = D(std::uint64_t{mhi & 0xffffe000u} << 32);
    z = z + (0.0 - c);
    if (z > k1By256) {
        j = j + 1;
        z = z + (0.0 - D(0x3f80000000000000)); // 140573208
    }
    double hi_part = ln2_hi * de;
    z = z * D(kLogTde0[j]);
    hi_part = hi_part + D(kLogT5c0[j]);
    double p = D(0x3fc999b4dfed6fee) - z * D(0x3fc5557666472e16); // 140573200, 1405731f8
    const double zz = z * z;
    p = p * z;
    p = p - D(0x3fcfffffffeb6529); // 1405731f0
    p = p * z;
    p = p + D(0x3fd5555555509bac); // 1405731e8
    p = p * z;
    p = p - kHalf;
    p = p * zz;
    double r = ln2_lo * de;
    r = r + D(kLogT9d0[j]);
    r = r + p;
    r = r + z;
    return r + hi_part;
}

// @src 1404b44c8  log10
double ps_log10(double x) { return ps_log(x) * D(0x3fdbcb7b1526e50e); } // 140573240
// @src 1404b44e0  log, by reference
double ps_log_r(const double* x) { return ps_log(*x); }
// @src 1404b44ec  log10, by reference
double ps_log10_r(const double* x) { return ps_log(*x) * D(0x3fdbcb7b1526e50e); }

// @src 1404b45a0  logf
float ps_logf(float x)
{
    constexpr float ln2_hi = F(0x3f317200); // 1404b45b2
    constexpr float ln2_lo = F(0x35bfbe8e); // 1404b45c0
    if (0.0f >= x) {
        if (0.0f > x) {
            return kNaNF;
        }
        return kNegZeroF - kInfF;
    }
    if (!(x >= F(0x3f88415b)) && !(F(0x3f707d60) >= x)) { // 140573890/88c
        const float f = x - kOneF;
        const float u = kOneF / (f + kTwoF);
        float v = u * f;
        v = v + v;
        const float v_hi = F(bitsf(v) & 0xfffff000u);
        const float f_hi = F(bitsf(f) & 0xfffff000u);
        const float w = v * v;
        const float w3 = w * v;
        float p = w * F(0x3c4d009a); // 140573888
        const float vf = v_hi * f_hi;
        p = p + F(0x3daaaaa9);       // 140573884
        p = p * w3;
        float a = (f - v_hi) * kTwoF;
        const float b = (f - f_hi) * v_hi;
        a = a - vf;
        a = a - b;
        a = a * u;
        a = a + v_hi;
        return a + p;
    }
    const std::uint32_t xb = bitsf(x);
    const std::uint32_t mb = (xb & ~(1u << 30)) | 0x3f800000u;
    const std::int32_t e = static_cast<std::int32_t>((xb >> 23) & 0xff) - 0x7f;
    if (e == 0x80) {
        return kInfF;
    }
    float z = F(mb);
    std::int32_t j = static_cast<std::int32_t>((mb >> 16) & 0x7f);
    const float c = F(mb & 0xffff0000u);
    const float de = static_cast<float>(e);
    z = z + (0.0f - c);
    if (z > F(0x3b800000)) { // 140573880
        j = j + 1;
        z = z + (0.0f - F(0x3c000000)); // 14057387c
    }
    float lo_part = ln2_lo * de;
    z = z * F(kLogfT670[j]);
    lo_part = lo_part + F(kLogfT460[j]);
    float p = z * F(0x3eaaaa3b); // 140573878
    const float zz = z * z;
    p = p - F(0x3f000032);       // 140573874
    p = p * zz;
    p = p + lo_part;
    float hi_part = ln2_hi * de;
    hi_part = hi_part + F(kLogfT250[j]);
    p = p + z;
    return p + hi_part;
}

// @src 1404b4794  log10f
float ps_log10f(float x) { return ps_logf(x) * F(0x3ede5bd9); } // 140573898
// @src 1404b47ac  logf, by reference (1404b47b8 is a second copy)
float ps_logf_r(const float* x) { return ps_logf(*x); }
// @src 1404b47c4  log10f, by reference (1404b47e0 is a second copy)
float ps_log10f_r(const float* x) { return ps_logf(*x) * F(0x3ede5bd9); }

// @src 1404b47fc  x**n, n INTEGER*4: binary powering
double ps_powi(double x, std::int32_t n)
{
    if (n == 0) {
        return kOne;
    }
    double r = kOne;
    std::uint32_t e = n < 0 ? 0u - static_cast<std::uint32_t>(n) : static_cast<std::uint32_t>(n);
    std::uint32_t bit = e & 1;
    e >>= 1;
    while (e != 0) {
        if (bit) {
            r = r * x;
        }
        x = x * x;
        bit = e & 1;
        e >>= 1;
    }
    x = x * r;
    if (n < 0) {
        return kOne / x;
    }
    return x;
}

// @src 1404b4848  x**n, n INTEGER*8
double ps_powi64(double x, std::int64_t n)
{
    if (n == 0) {
        return kOne;
    }
    double r = kOne;
    std::uint64_t e = n < 0 ? 0ull - static_cast<std::uint64_t>(n) : static_cast<std::uint64_t>(n);
    std::uint64_t bit = e & 1;
    e >>= 1;
    while (e != 0) {
        if (bit) {
            r = r * x;
        }
        x = x * x;
        bit = e & 1;
        e >>= 1;
    }
    x = x * r;
    if (n < 0) {
        return kOne / x;
    }
    return x;
}

// @src 1404b4508  x**n, REAL*4 x, INTEGER*4 n
float ps_powif(float x, std::int32_t n)
{
    if (n == 0) {
        return kOneF;
    }
    float r = kOneF;
    std::uint32_t e = n < 0 ? 0u - static_cast<std::uint32_t>(n) : static_cast<std::uint32_t>(n);
    std::uint32_t bit = e & 1;
    e >>= 1;
    while (e != 0) {
        if (bit) {
            r = r * x;
        }
        x = x * x;
        bit = e & 1;
        e >>= 1;
    }
    x = x * r;
    if (n < 0) {
        return kOneF / x;
    }
    return x;
}

// @src 1404b4550  x**n, REAL*4 x, INTEGER*8 n
float ps_powif64(float x, std::int64_t n)
{
    if (n == 0) {
        return kOneF;
    }
    float r = kOneF;
    std::uint64_t e = n < 0 ? 0ull - static_cast<std::uint64_t>(n) : static_cast<std::uint64_t>(n);
    std::uint64_t bit = e & 1;
    e >>= 1;
    while (e != 0) {
        if (bit) {
            r = r * x;
        }
        x = x * x;
        bit = e & 1;
        e >>= 1;
    }
    x = x * r;
    if (n < 0) {
        return kOneF / x;
    }
    return x;
}

// @src 1404b3440  x**y (Cody & Waite: log2 in sixteenths, then 2**w)
double ps_pow(double x, double y)
{
    if (y == 0.0) {
        return kOne;
    }
    if (x == kOne) {
        return x;
    }
    const std::uint64_t yb = bits(y);
    const std::uint32_t ylo = static_cast<std::uint32_t>(yb);
    const std::uint32_t yhi = static_cast<std::uint32_t>(yb >> 32);
    const std::int32_t ey = static_cast<std::int32_t>((yhi >> 20) & 0x7ff) - 0x3ff;
    double xx = x;
    if (ylo == 0) {
        if (static_cast<std::uint32_t>(ey) <= 0x1e) {
            if ((yhi & 0xfffff) == 0) {
                return ps_powi(x, cvtt(y));
            }
            if (ey < 0x15 && (yhi & ((0xfffffu >> ey) & 0xfffffu)) == 0) {
                return ps_powi(x, cvtt(y));
            }
        }
    } else {
        if (static_cast<std::uint32_t>(ey - 0x15) <= 9 &&
            (ylo & (0xffffffffu >> (ey - 0x14))) == 0) {
            return ps_powi(x, cvtt(y));
        }
        if (0.0 > x && ey >= 0x1f &&
            (ey >= 0x34 || (ylo & (0xffffffffu >> (ey - 0x14))) == 0)) {
            xx = kNegZero - x; // an even or odd integer power of a negative base: |x|**y
        }
    }
    if (0.0 >= xx) { // COMISD 0,xx; JC goes on for 0 < xx and for NaN
        if (0.0 > xx) {
            return kNaN;
        }
        if (0.0 > y) {
            return kInf;
        }
        return 0.0;
    }
    // g = mantissa of xx in [0.5, 1), m = its exponent
    const std::uint64_t xb = bits(xx);
    const std::uint32_t xhi = static_cast<std::uint32_t>(xb >> 32);
    const std::int32_t m = static_cast<std::int32_t>((xhi >> 20) & 0x7ff) - 0x3fe;
    const double g = D((std::uint64_t{(xhi & 0xbfefffffu) | 0x3fe00000u} << 32) | (xb & 0xffffffffu));
    std::int32_t p = 0;
    if (D(kPowA1[8]) >= g) {
        p = 8;
    }
    if (D(kPowA1[p + 4]) >= g) {
        p = p + 4;
    }
    if (D(kPowA1[p + 2]) >= g) {
        p = p + 2;
    }
    p = p + 1;
    const double u2 = static_cast<double>(m * 16 - p) * k1By16;
    const double a1 = D(kPowA1[p]);
    double z = ((g - a1) - D(kPowA2[p >> 1])) / (a1 + g);
    z = z + z;
    const double v = z * z;
    double r = v * D(0x3f3c78fddb4afc28); // 140572430
    r = r + D(0x3f6249242e278dac);        // 140572428
    r = r * v;
    r = r + D(0x3f899999999e080e);        // 140572420
    r = r * v;
    r = r + D(0x3fb555555555554d);        // 140572418
    r = r * v;
    r = r * z;
    constexpr double k = D(0x3fdc551d94ae0bf8); // 140572438 (log2(e) - 1)
    const std::int32_t iy = cvtt(y * kSixteen);
    double w = r * k;
    w = w + r;
    const double y1 = static_cast<double>(iy) * k1By16;
    w = w + z * k;
    w = w + z;
    w = w * y;
    double y2 = y - y1;
    double t = y1 * u2;
    y2 = y2 * u2;
    w = w + y2;
    const double w1 = static_cast<double>(cvtt(w * kSixteen)) * k1By16;
    t = t + w1;
    w = w - w1;
    const double w1b = static_cast<double>(cvtt(t * kSixteen)) * k1By16;
    t = t - w1b;
    w = w + t;
    const double w1c = static_cast<double>(cvtt(w * kSixteen)) * k1By16;
    std::int32_t iw = cvtt((w1b + w1c) * kSixteen);
    w = w + (0.0 - w1c);
    if (iw > 0x3ffe) {
        return kInf;
    }
    if (iw < -0x3fde) {
        return 0.0;
    }
    if (!(0.0 > w)) {
        iw = iw + 1;
        w = w + (0.0 - k1By16);
    }
    std::int32_t mp = iw / 16;
    if (iw > 0) {
        mp = mp + 1;
    }
    const std::int32_t idx = mp * 16 - iw;
    double zz = w * D(0x3eef4edde392cc80); // 140572410
    zz = zz + D(0x3f242f7ae0384c74);       // 140572408
    zz = zz * w;
    zz = zz + D(0x3f55d87e18d7cd9f);       // 140572400
    zz = zz * w;
    zz = zz + D(0x3f83b2ab6e131d98);       // 1405723f8
    zz = zz * w;
    zz = zz + D(0x3fac6b08d703026d);       // 1405723f0
    zz = zz * w;
    zz = zz + D(0x3fcebfbdff82c4ce);       // 1405723e8
    zz = zz * w;
    zz = zz + D(0x3fe62e42fefa39ef);       // 1405723e0
    zz = zz * w;
    zz = zz * D(kPowA1[idx]);
    zz = zz + D(kPowA1[idx]);
    // add mp to the exponent field (1404b382e)
    const std::uint32_t h = hi32(zz);
    const std::uint32_t nh =
        ((((h & 0xfff00000u) + (static_cast<std::uint32_t>(mp) << 20)) ^ h) & 0x7ff00000u) ^ h;
    return with_hi(zz, nh);
}

// ---- MSVC CRT sin/cos (AMD LibM) ----

namespace {

// 1404a0934 via 1404a0abc / 1404a0ae4: the value _handle_error returns for an infinite or NaN
// argument (errno and _matherr side effects are not reproduced)
double crt_special(double x)
{
    std::uint64_t b = bits(x);
    if ((b & 0x000fffffffffffffull) == 0) {
        return D(0xfff8000000000000ull);
    }
    if ((b & 0x0008000000000000ull) != 0x0008000000000000ull) {
        b |= 0x0008000000000000ull;
    }
    return D(b);
}

constexpr double kC80 = D(0x3fe0000000000000); // 14056dc80 / 14056de00: 0.5
constexpr double kC90 = D(0x3ff0000000000000); // 14056dc90 / 14056ddf0: 1.0
constexpr double kSix = D(0x3fc5555555555555); // 14056dca0 / 14056de10: 1/6

// argument reduction shared by sin and cos (140bb315d / 140bb35fa); returns false when the
// small-|r| shortcut has produced the result in *out
struct Reduced {
    double r;
    double rr;
    std::int32_t region;
    std::uint64_t rexp; // RCX: exponent of the first r
};

Reduced reduce_small(double x, std::uint64_t ax)
{
    const double xmm2 = x * D(0x3fe45f306dc9c883) + kC80; // 14056dcb0 (2/pi)
    const std::int32_t npi2 = _mm_cvtsi128_si32(_mm_cvttpd_epi32(_mm_set_sd(xmm2)));
    const double dn = static_cast<double>(npi2);
    double rhead = x - D(0x3ff921fb54400000) * dn;   // 14056dce0
    double rtail = D(0x3dd0b4611a626331) * dn;       // 14056dcf0
    double r = rhead - rtail;
    const std::uint64_t rexp = (bits(r) << 1) >> 53;
    const std::int64_t expdiff = static_cast<std::int64_t>((ax >> 52) - rexp);
    if (expdiff > 15) {
        const double t = rhead;
        const double rtail2 = D(0x3dd0b4611a600000) * dn; // 14056dd00
        rhead = t - rtail2;
        rtail = D(0x3ba3198a2e037073) * dn - ((t - rhead) - rtail2); // 14056dd10
        r = rhead - rtail;
    }
    const double rr = (rhead - r) - rtail;
    return {r, rr, npi2, rexp};
}

double sin_poly(double r, double rr, double x2, bool with_rr)
{
    // 140bb32f5 (sin) / 140bb3795 (cos, odd region)
    double s3 = D(0x3de5e0b2f9a43bb8) * x2; // 14056ddd0
    double s5 = D(0xbf2a01a019e83e5c) * x2; // 14056dda0
    double x6 = x2 * x2;
    double x3 = r;
    s3 = s3 + D(0xbe5ae600b42fdfa7);        // 14056ddc0
    x6 = x6 * x2;
    s3 = s3 * x2;
    x3 = x3 * x2;
    s5 = s5 + D(0x3f81111111110bb3);        // 14056dd90
    s5 = s5 * x2;
    s3 = s3 + D(0x3ec71de3796cde01);        // 14056ddb0
    if (!with_rr) {
        s3 = s3 * x6;
        s5 = s5 + D(0xbfc5555555555555);    // 14056dd80
        s3 = s3 + s5;
        x3 = x3 * s3;
        return r + x3;
    }
    const double hx2 = x2 * kC80;
    s3 = s3 * x6;
    s5 = s5 + D(0xbfc5555555555555);
    const double c = hx2 * rr;
    s3 = s3 + s5;
    x3 = x3 * s3;
    x3 = x3 - c;
    double out = rr + x3;
    return out + r;
}

double cos_poly(double r, double rr, double x2)
{
    // 140bb3390 (sin, odd region) / 140bb3830 (cos)
    const double rrr = rr * r;
    double hx2 = kC80 * x2;
    double c2 = D(0xbda907db47258aa7) * x2; // 14056dd70
    double c1 = D(0x3efa01a019f4ec91) * x2; // 14056dd40
    double x6 = x2;
    hx2 = hx2 - kC90;
    x6 = x6 * x2;
    c2 = c2 + D(0x3e21eeb690382eec);        // 14056dd60
    c1 = c1 + D(0xbf56c16c16c16967);        // 14056dd30
    hx2 = hx2 + kC90;
    x6 = x6 * x2;
    c2 = c2 * x2;
    c1 = c1 * x2;
    double r2 = x2 * kC80;
    c2 = c2 + D(0xbe927e4fa17f667b);        // 14056dd50
    c1 = c1 + D(0x3fa5555555555555);        // 14056dd20
    const double x4 = x2 * x2;
    hx2 = hx2 - r2;
    c2 = c2 * x6;
    c1 = c1 + c2;
    r2 = r2 - kC90;
    hx2 = hx2 - rrr;
    c1 = c1 * x4;
    c1 = c1 + hx2;
    return c1 - r2;
}

} // namespace

// @src 1404a0430  __remainder_piby2 (AMD LibM): reduction of a large argument by pi/2
void crt_remainder_piby2(double x, double* r, double* rr, std::int32_t* region)
{
    const std::uint64_t xb = bits(x);
    const std::uint64_t ux = (xb & 0x000fffffffffffffull) | 0x0010000000000000ull;
    std::int32_t xexp = static_cast<std::int32_t>((xb >> 52) & 0x7ff) - 0x3ff;
    const std::int32_t q = xexp / 10;
    std::int32_t resexp = xexp - q * 10;
    const std::int32_t first = q + 18;
    std::uint64_t res[19];
    // res[18] .. res[0]: 10-bit digits of ux * (2/pi), most significant last computed
    std::uint64_t c = kPiBits[first] * ux;
    res[18] = c & 0x3ff;
    for (int j = 1; j <= 18; ++j) {
        c = kPiBits[first - j] * ux + (c >> 10);
        res[18 - j] = c & 0x3ff;
    }
    std::int32_t i = 1;
    const std::uint64_t top = ((res[0] << 10) | res[1]) >> (9 - resexp);
    const std::uint32_t r3 = static_cast<std::uint32_t>(top) & 7;
    const std::uint32_t odd = r3 & 1;
    std::uint64_t mant;
    std::uint64_t nextbits;
    const std::uint64_t mask = (std::uint64_t{1} << (10 - resexp)) - 1;
    if (odd) {
        *region = static_cast<std::int32_t>(((r3 >> 1) + 1) & 3);
        mant = mask & ~res[1];
        while (mant < 0x20000000000000ull) {
            ++i;
            mant = (mant << 10) | (~res[i] & 0x3ff);
        }
        nextbits = ~res[i + 1];
    } else {
        *region = static_cast<std::int32_t>(r3 >> 1);
        mant = mask & res[1];
        while (mant < 0x20000000000000ull) {
            ++i;
            mant = (mant << 10) | res[i];
        }
        nextbits = res[i + 1];
    }
    nextbits <<= 54;
    resexp = resexp - i * 10 + 52;
    while (mant >= 0x20000000000000ull) {
        ++resexp;
        nextbits = (mant << 63) | (nextbits >> 1);
        mant >>= 1;
    }
    std::uint64_t rb = (static_cast<std::uint64_t>(static_cast<std::int64_t>(resexp) + 0x3ff) << 52) |
                       (mant & 0x000fffffffffffffull);
    if (odd) {
        rb |= 0x8000000000000000ull;
    }
    const double r1 = D(rb);
    const double sc = D(static_cast<std::uint64_t>(static_cast<std::int64_t>(resexp) + 0x3ca) << 52);
    double r2 = D((nextbits >> 12) | bits(sc));
    r2 = r2 + (0.0 - sc);
    r2 = r2 * (odd ? D(0xc000000000000000) : kTwo); // 140562a88 / 140509fb8
    constexpr double piby2 = D(0x3ff921fb54442d18);      // 1405708e8
    constexpr double piby2_1 = D(0x3ff921fb50000000);    // 1405708f0
    constexpr double piby2_1tail = D(0x3e5110b460000000); // 1405708f8
    constexpr double piby2_2tail = D(0x3c91a62633145c06); // 140570900
    r2 = r2 * piby2;
    const double z = r1 * piby2;
    double err = r1 * piby2_2tail;
    err = err + r2;
    const double r1_hi = D(bits(r1) & 0xfffffffff8000000ull);
    const double r1_lo = r1 - r1_hi;
    double e5 = r1_hi * piby2_1;
    e5 = e5 - z;
    e5 = e5 + r1_lo * piby2_1;
    e5 = e5 + r1_hi * piby2_1tail;
    e5 = e5 + r1_lo * piby2_1tail;
    e5 = e5 + err;
    const double rs = e5 + z;
    *r = rs;
    *rr = (z - rs) + e5;
}

// @src 140bb3000  sin (MSVC CRT, AMD LibM)
double crt_sin(double x)
{
    const std::uint64_t ux = bits(x);
    if ((ux & 0x7ff0000000000000ull) == 0x7ff0000000000000ull) {
        return crt_special(x); // 1404a0abc
    }
    const std::uint64_t ax = ux & 0x7fffffffffffffffull;
    if (static_cast<std::int64_t>(ax) <= 0x3fe921fb54442d18ll) {
        if (static_cast<std::int64_t>(ax) < 0x3f20000000000000ll) {
            if (static_cast<std::int64_t>(ax) < 0x3e40000000000000ll) {
                return x;
            }
            double t = x * x;
            t = t * x;
            t = t * kSix;
            return x - t;
        }
        const double x2 = x * x;
        return sin_poly(x, 0.0, x2, false);
    }
    std::uint32_t neg = 0;
    if (ux != ax) {
        neg = 1;
        x = 0.0 - x;
    }
    double r;
    double rr;
    std::int32_t region;
    bool done = false;
    if (ax < 0x411e848000000000ull) { // 14056dcc0 (5e5) compared as an integer
        const Reduced red = reduce_small(x, ax);
        r = red.r;
        rr = red.rr;
        region = red.region;
        if (red.rexp < 0x3f2) {
            done = true;
            if (red.rexp > 0x3de) {
                const double x2 = r * r;
                if ((region & 1) == 0) {
                    double t = kSix * r;
                    t = t * x2;
                    r = r - t;
                } else {
                    r = kC90 - x2 * kC80;
                }
            } else if ((region & 1) != 0) {
                r = kC90;
            }
        }
    } else {
        crt_remainder_piby2(x, &r, &rr, &region);
    }
    if (!done) {
        const double x2 = r * r;
        if ((region & 1) == 0) {
            r = sin_poly(r, rr, x2, true);
        } else {
            r = cos_poly(r, rr, x2);
        }
    }
    const std::uint32_t h = static_cast<std::uint32_t>(region) >> 1;
    if ((((h & neg) | (~h & ~neg)) & 1) == 0) {
        return 0.0 - r;
    }
    return r;
}

// @src 140bb3490  cos (MSVC CRT, AMD LibM)
double crt_cos(double x)
{
    const std::uint64_t ux = bits(x);
    if ((ux & 0x7ff0000000000000ull) == 0x7ff0000000000000ull) {
        return crt_special(x); // 1404a0ae4
    }
    const std::uint64_t ax = ux & 0x7fffffffffffffffull;
    if (static_cast<std::int64_t>(ax) <= 0x3fe921fb54442d18ll) {
        if (static_cast<std::int64_t>(ax) < 0x3f20000000000000ll) {
            if (static_cast<std::int64_t>(ax) < 0x3e40000000000000ll) {
                return kC90;
            }
            double t = x * x;
            t = t * kC80;
            return kC90 - t;
        }
        // 140bb3540
        const double x2 = x * x;
        double a = D(0xbf56c16c16c16967) * x2;  // 14056deb0
        double x4 = x2 * x2;
        double b = D(0xbe927e4fa17f667b) * x2;  // 14056ded0
        double c = D(0xbda907db47258aa7) * x2;  // 14056def0
        const double x8 = x4 * x4;
        a = a + D(0x3fa5555555555555);          // 14056dea0
        a = a * x4;
        b = b + D(0x3efa01a019f4ec91);          // 14056dec0
        const double rh = x2 * D(0xbfe0000000000000); // 14056de90 (-0.5)
        c = c + D(0x3e21eeb690382eec);          // 14056dee0
        b = b * x8;
        x4 = x4 * x8;
        x4 = x4 * c;
        a = a + b;
        const double t = rh + kC90;
        double out = kC90 - t;
        out = out + rh;
        a = a + x4;
        out = out + a;
        return out + t;
    }
    if (ux != ax) {
        x = 0.0 - x;
    }
    double r;
    double rr;
    std::int32_t region;
    bool done = false;
    if (ax < 0x411e848000000000ull) { // 14056de80
        const Reduced red = reduce_small(x, ax);
        r = red.r;
        rr = red.rr;
        region = red.region;
        if (red.rexp < 0x3f2) {
            done = true;
            if (red.rexp > 0x3de) {
                const double x2 = r * r;
                if ((region & 1) != 0) {
                    double t = kSix * r;
                    t = t * x2;
                    r = r - t;
                } else {
                    r = kC90 - x2 * kC80;
                }
            } else if ((region & 1) == 0) {
                r = kC90;
            }
        }
    } else {
        crt_remainder_piby2(x, &r, &rr, &region);
    }
    if (!done) {
        const double x2 = r * r;
        if ((region & 1) != 0) {
            r = sin_poly(r, rr, x2, true);
        } else {
            r = cos_poly(r, rr, x2);
        }
    }
    if (((region + 1) & 2) != 0) {
        return 0.0 - r;
    }
    return r;
}

} // namespace def::rt::libm
