#include "m2m/mst/stream.hpp"

// Base-order tests intercept every writer and never produce stream text.
namespace m2m::mst {
Stream& Stream::flush() { return *this; }
}
