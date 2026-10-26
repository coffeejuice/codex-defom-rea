// QByteArray::fromHex and qUncompress (Qt 4.8 behaviour) with a self-contained inflate (RFC 1950
// zlib wrapper around RFC 1951 deflate), packet P01.
#include <algorithm>
#include <cstdint>
#include <string>

#include "m2m/blob.hpp"

namespace m2m::qt {

std::string fromHex(const std::string& hex)
{
    std::string res((hex.size() + 1) / 2, '\0');
    size_t out = res.size();
    bool oddDigit = true;
    for (size_t i = hex.size(); i-- > 0;) {
        const int ch = static_cast<unsigned char>(hex[i]);
        int tmp;
        if (ch >= '0' && ch <= '9') tmp = ch - '0';
        else if (ch >= 'a' && ch <= 'f') tmp = ch - 'a' + 10;
        else if (ch >= 'A' && ch <= 'F') tmp = ch - 'A' + 10;
        else continue;
        if (oddDigit) {
            --out;
            res[out] = static_cast<char>(tmp);
            oddDigit = false;
        } else {
            res[out] = static_cast<char>(static_cast<unsigned char>(res[out]) | (tmp << 4));
            oddDigit = true;
        }
    }
    return res.substr(out);
}

namespace {

// zlib's uncompress() results that matter for qUncompress.
enum class Z { Ok, DataError, Truncated };

class Inflater {
public:
    Inflater(const unsigned char* p, size_t n) : in_(p), n_(n) {}

    Z run(std::string& out)
    {
        // RFC 1950 header
        if (n_ < 2) return Z::Truncated;
        const unsigned cmf = in_[0], flg = in_[1];
        if ((cmf & 0x0f) != 8 || (cmf >> 4) > 7 || ((cmf << 8) | flg) % 31 != 0) return Z::DataError;
        if (flg & 0x20) return Z::DataError;   // preset dictionary: uncompress() -> Z_DATA_ERROR
        pos_ = 2;
        int last;
        do {
            int b;
            if (!bits(1, last) || !bits(2, b)) return Z::Truncated;
            Z r;
            if (b == 0) r = stored(out);
            else if (b == 1) r = fixed(out);
            else if (b == 2) r = dynamic(out);
            else return Z::DataError;
            if (r != Z::Ok) return r;
        } while (!last);
        // Adler-32, big-endian, byte aligned
        bitCount_ = 0;
        if (pos_ + 4 > n_) return Z::Truncated;
        std::uint32_t want = (std::uint32_t(in_[pos_]) << 24) | (std::uint32_t(in_[pos_ + 1]) << 16) |
                             (std::uint32_t(in_[pos_ + 2]) << 8) | std::uint32_t(in_[pos_ + 3]);
        std::uint32_t a = 1, b2 = 0;
        for (unsigned char c : out) {
            a = (a + c) % 65521;
            b2 = (b2 + a) % 65521;
        }
        if (((b2 << 16) | a) != want) return Z::DataError;
        return Z::Ok;
    }

private:
    struct Huffman {
        short count[16];
        short symbol[320];
    };

    const unsigned char* in_;
    size_t n_;
    size_t pos_ = 0;
    std::uint32_t bitBuf_ = 0;
    int bitCount_ = 0;

    bool bits(int need, int& v)
    {
        std::uint32_t val = bitBuf_;
        while (bitCount_ < need) {
            if (pos_ >= n_) return false;
            val |= std::uint32_t(in_[pos_++]) << bitCount_;
            bitCount_ += 8;
        }
        bitBuf_ = val >> need;
        bitCount_ -= need;
        v = static_cast<int>(val & ((1u << need) - 1));
        return true;
    }

    Z stored(std::string& out)
    {
        bitBuf_ = 0;
        bitCount_ = 0;
        if (pos_ + 4 > n_) return Z::Truncated;
        unsigned len = in_[pos_] | (in_[pos_ + 1] << 8);
        unsigned nlen = in_[pos_ + 2] | (in_[pos_ + 3] << 8);
        pos_ += 4;
        if (len != (~nlen & 0xffff)) return Z::DataError;
        if (pos_ + len > n_) return Z::Truncated;
        out.append(reinterpret_cast<const char*>(in_ + pos_), len);
        pos_ += len;
        return Z::Ok;
    }

    // Returns the symbol, -1 when input ran out, -2 for an invalid code.
    int decode(const Huffman& h)
    {
        int code = 0, first = 0, index = 0;
        for (int len = 1; len <= 15; ++len) {
            int b;
            if (!bits(1, b)) return -1;
            code |= b;
            int count = h.count[len];
            if (code - count < first) return h.symbol[index + (code - first)];
            index += count;
            first += count;
            first <<= 1;
            code <<= 1;
        }
        return -2;
    }

    // 0 complete, <0 over-subscribed, >0 incomplete
    static int construct(Huffman& h, const short* length, int n)
    {
        for (auto& c : h.count) c = 0;
        for (int s = 0; s < n; ++s) h.count[length[s]]++;
        if (h.count[0] == n) return 0;
        int left = 1;
        for (int len = 1; len <= 15; ++len) {
            left <<= 1;
            left -= h.count[len];
            if (left < 0) return left;
        }
        short offs[16];
        offs[1] = 0;
        for (int len = 1; len < 15; ++len) offs[len + 1] = static_cast<short>(offs[len] + h.count[len]);
        for (int s = 0; s < n; ++s)
            if (length[s] != 0) h.symbol[offs[length[s]]++] = static_cast<short>(s);
        return left;
    }

