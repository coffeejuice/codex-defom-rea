// qCompress(data, -1) of Qt 4.8.4, whose QtCore4 embeds zlib 1.2.5 ("deflate 1.2.5 Copyright
// 1995-2010" in QtCore4_64.dll): a 4-byte big-endian length, then compress2(level 6) output.
// Packet moproj (.moproj writer 0x1400983c0, blobs of 0x1400a1890).
//
// This is zlib 1.2.5's deflate for one deflateInit2(6, Z_DEFLATED, 15, 8, Z_DEFAULT_STRATEGY) and
// one deflate(Z_FINISH) with room for the whole output (what compress2 does), written function by
// function after deflate.c (fill_window, longest_match without UNALIGNED_OK, deflate_slow) and
// trees.c (tr_static_init, build_tree, gen_bitlen, gen_codes, scan_tree, send_tree,
// build_bl_tree, send_all_trees, compress_block, _tr_flush_block, _tr_stored_block). Level 6 is
// configuration_table[6] = {8, 16, 128, 128, deflate_slow}. The window is zero-initialised, which
// gives the bytes zlib 1.2.5's high_water zeroing gives past the data.
#include <array>
#include <cstdint>
#include <cstring>
#include <string>
#include <vector>

#include "m2m/blob.hpp"

namespace m2m::qt {
namespace {

constexpr int MAX_BITS = 15;
constexpr int MAX_BL_BITS = 7;
constexpr int LENGTH_CODES = 29;
constexpr int LITERALS = 256;
constexpr int L_CODES = LITERALS + 1 + LENGTH_CODES;   // 286
constexpr int D_CODES = 30;
constexpr int BL_CODES = 19;
constexpr int HEAP_SIZE = 2 * L_CODES + 1;
constexpr int END_BLOCK = 256;
constexpr int REP_3_6 = 16, REPZ_3_10 = 17, REPZ_11_138 = 18;
constexpr int STORED_BLOCK = 0, STATIC_TREES = 1, DYN_TREES = 2;
constexpr int MIN_MATCH = 3, MAX_MATCH = 258;
constexpr int MIN_LOOKAHEAD = MAX_MATCH + MIN_MATCH + 1;
constexpr unsigned W_BITS = 15, W_SIZE = 1u << W_BITS, W_MASK = W_SIZE - 1;
constexpr unsigned MAX_DIST = W_SIZE - MIN_LOOKAHEAD;
constexpr unsigned HASH_BITS = 8 + 7, HASH_SIZE = 1u << HASH_BITS, HASH_MASK = HASH_SIZE - 1;
constexpr unsigned HASH_SHIFT = (HASH_BITS + MIN_MATCH - 1) / MIN_MATCH;   // 5
constexpr unsigned LIT_BUFSIZE = 1u << (8 + 6);                          // 16384
constexpr unsigned TOO_FAR = 4096;
constexpr unsigned GOOD_LENGTH = 8, MAX_LAZY = 16, NICE_LENGTH = 128, MAX_CHAIN = 128;

const int extra_lbits[LENGTH_CODES] = {0, 0, 0, 0, 0, 0, 0, 0, 1, 1, 1, 1, 2, 2, 2,
                                       2, 3, 3, 3, 3, 4, 4, 4, 4, 5, 5, 5, 5, 0};
const int extra_dbits[D_CODES] = {0, 0, 0, 0, 1, 1, 2, 2, 3, 3, 4, 4, 5, 5, 6,
                                  6, 7, 7, 8, 8, 9, 9, 10, 10, 11, 11, 12, 12, 13, 13};
const int extra_blbits[BL_CODES] = {0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 2, 3, 7};
const unsigned char bl_order[BL_CODES] = {16, 17, 18, 0, 8, 7, 9, 6, 10, 5, 11, 4, 12, 3, 13, 2, 14, 1, 15};

// ct_data: fc = Freq / Code, dl = Dad / Len (the unions of trees.c)
struct Ct {
    std::uint16_t fc = 0;
    std::uint16_t dl = 0;
};

struct Static {
    Ct ltree[L_CODES + 2];
    Ct dtree[D_CODES];
    unsigned char dist_code[512];
    unsigned char length_code[MAX_MATCH - MIN_MATCH + 1];
    int base_length[LENGTH_CODES];
    int base_dist[D_CODES];
};

unsigned biReverse(unsigned code, int len)
{
    unsigned res = 0;
    do {
        res |= code & 1;
        code >>= 1, res <<= 1;
    } while (--len > 0);
    return res >> 1;
}

void genCodes(Ct* tree, int max_code, const std::uint16_t* bl_count)
{
    std::uint16_t next_code[MAX_BITS + 1];
    unsigned code = 0;
    for (int bits = 1; bits <= MAX_BITS; bits++) next_code[bits] = static_cast<std::uint16_t>(code = (code + bl_count[bits - 1]) << 1);
    for (int n = 0; n <= max_code; n++) {
        int len = tree[n].dl;
        if (len == 0) continue;
        tree[n].fc = static_cast<std::uint16_t>(biReverse(next_code[len]++, len));
    }
}

// tr_static_init
const Static& statics()
{
    static const Static s = [] {
        Static t{};
        int length = 0, code = 0;
        for (code = 0; code < LENGTH_CODES - 1; code++) {
            t.base_length[code] = length;
            for (int n = 0; n < (1 << extra_lbits[code]); n++) t.length_code[length++] = static_cast<unsigned char>(code);
        }
        t.length_code[length - 1] = static_cast<unsigned char>(code);
        int dist = 0;
        for (code = 0; code < 16; code++) {
            t.base_dist[code] = dist;
            for (int n = 0; n < (1 << extra_dbits[code]); n++) t.dist_code[dist++] = static_cast<unsigned char>(code);
        }
        dist >>= 7;
        for (; code < D_CODES; code++) {
            t.base_dist[code] = dist << 7;
            for (int n = 0; n < (1 << (extra_dbits[code] - 7)); n++) t.dist_code[256 + dist++] = static_cast<unsigned char>(code);
        }
        std::uint16_t bl_count[MAX_BITS + 1] = {};
        int n = 0;
        while (n <= 143) t.ltree[n++].dl = 8, bl_count[8]++;
        while (n <= 255) t.ltree[n++].dl = 9, bl_count[9]++;
        while (n <= 279) t.ltree[n++].dl = 7, bl_count[7]++;
        while (n <= 287) t.ltree[n++].dl = 8, bl_count[8]++;
        genCodes(t.ltree, L_CODES + 1, bl_count);
        for (n = 0; n < D_CODES; n++) {
            t.dtree[n].dl = 5;
            t.dtree[n].fc = static_cast<std::uint16_t>(biReverse(static_cast<unsigned>(n), 5));
        }
        return t;
    }();
    return s;
}

struct TreeDesc {
    Ct* dyn_tree;
    int max_code;
    const Ct* static_tree;
    const int* extra_bits;
    int extra_base;
    int elems;
    int max_length;
};

class Deflater {
public:
    explicit Deflater(const std::string& in) : in_(in), st_(statics())
    {
        window_.assign(2 * W_SIZE, 0);
        prev_.assign(W_SIZE, 0);
        head_.assign(HASH_SIZE, 0);
        l_buf_.assign(LIT_BUFSIZE, 0);
        d_buf_.assign(LIT_BUFSIZE, 0);
        l_desc_ = {dyn_ltree_, 0, st_.ltree, extra_lbits, LITERALS + 1, L_CODES, MAX_BITS};
        d_desc_ = {dyn_dtree_, 0, st_.dtree, extra_dbits, 0, D_CODES, MAX_BITS};
        bl_desc_ = {bl_tree_, 0, nullptr, extra_blbits, 0, BL_CODES, MAX_BL_BITS};
        initBlock();
    }

