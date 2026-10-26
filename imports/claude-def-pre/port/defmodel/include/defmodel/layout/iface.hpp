// IFace (RTTI .?AVIFace@@): abstract face interface of the 2D topology; _DEF_FACE implements
// it. No data and no virtual destructor (the deleting dtor is _DEF_FACE slot 13).
//
//   vtable   0x1405489e8 (13 slots, all _purecall)
//   ctor     1402f8320 (vptr only)
//   size     0x8
#pragma once

#include <cstddef>
#include <cstdint>

#include <defmodel/geo/geo_alloc.hpp>
#include <defmodel/layout/layout_check.hpp>
#include <defmodel/sdctl/cow.hpp>

namespace def::model {

class ILoop;

class IFace : public geo::MallocObject {
public:
    // @src 1402f8320  ctor
    IFace();

    virtual void vslot0(void* a, void* b) = 0;                 // slot 0
    virtual void vslot1(double a, double b, bool c) = 0;       // slot 1
    virtual void vslot2() = 0;                                 // slot 2
    virtual CowVector<double>* coords() = 0;                   // slot 3
    virtual CowVector<std::int64_t>* u28_vector() = 0;         // slot 4
    virtual CowVector<std::int64_t>* u30_vector() = 0;         // slot 5
    virtual CowVector<std::int64_t>* u38_vector() = 0;         // slot 6
    virtual void vslot7(void* a) = 0;                          // slot 7
    virtual bool flag18() const = 0;                           // slot 8
    virtual void set_flag18(bool v) = 0;                       // slot 9
    virtual bool flag19() const = 0;                           // slot 10
    virtual void set_flag19(bool v) = 0;                       // slot 11
    virtual ILoop* loop_at(const std::int32_t* i) = 0;         // slot 12

protected:
    ~IFace() = default;
    // vptr at 0x000
};

static_assert(sizeof(IFace) == 0x8);

} // namespace def::model
