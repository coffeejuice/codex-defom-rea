# CP296 exact 2D color packing and palette-mapping correction

This read-only audit recovers a parameterized packed-word leaf; it does not release the public color path. Target: retained original DEF_GUI_MO_64.exe installation v11.0.2, `evidence/original-binaries/DEF_GUI_MO_64.exe`, 67,611,136 bytes, independently rehashed SHA-256 `fd8ae1f8dd50d068267b3b7e49efdf11eaa178889131e59ec4b6f8de64725acc`. This report concerns no reconstructed DEF_PRE executable. No original execution, GUI control, Frida, mutable REA, source edit or build occurred. Only this report was written.

Evidence is bounded immutable-PE DUMPBIN 14.44.35228.0 output, independently parsed PE headers/import descriptors/unwind records, and retained CP279/282/286/287 qualifications. Preferred image base is 0x140000000; addresses below are preferred VAs, not observed loaded addresses. Commands used `/DISASM /RANGE:0x14126A620,0x14126A8E6`, `/RANGE:0x14126F300,0x14126F350`, and `/RANGE:0x1402D2D20,0x1402D2D4E`. Only complete relevant instructions through 0x14126f330 were used in the second range. PE mapping reused CP280's strict full-span `_rva_to_file_offset`; Python import bytecode writes were disabled.

## Palette raw-byte claim must be corrected

CP287's quoted slot-12 file bytes are not mapped palette bytes. The .data section has RVA 0x0354c000, VirtualSize 0x01b09344, SizeOfRawData 0x0082a400, PointerToRawData 0x0354a000, characteristics 0xc0000040. Its raw-backed RVA interval is [0x0354c000,0x03d76400); virtual interval ends at 0x05055344. Palette base 0x143d77590 plus 12*0x10 is VA 0x143d77650, RVA 0x03d77650, which is 0x1250 beyond the raw-backed end and within the virtual-only tail.

Unchecked arithmetic produces file offset 0x03d75650, where bytes `14 90 11 03 a0 22 03 00 43 23 03 00 a8 90 11 03` do exist. That file offset belongs to .pdata and maps to RVA 0x05057250, not palette RVA 0x03d77650. CP280's strict mapper correctly refuses the palette span as not raw-backed. Therefore the CP287 file-byte attribution is retracted. A loader zero-filled virtual tail is an initial-image fact only; constructors and later mutation remain unqualified. No initialized runtime RGB or QColor can be inferred from those file bytes or initial zero-fill.

`0x1402d2d20` checks signed index >=0 and <0x5c at 0x1402d2d29..35, then computes base+index*16 at 0x1402d2d3b..49. Slot 12 is accepted. Crucially, the zero-flag callback passes this address directly to the packer's three float loads. It does not call QColor getters on the palette slot. Labeling this slot a proven initialized QColor is unsupported.

## Callback branches and ordered preparation

`0x14126a62e` loads curve+0x17c as a byte; `0x14126a637` selects nonzero at 0x14126a6a7. The zero branch loads a scalar float from .rdata VA 0x142d30298 at 0x14126a639, looks up palette slot 12 at 0x14126a653, and calls packer 0x14126a7e0 at 0x14126a665. It saves the returned 32-bit pattern at stack+0x24. It then sets a separate scalar to +0 with XORPS, performs a second slot-12 lookup at 0x14126a685, packs again at 0x14126a697, and saves stack+0x20. The two calls re-read palette memory; static code does not prove absence of concurrent mutation.

The nonzero branch calls eager QtGui4_64.dll IAT cells in this order, always RCX=curve+0x168:

| Call VA | IAT cell | Import ordinal-in-thunk array | IMAGE_IMPORT_BY_NAME RVA | Symbol |
| --- | --- | --- | --- | --- |
| 0x14126a6b3 | 0x141e089c0 | 1530 | 0x03536e08 | ?redF@QColor@@QEBANXZ |
| 0x14126a6cf | 0x141e089b8 | 1529 | 0x03536dee | ?greenF@QColor@@QEBANXZ |
| 0x14126a6eb | 0x141e089b0 | 1528 | 0x03536dd4 | ?blueF@QColor@@QEBANXZ |

Indices above are zero-based thunk indices, not PE ordinal imports. Independent descriptor/OriginalFirstThunk parsing agrees with CP282's corrected import mapping. Each return is a double in XMM0 and is immediately rounded to float by CVTSD2SS at 0x14126a6b9/6d5/6f1, then stored contiguously at stack+0x30/+0x34/+0x38. All three calls/conversions finish before either pack. The same saved RGB float triple feeds both packs at 0x14126a718 and 0x14126a73b, first with the .rdata scalar, then +0. No alphaF is called here.

CP286's separate initial-bind/post-bind distinction remains: initial render builder 0x14126a190 calls this callback before the post-bind setter and can reach the zero-flag route. Post-bind setter 0x14126f300 calls QColor assignment on secondary+0x38 via IAT 0x141e0b5c8 at 0x14126f31c, then writes secondary+0x4c=1 at 0x14126f327 only after assignment returns. Independent import parsing identifies `??4QColor@@QEAAAEAV0@AEBV0@@Z`. Thus a successful setter followed by the callback selects QColor mode; an assignment failure does not reach that flag store. This is conditional static ordering, not a witnessed Open occurrence or qualified public integration.

## Exact pack contract

`0x14126a7e0` receives RDX=pointer to three binary32 components in R,G,B memory order and R8=pointer to one binary32 scalar. Saved RCX is unused. It performs no calls and writes only its own stack. The scalar source constant is .rdata bytes `a4 70 7d 3f` at VA 0x142d30298, file offset 0x02d2ea98: binary32 bits 0x3f7d70a4, value 0.9900000095367431640625. The multiplier is .rdata bytes `00 00 7f 43` at VA 0x1421ebb18, file offset 0x021ea318: exactly 255.0f.