    std::string run()
    {
        // deflate(): zlib header, level 6 -> level_flags 2
        unsigned header = (8 + ((W_BITS - 8) << 4)) << 8;
        header |= 2u << 6;
        header += 31 - (header % 31);
        out_ += static_cast<char>(header >> 8);
        out_ += static_cast<char>(header & 0xff);
        deflateSlow();
        const std::uint32_t a = adler32();
        out_ += static_cast<char>(a >> 24);
        out_ += static_cast<char>((a >> 16) & 0xff);
        out_ += static_cast<char>((a >> 8) & 0xff);
        out_ += static_cast<char>(a & 0xff);
        return out_;
    }

private:
    const std::string& in_;
    std::size_t next_in_ = 0;
    const Static& st_;
    std::string out_;

    std::vector<unsigned char> window_;
    std::vector<std::uint16_t> prev_, head_;
    unsigned ins_h_ = 0;
    long block_start_ = 0;
    unsigned match_length_ = MIN_MATCH - 1, prev_match_ = 0, prev_length_ = MIN_MATCH - 1;
    int match_available_ = 0;
    unsigned strstart_ = 0, match_start_ = 0, lookahead_ = 0;

    Ct dyn_ltree_[HEAP_SIZE];
    Ct dyn_dtree_[2 * D_CODES + 1];
    Ct bl_tree_[2 * BL_CODES + 1];
    TreeDesc l_desc_{}, d_desc_{}, bl_desc_{};
    std::uint16_t bl_count_[MAX_BITS + 1] = {};
    int heap_[2 * L_CODES + 1] = {};
    int heap_len_ = 0, heap_max_ = 0;
    unsigned char depth_[2 * L_CODES + 1] = {};
    std::vector<unsigned char> l_buf_;
    std::vector<std::uint16_t> d_buf_;
    unsigned last_lit_ = 0;
    unsigned long opt_len_ = 0, static_len_ = 0;
    unsigned bi_buf_ = 0;
    int bi_valid_ = 0;

