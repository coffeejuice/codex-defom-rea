# Convert2Dto3D initial editor depends on chain-head state

The original tree builder0x1401c5000 adds Geometry Type before Configuration when0x140050bc0 reports no predecessor in the active chain. For non-head operations Configuration is first. The predicate is already reconstructed as `isChainHead`; it does not use the input-mode preference.

The Convert2Dto3D factory0x1401cca90 handles Configuration as SfConv23Config, but passes GeometryType to base factory0x1405ff0b0. Its first branch creates SfGeometryType0x1410580a0 regardless of input mode. The constructor sets up its widgets, initializes editor+0x50=0, then fills them from the model.

`setupUi` checks the2D Axisymmetric radio(ui+0x28) at0x141059736 and the3D Whole Part radio(ui+0x80) at0x14105a536. It selects stack page0. The2D fill0x141058370 reads GEOTYP at SC+0x100 and checks Axisymmetric/Plane/Torsion/PlaneStress for1/2/3/4. An unrecognized value leaves the setup default.

Save uses editor vt+0x1c0=0x141058600, which dispatches2D apply0x141058670. That function reads the four checked radios and writes GEOTYP1..4. Its additional type1/2 property setter requires an Express properties cast, which Convert2Dto3D does not satisfy. This2D path does not run the Configuration model copy, WPAXIS rewrite, or TEMP_STEP file writer. The leaving callback0x141058240 has separate model-reset behavior; it must not be confused with Save.

This supports replacing the inherited unconditional Configuration init→apply→init approximation with the actual selected editor branch and state. It does not independently establish every missing TEMP_STEP cause. Source and address selections, literal mappings, defaults, hashes and remaining qualifications are in the adjacent JSON. Original code was not executed; no L5 source was edited by this worker.
