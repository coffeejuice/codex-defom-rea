#include <cstdint>
#include <cstring>
#include <chrono>
#include <thread>

// Fixed benign startup target for debugger CREATE_PROCESS event validation.
// The primary thread calls the exported probe immediately from main.
struct View {
    std::uint8_t reserved[0x18];
    void* object;
};

static std::uint8_t object_storage[32] = {0x43, 0x50, 0x32, 0x38, 0x32};
static View startup_view = {{}, object_storage};
static volatile std::uintptr_t observed;

extern "C" __declspec(dllexport) __declspec(noinline)
std::uintptr_t EntryProbe(View* view) noexcept {
    // Touch both the argument and the known readable nonzero field at +0x18.
    auto* slot = reinterpret_cast<void* const*>(
        reinterpret_cast<const std::uint8_t*>(view) + 0x18);
    observed = reinterpret_cast<std::uintptr_t>(*slot);
    return observed != 0 ? 0x282u : 0x283u;
}

int main(int argc, char** argv) {
    if (argc == 2 && std::strcmp(argv[1], "--bounded-hold") == 0) {
        std::this_thread::sleep_for(std::chrono::seconds(3));
    }
    return EntryProbe(&startup_view) == 0x282u ? 0 : 1;
}