    std::uint32_t adler32() const
    {
        std::uint32_t a = 1, b = 0;
        for (unsigned char c : in_) {
            a = (a + c) % 65521u;
            b = (b + a) % 65521u;
        }
        return (b << 16) | a;
    }

    // ---- bit output (send_bits, bi_windup) -----------------------------------------------------
    void putByte(unsigned c) { out_ += static_cast<char>(c & 0xff); }
    void putShort(unsigned w)
    {
        putByte(w & 0xff);
        putByte((w >> 8) & 0xff);
    }
    void sendBits(unsigned value, int length)
    {
        constexpr int Buf_size = 16;
        if (bi_valid_ > Buf_size - length) {
            bi_buf_ |= (value << bi_valid_) & 0xffff;
            putShort(bi_buf_);
            bi_buf_ = (value & 0xffff) >> (Buf_size - bi_valid_);
            bi_valid_ += length - Buf_size;
        } else {
            bi_buf_ |= (value << bi_valid_) & 0xffff;
            bi_valid_ += length;
        }
    }
    void sendCode(int c, const Ct* tree) { sendBits(tree[c].fc, tree[c].dl); }
    void biWindup()
    {
        if (bi_valid_ > 8) putShort(bi_buf_);
        else if (bi_valid_ > 0) putByte(bi_buf_);
        bi_buf_ = 0;
        bi_valid_ = 0;
    }

    unsigned dCode(unsigned dist) const { return dist < 256 ? st_.dist_code[dist] : st_.dist_code[256 + (dist >> 7)]; }

    // ---- trees.c ----------------------------------------------------------------------------------
    void initBlock()
    {
        for (int n = 0; n < L_CODES; n++) dyn_ltree_[n].fc = 0;
        for (int n = 0; n < D_CODES; n++) dyn_dtree_[n].fc = 0;
        for (int n = 0; n < BL_CODES; n++) bl_tree_[n].fc = 0;
        dyn_ltree_[END_BLOCK].fc = 1;
        opt_len_ = static_len_ = 0;
        last_lit_ = 0;
    }

    static bool smaller(const Ct* tree, int n, int m, const unsigned char* depth)
    {
        return tree[n].fc < tree[m].fc || (tree[n].fc == tree[m].fc && depth[n] <= depth[m]);
    }

    void pqdownheap(const Ct* tree, int k)
    {
        int v = heap_[k];
        int j = k << 1;
        while (j <= heap_len_) {
            if (j < heap_len_ && smaller(tree, heap_[j + 1], heap_[j], depth_)) j++;
            if (smaller(tree, v, heap_[j], depth_)) break;
            heap_[k] = heap_[j];
            k = j;
            j <<= 1;
        }
        heap_[k] = v;
    }

