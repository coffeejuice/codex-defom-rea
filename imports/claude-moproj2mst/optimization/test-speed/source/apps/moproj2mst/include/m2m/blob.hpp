// m2m/blob.hpp: the hex + zlib blobs of a .moproj (owner P01; used by P01 ParseMoItem and
// P02 storeBlob 0x1400937b0). Both readers do qUncompress(QByteArray::fromHex(text.toAscii())).
// The inflate is self-contained (no zlib in the repo); it can be swapped for zlib's uncompress()
// without changing the callers.
#pragma once

#include <string>

namespace m2m::qt {

// QByteArray::fromHex (Qt 4.8): scans from the end, skips every non-hex byte, pairs digits from
// the right; an odd count leaves the first digit as a low nibble.
std::string fromHex(const std::string& hex);

// qUncompress(data, n) (Qt 4.8): 4-byte big-endian size hint, then a zlib stream decoded with
// uncompress(). Returns "" (Qt: empty QByteArray, with a qWarning) when n <= 4 or the stream is
// corrupt, truncated or fails its Adler-32 check. Bytes after the end of the zlib stream are
// ignored, as uncompress() does. `error` (optional) receives Qt's warning text.
std::string qUncompress(const std::string& data, std::string* error = nullptr);

// ---- added by the .moproj writer (src/moproj/deflate.cpp) ---------------------------------------
// qCompress(data, -1) (Qt 4.8.4 with its zlib 1.2.5): 4-byte big-endian length, then compress2 at
// level 6; an empty input gives four zero bytes.
std::string qCompress(const std::string& data);
// QByteArray::toHex: two lower-case digits per byte
std::string toHex(const std::string& bytes);

}  // namespace m2m::qt