    Z codes(std::string& out, const Huffman& lencode, const Huffman& distcode)
    {
        static const short lbase[29] = {3, 4, 5, 6, 7, 8, 9, 10, 11, 13, 15, 17, 19, 23, 27, 31,
                                        35, 43, 51, 59, 67, 83, 99, 115, 131, 163, 195, 227, 258};
        static const short lext[29] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2, 2,
                                       3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
        static const short dbase[30] = {1, 2, 3, 4, 5, 7, 9, 13, 17, 25, 33, 49, 65, 97, 129,
                                        193, 257, 385, 513, 769, 1025, 1537, 2049, 3073, 4097,
                                        6145, 8193, 12289, 16385, 24577};
        static const short dext[30] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
                                       6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
        for (;;) {
            int sym = decode(lencode);
            if (sym == -1) return Z::Truncated;
            if (sym < 0) return Z::DataError;
            if (sym < 256) {
                out += static_cast<char>(sym);
            } else if (sym == 256) {
                return Z::Ok;
            } else {
                sym -= 257;
                if (sym >= 29) return Z::DataError;
                int e;
                if (!bits(lext[sym], e)) return Z::Truncated;
                int len = lbase[sym] + e;
                int ds = decode(distcode);
                if (ds == -1) return Z::Truncated;
                if (ds < 0 || ds >= 30) return Z::DataError;
                if (!bits(dext[ds], e)) return Z::Truncated;
                size_t dist = static_cast<size_t>(dbase[ds] + e);
                if (dist > out.size()) return Z::DataError;
                size_t from = out.size() - dist;
                for (int k = 0; k < len; ++k) out += out[from + static_cast<size_t>(k)];
            }
        }
    }

    Z fixed(std::string& out)
    {
        struct Tables {
            Huffman len, dist;
        };
        // Built once; static initialisation is thread-safe (the DLL is called from several threads).
        static const Tables t = [] {
            Tables r{};
            short lengths[288];
            int s = 0;
            for (; s < 144; ++s) lengths[s] = 8;
            for (; s < 256; ++s) lengths[s] = 9;
            for (; s < 280; ++s) lengths[s] = 7;
            for (; s < 288; ++s) lengths[s] = 8;
            construct(r.len, lengths, 288);
            for (s = 0; s < 30; ++s) lengths[s] = 5;
            construct(r.dist, lengths, 30);
            return r;
        }();
        return codes(out, t.len, t.dist);
    }

    Z dynamic(std::string& out)
    {
        static const short order[19] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};
        int nlen, ndist, ncode;
        if (!bits(5, nlen) || !bits(5, ndist) || !bits(4, ncode)) return Z::Truncated;
        nlen += 257;
        ndist += 1;
        ncode += 4;
        if (nlen > 286 || ndist > 30) return Z::DataError;
        short lengths[320] = {};
        for (int i = 0; i < ncode; ++i) {
            int v;
            if (!bits(3, v)) return Z::Truncated;
            lengths[order[i]] = static_cast<short>(v);
        }
        Huffman lencode, distcode;
        if (construct(lencode, lengths, 19) != 0) return Z::DataError;
        for (int i = 0; i < 19; ++i) lengths[i] = 0;
        int index = 0;
        while (index < nlen + ndist) {
            int sym = decode(lencode);
            if (sym == -1) return Z::Truncated;
            if (sym < 0) return Z::DataError;
            if (sym < 16) {
                lengths[index++] = static_cast<short>(sym);
            } else {
                short len = 0;
                int rep;
                if (sym == 16) {
                    if (index == 0) return Z::DataError;
                    len = lengths[index - 1];
                    if (!bits(2, rep)) return Z::Truncated;
                    rep += 3;
                } else if (sym == 17) {
                    if (!bits(3, rep)) return Z::Truncated;
                    rep += 3;
                } else {
                    if (!bits(7, rep)) return Z::Truncated;
                    rep += 11;
                }
                if (index + rep > nlen + ndist) return Z::DataError;
                while (rep--) lengths[index++] = len;
            }
        }
        if (lengths[256] == 0) return Z::DataError;
        int err = construct(lencode, lengths, nlen);
        if (err < 0 || (err > 0 && nlen - lencode.count[0] != 1)) return Z::DataError;
        err = construct(distcode, lengths + nlen, ndist);
        if (err < 0 || (err > 0 && ndist - distcode.count[0] != 1)) return Z::DataError;
        return codes(out, lencode, distcode);
    }
};

}  // namespace

std::string qUncompress(const std::string& data, std::string* error)
{
    auto warn = [&](const char* m) {
        if (error) *error = m;
        return std::string();
    };
    const auto* d = reinterpret_cast<const unsigned char*>(data.data());
    const size_t n = data.size();
    if (n <= 4) {
        if (n < 4 || d[0] != 0 || d[1] != 0 || d[2] != 0 || d[3] != 0)
            return warn("qUncompress: Input data is corrupted");
        return std::string();
    }
    std::string out;
    out.reserve(std::max<size_t>(1, (size_t(d[0]) << 24) | (size_t(d[1]) << 16) | (size_t(d[2]) << 8) | d[3]) &
                0xffffff);
    switch (Inflater(d + 4, n - 4).run(out)) {
    case Z::Ok: return out;
    // uncompress() reports a truncated stream as Z_BUF_ERROR; qUncompress then doubles its buffer
    // until the size limit and gives up with this message.
    case Z::Truncated: return warn("qUncompress: Input data is corrupted");
    case Z::DataError: break;
    }
    return warn("qUncompress: Z_DATA_ERROR: Input data is corrupted");
}

}  // namespace m2m::qt