    void genBitlen(TreeDesc& desc)
    {
        Ct* tree = desc.dyn_tree;
        const int max_code = desc.max_code;
        const Ct* stree = desc.static_tree;
        const int* extra = desc.extra_bits;
        const int base = desc.extra_base;
        const int max_length = desc.max_length;
        int h = 0, overflow = 0;
        for (int bits = 0; bits <= MAX_BITS; bits++) bl_count_[bits] = 0;
        tree[heap_[heap_max_]].dl = 0;
        for (h = heap_max_ + 1; h < HEAP_SIZE; h++) {
            const int n = heap_[h];
            int bits = tree[tree[n].dl].dl + 1;
            if (bits > max_length) bits = max_length, overflow++;
            tree[n].dl = static_cast<std::uint16_t>(bits);
            if (n > max_code) continue;
            bl_count_[bits]++;
            int xbits = 0;
            if (n >= base) xbits = extra[n - base];
            const unsigned long f = tree[n].fc;
            opt_len_ += f * static_cast<unsigned long>(bits + xbits);
            if (stree) static_len_ += f * static_cast<unsigned long>(stree[n].dl + xbits);
        }
        if (overflow == 0) return;
        do {
            int bits = max_length - 1;
            while (bl_count_[bits] == 0) bits--;
            bl_count_[bits]--;
            bl_count_[bits + 1] = static_cast<std::uint16_t>(bl_count_[bits + 1] + 2);
            bl_count_[max_length]--;
            overflow -= 2;
        } while (overflow > 0);
        for (int bits = max_length; bits != 0; bits--) {
            int n = bl_count_[bits];
            while (n != 0) {
                const int m = heap_[--h];
                if (m > max_code) continue;
                if (static_cast<unsigned>(tree[m].dl) != static_cast<unsigned>(bits)) {
                    opt_len_ = static_cast<unsigned long>(static_cast<long>(opt_len_) +
                                                          (static_cast<long>(bits) - static_cast<long>(tree[m].dl)) *
                                                              static_cast<long>(tree[m].fc));
                    tree[m].dl = static_cast<std::uint16_t>(bits);
                }
                n--;
            }
        }
    }

    void buildTree(TreeDesc& desc)
    {
        Ct* tree = desc.dyn_tree;
        const Ct* stree = desc.static_tree;
        const int elems = desc.elems;
        int max_code = -1;
        heap_len_ = 0, heap_max_ = HEAP_SIZE;
        for (int n = 0; n < elems; n++) {
            if (tree[n].fc != 0) {
                heap_[++heap_len_] = max_code = n;
                depth_[n] = 0;
            } else {
                tree[n].dl = 0;
            }
        }
        while (heap_len_ < 2) {
            const int node = heap_[++heap_len_] = (max_code < 2 ? ++max_code : 0);
            tree[node].fc = 1;
            depth_[node] = 0;
            opt_len_--;
            if (stree) static_len_ -= stree[node].dl;
        }
        desc.max_code = max_code;
        for (int n = heap_len_ / 2; n >= 1; n--) pqdownheap(tree, n);
        int node = elems;
        do {
            const int n = heap_[1];                      // pqremove
            heap_[1] = heap_[heap_len_--];
            pqdownheap(tree, 1);
            const int m = heap_[1];
            heap_[--heap_max_] = n;
            heap_[--heap_max_] = m;
            tree[node].fc = static_cast<std::uint16_t>(tree[n].fc + tree[m].fc);
            depth_[node] = static_cast<unsigned char>((depth_[n] >= depth_[m] ? depth_[n] : depth_[m]) + 1);
            tree[n].dl = tree[m].dl = static_cast<std::uint16_t>(node);
            heap_[1] = node++;
            pqdownheap(tree, 1);
        } while (heap_len_ >= 2);
        heap_[--heap_max_] = heap_[1];
        genBitlen(desc);
        genCodes(tree, max_code, bl_count_);
    }

    void scanTree(Ct* tree, int max_code)
    {
        int prevlen = -1, curlen, nextlen = tree[0].dl, count = 0, max_count = 7, min_count = 4;
        if (nextlen == 0) max_count = 138, min_count = 3;
        tree[max_code + 1].dl = 0xffff;
        for (int n = 0; n <= max_code; n++) {
            curlen = nextlen;
            nextlen = tree[n + 1].dl;
            if (++count < max_count && curlen == nextlen) continue;
            else if (count < min_count) bl_tree_[curlen].fc = static_cast<std::uint16_t>(bl_tree_[curlen].fc + count);
            else if (curlen != 0) {
                if (curlen != prevlen) bl_tree_[curlen].fc++;
                bl_tree_[REP_3_6].fc++;
            } else if (count <= 10) bl_tree_[REPZ_3_10].fc++;
            else bl_tree_[REPZ_11_138].fc++;
            count = 0;
            prevlen = curlen;
            if (nextlen == 0) max_count = 138, min_count = 3;
            else if (curlen == nextlen) max_count = 6, min_count = 3;
            else max_count = 7, min_count = 4;
        }
    }

