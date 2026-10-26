// DEF_FACE = RTTI .?AV_DEF_FACE@@ (_DEF_FACE, base IFace): one face of a 2D cross-section
// (GEO2D_AND_MESH2D::faces), bounded by loops.
//
//   vtable   0x140548968 (15 slots: the 13 of IFace, scalar deleting dtor 1402f82e0, 1402f7ce0)
//   ctor     1402f7b20
//   dtor     1402f7be0 (P2; unlinks itself from each _DEF_LOOP, member dtors in reverse order)
//   size     0x48 (1402838d0: operator new(0x48))
//   open     element types of the three vectors at 0x28..0x38 (8-byte elements assumed; no
//            Phase 1 code reads them).
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/layout/iface.hpp>
#include <defmodel/layout/layout_check.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {

class DEF_FACE : public IFace {
public:
    // @src 1402f7b20  ctor
    DEF_FACE();
    DEF_FACE(const DEF_FACE&) = delete;
    DEF_FACE& operator=(const DEF_FACE&) = delete;

    // @src 1402f7d30  slot 0: no-op
    void vslot0(void* a, void* b) override;
    // @src 1402f7d50  slot 1: no-op
    void vslot1(double a, double b, bool c) override;
    // @src 1402f7d00  slot 2: no-op
    void vslot2() override;
    // @src 1402f7d70  slot 3: this + 0x20
    CowVector<double>* coords() override;
    // @src 1402f7d90  slot 4: this + 0x28
    CowVector<std::int64_t>* u28_vector() override;
    // @src 1402f7db0  slot 5: this + 0x30
    CowVector<std::int64_t>* u30_vector() override;
    // @src 1402f7dd0  slot 6: this + 0x38
    CowVector<std::int64_t>* u38_vector() override;
    // @src 1402f7d10  slot 7: no-op
    void vslot7(void* a) override;
    // @src 1402f8280  slot 8
    bool flag18() const override;
    // @src 1402f8290  slot 9
    void set_flag18(bool v) override;
    // @src 1402f82b0  slot 10
    bool flag19() const override;
    // @src 1402f82c0  slot 11
    void set_flag19(bool v) override;
    // @src 1402f7df0  slot 12: loop i (0 out of range)
    ILoop* loop_at(const std::int32_t* i) override;
    // @src 1402f82e0  slot 13: scalar deleting dtor (dtor body 1402f7be0)
    virtual ~DEF_FACE();
    // @src 1402f7ce0  slot 14: no-op
    virtual void vslot14(void* a);

    // @src 1402f7e80  append a loop
    void add_loop(ILoop* loop);
    // @src 1402f7f90  0x40 = *v
    void set_u40(const std::int32_t* v);
    // @src 1402f7fb0  0x40
    std::int32_t u40_value() const;

    // vptr at 0x000 (IFace)
    CowList<ILoop*> loops;          // 0x008 (140284c20)
    void* u10;                      // 0x010 ctor = 0
    bool u18;                       // 0x018 slots 8/9
    bool u19;                       // 0x019 slots 10/11
    std::byte pad_01a[6];           // 0x01a
    CowVector<double> u20;          // 0x020 (14019a6b0)
    CowVector<std::int64_t> u28;    // 0x028 (1402f8400)
    CowVector<std::int64_t> u30;    // 0x030 (1402f8400)
    CowVector<std::int64_t> u38;    // 0x038 (1402f8490)
    std::int32_t u40;               // 0x040 ctor = 0
    std::int32_t id;                // 0x044 1-based face number (140256710; set by p1-08)
};

static_assert(sizeof(DEF_FACE) == 0x48);
DEF_LAYOUT_OFFSET(DEF_FACE, loops, 0x08);
DEF_LAYOUT_OFFSET(DEF_FACE, u10, 0x10);
DEF_LAYOUT_OFFSET(DEF_FACE, u18, 0x18);
DEF_LAYOUT_OFFSET(DEF_FACE, u20, 0x20);
DEF_LAYOUT_OFFSET(DEF_FACE, u38, 0x38);
DEF_LAYOUT_OFFSET(DEF_FACE, u40, 0x40);
DEF_LAYOUT_OFFSET(DEF_FACE, id, 0x44);

} // namespace def::model
