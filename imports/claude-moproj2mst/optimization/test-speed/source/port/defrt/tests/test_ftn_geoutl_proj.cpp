// ctest-labels: geoutl
// Tier-1 tests of the quadrilateral projection kernels (defrt/ftn/geoutl_proj.hpp): 1404d203b
// (and through it 1404d4a0f, 1404d5a1d's caller, 1404d275e, 1404ddeba), 1404ddeba, 1404d3e13,
// 1404e37c4, 1404d1b1c, 1404d1d05, 1404d1ed7, 1404d93ae.
//
// The golden rows (1404d203b, 1404ddeba, the location codes) are bit patterns produced by the
// original routines, called in a copy of DEF_PRE_64.exe mapped at its link base into a test
// process (the binary is not modified or run), each call starting from the image's initial
// (all-zero) SAVE state. The 1404d203b quads are large (coordinates up to ~1e9, where rounding
// makes a quad with two equal corners non-planar) so that every path, including the triangle
// split, has two rows. The box and sort rows are exact by hand calculation. The random sweep that
// compares every routine with the original bit for bit (6.6 million calls per run, also with
// injected NaNs of random payload; outputs and all SAVE regions; 0 differences) is
// deform-rea-work/scripts_p67/gu-proj/projdiff.cpp ("BIG=1 projdiff 1 gold" prints the tables).
//
// 1404d203b reaches every other kernel of the library, including 1404d41f8 (PNTEDG, p67-3) on
// paths 1, 5 and 7.
#include <defrt/ftn/geoutl.hpp>
#include <testkit/check.hpp>

#include <bit>
#include <cstdint>
#include <cstring>

using namespace def::rt::geoutl;