    void sendTree(const Ct* tree, int max_code)
    {
        int prevlen = -1, curlen, nextlen = tree[0].dl, count = 0, max_count = 7, min_count = 4;
        if (nextlen == 0) max_count = 138, min_count = 3;
        for (int n = 0; n <= max_code; n++) {
            curlen = nextlen;
            nextlen = tree[n + 1].dl;
            if (++count < max_count && curlen == nextlen) continue;
            else if (count < min_count) {
                do sendCode(curlen, bl_tree_);
                while (--count != 0);
            } else if (curlen != 0) {
                if (curlen != prevlen) {
                    sendCode(curlen, bl_tree_);
                    count--;
                }
                sendCode(REP_3_6, bl_tree_);
                sendBits(static_cast<unsigned>(count - 3), 2);
            } else if (count <= 10) {
                sendCode(REPZ_3_10, bl_tree_);
                sendBits(static_cast<unsigned>(count - 3), 3);
            } else {
                sendCode(REPZ_11_138, bl_tree_);
                sendBits(static_cast<unsigned>(count - 11), 7);
            }
            count = 0;
            prevlen = curlen;
            if (nextlen == 0) max_count = 138, min_count = 3;
            else if (curlen == nextlen) max_count = 6, min_count = 3;
            else max_count = 7, min_count = 4;
        }
    }

    int buildBlTree()
    {
        scanTree(dyn_ltree_, l_desc_.max_code);
        scanTree(dyn_dtree_, d_desc_.max_code);
        buildTree(bl_desc_);
        int max_blindex;
        for (max_blindex = BL_CODES - 1; max_blindex >= 3; max_blindex--)
            if (bl_tree_[bl_order[max_blindex]].dl != 0) break;
        opt_len_ += 3 * (static_cast<unsigned long>(max_blindex) + 1) + 5 + 5 + 4;
        return max_blindex;
    }

    void sendAllTrees(int lcodes, int dcodes, int blcodes)
    {
        sendBits(static_cast<unsigned>(lcodes - 257), 5);
        sendBits(static_cast<unsigned>(dcodes - 1), 5);
        sendBits(static_cast<unsigned>(blcodes - 4), 4);
        for (int rank = 0; rank < blcodes; rank++) sendBits(bl_tree_[bl_order[rank]].dl, 3);
        sendTree(dyn_ltree_, lcodes - 1);
        sendTree(dyn_dtree_, dcodes - 1);
    }

    void compressBlock(const Ct* ltree, const Ct* dtree)
    {
        unsigned lx = 0;
        if (last_lit_ != 0) do {
                unsigned dist = d_buf_[lx];
                int lc = l_buf_[lx++];
                if (dist == 0) {
                    sendCode(lc, ltree);
                } else {
                    unsigned code = st_.length_code[lc];
                    sendCode(static_cast<int>(code + LITERALS + 1), ltree);
                    int extra = extra_lbits[code];
                    if (extra != 0) {
                        lc -= st_.base_length[code];
                        sendBits(static_cast<unsigned>(lc), extra);
                    }
                    dist--;
                    code = dCode(dist);
                    sendCode(static_cast<int>(code), dtree);
                    extra = extra_dbits[code];
                    if (extra != 0) {
                        dist -= static_cast<unsigned>(st_.base_dist[code]);
                        sendBits(dist, extra);
                    }
                }
            } while (lx < last_lit_);
        sendCode(END_BLOCK, ltree);
    }

    void storedBlock(const unsigned char* buf, unsigned long stored_len, int last)
    {
        sendBits(static_cast<unsigned>((STORED_BLOCK << 1) + last), 3);
        biWindup();                                          // copy_block(header = 1)
        putShort(static_cast<unsigned>(stored_len));
        putShort(static_cast<unsigned>(~stored_len));
        out_.append(reinterpret_cast<const char*>(buf), stored_len);
    }

