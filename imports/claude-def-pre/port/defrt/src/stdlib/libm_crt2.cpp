// MSVC CRT atan / atan2 / pow / acos (AMD LibM derived, VS2008 x64), transcribed from the listing of
// DEF_PRE_64.exe instruction by instruction, keeping the SSE2 operation order and the
// COMISD/UCOMISD comparison senses. Companion to libm.cpp (crt_sin/crt_cos live there); the x87
// core of pow is in libm_crt2_x87.asm. See defrt/stdlib/libm.hpp.
//
// errno / _matherr / FP-exception side effects are NOT reproduced (same choice as crt_sin/crt_cos
// in libm.cpp): the original routes overflow/underflow/domain cases through _controlfp_s and
// _handle_error (1404a02e0) only to raise FP exceptions and set errno; the DOUBLE value they hand
// back (the quiet NaN, +/-Inf, +/-0 or the computed result) is reproduced exactly, and that value
// is all a caller of these functions observes here. The x87 pow result, like the original, depends
// on the host CPU's FYL2X / F2XM1 / FSCALE microcode.
#include <defrt/stdlib/libm.hpp>

#include <bit>
#include <cmath>
#include <cstdint>

extern "C" double def_rt_libm_crt_pow_x87(double base, double y); // libm_crt2_x87.asm