The observable read/conversion order within each pack is scalar, B, G, R:

| Component | Float load | MULSS 255.0f | CVTTSS2SI signed 64-bit RAX | Save low byte AL |
| --- | --- | --- | --- | --- |
| scalar | 0x14126a814 | 0x14126a818 | 0x14126a820 | 0x14126a825 |
| B at RGB+8 | 0x14126a82e | 0x14126a833 | 0x14126a83b | 0x14126a840 |
| G at RGB+4 | 0x14126a849 | 0x14126a84e | 0x14126a856 | 0x14126a85b |
| R at RGB | 0x14126a864 | 0x14126a868 | 0x14126a870 | 0x14126a875 |

It assembles bytes R,G,B,scalar at increasing addresses 0x14126a887..8d9, then MOVSS returns the raw four-byte pattern in XMM0 at 0x14126a8db. This is a bit carrier, not an arithmetic uint32-to-float conversion. The caller preserves these bits with MOVSS and stores them into the render slot. Define U(x)=low8(CVTTSS2SI64(MULSS(x,255.0f))) under the applicable SSE environment. Then the source-ready leaf is:

    pack(rgbFloat, scalarFloat) -> uint32
    s = U(scalarFloat); b = U(rgbFloat[2]); g = U(rgbFloat[1]); r = U(rgbFloat[0]);
    return uint32(r) | uint32(g)<<8 | uint32(b)<<16 | uint32(s)<<24;

Use explicit bit storage/bit_cast for the float carrier. There is no clamping or rounding-to-nearest integer before narrowing. Do not replace the instruction sequence with a potentially undefined out-of-range C++ float-to-byte cast. A portable finite-domain implementation must state its input/FP assumptions; strict exceptional-value fidelity requires preserving the SSE conversion, MXCSR behavior and operation order. CVTSD2SS and MULSS depend on the floating-point environment; CVTTSS2SI explicitly truncates. Unmasked FP exceptions can interrupt preparation. For ordinary finite normalized components with normal masked exceptions, first scalar yields byte 0xfc (nearest-rounded product 252.4499969482421875); +0 yields byte 0x00. The RGB bytes remain unknown until their actual values are supplied.

Both pack operations complete before callback count is loaded at 0x14126a74b. Even N=0 does not bypass getter/palette/pack preparation. There is no render-vector write before both words are ready. The loop writes first word for first N rows and second word thereafter, at word index 3*i through accessor 0x141250d50, with native 32-bit arithmetic/signed comparisons. Container detach/allocation and partial-write behavior retain CP279/CP287 qualifications; they are outside this smallest leaf.

## Failure boundary, missing values and next increment

Packer and callback RUNTIME_FUNCTION entries cover RVAs [0x0126a7e0,0x0126a8e6) and [0x0126a620,0x0126a7da), with unwind RVAs 0x03410148 and 0x03410140; both UNWIND_INFO version=1, flags=0. Neither installs a local exception/unwind handler. No rollback is visible. A getter/conversion failure prevents later getters/packs; first-pack failure prevents second pack; second-pack failure prevents all render stores. Once the loop starts, prior stores remain if an accessor/allocation fails. Called Qt/library functions can have their own effects; this report does not recover their implementation or guarantee exception neutrality.

The smallest next source increment is a private parameterized binary32 RGB/scalar-to-uint32 pack leaf with meaningful instruction-derived finite/boundary tests and an explicitly qualified FP domain. It needs no invented palette values, owner object, GUI or public bind success. A callback-shaped test can supply already-qualified RGB samples plus both scalar words, but cannot establish initial-bind or public color fidelity. Do not enable public color integration from these leaf tests.

Missing original values are: actual initial palette slot-12 float components at both pack read occurrences; post-bind selected QColor redF/greenF/blueF return doubles (and hence their exact CVTSD2SS results); applicable MXCSR if exceptional/rounding fidelity is claimed; actual effective owner/mode/curve/render-vector state and ordered occurrence. Scalar constants are now established and need no live-value invention. Later per-loop accessor failure effects and public lifecycle reachability remain separate requirements. No new original run is authorized or prescribed by this report.

## Compact immutable byte receipts

SHA-256 of full original PE is given above. Selected spans exclude adjacent padding:

| Preferred VA / size | SHA-256 |
| --- | --- |
| 0x14126a620 / 442 bytes | 9528e1196980048e31630ca5aac4bdd24460508767b3d89adf827ce783c70225 |
| 0x14126a7e0 / 262 bytes | 69aa9b72f26ea4ed7f80fac8ab35f9f82e6c73ba3d0e52a37c0f74685f2cf946 |
| 0x14126f300 / 49 bytes | cbe9e34637e9e221ae1d21d82d96463fddf4ac91f75b49c1a63ccee5ef553f20 |
| 0x1402d2d20 / 46 bytes | 30112e8793b576981ec7ae3c8d3b0325ea123ffc869bbf260af1f9d13fda1716 |
| 0x1421ebb18 / 4 bytes | 270696d298cc689d8e5bd75897918aa617a25a5030ccae98017053dddd974995 |
| 0x142d30298 / 4 bytes | 5f8618f85b706805a449be464fb9446b864b08dc5f23bf7e53be320f3448dba7 |

Source qualifications read: CP279 2D callback/container/seam reports, CP282 import correction, CP286 owner/receiver report, CP287 palette/render readiness report, and CP280 strict PE mapping source. Retained pseudocode and prior analyst descriptions are hypotheses where superseded by the exact PE facts above. No claim of initialized palette values, runtime resolved Qt addresses, successful original occurrence, or complete public binding follows.