    void flushBlock(const unsigned char* buf, unsigned long stored_len, int last)
    {
        buildTree(l_desc_);
        buildTree(d_desc_);
        const int max_blindex = buildBlTree();
        unsigned long opt_lenb = (opt_len_ + 3 + 7) >> 3;
        const unsigned long static_lenb = (static_len_ + 3 + 7) >> 3;
        if (static_lenb <= opt_lenb) opt_lenb = static_lenb;
        if (stored_len + 4 <= opt_lenb && buf != nullptr) {
            storedBlock(buf, stored_len, last);
        } else if (static_lenb == opt_lenb) {
            sendBits(static_cast<unsigned>((STATIC_TREES << 1) + last), 3);
            compressBlock(st_.ltree, st_.dtree);
        } else {
            sendBits(static_cast<unsigned>((DYN_TREES << 1) + last), 3);
            sendAllTrees(l_desc_.max_code + 1, d_desc_.max_code + 1, max_blindex + 1);
            compressBlock(dyn_ltree_, dyn_dtree_);
        }
        initBlock();
        if (last) biWindup();
    }

    // FLUSH_BLOCK_ONLY
    void flushBlockOnly(int last)
    {
        flushBlock(block_start_ >= 0 ? &window_[static_cast<std::size_t>(block_start_)] : nullptr,
                   static_cast<unsigned long>(static_cast<long>(strstart_) - block_start_), last);
        block_start_ = static_cast<long>(strstart_);
    }

    bool tallyLit(unsigned c)
    {
        d_buf_[last_lit_] = 0;
        l_buf_[last_lit_++] = static_cast<unsigned char>(c);
        dyn_ltree_[c].fc++;
        return last_lit_ == LIT_BUFSIZE - 1;
    }

    bool tallyDist(unsigned distance, unsigned length)
    {
        d_buf_[last_lit_] = static_cast<std::uint16_t>(distance);
        l_buf_[last_lit_++] = static_cast<unsigned char>(length);
        distance--;
        dyn_ltree_[st_.length_code[length] + LITERALS + 1].fc++;
        dyn_dtree_[dCode(distance)].fc++;
        return last_lit_ == LIT_BUFSIZE - 1;
    }

    // ---- deflate.c ------------------------------------------------------------------------------
    void updateHash(unsigned c) { ins_h_ = ((ins_h_ << HASH_SHIFT) ^ c) & HASH_MASK; }

    unsigned insertString(unsigned str)
    {
        updateHash(window_[str + (MIN_MATCH - 1)]);
        const unsigned match_head = prev_[str & W_MASK] = head_[ins_h_];
        head_[ins_h_] = static_cast<std::uint16_t>(str);
        return match_head;
    }

    void fillWindow()
    {
        do {
            unsigned more = 2 * W_SIZE - lookahead_ - strstart_;
            if (strstart_ >= W_SIZE + MAX_DIST) {
                std::memcpy(window_.data(), window_.data() + W_SIZE, W_SIZE);
                match_start_ -= W_SIZE;
                strstart_ -= W_SIZE;
                block_start_ -= static_cast<long>(W_SIZE);
                for (auto& m : head_) m = static_cast<std::uint16_t>(m >= W_SIZE ? m - W_SIZE : 0);
                for (auto& m : prev_) m = static_cast<std::uint16_t>(m >= W_SIZE ? m - W_SIZE : 0);
                more += W_SIZE;
            }
            const std::size_t avail = in_.size() - next_in_;
            if (avail == 0) return;
            const unsigned n = static_cast<unsigned>(avail < more ? avail : more);   // read_buf
            std::memcpy(window_.data() + strstart_ + lookahead_, in_.data() + next_in_, n);
            next_in_ += n;
            lookahead_ += n;
            if (lookahead_ >= MIN_MATCH) {
                ins_h_ = window_[strstart_];
                updateHash(window_[strstart_ + 1]);
            }
        } while (lookahead_ < MIN_LOOKAHEAD && next_in_ < in_.size());
    }

