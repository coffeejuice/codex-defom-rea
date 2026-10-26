// Form readers and string helpers of the edit menus (packet p10-01; also used by menus 5-6).
// The VS2008 std::string temporaries of the original (by-value copies that the callee destroys)
// are plain std::string values here; nothing observable depends on them.
#include <def_pre/forms.hpp>

#include <cstdio>
#include <cstring>
#include <iostream>

#include <def_pre/menu7.hpp>
#include <defmodel/layout/dynary.hpp>
#include <defrt/stdlib/legacy_printf.hpp>

namespace def::pre {

namespace {

// 140007460 istream::getline(buf, 256, widen('\n')) on cin (140006c70 / 140001570 + 1400067a0:
// the widen through the stream's locale)
std::string getline256()
{
    char buf[0x100];
    buf[0] = '\0';
    std::cin.getline(buf, 0x100, '\n');
    return std::string(buf);
}

} // namespace

// @src 14012fa90  DYNARY+4 == 0
bool dynary_is_constant(const model::DYNARY& d)
{
    return d.ndim == 0;
}

// @src 1401b08d0  "%lf"
double to_double(std::string s)
{
    double v = 0.0;
    std::sscanf(s.c_str(), "%lf", &v); // UCRT strtod: correctly rounded, see defkey/crt_scan.hpp
    return v;
}

// @src 1401b1240  n-th field
std::string field_of(std::string s, char delim, std::int32_t n)
{
    const char* str = s.c_str();
    const std::int32_t len = static_cast<std::int32_t>(std::strlen(str));
    std::int32_t first = 0;
    std::int32_t last = len - 1;
    std::int32_t count = 0;
    for (std::int32_t i = 0; i < len; i = i + 1) {
        if (str[i] == delim) {
            count = count + 1;
            if (count == n) {
                first = i + 1;
            }
            else if (count == n + 1) {
                last = i; // the delimiter itself stays in the field
                break;
            }
        }
    }
    std::int32_t width = last - first + 1;
    if (width < 0) {
        width = 0;
    }
    return s.substr(static_cast<std::size_t>(first), static_cast<std::size_t>(width)); // 140006760
}

// @src 1401b1450  digits, '-' and '.'
bool is_number(std::string s, std::int32_t start)
{
    const std::int32_t n = static_cast<std::int32_t>(std::strlen(s.c_str()));
    for (std::int32_t i = start; i < n - start; i = i + 1) {
        const char c = s.c_str()[i];
        if ((c < '0' || '9' < c) && c != '-' && c != '.') {
            return false;
        }
    }
    return true;
}

// @src 1400016b0  editing a DYNARY is not supported
void edit_not_supported()
{
    std::fprintf(stdout, "Editing function is not supported in this version.\n");
}

// @src 1400017d0
void not_implemented()
{
    std::fprintf(stdout, "\n\n Function not implemented yet\n");
}

// @src 1400016d0  DYNARY row
void print_dynary_row(const model::DYNARY& d, std::int32_t number, const char* name)
{
    if (dynary_is_constant(d)) {
        rt::crt::fprintf(stdout, "   %d     %s   %lf\n", number, name, d.value);
        return;
    }
    rt::crt::fprintf(stdout, "   %d     %s   %d Function\n", number, name, d.var[0]);
    if (d.axis[0] != nullptr) {
        // 14000174c: the count is the int at +0x40 (ncomp), not n[0]
        for (std::int32_t i = 0; i < d.ncomp; i = i + 1) {
            rt::crt::fprintf(stdout, "                    <  %3d>\t%lf\t%lf\n", i + 1, d.axis[0][i], d.values[i]);
        }
    }
}

// @src 140007a80  read up to n doubles from one line
void read_doubles(MenuRec*, double* out, std::int32_t n)
{
    std::fflush(stdout);
    std::string line = trim(getline256());
    for (std::int32_t i = 0; i < n; i = i + 1) {
        const std::string f = field_of(line, ' ', i);
        if (!is_digits(f, 0)) {
            break;
        }
        out[i] = to_double(f);
    }
}

// @src 140008030  read one double
double read_double(MenuRec*)
{
    std::fflush(stdout);
    std::string line = trim(getline256());
    if (!is_number(line, 0)) {
        return kNoValue;
    }
    return to_double(line);
}

// @src 140008220  read one double in [lo, hi]
double read_double_range(MenuRec*, double lo, double hi, const std::string& prompt)
{
    while (true) {
        std::fprintf(stdout, "%s", prompt.c_str()); // 140002ff0: c_str
        std::fflush(stdout);
        std::string line = trim(getline256());
        if (!is_number(line, 0)) {
            return kNoValue;
        }
        // 1400084b5..: lo <= v, then v <= hi (comisd; the value cannot be a NaN: is_number
        // admits only digits, '-' and '.')
        if (lo <= to_double(line) && to_double(line) <= hi) {
            return to_double(line);
        }
        rt::crt::fprintf(stdout, " ??? Invalid Data; Valid Data Range = (     %lf to      %lf)\n", lo, hi);
    }
}

} // namespace def::pre
