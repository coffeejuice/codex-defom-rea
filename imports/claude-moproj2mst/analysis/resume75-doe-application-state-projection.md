# CP75: application-scope process-state projection

## Implemented edge

Added a standalone `ApplicationStateProjection`, outside `ConversionSession`, with separate unknown/known DWORD observations for native process type (`0x143d766b8+0x0c`) and DBLIST admission (`+0x1c`). Its only state transition models the retained mode-0 process-type refresh: no current Project selects 5; a present Project with native-open-success selects 5; a present Project without it selects 1. Inconsistent/missing Project/open observations refuse without changing the prior state.

DBLIST admission remains unknown for the entire projection lifetime. There is deliberately no API to set, clear, or infer `+0x1c`; no setter caller/reset path was qualified in the retained original evidence. A new Session, Project, or process-type refresh cannot manufacture a known admission value.

## Focused test and static preflight

The focused test holds one explicit application-state owner while two distinct `ConversionSession` instances are created and destroyed sequentially. It checks that the application projection persists across those Session lifetimes, that mode-0 refresh changes only the known process-type value, that an invalid observation preserves the prior value, and that DBLIST admission stays unavailable. This does not add a Session interface or imply that candidate sessions are actually attached to the original application's global owner.

Static preflight confirmed the test's private-header include resolves, source and test use the existing CMake configured source/test globs, and all owned files have final newlines and no trailing whitespace. No build or tests were run. No shared Session interface, public DBLIST branch, Type2 admission, CMake, GUI, or mutable REA state was changed.

## Owned files and SHA-256

| File | SHA-256 |
| --- | --- |
| `source/apps/moproj2mst/src/doe/application_state_projection.hpp` | `25d236d51f278ab7e57e3773d8f6e12df68c57900879d150cf3deba7d9cc597c` |
| `source/apps/moproj2mst/src/doe/application_state_projection.cpp` | `ba861fa708ff766c3ffaf5f8ccaabf5ee328ca5150e5e4fe6ae29289530f8803` |
| `source/apps/moproj2mst/tests/doe/test_doe_application_state_projection.cpp` | `3ecc3966992ba322f4971e38d17a8e00a764d7071ea6c6dc9d8c6ae74c414512` |

This is an isolated value projection, not a native application singleton, not a ModelManager owner, and not sufficient to admit DBLIST. The public DBLIST/ReportList guard remains required until the `+0x1c` setter/reset lifecycle and actual application-level owner are source-qualified.
