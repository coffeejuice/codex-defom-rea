// P01: QByteArray::fromHex and qUncompress with the self-contained inflate.
// Vectors made with Python's zlib (stored, fixed, dynamic, multi-block, empty); every <Data> blob
// of the corpus must inflate to the size its 4-byte header announces.
#include <testkit/check.hpp>

#include <cstdio>
#include <cstdlib>
#include <filesystem>
#include <fstream>
#include <iterator>
#include <string>

#include "m2m/blob.hpp"

using m2m::qt::fromHex;
using m2m::qt::qUncompress;

namespace {

std::string unhexStrict(const std::string& h)
{
    std::string r;
    for (size_t i = 0; i + 1 < h.size(); i += 2) r += static_cast<char>(std::stoi(h.substr(i, 2), nullptr, 16));
    return r;
}

const char* kDynamic =
    "00000c3278da55964b8e60370845e759c55b81e53f669a61261d29fbdf4bcea553c65149552a78b6e1f81af8f5e75f7ffffae7abfa29b5fef7fb8f5f"
    "bfcded330cc3c2dcda8fb9e3a8c56698e7fc318fef7cadb41666f71ff3d4f7651e999bed1ff3fa1cf35932775b3fe6cd36bdf42ef3f0fbb57d6cdbcb"
    "72d9d7b89b1f6d5f7ccb6cf586c2d6ed1b650cd98fdfc8d96461371225d227d1f6b5fecd524d8e31724127a259e6947d9d7b401b109be5b41a49dc78"
    "da24d455da51b46dddf8db82d92a6bc93e33ddb6496115efb29f96df1bd476e9e43ba096fb1f72db656fd9d7ce781c6e56ea90dd2dafaa2a672bb32a"
    "dc9919f706392b6691c6ca055d304e6964bccae8f7843e4077ca124222bd11f52948a71c52dea0bb29f4053a2f7dc97e32e5be41e765f788ebf9de40"
    "e7c549f9a0b63c00559144195b8efd44e4121cf73e6a9c7373e07f65a10bd68699f468a10c2e4058ca4a4ea3c30fcfd145b7d293ec1822d8d0640b97"
    "e7658cf9297974111f585edf58bf25559cecdb786f7c6c38621aba9a49d8b9c84412d706415ba5a6acc681255fd71d9e93421cfec53e93a4db86c54d"
    "894c4c9e23b15bd98961361145a5c270caccf731bb90a2536170f472cf99434c51aade2197914f704e4145abd23ce8f2d1ce25a8a855aaefef2b9f3b"
    "a02258097fe0ca4526a8c6e3c7039fa7909c808a68a1d01167969ee9828a6a75f5bbccac55e2dfe55a60e8a63fd7d50415e582a1a3ac6751ff22ff0e"
    "86ee54ba7bd01a828a7a097fa8b4dce8d61454e40686d1a92459e97837c18c58c67831ac0d543c0685c11d26bac55310cdaa02b0b8d63ce7a842a261"
    "950000e6152d8f22898855050ee86e427c3be30a8030fcd5c26e828a6ba912d4e229a0dd818ac7a130599ba2db4350b9b7c18113c1a652f7042a9e0d"
    "86290ddf94f6fae2462b18e67e9fc4de40c533c1300d89e51a60cb6360984e27ca738ea0a261525b5c443ed8ed828a86a1b03a6b6e4264e8f25011b8"
    "c8b73058135434cc276b9599b5c47a4045c42a8812ec3dc886a0625249947e6e7036032a225651a4bf6595b325a8685865b1bd75d1764045c460d854"
    "f7679109aa5e2a9e4525ca93ce17e23d60d89b3e92d1b9a0a261306ca2cb9454ade4410520799bc269828a86c160f4af4477884c821faa2aa04bdc67"
    "00158fa9cfa0d4bca233813ad030071a0528aff52c41c535a160fe6ae1d06ee5394038b4e2d4cfb168ea84a50ec13b4ad19da3b68e86d522d47d333a"
    "17545c0e866314889b9258ea3d0e301c7fa70167bc91674b307a83b9a60395975ac1e034e667e2e049c41b263127847cb03e818ac7c0e044908fdc97"
    "a0f23114dcdfc2e05b50d170f46f3064357113557a809a44a5b56409f2135891b1da445d6836e3737145c6ea13d5e8e0cf505483acc500c8fcf6bf11"
    "b036b1b598025bb4c167610fba16b3605317ca03894d7c4f0c844d5d200385db17254a5361a3047be647dc627c62346c6a28cfb454b7287bcc878d2a"
    "b3fc5967e2ec312336bdf2f19c7744da63506c3c97519f381dd6b3c6b048ff20ce4c5038e58b81718cff8f8c9a4ae8a5bf87465dd633366a9a942f06"
    "4700f9333a364d1a72c6f4c8eef3991f192097aaad26c836354064861a22bb9c1a231bd5f51d249924b77c1e738526a867a17d51a77b0c161a7c9e03"
    "0fb855c363b420c567a864aa24d01963655b7adced8f7f0148057f31"    ;

}  // namespace

