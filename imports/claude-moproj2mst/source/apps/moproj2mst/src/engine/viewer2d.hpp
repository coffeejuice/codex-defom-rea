// The 2D items of the 3D viewer (packet V extension, packet moproj SfDispView): the point lists the
// curve items (SfGL2DCurveItem, ctor 0x1412671d0) and the mesh item (ctor 0x14124bb30) build when a
// 2D object is bound, and the Rect2 their vt+0x48 computes from them. Spec:
// docs/re/moproj2mst/moproj/dispview.md.
#pragma once

#include <cstdint>
#include <vector>

namespace def::model {
class LLOBJ_DATA;
class OBJ_GEO2D;
}  // namespace def::model

namespace m2m::engine {

// Rect2 (0x40 bytes; ctor 0x141238b90): valid, min (+0x08), max (+0x18) as (first, second) doubles;
// the derived centre (+0x30) and radius (+0x28) of 0x141239060 are not kept (no reader on this path)
struct Rect2 {
    bool valid = false;
    double mn[2] = {0.0, 0.0};
    double mx[2] = {0.0, 0.0};
};

// @src 0x141238d00  reset: invalid, points (0, 0)
void rectReset(Rect2& r);
// @src 0x141238da0  extend by a point of doubles
void rectExtend(Rect2& r, const double* p);
// @src 0x141238f00  extend by a point of floats (widened)
void rectExtend(Rect2& r, const float* p);
// @src 0x141239830  union: an invalid side gives the other
Rect2 rectUnion(const Rect2& a, const Rect2& b);

// @src 0x141267410  the curve item's vertex rows (index, x, y) for curve c (tolerance 0.01)
std::vector<double> curveItemPoints(def::model::OBJ_GEO2D* c, double tol);
// @src 0x141267530  vt+0x48 of the curve item: the rect of its vertices
Rect2 curveItemRect(const std::vector<double>& rows);

// Owned portion of SfGL2DCurveItem: binary32 width (+0x178), signed count
// (+0x188), vertex rows (+0x190), indices (+0x1a0) and render/color words (+0x1a8).
// Index/render containers begin empty; their population is not reconstructed here.
// This is internal converter state, not a native-layout or C ABI declaration.
class CurveItemState {
public:
    void bind(def::model::OBJ_GEO2D* curve, double tol);
    void reset() noexcept;
    // @src 0x14126f380 / 0x14126f3b0: store/read only, with no geometry rebuild.
    void setWidth(float width) noexcept { width_ = width; }
    float width() const noexcept { return width_; }
    std::int32_t vertexCount() const noexcept { return vertexCount_; }
    const std::vector<double>& vertexRows() const noexcept { return vertexRows_; }
    const std::vector<std::int32_t>& indices() const noexcept { return indices_; }
    const std::vector<std::uint32_t>& renderWords() const noexcept { return renderWords_; }
    Rect2 rectangle() const;

private:
    std::vector<double> vertexRows_;
    // 0x1412671d0 constructs these separately; 0x1412672c0 destroys them.
    // Direct row bind/reset leaves both owned containers intact. Raw render words
    // preserve the distinction between color bits and floating-point coordinates.
    std::vector<std::int32_t> indices_;
    std::vector<std::uint32_t> renderWords_;
    std::int32_t vertexCount_ = 0;
    // 0x14126ee40: binary32 0x3d0f5c29; independent of row tolerance 0.01.
    float width_ = 0.035f;
};

// @src 0x14124be10  the mesh item's node positions (x, y as floats, two per node)
std::vector<float> meshItemPoints(def::model::LLOBJ_DATA* obj);
// @src 0x14124c3d0  vt+0x48 of the mesh item: the rect of its nodes
Rect2 meshItemRect(const std::vector<float>& xy);

}  // namespace m2m::engine