namespace {

double D(std::uint64_t u) { return std::bit_cast<double>(u); }
std::uint64_t U(double d) { return std::bit_cast<std::uint64_t>(d); }

void reset_all() {
    reset_proj_globals();
    reset_globals_basic();
    reset_poly_globals();
    reset_globals_geoutl_tri();
    g_common_140b84720 = Common140b84720{};
}

struct ProjRow {
    std::uint64_t x[12], p[3], s_in, t_in;
    std::uint64_t s, t, q[3], dist;
    std::int32_t flag;
    int path; // 0 planar inside, 1 planar outside (1404d41f8), 2 Newton, 3 Newton failed ->
              // 1404d275e, 4 triangle split, 5 Newton failed -> 1404d41f8, 6 triangle X4 = X1,
              // 7 Newton converged outside [-1,1]^2 -> 1404d41f8
};
const ProjRow kProj[] = {
    {{0x4141707ed34a0d35, 0xc12864548f7e283e, 0x41321f9a032f53ee, 0xc122c19591c65aca, 0xc0bd9cb9623657c5, 0x41411d0e6e02b07a, 0xc148ba0b1b5339ca, 0x4131a878db488349, 0xc13ddff07fe354b5, 0xc141c161d069e7a7, 0x4129a112c3d25f66, 0xc1370060b34278c8}, {0xc139626071ee08e6, 0x415280c14b43b51b, 0x4123910cc18b276f}, 0x3ff004188d867bfb, 0x849b14bc3a261365, 0x3ff0000000000000, 0x3fc5aa019b101120, {0xc1406751601b1b74, 0x41248cf6fa080693, 0xc109fcf4f22f956c}, 0x41505e8ab49a8364, 0, 1},
    {{0xc1f4fb846a6cb8ce, 0x41d171031770951d, 0xc2256000c2c1d03b, 0x42035bd25f528a51, 0x41ca819b04db9cf2, 0xc224fcaf4f99c8f6, 0x42035bd25f528a51, 0x41ca819b04db9cf2, 0xc224fcaf4f99c8f6, 0xc2036564fd94d874, 0xc1f8d472c748856b, 0x4224efd20ef1939d}, {0x42035c5026899502, 0xc1d5bd81d28e2095, 0xc225048eb556dc24}, 0xbfea119f457c977a, 0x0000000000000000, 0x3fefe98ec14610e8, 0x3ff0000000000000, {0x42034e3b612915d0, 0x41ca32a7d998d63f, 0xc224edfbd475f488}, 0x41e17a5fe2638ff9, 0, 1},
    {{0xc200300619afff59, 0xc1e3bf8fc40316d6, 0xc1e21e373c9623fd, 0x41fe3cb4e8ae98b2, 0x41f143cfbbce0d5e, 0x41ce0d010a40d4fd, 0x42002f7d0368230d, 0x41e4c1162a425066, 0x41e245f2871b4c6b, 0xc1fe848c2c245c3c, 0xc1f17ab4b4c7b156, 0xc1c8fc9c73c6f501}, {0xc20356ac7413be84, 0xc1f478392eeea1d6, 0xc1f4c1f54cc5aa2a}, 0xbfeff9d55ab57097, 0xbfd9866e67499ac8, 0xbff0000000000000, 0xbff0000000000000, {0xc200300619afff59, 0xc1e3bf8fc40316d6, 0xc1e21e373c9623fd}, 0x41f0fec564ba485e, 0, 7},
    {{0x422ff5deeeb58ff0, 0xc2025ad78c164934, 0x4210fca7ee7ccf4b, 0xc228ef49c6217267, 0x4214c51061e49bbc, 0xc2221cab9d1137c4, 0x422ff5deeeb58ff0, 0xc2025ad78c164934, 0x4210fca7ee7ccf4b, 0x42296a89a8f34a7f, 0xc21224822dbcd623, 0x4222a6b5984364b3}, {0x41f6079db5bf718f, 0xc215da1c0b2f6bc7, 0x4228b5c74b8b8aa2}, 0x0000000000000001, 0x3fe19425c999488c, 0xbfcda2e6d1067790, 0x3fcda2e6d1067797, {0x4225144f00ef043a, 0xc2004f9c201725e5, 0x4210d58a137d1f67}, 0x422975a4e23ee065, 0, 2},
    {{0x42510a0544d20e04, 0x421201018e120873, 0x4246c16b278101b3, 0x4239e2b974eebec2, 0x4243c048bf1c2451, 0x425291cfd4b87ea9, 0xc231ec6302e0d56e, 0xc22068c4797a1893, 0xc2528f2e327bd0fd, 0xc24710aa61e94359, 0x4232c30d8823cd99, 0x420bd12590f09963}, {0x42267f637c50f562, 0x4237a849b4bc7d1a, 0x423939c3a1d1f7fb}, 0x3fd0973c16154120, 0x3feff7ced916872b, 0x3ff0000000000000, 0xbfd5aac9911d6de1, {0x4226cb33e6c70f2d, 0x4237b96f115adb92, 0x423929131e55373a}, 0x41c669de3b96b821, 0, 5},
    {{0xc2461b7ebbaeae14, 0x42582859136614be, 0x42587360c7046f31, 0x42029696049c702b, 0xc25b5a805df9490a, 0xc258e9f8cd180cd8, 0x4246696efb103599, 0xc24b40ca7128a077, 0x426037645f8992f2, 0xc2461b7ebbaeae14, 0x42582859136614be, 0x42587360c7046f31}, {0xc2462107c890f70e, 0x42582cfa057b2a0b, 0x42587244d4787fa4}, 0x3ff6f6cade5a319a, 0xbff004189374bc6a, 0x0000000000000000, 0x0000000000000000, {0xc224a4251c035f8c, 0x421d573240fa40d8, 0x424c35c8c001fbb7}, 0x425a0a4541ac4ae3, 0, 4},
    {{0xc1ac45c729075b63, 0x41905ec262b81c20, 0xc1a62178d15f61a9, 0x41a615089f1fb546, 0x41b0b9d2acee5556, 0xc1a0a1bb65637b78, 0xc1934f4a6252ad75, 0xc1a3d825ea941757, 0x418ab000f4387319, 0x419abd094a3ac291, 0x41abed2758c65229, 0xc18d9879fcc7b768}, {0x41bf3182e195329b, 0x41c07c352e633557, 0xc1a3fb49b05e8adf}, 0x3fffffffffffffff, 0xbfe0ae1876a2261a, 0x3ff0000000000000, 0xbff0000000000000, {0x41a615089f1fb545, 0x41b0b9d2acee5556, 0xc1a0a1bb65637b78}, 0x41b9f040e1244e4e, 0, 5},
    {{0x419c948e27c66be1, 0x41fb13977c9d4d35, 0x41e33bde536c9c76, 0x419c948e27c66be1, 0x41fb13977c9d4d35, 0x41e33bde536c9c76, 0xc1dc7085bf57fadf, 0xc1fa0c18ea86eac9, 0xc1e549f9f9efe8b7, 0xc1ca2c9385350167, 0x41f23cdce98496c2, 0x41f6036aeaffe405}, {0xc1ca28c4c81d2c86, 0x41f23dfe8a9d8fad, 0x41f601d4b64b807e}, 0x8000000000000001, 0xbff1b9b73327f404, 0xbff0000000000000, 0x3feff7ced916872e, {0xc1ca28c4c81d2cb3, 0x41f23dfe8a9d8fae, 0x41f601d4b64b807e}, 0x3ed6da506e12f87e, 0, 0},
    {{0xc204f8edf7063914, 0x41e3c47c911f7e49, 0x4220dba6db531669, 0xc21b3edda606012c, 0x420751b49b08f2b2, 0x42144260a28daf9f, 0xc204f8edf7063914, 0x41e3c47c911f7e49, 0x4220dba6db531669, 0x42188e546c0eb6bd, 0xc20bad3af83b8be5, 0xc21559476438c878}, {0x420654e0ea668af0, 0x4214086c2985ed8a, 0x422542b3638eb9aa}, 0x3fdee83d890ebf80, 0xbfee25ffd75e916f, 0x3fc85af9694d0518, 0xbfc85af9694d051d, {0xc2055b4c16279b4a, 0x41e86ad0327ac4e6, 0x4213ec88509ebe21}, 0x4221dc8f1725aeab, 0, 2},
    {{0x41eff65937de0326, 0x41e675236accf944, 0x41fb151b5db08957, 0x41e9aa1aabe379a1, 0x41eea44a7176f34a, 0x41fb4d452f05d2b4, 0xc1f034b518bdacea, 0xc1e54249e32b88c5, 0xc1facbc886c77400, 0x41eff65937de0326, 0x41e675236accf944, 0x41fb151b5db08957}, {0x41f19f5ce1fd4d86, 0x41e69564d657284c, 0x41fce739d88602d5}, 0xbff0000000000000, 0xbff004189374bc6a, 0x0000000000000000, 0x0000000000000000, {0x41dc96b17512130d, 0x41db2623b1f2ae86, 0x41eb55d9b1cfb8b2}, 0x41f307b687d3a7dc, 0, 6},
    {{0x419adf6cab460962, 0xc191afe307d155c6, 0xc18340d11dd60049, 0xc18fa322df9cbb9a, 0xc19fd3ce6ed0f295, 0xc1a141be717168c9, 0x419b9e0a33eefc42, 0xc190fa73cf3b37f4, 0xc1a53bfe2e3456a4, 0x4193fc90ec97dbb3, 0x41a0757c71ef1091, 0xc17b8a0cc87d4a88}, {0xc1a192d41605dc2a, 0xc179eb398567888c, 0xc1a1960c18c5f1c6}, 0xbfeff7ced916872c, 0x3ff20825802ba134, 0x3ff0000000000000, 0xbff0000000000000, {0xc18fa322df9cbb9c, 0xc19fd3ce6ed0f295, 0xc1a141be717168c9}, 0x419fe20df810764b, 0, 7},
    {{0x41672c91c7d1d9d0, 0xc14e2b726f0c2643, 0x418b3a16a5b0c986, 0x41672c91c7d1d9d0, 0xc14e2b726f0c2643, 0x418b3a16a5b0c986, 0xc179b1cd3f902c6c, 0x41858dd73b876fbd, 0x41786835706e123c, 0xc1664120c5d73117, 0x4150872d9755d1b3, 0xc18b467bbdba8382}, {0x416518aa0fd32b1d, 0xc1522a7e935fec36, 0x418b5515135ac178}, 0x3fefffffffffffff, 0x3ff004189374bc6a, 0x0000000000000000, 0x0000000000000000, {0xc14b4b97b553d64f, 0x4163d94ea090a52b, 0x4173b0e622ef0c53}, 0x41842b814f8f5d10, 0, 0},
    {{0xc24371b659527eb3, 0xc2206acfb096bf96, 0xc22da1a2205c4c93, 0xc24266f51237f761, 0xc2221aba8dbe13f2, 0xc21f2f056d4312e6, 0x4245f67e8234d568, 0x422374b8e9200406, 0x422d0b9198069b76, 0xc24371b659527eb3, 0xc2206acfb096bf96, 0xc22da1a2205c4c93}, {0xc22bac9904b7e0d1, 0x422bea308636a411, 0x421ac38def170a3c}, 0x0000000000000000, 0xbfeff7ced916872b, 0x0000000000000000, 0x0000000000000000, {0xc231a9f1a1540fb0, 0xc20f7ba105cb8f18, 0xc216e79aafa9c392}, 0x42361ce75ec39fe5, 0, 4},
    {{0xc1cb2887aff74813, 0x41fae3eb48f5a984, 0xc1d9d3cb21b70348, 0x41a346a866b09075, 0xc2039fca1c8c99e7, 0x41fae84692d75f20, 0x41ded3a7d2ff8b4a, 0xc20286f2eb490465, 0xc1ca25767c4c4e4a, 0xc1cb2887aff74813, 0x41fae3eb48f5a984, 0xc1d9d3cb21b70348}, {0xc20911ffb17128c5, 0xc2019e082cfa3524, 0x41d83953523cc9e0}, 0x3ff0000000000000, 0xbf40e8c8ae8d1700, 0x3fe32bf9cfed7c90, 0xbfebaa03fa696600, {0x4187cf3292ba7299, 0xc1f9df636946c608, 0x41f295d0907a47cc}, 0x420a59bb425828b0, 0, 6},
    {{0xc2292746eb5525f5, 0xc222c8cc646b4b6c, 0xc1dd9423acfb328d, 0x420d19f48a109078, 0xc210400a5822e8c1, 0x420a9437e8166046, 0xc22a5f45376c8c77, 0xc2315a1c2e40e4e6, 0x4202f20940fda3a7, 0xc2342bc5910622f3, 0x4222c9711b231b95, 0xc22420c8a5164ac5}, {0xc22d99cd520c4a32, 0xc206ee20485d92dd, 0xc20d31854e5b4d5e}, 0xbffe78b677f06076, 0xbff51a9a0bf1d56e, 0xbfed70baa237ae20, 0xbfd1d586806997f2, {0xc22d99cd520c4a32, 0xc206ee20485d92da, 0xc20d31854e5b4d5f}, 0x3ed94c583ada5b53, 0, 3},
    {{0x42434f35e0059768, 0x423c6a79cfe32a3f, 0xc2404ecc4a49d36a, 0xc24072d181e67c1a, 0xc2384f72c53340de, 0x4241df1b03b17ef3, 0xc24a425a0e199eec, 0xc23985fbf60ad0f0, 0x42037368787daa49, 0x4248ee87954d7f44, 0x423e0488458f940a, 0xc1f9c17cc6063678}, {0x42461ee3ddac9b6a, 0x423d322ab5898131, 0xc230f966d9d9ea35}, 0x3fd5fedd6f77c354, 0xbff6824874061120, 0xbfeff7ced9170289, 0x3f7fd479a6de62b1, {0x42461ee3ddac9b6c, 0x423d322ab5898134, 0xc230f966d9d9ea37}, 0x3f158a68a4a8d9f3, 0, 3},
};

struct InvRow {
    std::uint64_t x[12], p[3], tol, len, st[2];
    std::int32_t nroot, ierr;
};
const InvRow kInv[] = {
    {{0x412fdcbe98e00e28, 0xc198a21c8c60d237, 0xc18e16604c1577d3, 0xc134ae2765e9c8d3, 0x419b4788ae9a939b, 0x4182db7d5cd5f121, 0xc12fdcbe98e00e28, 0x4198a21c8c60d237, 0x418e16604c1577d3, 0x4134ae2765e9c8d3, 0xc19b4788ae9a939b, 0xc182db7d5cd5f121}, {0x4142d09195cc5915, 0xc1aa3e99b2e0e1bb, 0xc1974a130439e1a3}, 0x3f847ae147ae147b, 0x4008000000000000, {0x0000000000000000, 0x0000000000000000}, 1, 1},
    {{0x4191bad7cb0b4a68, 0xc1af967d54916c06, 0x41849b527144acf1, 0x4185f60cdc3c8c09, 0xc19a6f234376d78e, 0xc1adf7d39b0d578e, 0xc191bad72ec2bdea, 0x41af967d22d36f60, 0xc1849b55e4d65e7d, 0xc185f60ba3ab730b, 0x419a6f22dffade44, 0x41adf7d2be28eb2a}, {0x41a8c5d4d33d60fa, 0xc1ab0383d50f0848, 0x418827392e3d573f}, 0x0000000000000000, 0x4008000000000000, {0x0000000000000000, 0x0000000000000000}, 1, 1},
    {{0x41852e01532e0faf, 0xc187fd00346cfa0f, 0xc18770f590efef7c, 0x4159a9895f57a68a, 0xc192c331f61d9969, 0x4182ae3228aa0aef, 0xc186ec796d7a9d41, 0x418b0d4c2c5b6a0e, 0x418833ebca05c58b, 0xc16175489aacc7c3, 0x4192cead250be39c, 0xc180e1250a66bc83}, {0xc1a05441ad6990f4, 0x418dc5c92ed2107f, 0x41676b003311052c}, 0x3f847ae147ae147b, 0x4008000000000000, {0x0000000000000000, 0x0000000000000000}, 1, 1},
    {{0xc185c7e5c87f6c6f, 0x4186650c4551002d, 0x4195b6092bd597cb, 0xc1917f7641dc470e, 0x418095de65b4da11, 0x419365951a2ccd59, 0x4183fe884a8e7c06, 0xc187c524e676336e, 0xc1960b1bd98bf14b, 0x41914687f458b77c, 0xc17949babe9f4f0b, 0xc190d0507496ca16}, {0xc15096a12a59c690, 0xc18327943eb35606, 0x41714eecff361844}, 0x3f847ae147ae147b, 0x4008000000000000, {0x3fea43750b82a9fa, 0xbf31dff904bbbb37}, 1, 1},
};

struct LocRow {
    double s, t;
    std::int32_t quad, tri;
};
const LocRow kLoc[] = {
    {0, 0, 0, -3},
    {0.99890000000000001, -0.99890000000000001, 0, -3},
    {0.999, 0, 2, 3},
    {1, 0.5, 2, -1},
    {-1.0004999999999999, 0.20000000000000001, 4, 0},
    {0.29999999999999999, 1.0009999999999999, 3, -2},
    {0.20000000000000001, -1.0011000000000001, -2, 0},
    {1, 1, -3, -1},
    {-1, -1, -1, 0},
    {1.0004999999999999, -0.99950000000000006, -2, -1},
    {-0.99950000000000006, 1.0008999999999999, -4, -2},
    {3, 3, 0, 0},
    {1.5, 0.20000000000000001, -3, 0},
    {0.00050000000000000001, 0.40000000000000002, 0, 2},
    {0.40000000000000002, 0.59960000000000002, 0, 1},
    {0.99950000000000006, 0.00040000000000000002, 2, -1},
    {0.29999999999999999, 0.69999999999999996, 0, 1},
    {-0.00040000000000000002, 0.00029999999999999997, 0, -3},
};

} // namespace

