@AGENTS.md

## Claude Code specifics

* `AGENTS.md` (imported above) is the canonical guide; put shared rules there, not here. This file
  only adds what is specific to Claude Code.
* Skills are in `.claude/skills/` and load by name (`/gate`, `/record-oracle`, ...).
* The REA MCP server `rea` is registered for this folder at local scope (`~/.claude.json`). If a
  target is already open, do not call `open_binary` / `close_binary` again from a helper.
* Project memory (`~/.claude/projects/C--Users-alext-Projects-claude-deform-rea/memory/`) holds
  per-packet history and is keyed to this folder's path, so do not rename the folder. Durable rules
  belong in `AGENTS.md` or `docs/port-conventions.md`; memory is for state that changes.
* Use the Edit or Write tools for C++ edits; heredocs with escapes have corrupted sources before.
* Parallel workers: one build folder (`port\build_<x>`) and one Ghidra copy each; workers do not
  commit, and only the session that owns the branch pushes.
