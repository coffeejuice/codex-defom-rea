# CP127 integration correction

The grouped focused compile rejected the SPRING2 reader because the new
`allowSingleRowQualifiedToken` parameter left one old reference to
`allowOneRowInvalidHexPrefix` in the guarded `0xG` branch. Root replaced that
reference with the current parameter name. No parsing branch or admission
condition changed.

The repaired `simcontrol300_native_io.cpp` SHA-256 is
`9F3842F93EA61D29401EC0025EDFFE324E20F15BD805F9DEBC81553F47AB2186`;
the focused reader test SHA-256 remains
`BF7866B93C4193F271812BE4426AAB60DD41F3D5F4F3EDCB6095F7DD7D280CC5`.
The first preflight failure is retained in
`build/logs/resume127-preflight-build.log`. The repaired preflight and all
three focused tests passed before the source-stable full build in
`analysis/resume127-final-build.json`. The final full test binding is retained
by the checkpoint freeze. Public Spring admission remains guarded.
