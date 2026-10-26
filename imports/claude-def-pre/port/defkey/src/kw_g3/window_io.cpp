// KEY I/O of window geometry (packet kw-g3): the point readers 14041b7d0 / 14041b6a0 and the
// geometry writer 1402f4890. Spec: docs/re/keywords/FRCWIN.md.
#include <defkey/kw_g3/kw_g3.hpp>

#include <cstdio>

#include <defkey/line_io.hpp>
#include <defkey/table_io.hpp>
#include <defmodel/geo_kw/surface_model.hpp>
#include <defmodel/port.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::key::g3 {

// @src 14041b7d0  read the points of a 3D window (`indexed`: each line starts with its 1-based
// index, which must match). Shape 3 fills the window's OBJ_GEO3D: a node count, one
// "index x y z" line per node, an element count, one "index n1 n2 n3 n4" line per element; the
// leading indices are read in every mode and never checked.
int window_read_points_3d(std::FILE* f, LLWINDEF_DATA* w, bool indexed)
{
    std::int32_t idx = 0; // the line helpers zero it before scanning
    int st;
    if (w->shape == 1) {
        window_alloc_points(w, 8);
        for (std::int32_t i = 0; i < w->npts; i = i + 1) {
            double* p = w->pts + i * 3;
            if (indexed)
                st = def::key::read_i_d3(f, &idx, p, p + 1, p + 2);
            else
                st = def::key::read_d3(f, p, p + 1, p + 2);
            if (st == 1) return 1;
            if (indexed && idx != i + 1) return 1;
        }
    } else if (w->shape == 2) {
        window_alloc_points(w, 2);
        for (std::int32_t i = 0; i < w->npts; i = i + 1) {
            double* p = w->pts + i * 3;
            st = def::key::read_d3(f, p, p + 1, p + 2);
            if (st == 1) return 1;
        }
        if (def::key::read_dd(f, &w->u1b8, &w->u1c0) == 1) return 1;
    } else if (w->shape == 3) {
        namespace gk = def::model::geo_kw;
        std::int32_t nnode = 0;
        if (def::key::read_i(f, &nnode, false) == 1) return 1;     // 140413de0
        w->geo3d.resize_nodes(&nnode);                             // 1402f9600
        double* xyz = gk::geo3d_coords(&w->geo3d);                 // 140006410
        for (std::int32_t i = 0; i < nnode; i = i + 1) {
            st = def::key::read_i_d3(f, &idx, xyz + i * 3, xyz + (i * 3 + 1),
                                     xyz + (i * 3 + 2));           // 140415880
            if (st == 1) return 1;
        }
        std::int32_t nelem = 0;
        if (def::key::read_i(f, &nelem, false) == 1) return 1;     // 140413de0
        w->geo3d.resize_elems(&nelem);                             // 1402f96a0
        std::int32_t* el = gk::geo3d_elems(&w->geo3d);             // 140005510
        for (std::int32_t i = 0; i < nelem; i = i + 1) {
            st = def::key::read_i5(f, &idx, el + i * 4, el + (i * 4 + 1), el + (i * 4 + 2),
                                   el + (i * 4 + 3), false);       // 140414200
            if (st == 1) return 1;
        }
        gk::geo3d_fun_1402fbb30(&w->geo3d);                        // 1402fbb30 (empty)
    } else {
        if (w->shape != 0xb && w->shape != 0xc && w->shape != 0xd) {
            def::rt::crt::printf("ERROR: WindefShape invalid!\n");
            return 3;
        }
        if (def::key::read_i(f, &w->npts, false) == 1) return 1;
        window_alloc_points(w, w->npts);
        for (std::int32_t i = 0; i < w->npts; i = i + 1) {
            double* p = w->pts + i * 3;
            if (indexed)
                st = def::key::read_i_d3(f, &idx, p, p + 1, p + 2);
            else
                st = def::key::read_d3(f, p, p + 1, p + 2);
            if (st == 1) return 1;
            if (indexed && idx != i + 1) return 1;
        }
    }
    return 0;
}

// @src 14041b6a0  read n points of a 2D window
int window_read_points_2d(std::FILE* f, LLWINDEF_DATA* w, std::int32_t n, bool indexed)
{
    std::int32_t idx = 0; // the line helpers zero it before scanning
    int st;
    window_alloc_points(w, n);
    for (std::int32_t i = 0; i < w->npts; i = i + 1) {
        double* p = w->pts + i * 2;
        if (indexed)
            st = def::key::read_i_dd(f, &idx, p, p + 1);
        else
            st = def::key::read_dd(f, p, p + 1);
        if (st == 1) return 1;
        if (indexed && idx != i + 1) return 1;
    }
    return 0;
}

// @src 1402f4890  write the window geometry (mode 0: points carry their index; shape 3 always
// writes the indices)
int window_write_points(std::FILE* f, LLWINDEF_DATA* w, std::int32_t mode)
{
    int st = 0;
    if (f == nullptr) return 2;
    const bool with_index = mode == 0;
    if (!window_is_2d(w)) {
        if (w->shape == 1) {
            for (std::int32_t i = 0; i < w->npts; i = i + 1) {
                if (with_index) def::rt::crt::fprintf(f, " %7i", i + 1);
                st = def::key::write_doubles(f, 3, w->pts + i * 3, 3, false);
            }
        } else if (w->shape == 2) {
            for (std::int32_t i = 0; i < w->npts; i = i + 1) {
                st = def::key::write_doubles(f, 3, w->pts + i * 3, 3, false);
            }
            def::rt::crt::fprintf(f, " %20.10E %20.10E\n", w->u1b8, w->u1c0);
        } else if (w->shape == 3) {
            namespace gk = def::model::geo_kw;
            const std::int32_t nnode = w->geo3d.node_count();      // 1400034d0
            const double* xyz = gk::geo3d_coords(&w->geo3d);       // 140006410
            def::rt::crt::fprintf(f, "%d\n", nnode);               // str 1405423ec
            for (std::int32_t i = 0; i < nnode; i = i + 1) {
                // str 140542948
                def::rt::crt::fprintf(f, " %7i %20.10E %20.10E %20.10E\n", i + 1, xyz[i * 3],
                                      xyz[i * 3 + 1], xyz[i * 3 + 2]);
            }
            const std::int32_t nelem = w->geo3d.elem_count();      // 140003500
            def::rt::crt::fprintf(f, "%d\n", nelem);               // str 140542968
            const std::int32_t* el = gk::geo3d_elems(&w->geo3d);   // 140005510
            for (std::int32_t i = 0; i < nelem; i = i + 1) {
                // str 140542970
                def::rt::crt::fprintf(f, " %7i %7i %7i %7i %7i\n", i + 1, el[i * 4],
                                      el[i * 4 + 1], el[i * 4 + 2], el[i * 4 + 3]);
            }
        } else if (w->shape == 0xb || w->shape == 0xc || w->shape == 0xd) {
            def::rt::crt::fprintf(f, "%d\n", w->npts);
            for (std::int32_t i = 0; i < w->npts; i = i + 1) {
                if (with_index) def::rt::crt::fprintf(f, " %7i", i + 1);
                st = def::key::write_doubles(f, 3, w->pts + i * 3, 3, false);
            }
        }
    } else {
        for (std::int32_t i = 0; i < w->npts; i = i + 1) {
            if (with_index) def::rt::crt::fprintf(f, " %7i", i + 1);
            st = def::key::write_doubles(f, 2, w->pts + i * 2, 2, false);
        }
    }
    return st;
}

} // namespace def::key::g3
