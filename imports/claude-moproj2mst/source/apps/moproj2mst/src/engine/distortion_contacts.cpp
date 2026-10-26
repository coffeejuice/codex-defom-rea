#include "distortion_contacts.hpp"

#include "m2m/data/distortion.hpp"

namespace m2m::engine {

// Verified PE fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc,
// 0x141364140..0x1413642c3. See analysis/resume48-distortion-vt198-rea.json.
// This body does not inspect engine objects, dimensions, or contact properties.
std::vector<ContactPair> distortionDefaultPairs(Operation& op)
{
    std::vector<ContactPair> out;                          // 0x14018f9e0
    ContactPair pair;                                    // 0x1400657a0
    const auto* props = dynamic_cast<const DistortionProps*>(op.props.get());
    if (!props || !props->data)
        throw Error(M2M_E_UNSUPPORTED,
                    "Distortion vt+0x198 requires Distortion properties and pass data "
                    "(0x141364140); original null access is outside the recovered valid state");
    const data::DistortionData& data = *props->data;        // props+0x40
    const int count = data.fixtureCount;                  // 0x14142ff20: data+4
    // The native getter is unchecked. Preserve the public unsupported boundary
    // for counts that cannot be represented by the recovered fixed fixture array.
    if (count < 0 || static_cast<std::size_t>(count) > data.fixtures.size())
        throw Error(M2M_E_UNSUPPORTED,
                    "Distortion vt+0x198 fixture count is outside 0..100 (0x141365210)");
    const int workpiece = data.workpiece;                 // 0x1413651b0: data+0xb4
    if (workpiece <= 0) return out;                       // signed gate at 0x141364216
    for (int i = 0; i < count; ++i) {
        pair.master = data.fixtures[static_cast<std::size_t>(i)]; // 0x141365210: +0xb8+4*i
        pair.slave = workpiece;
        pair.lub = 1;                                    // 0x141364270: pair+0x18
        // friction/IHTC remain zero; pair+0x19 remains the constructor's one.
        out.push_back(pair);                             // 0x1400ad3b0; no ID filtering
    }
    return out;
}

} // namespace m2m::engine