namespace def::rt::libm {

namespace {

constexpr double D(std::uint64_t b) { return std::bit_cast<double>(b); }
std::uint64_t bits(double x) { return std::bit_cast<std::uint64_t>(x); }

constexpr std::uint64_t kSignBit = 0x8000000000000000ull;    // 14051bec0 (xorpd -0.0)
constexpr std::uint64_t kAbsMask = 0x7fffffffffffffffull;
constexpr std::uint64_t kInfBits = 0x7ff0000000000000ull;
constexpr std::uint64_t kMantMask = 0x000fffffffffffffull;
constexpr std::uint64_t kQuietBit = 0x0008000000000000ull;   // mantissa msb (quiet a NaN)

// @data 14056e000  atan2: table base atan values (high), 241 entries (index idx-0x10)
constexpr std::uint64_t kAt2Hi[241] = {
    0x3faff55b00000000ull, 0x3fb0f99e00000000ull, 0x3fb1f86d00000000ull, 0x3fb2f71900000000ull,
    0x3fb3f59f00000000ull, 0x3fb4f3fd00000000ull, 0x3fb5f23200000000ull, 0x3fb6f03b00000000ull,
    0x3fb7ee1800000000ull, 0x3fb8ebc500000000ull, 0x3fb9e94100000000ull, 0x3fbae68a00000000ull,
    0x3fbbe39e00000000ull, 0x3fbce07c00000000ull, 0x3fbddd2100000000ull, 0x3fbed98c00000000ull,
    0x3fbfd5ba00000000ull, 0x3fc068d500000000ull, 0x3fc0e6ad00000000ull, 0x3fc1646500000000ull,
    0x3fc1e1fa00000000ull, 0x3fc25f6e00000000ull, 0x3fc2dcbd00000000ull, 0x3fc359e800000000ull,
    0x3fc3d6ee00000000ull, 0x3fc453ce00000000ull, 0x3fc4d08700000000ull, 0x3fc54d1800000000ull,
    0x3fc5c98100000000ull, 0x3fc645bf00000000ull, 0x3fc6c1d400000000ull, 0x3fc73dbd00000000ull,
    0x3fc7b97b00000000ull, 0x3fc8350b00000000ull, 0x3fc8b06e00000000ull, 0x3fc92ba300000000ull,
    0x3fc9a6a800000000ull, 0x3fca217e00000000ull, 0x3fca9c2300000000ull, 0x3fcb169600000000ull,
    0x3fcb90d700000000ull, 0x3fcc0ae500000000ull, 0x3fcc84bf00000000ull, 0x3fccfe6500000000ull,
    0x3fcd77d500000000ull, 0x3fcdf11000000000ull, 0x3fce6a1400000000ull, 0x3fcee2e100000000ull,
    0x3fcf5b7500000000ull, 0x3fcfd3d100000000ull, 0x3fd025fa00000000ull, 0x3fd061ee00000000ull,
    0x3fd09dc500000000ull, 0x3fd0d97e00000000ull, 0x3fd1151a00000000ull, 0x3fd1509700000000ull,
    0x3fd18bf500000000ull, 0x3fd1c73500000000ull, 0x3fd2025500000000ull, 0x3fd23d5600000000ull,
    0x3fd2783700000000ull, 0x3fd2b2f700000000ull, 0x3fd2ed9800000000ull, 0x3fd3281800000000ull,
    0x3fd3627700000000ull, 0x3fd39cb400000000ull, 0x3fd3d6d100000000ull, 0x3fd410cb00000000ull,
    0x3fd44aa400000000ull, 0x3fd4845a00000000ull, 0x3fd4bdee00000000ull, 0x3fd4f75f00000000ull,
    0x3fd530ad00000000ull, 0x3fd569d800000000ull, 0x3fd5a2e000000000ull, 0x3fd5dbc300000000ull,
    0x3fd6148400000000ull, 0x3fd64d1f00000000ull, 0x3fd6859700000000ull, 0x3fd6bdea00000000ull,
    0x3fd6f61900000000ull, 0x3fd72e2200000000ull, 0x3fd7660700000000ull, 0x3fd79dc600000000ull,
    0x3fd7d56000000000ull, 0x3fd80cd400000000ull, 0x3fd8442200000000ull, 0x3fd87b4b00000000ull,
    0x3fd8b24d00000000ull, 0x3fd8e92900000000ull, 0x3fd91fde00000000ull, 0x3fd9566d00000000ull,
    0x3fd98cd500000000ull, 0x3fd9c31600000000ull, 0x3fd9f93000000000ull, 0x3fda2f2300000000ull,
    0x3fda64ee00000000ull, 0x3fda9a9200000000ull, 0x3fdad00f00000000ull, 0x3fdb056400000000ull,
    0x3fdb3a9100000000ull, 0x3fdb6f9600000000ull, 0x3fdba47300000000ull, 0x3fdbd92800000000ull,
    0x3fdc0db400000000ull, 0x3fdc421900000000ull, 0x3fdc765500000000ull, 0x3fdcaa6800000000ull,
    0x3fdcde5300000000ull, 0x3fdd121500000000ull, 0x3fdd45ae00000000ull, 0x3fdd791f00000000ull,
    0x3fddac6700000000ull, 0x3fdddf8500000000ull, 0x3fde127b00000000ull, 0x3fde454800000000ull,
    0x3fde77eb00000000ull, 0x3fdeaa6500000000ull, 0x3fdedcb600000000ull, 0x3fdf0ede00000000ull,
    0x3fdf40dd00000000ull, 0x3fdf72b200000000ull, 0x3fdfa45d00000000ull, 0x3fdfd5e000000000ull,
    0x3fe0039c00000000ull, 0x3fe01c3400000000ull, 0x3fe034b700000000ull, 0x3fe04d2500000000ull,
    0x3fe0657e00000000ull, 0x3fe07dc300000000ull, 0x3fe095f300000000ull, 0x3fe0ae0e00000000ull,
    0x3fe0c61400000000ull, 0x3fe0de0500000000ull, 0x3fe0f5e200000000ull, 0x3fe10daa00000000ull,
    0x3fe1255d00000000ull, 0x3fe13cfb00000000ull, 0x3fe1548500000000ull, 0x3fe16bfa00000000ull,
    0x3fe1835a00000000ull, 0x3fe19aa500000000ull, 0x3fe1b1dc00000000ull, 0x3fe1c8fe00000000ull,
    0x3fe1e00b00000000ull, 0x3fe1f70400000000ull, 0x3fe20de800000000ull, 0x3fe224b700000000ull,
    0x3fe23b7100000000ull, 0x3fe2521700000000ull, 0x3fe268a900000000ull, 0x3fe27f2600000000ull,
    0x3fe2958e00000000ull, 0x3fe2abe200000000ull, 0x3fe2c22100000000ull, 0x3fe2d84c00000000ull,
    0x3fe2ee6200000000ull, 0x3fe3046400000000ull, 0x3fe31a5200000000ull, 0x3fe3302b00000000ull,
    0x3fe345f000000000ull, 0x3fe35ba000000000ull, 0x3fe3713d00000000ull, 0x3fe386c500000000ull,
    0x3fe39c3900000000ull, 0x3fe3b19800000000ull, 0x3fe3c6e400000000ull, 0x3fe3dc1c00000000ull,
    0x3fe3f13f00000000ull, 0x3fe4064f00000000ull, 0x3fe41b4a00000000ull, 0x3fe4303200000000ull,
    0x3fe4450600000000ull, 0x3fe459c600000000ull, 0x3fe46e7200000000ull, 0x3fe4830a00000000ull,
    0x3fe4978f00000000ull, 0x3fe4ac0000000000ull, 0x3fe4c05e00000000ull, 0x3fe4d4a800000000ull,
    0x3fe4e8de00000000ull, 0x3fe4fd0100000000ull, 0x3fe5111000000000ull, 0x3fe5250c00000000ull,
    0x3fe538f500000000ull, 0x3fe54cca00000000ull, 0x3fe5608d00000000ull, 0x3fe5743c00000000ull,
    0x3fe587d800000000ull, 0x3fe59b6000000000ull, 0x3fe5aed600000000ull, 0x3fe5c23900000000ull,
    0x3fe5d58900000000ull, 0x3fe5e8c600000000ull, 0x3fe5fbf000000000ull, 0x3fe60f0800000000ull,
    0x3fe6220d00000000ull, 0x3fe634ff00000000ull, 0x3fe647de00000000ull, 0x3fe65aab00000000ull,
    0x3fe66d6600000000ull, 0x3fe6800e00000000ull, 0x3fe692a400000000ull, 0x3fe6a52700000000ull,
    0x3fe6b79800000000ull, 0x3fe6c9f700000000ull, 0x3fe6dc4400000000ull, 0x3fe6ee7f00000000ull,
    0x3fe700a700000000ull, 0x3fe712be00000000ull, 0x3fe724c300000000ull, 0x3fe736b600000000ull,
    0x3fe7489700000000ull, 0x3fe75a6700000000ull, 0x3fe76c2400000000ull, 0x3fe77dd100000000ull,
    0x3fe78f6b00000000ull, 0x3fe7a0f400000000ull, 0x3fe7b26c00000000ull, 0x3fe7c3d300000000ull,
    0x3fe7d52800000000ull, 0x3fe7e66c00000000ull, 0x3fe7f79e00000000ull, 0x3fe808c000000000ull,
    0x3fe819d000000000ull, 0x3fe82ad000000000ull, 0x3fe83bbe00000000ull, 0x3fe84c9c00000000ull,
    0x3fe85d6900000000ull, 0x3fe86e2500000000ull, 0x3fe87ed000000000ull, 0x3fe88f6b00000000ull,
    0x3fe89ff500000000ull, 0x3fe8b06f00000000ull, 0x3fe8c0d900000000ull, 0x3fe8d13200000000ull,
    0x3fe8e17a00000000ull, 0x3fe8f1b300000000ull, 0x3fe901db00000000ull, 0x3fe911f300000000ull,
    0x3fe921fb00000000ull,
};
// @data 14056e790  atan2: table base atan values (low correction), 241 entries
constexpr std::uint64_t kAt2Lo[241] = {
    0x3e56e59fbd38db2cull, 0x3e64e3aa54dedf96ull, 0x3e67e105ab1bda88ull, 0x3e48c5254d013fd0ull,
    0x3e2cf8ab3ad62670ull, 0x3e59dca4bec80468ull, 0x3e53f4b5ec98a8daull, 0x3e6b9d49619d81feull,
    0x3e43017887460934ull, 0x3e511e3eca0b9944ull, 0x3e54f3f73c5a332eull, 0x3e5c71c8ae0e00a6ull,
    0x3e67cde0f86fbdc7ull, 0x3e570f328c889c72ull, 0x3e5c07ae9b994efeull, 0x3e40c8021d7b1698ull,
    0x3e635585edb8cb22ull, 0x3e70842567b30e96ull, 0x3e799e811031472eull, 0x3e6041821416bceeull,
    0x3e7f6086e4dc96f4ull, 0x3e471a535c5f1b58ull, 0x3e765f743fe63ca1ull, 0x3e7dbd733472d014ull,
    0x3e7d18cc4d8b0d1dull, 0x3e78c12553c8fb29ull, 0x3e753b49e2e8f991ull, 0x3e77422ae148c141ull,
    0x3e4e3ec269df56a8ull, 0x3e7ff6754e7e0ac9ull, 0x3e7131267b1b5aadull, 0x3e7d14fa403a94bcull,
    0x3e62f396c089a3d8ull, 0x3e7c731d78fa95bbull, 0x3e7c50f385177399ull, 0x3e6f41409c6f2c20ull,
    0x3e7d2d90c4c39ec0ull, 0x3e680420696f2106ull, 0x3e4b40327943a2e8ull, 0x3e65d35e02f3d2a2ull,
    0x3e64a498288117b0ull, 0x3e635da119afb324ull, 0x3e714e85cdb9a908ull, 0x3e638754e5547b9aull,
    0x3e7be40ae6ce3246ull, 0x3e70c993b3bea7e7ull, 0x3e71d2dd89ac3359ull, 0x3e61476603332c46ull,
    0x3e7f25901bac55b7ull, 0x3e7f881b7c826e28ull, 0x3e7441996d698d20ull, 0x3e8407ac521ea089ull,
    0x3e82fb0c6c4b1723ull, 0x3e8ca135966a3e18ull, 0x3e6b1218e4d646e4ull, 0x3e6d4e72a350d288ull,
    0x3e84617e2f04c329ull, 0x3e6096ec41e82650ull, 0x3e79f91f25773e6eull, 0x3e659c0820f1d674ull,
    0x3e602bf7a2df1064ull, 0x3e8fb36bfc40508full, 0x3e7ea08f3f8dc892ull, 0x3e73ed6254656a0eull,
    0x3e6b83f5e5e69c58ull, 0x3e8d6ec2af768592ull, 0x3e6493889a226f94ull, 0x3e85ad8fa65279baull,
    0x3e6b615784d45434ull, 0x3e809a184368f145ull, 0x3e761a2439b0d91cull, 0x3e7ce1a65e39a978ull,
    0x3e832a39a93b6a66ull, 0x3e81c3699af804e7ull, 0x3e575e0f4e44ede8ull, 0x3e8f77ced1a7a83bull,
    0x3e284e7f0cb1b500ull, 0x3e8ec6b838b02dfeull, 0x3e83ebf4dfbeda87ull, 0x3e89397aed9cb475ull,
    0x3e707937bc239c54ull, 0x3e8aa754553131b6ull, 0x3e74a05d407c45dcull, 0x3e8132231a206dd0ull,
    0x3e72d8ecfdd69c88ull, 0x3e7a852c74218606ull, 0x3e871bf2baeebb50ull, 0x3e483d7db7491820ull,
    0x3e6ca50d92b6da14ull, 0x3e56f5cde8530298ull, 0x3e7f343198910740ull, 0x3e70e8d241ccd80aull,
    0x3e71535ac619e6c8ull, 0x3e77316041c36cd2ull, 0x3e7985a000637d8eull, 0x3e6f2f29858c0a68ull,
    0x3e8879847f96d909ull, 0x3e8ab3d319e12e42ull, 0x3e75088162dfc4c2ull, 0x3e605749a1cd9d8cull,
    0x3e5da65c6c6b8618ull, 0x3e6739bf7df1ad64ull, 0x3e6bc31252aa3340ull, 0x3e5e528191ad3aa8ull,
    0x3e8929d93df19f18ull, 0x3e5ff11eb693a080ull, 0x3e455ae3f145a3a0ull, 0x3e7cbcd8c6c0ca82ull,
    0x3e70cb04d425d304ull, 0x3e79adfcab5be678ull, 0x3e893d90c5662508ull, 0x3e768489bd35ff40ull,
    0x3e3586ed3da2b7e0ull, 0x3e87604d2e850eeeull, 0x3e7ac1d12bfb53d8ull, 0x3e39b3d468274740ull,
    0x3e7fc5d68d10e53cull, 0x3e88f9e51884becbull, 0x3e8a87f0869c06d1ull, 0x3e831e7279f685faull,
    0x3e46a8282f9719b0ull, 0x3e60d2724a8a44e0ull, 0x3e8a60524b11ad4eull, 0x3e575fdf832750f0ull,
    0x3e8cf06902e4cd36ull, 0x3e6e82422d4f6d10ull, 0x3e524a091063e6c0ull, 0x3e78a1a172dc6f38ull,
    0x3e929b6619f8a92dull, 0x3e79274d9c1b70c8ull, 0x3e50c34b1fbb7930ull, 0x3e6639866c20eb50ull,
    0x3e86d6d0f6832e9eull, 0x3e9af54def99f25eull, 0x3e916cfc52a00262ull, 0x3e8dcc1e83569c32ull,
    0x3e937f7a551ed425ull, 0x3e9f6360adc98887ull, 0x3e92c6ec8d35a2c1ull, 0x3e8bd44df84cb036ull,
    0x3e9117cf826e310eull, 0x3e9ca533f332cfc9ull, 0x3e90f208509dbc2eull, 0x3e8cd07d93c945deull,
    0x3e957bdfd67e6d72ull, 0x3e7aab89c516c658ull, 0x3e63e823b1a1b8a0ull, 0x3e8307464a9d6d3cull,
    0x3e9c5993cd438843ull, 0x3e9ba2fca02ab554ull, 0x3e801a5b6983a268ull, 0x3e6273d1b350efc8ull,
    0x3e864c238c37b0c6ull, 0x3e6aded07370a300ull, 0x3e878091197eb47eull, 0x3e74b0f245e0dabcull,
    0x3e9080d9794e2eafull, 0x3e8d4ec242b60c76ull, 0x3e4221d2f940caa0ull, 0x3e7cdbc42b2bba5cull,
    0x3e6cce37bb440840ull, 0x3e96c1d999cf1dd0ull, 0x3e5bed8a07eb0870ull, 0x3e769ed88f490e3cull,
    0x3e6cd41719b73ef0ull, 0x3e9cbc4ac95b41b7ull, 0x3e9238f1b890f5d7ull, 0x3e750c4282259cc4ull,
    0x3e9713d2de87b3e2ull, 0x3e81d5a7d2255276ull, 0x3e9c0dfd48227ac1ull, 0x3e91c964dab76753ull,
    0x3e86de56d5704496ull, 0x3e84aeb71fd19968ull, 0x3e8fbf91c57b1918ull, 0x3e9d6bef7fbe5d9aull,
    0x3e9464d3dc249066ull, 0x3e9638e2ec4d9073ull, 0x3e716f4a7247ea7cull, 0x3e31a0a740f1d440ull,
    0x3e86edbb0114a33cull, 0x3e7dbee8bf1d513cull, 0x3e95b8bdb0248f73ull, 0x3e97de3d3f5eac64ull,
    0x3e8ee24187ae448aull, 0x3e9e06c591ec5192ull, 0x3e74e3861a332738ull, 0x3e7a9599dcc2bfe4ull,
    0x3e6f732fbad43468ull, 0x3e9eb9f573b727d9ull, 0x3e98b212a2eb9897ull, 0x3e9384884c167215ull,
    0x3e90e2d363020051ull, 0x3e92820879fbd022ull, 0x3e9a1ab9893e4b30ull, 0x3e82d1b817a24478ull,
    0x3e615d7b8ded4878ull, 0x3e78968f9db3a5e4ull, 0x3e971c4171fe135full, 0x3e96d80f605d0d8cull,
    0x3e7c91f043691590ull, 0x3e839f8a15fce2b2ull, 0x3e455beda9d94b80ull, 0x3e8b12c15d60949aull,
    0x3e924167b312bfe3ull, 0x3e90ab8633070277ull, 0x3e854554ebbc80eeull, 0x3e60204aef5a4bb8ull,
    0x3e98af08c679cf2cull, 0x3e90852a330ae6c8ull, 0x3e86d3eb9ec32916ull, 0x3e8685cb7fcbbafeull,
    0x3e91f751c1e0bd95ull, 0x3e5705b1b0f72560ull, 0x3e9b98d8d808ca92ull, 0x3e62ea22c75cc980ull,
    0x3e97aba62bca0350ull, 0x3e9d73833442278cull, 0x3e95a5ca1fb18bf9ull, 0x3e61a6092b6ecf28ull,
    0x3e744fd049aac104ull, 0x3e2c114fd8df5180ull, 0x3e95972f130feae5ull, 0x3e7ca034a55fe198ull,
    0x3e96e2b149990227ull, 0x3e7b00000294592cull, 0x3e98b9bdc442620eull, 0x3e8d94fdfabf3e4eull,
    0x3e85db30b145ad9aull, 0x3e8e3e1eb95022b0ull, 0x3e9d5b8b45442bd6ull, 0x3e97a046231ecd2eull,
    0x3e9feafe3ef55232ull, 0x3e9839e7bfd78267ull, 0x3e645cf49d6fa900ull, 0x3e4be3132b27f380ull,
    0x3e9533980bb84f9full, 0x3e5889e2ce3ba390ull, 0x3e7f7778c3ad0cc8ull, 0x3e846660cec4eba2ull,
    0x3e85110b4611a626ull,
};
} // namespace

// @src 1404865ac  atan (MSVC CRT, AMD LibM): branch-reduced minimax rational on [0,2.4375]
double crt_atan(double x)
{
    const std::uint64_t ux = bits(x);
    const std::uint64_t ax = ux & kAbsMask;
    const bool neg = (ux != ax);                       // SETNZ on (R8 != RAX)
    double absx = x;
    if (neg) absx = D(bits(absx) ^ kSignBit);          // |x|

    double hi, lo, r;
    if (ax > 0x4003800000000000ull) {                  // |x| > 2.4375
        if (ax > kInfBits) {                           // NaN: quiet it
            return D(ux | kQuietBit);
        }
        if (absx <= D(0x43d0dc0000000000ull)) {        // |x| <= 4.859383997932765e18
            r = D(0xbff0000000000000ull) / absx;       // -1.0 / |x|
            hi = D(0x3ff921fb54442d18ull);             // pi/2
            lo = D(0x3c91a62633145c06ull);             // 6.123233995736765e-17
        } else {                                       // huge |x| (incl Inf): +/- pi/2
            double res = D(0x3ff921fb54442d18ull);
            if (neg) res = D(bits(res) ^ kSignBit);
            return res;
        }
    } else if (ax <= 0x3ff3000000000000ull) {          // |x| <= 1.1875
        if (ax <= 0x3fe6000000000000ull) {             // |x| <= 0.6875
            if (ax <= 0x3fdc000000000000ull) {         // |x| <= 0.4375
                hi = 0.0;
                lo = 0.0;
                r = absx;
            } else {                                   // reduce by 2.0: (2|x|-1)/(|x|+2)
                const double c = D(0x4000000000000000ull);
                hi = D(0x3fddac670561bb4full);          // atan(0.5)
                lo = D(0x3c7a2b7f222f65e0ull);
                double num = absx * c;
                num = num - D(0x3ff0000000000000ull);
                double den = absx + c;
                r = num / den;
            }
        } else {                                       // reduce by 1.0: (|x|-1)/(|x|+1)
            const double c = D(0x3ff0000000000000ull);
            hi = D(0x3fe921fb54442d18ull);              // pi/4
            lo = D(0x3c81a62633145c06ull);
            double num = absx - c;
            double den = absx + c;
            r = num / den;
        }
    } else {                                           // 1.1875 < |x| <= 2.4375: (|x|-1.5)/(1+1.5|x|)
        const double c = D(0x3ff8000000000000ull);      // 1.5
        hi = D(0x3fef730bd281f69bull);                   // atan(1.5)
        lo = D(0x3c7007887af0cbbcull);
        double den = absx * c;
        den = den + D(0x3ff0000000000000ull);
        double num = absx - c;
        r = num / den;
    }

    // 14048676c: num/den of a degree-4/4 rational in r^2, times r^3
    const double r2 = r * r;
    double num = r2 * D(0x3f22a75ce41b9f87ull);        // 14056efb8
    double den = r2 * D(0x3fa3f197f1e85ed9ull);        // 14056efb0
    const double r3 = r2 * r;
    num = num + D(0x3f9f2d2116f053f2ull);              // 14056efa8
    den = den + D(0x3fdb2cb05bf9beffull);              // 14056efa0
    num = num * r2;
    den = den * r2;
    num = num + D(0x3fcc3de43db425c0ull);              // 14056ef98
    den = den + D(0x3ff699c644c48d2eull);              // 14056ef90
    num = num * r2;
    den = den * r2;
    num = num + D(0x3fdca6be4c993b3cull);              // 14056ef88
    den = den + D(0x3ffd372a17cdf5a0ull);              // 14056ef80
    num = num * r2;
    den = den * r2;
    num = num + D(0x3fd12bcb0a9169f3ull);              // 14056ef78
    den = den + D(0x3fe9c1b08fda1eecull);              // 14056ef70
    num = num * r3;
    num = num / den;
    num = num - lo;
    num = num - r;
    double res = hi - num;
    if (neg) res = D(bits(res) ^ kSignBit);
    return res;
}

// atan2 big-ratio table core and small-ratio polynomial are reduction methods of the same
// AMD LibM atan; see the @src inline comments.
// @src 140485d6c  atan2(y, x) (MSVC CRT, AMD LibM): y in xmm0, x in xmm1
double crt_atan2(double y, double x)
{
    std::uint64_t ybits = bits(y);                     // R13
    std::uint64_t xbits = bits(x);                     // R12
    const std::uint64_t ay = ybits & kAbsMask;         // RCX
    const std::uint64_t ax = xbits & kAbsMask;         // RDX
    const std::uint32_t expX0 = static_cast<std::uint32_t>((xbits >> 52) & 0x7ff); // uVar12
    const std::uint32_t expY0 = static_cast<std::uint32_t>((ybits >> 52) & 0x7ff); // uVar3
    const std::uint64_t xsign = xbits & kSignBit;      // uVar11
    const std::uint64_t ysign = ybits & kSignBit;      // uVar9
    int iVar10 = static_cast<int>(expY0) - static_cast<int>(expX0);
    const double dy = D(ay);                           // |y| (ABS, original)
    const double dx = D(ax);                           // |x| (ABS, original)

    // either operand a NaN: hand back the quieted NaN
    if (ax > kInfBits || ay > kInfBits) {
        const std::uint64_t v = (ax > kInfBits) ? xbits : ybits;
        return D(v | kQuietBit);
    }

    constexpr std::uint64_t kPi    = 0x400921fb54442d18ull; // 14063a8e8
    constexpr std::uint64_t kPi_2  = 0x3ff921fb54442d18ull; // 14063a8f0 (= 63a918)
    constexpr std::uint64_t kPi_4  = 0x3fe921fb54442d18ull; // 14063a8f8
    constexpr std::uint64_t k3Pi_4 = 0x4002d97c7f3321d2ull; // 14063a900
    constexpr std::uint64_t kPiHi  = 0x400921fb50000000ull; // 14063a908
    constexpr std::uint64_t kPiLo  = 0x3e6110b4611a6263ull; // 14063a910
    constexpr std::uint64_t kPi_2Lo= 0x3c91a62633145c07ull; // 14063a920

    double result;                                     // dVar13

    if (dy == 0.0) {                                   // |y| == 0 (y is +/-0)
        if (xsign == 0) return y;                      // x >= 0 -> y
        result = D(kPi);                               // LAB_140485eb7: +/- pi
        if (ysign != 0) result = D(bits(result) ^ kSignBit);
        return result;
    }

    if (dx == 0.0) {                                   // |x| == 0, y != 0 -> +/- pi/2
        result = D(kPi_2);
        if (ysign != 0) {
            return D(bits(result) ^ kSignBit);         // -pi/2
        }
        // y >= 0: fall through into the main block (LAB_140485f2f)
    }

    // LAB_140485f2f ------------------------------------------------------------------
    double base = 0.0;                                 // dVar4 (uVar17:18): atan table high part
    double dVar8 = 0.0;
    if (expX0 < 0x3fd && expY0 < 0x3fd) {              // both tiny: scale x and y up
        if ((xbits & kInfBits) == 0) {                 // x subnormal
            const double t = (xsign == 0) ? D(0xc010000000000000ull) : D(0x4010000000000000ull); // -+4.0
            x = D(xbits | 0x4010000000000000ull) + t;
        } else {
            x = D(static_cast<std::uint64_t>(static_cast<std::int64_t>(xbits) + 0x4000000000000000ll)); // *4
        }
        if ((ybits & kInfBits) == 0) {                 // y subnormal
            const double t = (ysign == 0) ? D(0xc010000000000000ull) : D(0x4010000000000000ull);
            y = D(ybits | 0x4010000000000000ull) + t;
        } else {
            y = D(static_cast<std::uint64_t>(static_cast<std::int64_t>(ybits) + 0x4000000000000000ll));
        }
        xbits = bits(x);
        ybits = bits(y);
        iVar10 = static_cast<int>((ybits >> 52) & 0x7ff) - static_cast<int>((xbits >> 52) & 0x7ff);
    }

    result = D(kPi_2);                                 // default (iVar10 >= 0x39): pi/2
    if (iVar10 < 0x39) {
        if (iVar10 < -0x1c && xsign == 0) {            // |y| << |x|, x >= 0
            if (iVar10 < -0x432) {                     // total underflow
                if (ysign != 0) return D(0x8000000000000000ull); // -0.0 (14056ef60)
                return dVar8;                          // +0.0
            }
            if (-0x3ff < iVar10) {
                return D(ybits) / D(xbits);            // plain ratio
            }
            // -0x432 <= iVar10 <= -0x3ff: scale up, divide, denormal-round
            double q = (D(ybits) * D(0x4630000000000000ull)) / D(xbits); // *2^100
            std::uint64_t qb = bits(q);
            int e = static_cast<int>((static_cast<std::uint32_t>(qb >> 32) & 0x7fffffff) >> 20) - 100;
            std::uint64_t uVar7 = 0;
            if (e < 1) {
                if (-e + 1 < 0x37) {
                    uVar7 = ((qb & 0x001fffffffffffffull) | 0x0010000000000000ull)
                            >> (static_cast<unsigned>(-e) & 0x3f);
                    uVar7 = (uVar7 >> 1) + (uVar7 & 1);
                }
            } else {
                uVar7 = (static_cast<std::uint64_t>(static_cast<std::int64_t>(e) << 52)) | (qb & kMantMask);
            }
            dVar8 = D(uVar7 | (qb & kSignBit));
            if ((uVar7 & kInfBits) != 0) return dVar8;
            return dVar8;
        }
        if (iVar10 < -0x38 && xsign != 0) {            // |y| << |x|, x < 0 -> +/- pi
            result = D(kPi);
            if (ysign != 0) result = D(bits(result) ^ kSignBit);
            return result;
        }
        if (dy != D(kInfBits) || dx != D(kInfBits)) {
            // ---- main ratio: atan of min/max magnitude, placed in the right quadrant -------
            const double xa = D(xbits & kAbsMask);     // |x| (scaled)
            const double ya = D(ybits & kAbsMask);     // |y| (scaled)
            const bool bV1 = (ya != xa);
            const bool bV2 = (xa <= ya);
            const bool swap = bV2 && bV1;              // |y| > |x|
            const double aval = swap ? xa : ya;        // numerator (min magnitude)
            const double bval = swap ? ya : xa;        // denominator (max magnitude)
            const std::uint64_t bb = bits(bval);
            const double ratio = aval / bval;          // in (0, 1]
            double core;                               // dVar13

            if (ratio <= D(0x3fb0000000000000ull)) {   // ratio <= 0.0625
                if (D(0x3e45798ee2308c3aull) <= ratio) { // 1e-8 <= ratio: degree-5 odd poly
                    const double r2 = ratio * ratio;
                    const double rhi = D(bits(ratio) & 0xffffffff00000000ull);
                    const double rlo = ratio - rhi;
                    const double bhi = D(bb & 0xffffffff00000000ull);
                    const double blo = bval - bhi;
                    double corr = aval - rhi * bhi;    // Dekker remainder of aval - ratio*bval
                    corr = corr - blo * rhi;
                    corr = corr - rlo * bval;
                    corr = corr / bval;
                    double p = D(0x3fbc718839b8c4ebull) - r2 * D(0x3fb70c3192d751c2ull); // ef38 - r2*ef30
                    p = D(0x3fc2492482bd6be1ull) - p * r2;   // ef28 -
                    p = D(0x3fc99999999643a3ull) - p * r2;   // ef20 -
                    p = D(0x3fd5555555555538ull) - p * r2;   // ef18 -
                    core = corr - p * (r2 * ratio);
                    core = core + ratio;
                } else {                               // ratio < 1e-8: atan(ratio) = ratio
                    core = ratio;
                }
            } else {                                   // 0.0625 < ratio <= 1: table reduction
                const double scaled = ratio * D(0x4070000000000000ull) + D(0x3fe0000000000000ull); // *256 + 0.5
                const std::int64_t idx = static_cast<std::int64_t>(scaled); // cvttsd2si (trunc)
                const std::uint64_t ti = static_cast<std::uint64_t>(idx - 0x10);
                base = D(kAt2Hi[ti]);
                const double c = static_cast<double>(static_cast<std::uint32_t>(idx)) * D(0x3f70000000000000ull); // idx/256
                const int ie = -(static_cast<int>((bb >> 52) & 0x7ff) - 0x3ff);
                const int q1 = ie / 2;
                const int q2 = ie - q1;
                const double s1 = D(static_cast<std::uint64_t>(static_cast<std::int64_t>(q1 + 0x3ff) << 52));
                const double s2 = D(static_cast<std::uint64_t>(static_cast<std::int64_t>(q2 + 0x3ff) << 52));
                double b = s1 * bval;
                b = b * s2;                            // bval scaled to ~[1,2)
                double a = s1 * aval;
                a = a * s2;
                const double bhi = D(bits(b) & 0xfffffffff8000000ull); // clear low 27 bits
                const double blo = b - bhi;
                double num = a - bhi * c;
                double denom = c * a;
                num = num - blo * c;
                denom = denom + b;
                double w = num / denom;
                core = w + D(kAt2Lo[ti]);
                const double w2 = w * w;
                double t = D(0x3fd5555555550877ull) - w2 * D(0x3fc99992b9802d7dull); // ef48 - w2*ef40
                t = t * w2;
                t = t * w;
                core = core - t;
            }

            if (swap) {
                base = D(kPi_2) - base;
                core = D(kPi_2Lo) - core;
            }
            if (xsign != 0) {
                base = D(kPiHi) - base;
                core = D(kPiLo) - core;
            }
            const double sum = base + core;
            if (ysign == 0) return sum;
            return D(bits(sum) ^ kSignBit);
        }
        // both infinite
        result = (xsign == 0) ? D(kPi_4) : D(k3Pi_4);
    }

    // LAB_140486041: result is pi/2 (|y|>>|x|) or pi/4 / 3pi/4 (both infinite)
    if (ysign == 0) return result;
    return D(bits(result) ^ kSignBit);
}

// @src 140483f60  pow(x, y) = x**y (MSVC CRT, AMD LibM). Integer/odd-even parity of y decides the
// sign for negative x; the magnitude comes from the x87 FYL2X/F2XM1/FSCALE core (libm_crt2_x87.asm).
double crt_pow(double x, double y)
{
    const std::uint64_t X = bits(x);
    const std::uint64_t Y = bits(y);
    const std::uint64_t ax = X & kAbsMask;             // |x| bits
    const std::uint64_t ay = Y & kAbsMask;             // |y| bits

    if (X == 0x3ff0000000000000ull) return x;          // x == 1.0 -> 1.0
    if (ay == 0) return 1.0;                            // y == 0 -> 1.0

    const std::uint64_t xexp = ax & kInfBits;
    if (xexp == kInfBits && (ax & kMantMask) != 0) {    // x is NaN
        if (X == 0xfff8000000000000ull && (ay & kInfBits) == kInfBits && (Y & kMantMask) != 0) {
            return y;                                   // indefinite x, NaN y -> y (code 1)
        }
        if (X == ax || Y != ay || (ay & kInfBits) != kInfBits || (Y & kMantMask) == 0) {
            return x;                                   // -> x (code 1)
        }
        return y;                                       // -> y (code 1)
    }
    if ((ay & kInfBits) == kInfBits && (ay & kMantMask) != 0) { // y is NaN
        if (Y == 0xfff8000000000000ull && xexp == kInfBits && (X & kMantMask) != 0) {
            return x;                                   // indefinite y, NaN x -> x
        }
        return y;                                       // -> y
    }
    if (Y == 0x3ff0000000000000ull) return x;          // y == 1.0 -> x

    if (ay > 0x43e0000000000000ull) {                   // |y| > 2^63 (y acts as +/-inf exponent)
        if (Y == ay) {                                  // y > 0
            if (ax == 0) return 0.0;                    // pow(0, +huge) = 0
            if (ax >= 0x3ff0000000000000ull) {          // |x| >= 1
                if (ax == 0x3ff0000000000000ull) return 1.0; // |x| == 1 -> 1
                if (ay == kInfBits) return D(ay);       // |x| > 1, y = +inf -> +inf
                return D(kInfBits);                     // |x| > 1, y = +huge -> +inf (code 0xa)
            }
            if (ay == kInfBits) return 0.0;             // |x| < 1, y = +inf -> 0
            return 0.0;                                 // |x| < 1, y = +huge -> +0 (code 8)
        }
        // y < 0
        if (ax == 0) return D(kInfBits);                // pow(0, -huge) = +inf (code 0xb)
        if (ax >= 0x3ff0000000000000ull) {              // |x| >= 1
            if (ax == 0x3ff0000000000000ull) return D(ax); // |x| == 1 -> 1
            if (ay == kInfBits) return 0.0;             // |x| > 1, y = -inf -> 0
            return 0.0;                                 // |x| > 1, y = -huge -> +0 (code 0xe)
        }
        if (ay == kInfBits) return D(ay);               // |x| < 1, y = -inf -> +inf
        return D(kInfBits);                             // |x| < 1, y = -huge -> +inf (code 0xc)
    }

    // ---- general core (140484360): parity of y, then x87 pow of |x| ----
    int parity;                                        // 0 non-integer, 1 odd, 2 even
    {
        const int e = static_cast<int>((ay >> 52) & 0x7ff) - 0x3fe; // expY - 0x3fe
        if (e < 1) {
            parity = 0;
        } else if (e > 0x35) {
            parity = 2;
        } else {
            const int sh = 0x35 - e;
            const std::uint64_t mask = (std::uint64_t{1} << sh) - 1;
            if ((Y & mask) != 0) {
                parity = 0;                             // fractional bits set -> non-integer
            } else if (((Y & ~mask) >> sh) & 1) {
                parity = 1;                             // odd
            } else {
                parity = 2;                             // even
            }
        }
    }

    if (xexp == kInfBits) {                             // x is +/- inf (mantissa 0 here)
        if (X != ax) {                                  // x = -inf
            if (parity == 1) {                          // odd power keeps sign
                if (Y == ay) return x;                  // y > 0 -> -inf
                return D(0x8000000000000000ull);        // y < 0 -> -0.0
            }
            if (Y != ay) return 0.0;                    // even/non-int, y < 0 -> +0
            return D(ax);                               // even/non-int, y > 0 -> +inf
        }
        // x = +inf
        if (Y == ay) return x;                          // y > 0 -> +inf
        return 0.0;                                     // y < 0 -> +0
    }

    if (ax == 0) {                                      // x == +/- 0
        if (X == ax) {                                  // x = +0
            if (Y == ay) return 0.0;                    // y > 0 -> +0
            return D(kInfBits);                         // y < 0 -> +inf (code 0xf)
        }
        // x = -0
        if (Y == ay) {                                  // y > 0
            if (parity == 1) return x;                  // odd -> -0
            return 0.0;                                 // even/non-int -> +0
        }
        // y < 0
        if (parity == 1) return D(0xfff0000000000000ull); // odd -> -inf (code 0x10)
        return D(kInfBits);                             // even/non-int -> +inf (code 0x11)
    }

    int signflag = 0;                                  // negate result (x<0, odd y)
    if (X != ax) {                                     // x < 0
        if (parity == 0) return D(0xfff8000000000000ull); // non-integer y -> domain NaN (code 0x12)
        if (parity == 1) signflag = 1;
    }

    if (ay < 0x3c00000000000000ull) {                  // |y| < 2^-63 -> 1 + y (~1)
        return 1.0 + y;
    }

    const double res = def_rt_libm_crt_pow_x87(D(ax), y); // |x| ** y
    if (signflag) {
        if ((bits(res) & kAbsMask) == 0) {              // +/-0 result (code 0x19): -0.0
            return D(bits(res) | kSignBit);
        }
        return 0.0 - res;                               // normal/subnormal/inf: negate (SUBPD)
    }
    return res;
}

// @src 140486fd4  MSVC CRT acos (AMD LibM): |x| < 0.5 via acos = pi/2 - (x + x*u), else through
// s = sqrt((1 - |x|)/2) with a split s for x > 0. u = r*P(r)/Q(r), constants 14056f030..14056f080.
// The error and inexact paths (_handle_error 1404a02e0, 1404a0af8) only raise flags / set errno;
// their return values are reproduced.
double crt_acos(double x)
{
    constexpr std::uint64_t kPi       = 0x400921fb54442d18ull; // 14056f010
    constexpr std::uint64_t kPiby2    = 0x3ff921fb54442d18ull; // 14056f018
    constexpr std::uint64_t kPiby2Tl  = 0x3c91a62633145c07ull; // 14056f028
    constexpr std::uint64_t kP0       = 0x3fcd1e4180029834ull; // 14056f030
    constexpr std::uint64_t kQ0       = 0x3ff5d6b12001f228ull; // 14056f038
    constexpr std::uint64_t kP1       = 0x3fdc7b297e269eacull; // 14056f040
    constexpr std::uint64_t kQ1       = 0x400a4646f903cdeaull; // 14056f048
    constexpr std::uint64_t kP2       = 0x3fd1a2bec1b7ef59ull; // 14056f050
    constexpr std::uint64_t kQ2       = 0x40062021571dccfcull; // 14056f058
    constexpr std::uint64_t kP3       = 0x3fac28d390c29690ull; // 14056f060
    constexpr std::uint64_t kQ3       = 0x3fee324ab418f78dull; // 14056f068
    constexpr std::uint64_t kP4       = 0x3f51e5f887a62135ull; // 14056f070
    constexpr std::uint64_t kQ4       = 0x3fbb1a422982ce76ull; // 14056f078
    constexpr std::uint64_t kP5       = 0x3f0951665d321061ull; // 14056f080
    constexpr double kOne = 1.0;                               // 140509c90
    constexpr double kMinusOne = -1.0;                         // 140522fc8
    constexpr double kHalf = 0.5;                              // 14051f240
    constexpr double kTwo = 2.0;                               // 140509fb8

    const std::uint64_t ux = bits(x);
    const std::uint64_t xneg = ux & kSignBit;                  // rdx
    const std::int32_t xexp = static_cast<std::int32_t>((ux >> 52) & 0x7ff) - 0x3ff;
    const std::uint64_t ax = ux & kAbsMask;
    if (ax > kInfBits) {
        return D(ux | kQuietBit);                              // NaN: _handle_error returns x quieted
    }
    if (xexp < -56) {
        return D(kPiby2);                                      // tiny: pi/2 (inexact raised)
    }
    if (xexp >= 0) {                                           // |x| >= 1
        if (x == kOne) return 0.0;
        if (x == kMinusOne) return D(kPi);                     // inexact raised
        return D(0xfff8000000000000ull);                       // |x| > 1: _handle_error, default NaN
    }
    double y = x;                                              // xmm6
    if (xneg != 0) y = D(bits(y) ^ kSignBit);                  // |x| (xorpd 14051bec0)
    const bool transform = xexp >= -1;                         // |x| >= 0.5
    double r;                                                  // xmm3, [rsp+78]
    double s = 0.0;                                            // xmm4, [rsp+70]
    if (transform) {
        r = kOne;
        r = r - y;
        r = r * kHalf;
        s = std::sqrt(r);                                      // sqrtsd
        y = s;
    } else {
        r = y;
        r = r * y;
    }
    double num = r;                                            // xmm5
    double den = r;                                            // xmm0
    num = num * D(kP5);
    den = den * D(kQ4);
    num = num + D(kP4);
    den = den - D(kQ3);
    num = num * r;
    den = den * r;
    num = num - D(kP3);
    den = den + D(kQ2);
    num = num * r;
    den = den * r;
    num = num + D(kP2);
    den = den - D(kQ1);
    num = num * r;
    den = den * r;
    num = num - D(kP1);
    den = den + D(kQ0);
    num = num * r;
    num = num + D(kP0);
    num = num * r;
    double u = num / den;
    if (transform) {
        if (xneg != 0) {                                       // pi - 2*(s + (s*u - pi/2 tail))
            u = u * y;
            u = u - D(kPiby2Tl);
            u = u + s;
            u = u * kTwo;
            return D(kPi) - u;
        }
        // 2*(c + (r - c*c)/(s + c) + s*u) with c = s, low 32 bits cleared
        const double c = D(bits(s) & 0xffffffff00000000ull);
        double t = y * kTwo;                                   // xmm6
        t = t * u;
        double cc = c * c;                                     // xmm0
        double cs = c + s;                                     // xmm1
        double c2 = c * kTwo;                                  // xmm4
        double v = r - cc;                                     // xmm3
        v = v / cs;
        v = v * kTwo;
        v = v + t;
        v = v + c2;
        return v;
    }
    // pi/2 - (x - (pi/2 tail - x*u))
    double xu = u * x;
    double t = D(kPiby2Tl) - xu;
    double w = x - t;
    return D(kPiby2) - w;
}

namespace {

// @src 14048799c  tan core (AMD LibM tan_piby4): tan(x + xx) for |x + xx| <= pi/4 (recip == 0),
// or -1/tan(x + xx) (recip != 0)
double tan_piby4(double x, double xx, std::int32_t recip)
{
    std::int32_t transform = 0;
    double c = xx;                                        // xmm3
    double t = x;                                         // xmm6
    if (x > D(0x3fe5c28f5c28f5c3ull)) {                   // 0.68 (14056f140)
        t = D(0x3fe921fb54442d18ull) - x;                 // pi/4 (14056f0b8)
        transform = 1;
        double u = D(0x3c81a62633145c06ull) - xx;         // pi/4 tail (14056f0c0)
        t = t + u;
        c = 0.0;
    } else if (D(0xbfe5c28f5c28f5c3ull) > t) {           // -0.68 (14056f138)
        t = t + D(0x3fe921fb54442d18ull);
        c = xx + D(0x3c81a62633145c06ull);
        transform = -1;
        t = t + c;
        c = 0.0;
    }
    const double two = D(0x4000000000000000ull);          // 140509fb8
    double x2 = t;
    const double tt = t * t;
    x2 = x2 * two;
    x2 = x2 * c;
    x2 = x2 + tt;                                         // r2 = (2t)*c + t*t
    double p = x2 * D(0x3f2d5daf289c385aull);             // 14056f130
    double q1 = x2 * D(0x3f2e7517ef6d98f8ull);            // 14056f128
    const double r3 = x2 * t;
    p = p - D(0x3f977c24c7569abbull);                     // 14056f120
    p = p * x2;
    p = p + D(0x3fd7d50f6638564aull);                     // 14056f118
    p = p * r3;
    double q = D(0x3f9ab0f4f80a0acfull) - q1;             // 14056f110
    q = q * x2;
    q = q - D(0x3fe08046499eb90full);                     // 14056f108
    q = q * x2;
    q = q + D(0x3ff1dfcb8caa40b8ull);                     // 14056f100
    p = p / q;
    p = p + c;
    const double one = D(0x3ff0000000000000ull);          // 140509c90
    if (transform != 0) {
        p = p + t;
        double res;
        if (recip != 0) {
            double a = p * two;
            const double b = p - one;
            a = a / b;
            res = a - one;                                // 2p/(p-1) - 1
        } else {
            double a = p * two;
            const double b = p + one;
            a = a / b;
            res = one - a;                                // 1 - 2p/(p+1)
        }
        return res * static_cast<double>(transform);
    }
    if (recip != 0) {
        // -1/(t + p) with the head/tail split (low 32 bits of the mantissa cleared)
        const double z = p + t;
        const double rz = D(0xbff0000000000000ull) / z;  // -1.0 (140522fc8)
        const double zh = D(bits(z) & 0xffffffff00000000ull);
        double u = zh - t;
        p = p - u;
        const double rh = D(bits(rz) & 0xffffffff00000000ull);
        p = p * rh;
        double v = rh * zh;
        v = v + one;
        v = v + p;
        v = v * rz;
        return v + rh;
    }
    return p + t;
}

} // namespace

// @src 140487b70  tan (MSVC CRT, AMD LibM): reduction by n*pi/2 (Cody-Waite in three steps below
// 5e5, __remainder_piby2 1404a0430 above), core 14048799c on region parity
double crt_tan(double x)
{
    const std::uint64_t ux = bits(x);
    const std::uint64_t ax = ux & kAbsMask;
    if (ax <= 0x3fe921fb54442d18ull) {                    // |x| <= pi/4
        if (ax < 0x3f20000000000000ull) {                 // |x| < 2^-13
            if (ax < 0x3e40000000000000ull) {             // |x| < 2^-27: x (inexact raised, 1404a0af8)
                return x;
            }
            double t = x * x;
            t = t * x;
            t = t * D(0x3fd5555555555555ull);             // 1/3 (140541468)
            return t + x;
        }
        return tan_piby4(x, 0.0, 0);
    }
    if ((ux & kInfBits) == kInfBits) {                    // Inf / NaN via _handle_error 1404a02e0
        if ((ux & kMantMask) != 0) return D(ux | kQuietBit);
        return D(0xfff8000000000000ull);
    }
    const bool neg = (ax != ux);
    double ax_d = x;
    if (neg) ax_d = D(bits(ax_d) ^ kSignBit);
    double r, rr;
    std::int32_t region;
    if (D(0x411e848000000000ull) > ax_d) {                // 500000.0 (14056f148)
        std::uint64_t xexp = ax >> 52;
        std::int32_t n;
        if (ax <= 0x400f6a7a2955385eull) {                // 5pi/4
            n = (ax > 0x4002d97c7f3321d2ull ? 1 : 0) + 1; // 3pi/4
        } else if (ax <= 0x401c463abeccb2bbull) {         // 9pi/4
            n = (ax > 0x4015fdbbe9bba775ull ? 1 : 0) + 3; // 7pi/4
        } else {
            double t = ax_d * D(0x3fe45f306dc9c883ull);   // 2/pi (14056f0c8)
            t = t + D(0x3fe0000000000000ull);             // 0.5 (14051f240)
            n = static_cast<std::int32_t>(t);             // CVTTSD2SI
        }
        const double dn = static_cast<double>(n);
        double rhead = dn * D(0x3ff921fb54400000ull);     // piby2_1 (14056f0d0)
        double rtail = dn * D(0x3dd0b4611a626331ull);     // piby2_1tail (14056f0d8)
        double xv = ax_d - rhead;
        const std::uint64_t expdiff = xexp - ((bits(xv) >> 52) & 0x7ff);
        if (expdiff > 0xf) {
            double t = xv;
            const double h2 = dn * D(0x3dd0b4611a600000ull);   // piby2_2 (14056f0e0)
            double m = 0.0 - h2;
            rtail = dn * D(0x3ba3198a2e037073ull);              // piby2_2tail (14056f0e8)
            xv = xv + m;
            t = t - xv;
            t = t - h2;
            rtail = rtail - t;
            if (expdiff > 0x30) {
                double t3 = xv;
                const double h3 = dn * D(0x3ba3198a2e000000ull);   // piby2_3 (14056f0f0)
                rtail = dn * D(0x397b839a252049c1ull);              // piby2_3tail (14056f0f8)
                double m3 = 0.0 - h3;
                xv = xv + m3;
                t3 = t3 - xv;
                t3 = t3 - h3;
                rtail = rtail - t3;
            }
        }
        r = xv - rtail;
        region = n & 3;
        xv = xv - r;
        rr = xv - rtail;
    } else {
        crt_remainder_piby2(ax_d, &r, &rr, &region);
    }
    const double res = tan_piby4(r, rr, region & 1);
    return neg ? D(bits(res) ^ kSignBit) : res;
}

} // namespace def::rt::libm
