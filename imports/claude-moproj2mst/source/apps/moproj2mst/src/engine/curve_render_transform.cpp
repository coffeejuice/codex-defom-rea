#include "curve_render_transform.hpp"

#include <cmath>
#include <cstddef>
#include <limits>
#include <new>
#include <stdexcept>

// Every arithmetic expression follows the recovered scalar operation order.
// In particular, do not fuse multiply/add or replace biased ternaries by min/max.
#if defined(_MSC_VER)
#pragma float_control(precise, on, push)
#pragma fp_contract(off)
#endif

namespace m2m::engine {
namespace {
constexpr double epsilon = 1e-12;
struct ArithmeticFault {};
double finite(double value) {
    if (!std::isfinite(value)) throw ArithmeticFault{};
    return value;
}
double add(double a, double b) { return finite(a + b); }
double sub(double a, double b) { return finite(a - b); }
double mul(double a, double b) { return finite(a * b); }
double div(double a, double b) {
    if (b == 0.0) throw ArithmeticFault{};
    return finite(a / b);
}
struct XY { double x = 0.0, y = 0.0; };
struct Normal { XY xy; double unused = 0.0; };
static_assert(sizeof(Normal) == 24);
XY plus(XY a, XY b) { return {add(a.x,b.x), add(a.y,b.y)}; }
XY minus(XY a, XY b) { return {sub(a.x,b.x), sub(a.y,b.y)}; }
XY times(double s, XY a) { return {mul(s,a.x), mul(s,a.y)}; }
double cross(XY a, XY b) { return sub(mul(a.x,b.y),mul(a.y,b.x)); }
double squared(XY a) { return add(mul(a.x,a.x),mul(a.y,a.y)); }
double length(XY a) { return finite(std::sqrt(squared(a))); }
double distance(XY a, XY b) {
    const double s = squared(minus(a,b));
    return s == 0.0 ? s : finite(std::sqrt(s));
}
XY normalize(XY a) {
    const double l = length(a);
    if (std::fabs(l) > epsilon) return {div(a.x,l),div(a.y,l)};
    return {}; // The recovered helper writes positive zeros.
}
double diagonal(const std::vector<XY>& p) {
    XY lo = p.front(), hi = lo;
    for (const XY q : p) {
        if (q.x < lo.x) lo.x = q.x;
        if (q.y < lo.y) lo.y = q.y;
        if (q.x > hi.x) hi.x = q.x;
        if (q.y > hi.y) hi.y = q.y;
    }
    return length(minus(hi,lo));
}
TransformAdmission admission(std::int32_t n, const void* xy, double w, void* out) {
    if (n <= 0) return TransformAdmission::nonpositiveCount;
    if (epsilon >= std::fabs(w)) return TransformAdmission::negligibleWidth;
    if (!xy) return TransformAdmission::nullInput;
    if (!out) return TransformAdmission::nullOutput;
    return TransformAdmission::none;
}

// @src 0x140a288b0. A failed mode-0 call may have written candidate;
// only successful mode-0 results are consumed by the correction caller.
bool intersect(XY a, XY b, XY c, XY d, XY& candidate, bool infinite) {
    if (!infinite) {
        // Exact interval comparisons; equality remains admitted.
        const auto separated = [](double a0,double a1,double c0,double c1) {
            return (a0 < c0 && a0 < c1 && a1 < c0 && a1 < c1) ||
                   (a0 > c0 && a0 > c1 && a1 > c0 && a1 > c1);
        };
        if (separated(a.x,b.x,c.x,d.x) || separated(a.y,b.y,c.y,d.y)) return false;
    }
    const bool firstVertical = a.x == b.x;
    const bool secondVertical = c.x == d.x;
    if (firstVertical && secondVertical) return false;
    if (firstVertical) {
        candidate.x = a.x;
        candidate.y = add(mul(div(sub(c.y,d.y),sub(c.x,d.x)),
                              sub(candidate.x,d.x)), d.y);
    } else if (secondVertical) {
        candidate.x = d.x;
        candidate.y = add(mul(div(sub(a.y,b.y),sub(a.x,b.x)),
                              sub(candidate.x,b.x)), b.y);
    } else {
        const double m1 = div(sub(a.y,b.y),sub(a.x,b.x));
        const double m2 = div(sub(d.y,c.y),sub(d.x,c.x));
        if (m1 == m2) return false;
        candidate.x = div(sub(add(sub(c.y,b.y),mul(m1,b.x)),mul(m2,c.x)),sub(m1,m2));
        candidate.y = add(mul(m1,sub(candidate.x,b.x)),b.y);
    }
    if (infinite) return true;
    const auto contains = [](XY a0,XY b0,XY p) {
        if (std::fabs(sub(a0.x,b0.x)) >= std::fabs(sub(a0.y,b0.y)))
            return !(mul(sub(p.x,a0.x),sub(p.x,b0.x)) > 0.0);
        return !(mul(sub(p.y,a0.y),sub(p.y,b0.y)) > 0.0);
    };
    return contains(a,b,candidate) && contains(c,d,candidate);
}
bool strictlyBetween(XY a, XY b, XY candidate) {
    if (std::fabs(sub(a.x,b.x)) < epsilon)
        return mul(sub(candidate.y,a.y),sub(candidate.y,b.y)) < 0.0;
    return mul(sub(candidate.x,a.x),sub(candidate.x,b.x)) < 0.0;
}
bool convexQuad(XY a, XY b, XY displacedB, XY displacedA) {
    const XY q[] = {a,b,displacedB,displacedA};
    for (int i = 0; i != 4; ++i)
        if (cross(minus(q[i],q[(i+3)%4]),minus(q[(i+1)%4],q[i])) < 0.0) return false;
    return true;
}
// False is an indeterminate-candidate stop, not a native helper failure return.
bool correct(XY a, XY b, XY& displacedB, XY& displacedA, const std::vector<XY>& p) {
    if (!convexQuad(a,b,displacedB,displacedA)) {
        XY candidate;
        if (!intersect(a,displacedA,b,displacedB,candidate,true)) return false;
        if (strictlyBetween(b,displacedB,candidate)) displacedB = candidate;
        if (strictlyBetween(a,displacedA,candidate)) displacedA = candidate;
    }
    for (std::size_t j = 0; j+1 < p.size(); ++j) {
        XY candidate;
        if (intersect(a,displacedA,p[j],p[j+1],candidate,false) &&
            std::fabs(sub(candidate.x,a.x)) > 1e-6 &&
            std::fabs(sub(candidate.y,a.y)) > 1e-6) displacedA = candidate;
        if (intersect(b,displacedB,p[j],p[j+1],candidate,false) &&
            std::fabs(sub(candidate.x,b.x)) > 1e-6 &&
            std::fabs(sub(candidate.y,b.y)) > 1e-6) displacedB = candidate;
    }
    return true;
}

void initialNormals(const std::vector<XY>& p, std::size_t m, bool closed,
                    std::vector<Normal>& normals) {
    for (std::size_t i = 0; i != m; ++i) {
        const std::size_t prev = i ? i-1 : m-1;
        const std::size_t next = i+1 == m ? 0 : i+1;
        const XY u = normalize(minus(p[i],p[prev]));
        const XY v = normalize(minus(p[next],p[i]));
        XY chosen;
        if (!closed && i == 0) chosen = v;
        else if (!closed && i+1 == m) chosen = u;
        else chosen = plus(u,v);
        normals[i].xy = normalize({-chosen.y,chosen.x});
    }
}
void smooth(const std::vector<XY>& p, std::size_t m, bool closed, double width,
            std::vector<Normal>& normals, std::vector<double>& scales) {
    std::vector<Normal> updated(m); // Native zero-filled m*24 scratch.
    for (std::size_t i = 0; i != m; ++i) {
        if (!closed && (i == 0 || i+1 == m)) {
            updated[i] = normals[i]; scales[i] = 1.0; continue;
        }
        const std::size_t prev = i ? i-1 : m-1;
        const std::size_t next = i+1 == m ? 0 : i+1;
        const XY e1 = minus(p[i],p[prev]), e2 = minus(p[next],p[i]);
        const double l1 = length(e1), l2 = length(e2);
        double a = div(width,l1), b = div(width,l2);
        a = mul(mul(10.0,a),a); b = mul(mul(10.0,b),b);
        const XY n = normals[i].xy, nprev = normals[prev].xy, nnext = normals[next].xy;
        if (cross(nprev,n) < 0.0) a = 0.0;
        if (cross(n,nnext) < 0.0) b = 0.0;
        XY candidate = plus(plus(n,times(a,normalize(plus(n,nprev)))),
                            times(b,normalize(plus(n,nnext))));
        const XY u = normalize(e1), v = normalize(e2);
        const XY c = plus({-u.x,-u.y},times(0.2,v));
        const XY d = minus(v,times(0.2,u));
        const double x = cross(candidate,c), y = cross(d,candidate);
        if (x < 0.0 && y < 0.0) candidate = x > y ? c : d;
        else if (x < 0.0) candidate = c;
        else if (y < 0.0) candidate = d;
        candidate = normalize(candidate);
        updated[i].xy = candidate;
        const double s1 = div(std::fabs(cross(e1,candidate)),l1);
        const double s2 = div(std::fabs(cross(e2,candidate)),l2);
        const double minimum = s2 > s1 ? s1 : s2;
        const double denominator = 0.2 > minimum ? 0.2 : minimum;
        scales[i] = div(1.0,denominator);
    }
    for (std::size_t i = 0; i != m; ++i) normals[i] = updated[i];
}

bool core(const std::vector<XY>& p, double width, bool relative,
          std::vector<XY>& transformed, CurveRenderTransformResult& result) {
    // @src 0x1417d0b90 repeats the same admission before bbox/scaling.
    const auto coreAdmission = admission(static_cast<std::int32_t>(p.size()),
                                         p.data(),width,transformed.data());
    if (coreAdmission != TransformAdmission::none) {
        result.outcome = TransformOutcome::admissionRejected;
        result.admission = coreAdmission;
        result.nativeStatus = -1;
        // Wrapper still expands its calloc-zero transformed scratch on -1.
        return true;
    }
    const double d = diagonal(p);
    const bool closed = distance(p.front(),p.back()) < mul(d,1e-5);
    result.closed = closed;
    if (relative) width = mul(width,d); // No repeated negligible-width admission.
    std::vector<Normal> normals(p.size());
    std::vector<double> scales(p.size());
    const std::size_t m = closed ? p.size()-1 : p.size();
    initialNormals(p,m,closed,normals);
    smooth(p,m,closed,width,normals,scales);
    if (closed) { normals.back().xy = normals[0].xy; scales.back() = scales[0]; }
    for (std::size_t i = 0; i != p.size(); ++i) {
        const double scaledWidth = mul(width,scales[i]);
        transformed[i].x = add(p[i].x,mul(scaledWidth,normals[i].xy.x));
        transformed[i].y = add(p[i].y,mul(scaledWidth,normals[i].xy.y));
    }
    for (std::size_t i = 0; i+1 < p.size(); ++i) {
        if (!correct(p[i],p[i+1],transformed[i+1],transformed[i],p)) {
            result.outcome = TransformOutcome::indeterminateCandidate;
            result.correctionSegment = static_cast<std::int32_t>(i);
            return false;
        }
    }
    if (closed) {
        if (distance(p.front(),transformed.front()) > distance(p.back(),transformed.back()))
            transformed.front() = transformed.back();
        else transformed.back() = transformed.front();
    }
    result.outcome = TransformOutcome::completed;
    result.nativeStatus = 0;
    return true;
}
} // namespace

CurveRenderTransformResult transformCurveRender2D(
    std::int32_t count, const double* xy, double width, double* out, bool relative) {
    CurveRenderTransformResult result;
    result.admission = admission(count,xy,width,out);
    if (result.admission != TransformAdmission::none) { result.nativeStatus = -1; return result; }
    // Native doubles count in signed 32-bit arithmetic before sign extension.
    // Do not execute that overflow or attempt to define its allocator outcome.
    if (count > std::numeric_limits<std::int32_t>::max()/2 ||
        static_cast<std::uint64_t>(count) > std::numeric_limits<std::size_t>::max()/sizeof(Normal)) {
        result.outcome = TransformOutcome::outsideQualifiedSize; return result;
    }
    try {
        finite(width);
        std::vector<XY> retained(static_cast<std::size_t>(count));
        std::vector<XY> transformed(static_cast<std::size_t>(count));
        std::vector<std::int32_t> mapping(static_cast<std::size_t>(count));
        // Retained input has the native allocation's capacity and zero scratch.
        // The independent input view is only needed to compute the original bbox.
        for (std::int32_t i = 0; i != count; ++i)
            retained[i] = {finite(xy[2*static_cast<std::size_t>(i)]),finite(xy[2*static_cast<std::size_t>(i)+1])};
        const double threshold = mul(diagonal(retained),0.0001);
        std::int32_t k = 0;
        for (std::int32_t i = 0; i != count; ++i) {
            const XY point = {xy[2*static_cast<std::size_t>(i)],xy[2*static_cast<std::size_t>(i)+1]};
            if (i == 0 || distance(point,retained[k-1]) > threshold) retained[k++] = point;
            mapping[i] = k-1;
        }
        retained[k-1] = {xy[2*static_cast<std::size_t>(count-1)],xy[2*static_cast<std::size_t>(count-1)+1]};
        retained.resize(static_cast<std::size_t>(k));
        transformed.resize(static_cast<std::size_t>(k));
        result.retainedCount = k;
        // The subordinate repeats native admission. For admitted finite wrapper
        // inputs k is positive and pointers/width still pass; no new guard exists.
        const bool completed = core(retained,width,relative,transformed,result);
        if (!completed) {
            result.diagnosticTransformedXY.reserve(2*transformed.size());
            for (XY p : transformed) {
                result.diagnosticTransformedXY.push_back(p.x);
                result.diagnosticTransformedXY.push_back(p.y);
            }
            return result;
        }
        for (std::int32_t i = 0; i != count; ++i) {
            out[2*static_cast<std::size_t>(i)] = transformed[mapping[i]].x;
            out[2*static_cast<std::size_t>(i)+1] = transformed[mapping[i]].y;
        }
        result.outputWritten = true;
    } catch (const ArithmeticFault&) {
        result.outcome = TransformOutcome::outsideQualifiedArithmetic;
    } catch (const std::bad_alloc&) {
        result.outcome = TransformOutcome::outsideQualifiedAllocation;
    } catch (const std::length_error&) {
        result.outcome = TransformOutcome::outsideQualifiedSize;
    }
    return result;
}
} // namespace m2m::engine
#if defined(_MSC_VER)
#pragma float_control(pop)
#endif