    unsigned longestMatch(unsigned cur_match)
    {
        unsigned chain_length = MAX_CHAIN;
        const unsigned char* const w = window_.data();
        const unsigned char* scan = w + strstart_;
        int best_len = static_cast<int>(prev_length_);
        unsigned nice_match = NICE_LENGTH;
        const unsigned limit = strstart_ > MAX_DIST ? strstart_ - MAX_DIST : 0;
        const unsigned char* const strend = w + strstart_ + MAX_MATCH;
        unsigned char scan_end1 = scan[best_len - 1];
        unsigned char scan_end = scan[best_len];
        if (prev_length_ >= GOOD_LENGTH) chain_length >>= 2;
        if (nice_match > lookahead_) nice_match = lookahead_;
        do {
            const unsigned char* match = w + cur_match;
            if (match[best_len] != scan_end || match[best_len - 1] != scan_end1 || *match != *scan || *++match != scan[1])
                continue;
            scan += 2, match++;
            do {
            } while (*++scan == *++match && *++scan == *++match && *++scan == *++match && *++scan == *++match &&
                     *++scan == *++match && *++scan == *++match && *++scan == *++match && *++scan == *++match &&
                     scan < strend);
            const int len = MAX_MATCH - static_cast<int>(strend - scan);
            scan = strend - MAX_MATCH;
            if (len > best_len) {
                match_start_ = cur_match;
                best_len = len;
                if (static_cast<unsigned>(len) >= nice_match) break;
                scan_end1 = scan[best_len - 1];
                scan_end = scan[best_len];
            }
        } while ((cur_match = prev_[cur_match & W_MASK]) > limit && --chain_length != 0);
        if (static_cast<unsigned>(best_len) <= lookahead_) return static_cast<unsigned>(best_len);
        return lookahead_;
    }

    void deflateSlow()
    {
        for (;;) {
            if (lookahead_ < MIN_LOOKAHEAD) {
                fillWindow();
                if (lookahead_ == 0) break;            // flush is Z_FINISH
            }
            unsigned hash_head = 0;
            if (lookahead_ >= MIN_MATCH) hash_head = insertString(strstart_);
            prev_length_ = match_length_, prev_match_ = match_start_;
            match_length_ = MIN_MATCH - 1;
            if (hash_head != 0 && prev_length_ < MAX_LAZY && strstart_ - hash_head <= MAX_DIST) {
                match_length_ = longestMatch(hash_head);
                if (match_length_ <= 5 && (match_length_ == MIN_MATCH && strstart_ - match_start_ > TOO_FAR))
                    match_length_ = MIN_MATCH - 1;
            }
            if (prev_length_ >= MIN_MATCH && match_length_ <= prev_length_) {
                const unsigned max_insert = strstart_ + lookahead_ - MIN_MATCH;
                const bool bflush = tallyDist(strstart_ - 1 - prev_match_, prev_length_ - MIN_MATCH);
                lookahead_ -= prev_length_ - 1;
                prev_length_ -= 2;
                do {
                    if (++strstart_ <= max_insert) insertString(strstart_);
                } while (--prev_length_ != 0);
                match_available_ = 0;
                match_length_ = MIN_MATCH - 1;
                strstart_++;
                if (bflush) flushBlockOnly(0);
            } else if (match_available_) {
                const bool bflush = tallyLit(window_[strstart_ - 1]);
                if (bflush) flushBlockOnly(0);
                strstart_++;
                lookahead_--;
            } else {
                match_available_ = 1;
                strstart_++;
                lookahead_--;
            }
        }
        if (match_available_) {
            tallyLit(window_[strstart_ - 1]);
            match_available_ = 0;
        }
        flushBlockOnly(1);
    }
};

}  // namespace

std::string qCompress(const std::string& data)
{
    std::string r;
    if (data.empty()) return std::string(4, char(0));
    const std::uint32_t n = static_cast<std::uint32_t>(data.size());
    r += static_cast<char>(n >> 24);
    r += static_cast<char>((n >> 16) & 0xff);
    r += static_cast<char>((n >> 8) & 0xff);
    r += static_cast<char>(n & 0xff);
    Deflater d(data);
    r += d.run();
    return r;
}

std::string toHex(const std::string& bytes)
{
    static const char hexd[] = "0123456789abcdef";
    std::string r;
    r.reserve(bytes.size() * 2);
    for (unsigned char c : bytes) {
        r += hexd[c >> 4];
        r += hexd[c & 15];
    }
    return r;
}

}  // namespace m2m::qt