TEST_CASE(quad_projection_golden) {
    for (const ProjRow& r : kProj) {
        reset_all();
        double x[12], p[3], q[3] = {0, 0, 0}, dist = 0.0;
        for (int k = 0; k < 12; ++k) x[k] = D(r.x[k]);
        for (int k = 0; k < 3; ++k) p[k] = D(r.p[k]);
        double s = D(r.s_in), t = D(r.t_in);
        std::int32_t flag = 0;
        fun_1404d203b(x, p, &s, &t, q, &dist, &flag);
        CHECK_EQ(U(s), r.s);
        CHECK_EQ(U(t), r.t);
        for (int k = 0; k < 3; ++k) CHECK_EQ(U(q[k]), r.q[k]);
        CHECK_EQ(U(dist), r.dist);
        CHECK_EQ(flag, r.flag);
        CHECK_EQ(g_quad_proj.nonplanar != 0, r.path >= 2 && r.path != 6);
    }
}

TEST_CASE(quad_inverse_map_golden) {
    for (const InvRow& r : kInv) {
        reset_all();
        g_common_140b84720.u08 = D(r.tol);
        g_common_140b84720.u10 = D(r.len);
        double x[12], p[3], st[2] = {0.0, 0.0};
        for (int k = 0; k < 12; ++k) x[k] = D(r.x[k]);
        for (int k = 0; k < 3; ++k) p[k] = D(r.p[k]);
        std::int32_t nroot = 1, ierr = 0;
        fun_1404ddeba(x, p, st, &nroot, &ierr);
        CHECK_EQ(U(st[0]), r.st[0]);
        CHECK_EQ(U(st[1]), r.st[1]);
        CHECK_EQ(nroot, r.nroot);
        CHECK_EQ(ierr, r.ierr);
    }
}

