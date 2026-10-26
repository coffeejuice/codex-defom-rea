// ctest-labels: abi
#define WIN32_LEAN_AND_MEAN
#include <windows.h>
#include <cstring>
#include <initializer_list>
#include <string>
#include <testkit/check.hpp>
#include "moproj2mst.h"

namespace {
struct EnvironmentMode {
    bool present = false;
    std::wstring prior;
    EnvironmentMode() {
        wchar_t buffer[32768];
        SetLastError(ERROR_SUCCESS);
        DWORD n = GetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", buffer, 32768);
        present = n != 0 || GetLastError() == ERROR_SUCCESS;
        if (n) prior.assign(buffer, n);
    }
    ~EnvironmentMode() { SetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", present ? prior.c_str() : nullptr); }
    void set(const wchar_t* value) { REQUIRE(SetEnvironmentVariableW(L"M2M_MO_INPUT_MODE", value) != FALSE); }
};
}

TEST_CASE(adapter_settings_parse_each_call)
{
    EnvironmentMode env;
    // A nonexistent input reaches the existing Open error only after successful adapter parsing.
    // These calls never create output or consult installation settings.
    for (const wchar_t* value : std::initializer_list<const wchar_t*>{nullptr, L"", L"0", L"1", L"-1", L"+1", L"00001",
                                L"2147483647", L"-2147483648"}) {
        env.set(value);
        CHECK_EQ(m2m_convert_w(L"C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_OPEN);
        CHECK_EQ(m2m_convert_utf8("C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_OPEN);
    }
    for (const wchar_t* value : {L" ", L" 1", L"1 ", L"0x1", L"+", L"-", L"1.0",
                                L"2147483648", L"-2147483649", L"99999999999999999"}) {
        env.set(value);
        char message[256] = {};
        CHECK_EQ(m2m_convert_w(L"C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_ARG);
        CHECK_EQ(m2m_convert_utf8("C:/no/such/m2m-settings.moproj", nullptr, 0, message, sizeof message), M2M_E_ARG);
        CHECK(std::strstr(message, "M2M_MO_INPUT_MODE") != nullptr);
    }
    env.set(L"0");
    CHECK_EQ(m2m_convert_w(L"C:/no/such/m2m-settings.moproj", nullptr, 0, nullptr, 0), M2M_E_OPEN);
}

int main() { return testkit::run_all(); }
