# CP67 isolated SW2SP toggle regression

Added one Release-active `TEST_CASE` to
`source/apps/moproj2mst/tests/mdt/test_editor_setup.cpp` for the source-qualified
setup projection edge `m_chkbxSW2SP` (`0x1480`) to `groupBox72_2` (`0x14f8`).
The direct source contract is the receiver's explicit `Enabled(false)` setter
at `0x140f1780c` and one `toggled(bool) -> setEnabled(bool)` AutoConnection in
the internal connection table. The retained constructor-prefix JSON records
the same signal and slot identities.

The test binds `FreshDefaultQtModel` and `NoActiveTranslator`, checks both
object identities, and distinguishes `requestedEnabled` from unknown
effective `enabled`. It preserves the sender's unknown `checked` state. It
proves no projected delivery before connection and no connection-time state
synchronization, then checks exactly one correctly ordered delivery for true
and false inputs and the corresponding requested local enable values. The
test uses the existing public bounded projection; it does not release the
native wrapper/setupUi gate, invoke a Qt signal, or claim effective parent-tree
enablement.

Static preflight: the target already includes `testkit/check.hpp` and
`m2m/engine/editor_setup.hpp`; all names and assertion macros used by this
case already occur in neighboring Release-active cases. No C `assert` is used.
No include, CMake, production, build, CTest, GUI, or mutable REA changes were
made. Root owns build and test validation.

Frozen test-source SHA-256:
`CE7C9BA6382F7FD05DC5E64FDAEE8EA64EF45DBB229C64B9A89B16E11B3AE9D5`