TEST_CASE(location_codes_golden) {
    for (const LocRow& r : kLoc) {
        std::int32_t c = 99;
        fun_1404d3e13(&r.s, &r.t, &c);
        CHECK_EQ(c, r.quad);
        fun_1404e37c4(&r.s, &r.t, &c);
        CHECK_EQ(c, r.tri);
    }
    // SAVE: the nearest-corner distance of the last quad call ((3,3) is sqrt(8) from (1,1))
    const double s = 3.0, t = 3.0;
    std::int32_t c = 99;
    fun_1404d3e13(&s, &t, &c);
    CHECK_EQ(c, 0);
    CHECK_EQ(g_quad_loc.dmin, 2.0);
    CHECK_EQ(g_quad_loc.k, 5);
}

TEST_CASE(boxes) {
    // X(3,4); box of points 2 and 4 (list) and of points 1..3, margin 0.5
    const double x[12] = {0.0, 1.0, 2.0, -1.0, 4.0, 0.5, 3.0, -2.0, 1.0, 2.0, 2.0, -3.0};
    const std::int32_t idx[2] = {2, 4};
    const std::int32_t n2 = 2, n3 = 3, n0 = 0;
    const double m = 0.5;
    double box[6];
    fun_1404d1b1c(x, box, &m, &n2, idx);
    const double e1[6] = {-1.5, 1.5, -3.5, 2.5, 4.5, 1.0};
    for (int k = 0; k < 6; ++k) CHECK_EQ(box[k], e1[k]);
    CHECK_EQ(g_quad_proj.b1b_node, 4);
    fun_1404d1d05(x, box, &m, &n3);
    const double e2[6] = {-1.5, -2.5, 0.0, 3.5, 4.5, 2.5};
    for (int k = 0; k < 6; ++k) CHECK_EQ(box[k], e2[k]);
    fun_1404d1d05(x, box, &m, &n0); // no points: (1e10 - m, -1e10 + m)
    CHECK_EQ(box[0], 1e10 - 0.5);
    CHECK_EQ(box[3], 0.5 + -1e10);
    std::int32_t ov = 7;
    const double a[6] = {0, 0, 0, 1, 1, 1}, b[6] = {1, 1, 1, 2, 2, 2}, c[6] = {1, 1, 1.5, 2, 2, 2};
    fun_1404d1ed7(a, b, &ov);
    CHECK_EQ(ov, 1); // touching boxes overlap
    fun_1404d1ed7(a, c, &ov);
    CHECK_EQ(ov, 0);
}

TEST_CASE(column_sort) {
    // a(2,4) sorted by row 2: ascending (order 1) and descending (order 0)
    double a[8] = {1, 3.0, 2, -1.0, 3, 2.0, 4, -1.0};
    const std::int32_t key = 2, n = 4, ld = 2, up = 1, down = 0;
    fun_1404d93ae(a, &key, &n, &up, &ld);
    const double e1[8] = {2, -1.0, 4, -1.0, 3, 2.0, 1, 3.0};
    for (int k = 0; k < 8; ++k) CHECK_EQ(a[k], e1[k]);
    fun_1404d93ae(a, &key, &n, &down, &ld);
    const double e2[8] = {1, 3.0, 3, 2.0, 2, -1.0, 4, -1.0};
    for (int k = 0; k < 8; ++k) CHECK_EQ(a[k], e2[k]);
    CHECK_EQ(g_col_sort.i, 4);
}

int main() { return testkit::run_all(); }
