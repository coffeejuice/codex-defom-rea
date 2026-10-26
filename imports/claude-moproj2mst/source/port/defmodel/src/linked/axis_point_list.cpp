// List of {id, point, direction} records (defmodel/linked/axis_point_list.hpp).
#include <defmodel/linked/axis_point_list.hpp>

#include <cmath>

#include <defmodel/members/ftn_geom.hpp>
#include <defmodel/port.hpp>
#include <defrt/ftn/helpers.hpp>

namespace def::model::linked {

namespace {

// Calls into other packets, wired at the Phase 1 gate: 140325691 (p1-10) rotates the
// 3-vector v in place about the axis (a, b) by *angle degrees; 14023b1f0 / 14023b720 are p1-12's
// vector difference and normalise (defrt).
void rotate_vector(double* v, const double* a, const double* b, const double* angle)
{
    model::fun_140325691(v, a, b, angle);
}

} // namespace

// ---- node ---------------------------------------------------------------------------------------

// @src 140257340
Cls_140257340::Cls_140257340()
{
    id = 0;
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        direction[i] = 0.0;
        point[i] = 0.0;
    }
    next = nullptr;
}

// @src 140257440
// @src 14025bf00
Cls_140257340::~Cls_140257340() = default;

// @src 1402573c0
void Cls_140257340::copy_from(const Cls_140257340* other)
{
    id = other->id;
    for (std::int32_t i = 0; i < 3; i = i + 1) {
        point[i] = other->point[i];
        direction[i] = other->direction[i];
    }
}

// @src 140257750
void Cls_140257340::rotate(const double* a, const double* b, double angle)
{
    double origin[3] = {0.0, 0.0, 0.0};
    if (a != nullptr && b != nullptr) {
        rotate_vector(point, a, b, &angle);
        rotate_vector(origin, a, b, &angle);
        rotate_vector(direction, a, b, &angle);
        rt::ftn::vec3_sub(direction, origin, direction); // 14023b1f0
        rt::ftn::vec3_normalize(direction);             // 14023b720
    }
}

// ---- list ---------------------------------------------------------------------------------------

// @src 140257c60
Cls_140257340List::~Cls_140257340List() { clear(); }

// @src 140257e90
Cls_140257340* Cls_140257340List::add(std::int32_t id)
{
    Cls_140257340* node = new Cls_140257340(); // operator new(0x40), 140257340
    if (count == 0) {
        tail = node;
        head = node;
    } else {
        tail->next = node;
        tail = node;
    }
    count = count + 1;
    node->id = id;
    return node;
}

// @src 1402581b0
void Cls_140257340List::clear()
{
    Cls_140257340* node = head;
    while (node != nullptr) {
        Cls_140257340* following = node->next;
        delete node; // 14025bf00(node, 1)
        node = following;
    }
    tail = nullptr;
    head = nullptr;
    count = 0;
}

// @src 140258090
Cls_140257340* Cls_140257340List::nth(std::int32_t k) const
{
    std::int32_t i = 0;
    Cls_140257340* node = head;
    while (node != nullptr && (i = i + 1, i < k)) {
        node = node->next;
    }
    return node;
}

// @src 140257c80
void Cls_140257340List::copy_from(const Cls_140257340List& other)
{
    clear();
    for (std::int32_t i = 0; i < other.size(); i = i + 1) {
        Cls_140257340* node = add(0);
        node->copy_from(other.nth(i + 1));
        node->next = nullptr;
    }
}

// @src 140258400
void Cls_140257340List::rotate_all(const double* a, const double* b, double angle)
{
    for (std::int32_t i = 0; i < size(); i = i + 1) {
        Cls_140257340* node = nth(i + 1);
        if (node != nullptr) {
            node->rotate(a, b, angle);
        }
    }
}

} // namespace def::model::linked
