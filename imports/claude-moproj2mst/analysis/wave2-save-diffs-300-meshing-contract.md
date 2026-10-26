The meshing helper now retains four scalar controls per eligible object and the original selected form's text, radio and spin state. It remains internal; public editor300 Save is still refused.

Direct original bytes corrected the earlier broad-copy hypothesis. GUI14085e050 copies only four doubles, not the port's mesh shapes or owning arrays. The original sorts labels before selecting form0, so eligible object10 can be selected before object2. Only form0 receives a text/spin roundtrip; unvisited scalar values retain their bits. Apply writes only the four recovered fields, preserving unrelated native mesh state.

The separate remesh state now has source-proven constructor defaults and explicit filled controls. Enabled Save writes the method, clamped scaling factor and inverted internal-element checkbox; disabled Save only writes the enabled flag. Five new tests cover this behavior, including sorting, snapshot persistence, signed zero, node conversion/clamps, unvisited NaN payloads and unrelated native fields. The coordinator owns build and execution.

Project Open also loads this state from LOCAL_REMESH.DAT or fallback LOC_DEN_CTRL.DAT. If both are absent, the original returns false before changing any state; Open assigns that result to enabled. The fresh constructor defaults therefore establish a bounded disabled branch. File loading/writing is not yet connected to these helpers. Original project Save removes LOCAL_REMESH.DAT when disabled and otherwise invokes its writer.

The JSON contract retains exact addresses, guards, qualifications and source/evidence hashes. Environment recovery is the active successor; its unknown feature state remains explicit rather than inferred.