TEST_CASE(from_hex_qt_rules)
{
    CHECK(fromHex("") == "");
    CHECK(fromHex("414243") == "ABC");
    CHECK(fromHex("4a4B") == "JK");
    CHECK(fromHex("41 42\n43") == "ABC");                  // non-hex bytes are skipped
    CHECK(fromHex("abc") == std::string("\x0a\xbc", 2));    // odd count: first digit is a low nibble
    CHECK(fromHex("zz") == "");
    CHECK(fromHex("0") == std::string(1, '\0'));
}

TEST_CASE(uncompress_vectors)
{
    std::string err;
    CHECK(qUncompress(fromHex("00000000789c030000000001"), &err).empty());
    CHECK(err.empty());
    CHECK(qUncompress(fromHex("000000057801010500faff68656c6c6f062c0215")) == "hello");   // stored
    CHECK(qUncompress(fromHex("000000177801cb48cdc9c957c8402701680308b1")) ==
          "hello hello hello hello");                                                         // fixed
    std::string key = "*KEY\n";
    for (int i = 0; i < 50; ++i) key += "OPSTOP 1 2 3\n";
    CHECK(qUncompress(fromHex("0000028f78dad3f2768de4f20f080ef10f5030543052301ee58c72068e0300aa8a91d8")) == key);
    std::string mb;
    for (int i = 0; i < 10; ++i) mb += "abc";
    for (int i = 0; i < 5; ++i) mb += "xyz";
    CHECK(qUncompress(fromHex("0000002d789c4a4c4a4ec48d00000000ffffaba8acaa404200971c1294")) == mb);  // full flush
}

TEST_CASE(uncompress_dynamic_block)
{
    std::string want;
    char line[128];
    for (int i = 0; i < 120; ++i) {
        std::snprintf(line, sizeof line, "OBJPOS %d %d %.3f %.3f\n", i, i * 7 % 13, i * 0.37, i * i * 0.011);
        want += line;
    }
    const std::string got = qUncompress(fromHex(kDynamic));
    CHECK_EQ(got.size(), want.size());
    CHECK(got == want);
}

TEST_CASE(uncompress_errors)
{
    std::string err;
    CHECK(qUncompress("", &err).empty());
    CHECK(err == "qUncompress: Input data is corrupted");
    err.clear();
    CHECK(qUncompress(std::string(4, '\0'), &err).empty());   // exactly 4 zero bytes: empty, no warning
    CHECK(err.empty());
    std::string good = fromHex("000000177801cb48cdc9c957c8402701680308b1");
    std::string bad = good;
    bad.back() = static_cast<char>(bad.back() ^ 1);                // Adler-32 mismatch
    CHECK(qUncompress(bad, &err).empty());
    CHECK(err == "qUncompress: Z_DATA_ERROR: Input data is corrupted");
    CHECK(qUncompress(good.substr(0, good.size() - 6), &err).empty());   // truncated
    CHECK(err == "qUncompress: Input data is corrupted");
    CHECK(qUncompress(good + "trailing") == "hello hello hello hello");  // bytes after the stream ignored
    std::string hdr = good;
    hdr[5] = static_cast<char>(hdr[5] ^ 0x10);                     // FCHECK broken
    CHECK(qUncompress(hdr, &err).empty());
    CHECK(err == "qUncompress: Z_DATA_ERROR: Input data is corrupted");
}

TEST_CASE(corpus_blobs)
{
    const char* corpus = std::getenv("M2M_CORPUS");
    REQUIRE(corpus != nullptr);
    REQUIRE(std::filesystem::is_directory(corpus));
    int files = 0, blobs = 0;
    for (const auto& dir : std::filesystem::directory_iterator(corpus)) {
        if (!dir.is_directory()) continue;
        for (const auto& f : std::filesystem::directory_iterator(dir.path())) {
            if (f.path().extension() != ".moproj") continue;
            ++files;
            std::ifstream in(f.path(), std::ios::binary);
            const std::string s((std::istreambuf_iterator<char>(in)), std::istreambuf_iterator<char>());
            for (size_t p = s.find("<Data>"); p != std::string::npos; p = s.find("<Data>", p + 1)) {
                const size_t e = s.find("</Data>", p);
                REQUIRE(e != std::string::npos);
                const std::string raw = fromHex(s.substr(p + 6, e - p - 6));
                REQUIRE(raw.size() > 4);
                const size_t want = (size_t(static_cast<unsigned char>(raw[0])) << 24) |
                                    (size_t(static_cast<unsigned char>(raw[1])) << 16) |
                                    (size_t(static_cast<unsigned char>(raw[2])) << 8) |
                                    size_t(static_cast<unsigned char>(raw[3]));
                std::string err;
                const std::string out = qUncompress(raw, &err);
                CHECK(err.empty());
                CHECK_EQ(out.size(), want);
                ++blobs;
            }
        }
    }
    std::printf("corpus: %d .moproj, %d blobs\n", files, blobs);
    CHECK_EQ(files, 56);
    CHECK_EQ(blobs, 598);
}

int main() { return testkit::run_all(); }
