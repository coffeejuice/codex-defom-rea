# CP116 bounded SPRING reader increment

## Implemented profile

The native reader now accepts two exact count-two, complete ASCII spellings only when `SPRING2.INI` is absent. The first is the seed CRLF fixture from `resume116-spring-multirow-parser.md`:

```text
2\r\n10\r\n0.5\r\n1.5\r\n20\r\n0\r\n1\r\n0\r\n30\r\n2.5\r\n3.5\r\n40\r\n1\r\n0\r\n1\r\n
```

It returns rows in file order:

- `{10, 0.5, 1.5, 20, [0, 1, 0], 0}`
- `{30, 2.5, 3.5, 40, [1, 0, 1], 0}`

The second is the exact normalized writer output for those same rows:

```text
2\t\n10\t\n0.5\t1.5\t20\t0\t\n1\t0\t\n30\t\n2.5\t3.5\t40\t1\t\n0\t1\t\n\r\n
```

Both spellings decode to the same ordered rows; both flags are zero from the absent companion profile. This is an exact fixture admission, not a generalized multirow token grammar. The existing two count-one spellings remain admitted unchanged. Count three, an incomplete count-two row, any other count-two bytes, and a present companion file remain unsupported. Public nonempty-spring page/save admission remains root-owned and unchanged in this lane.

## Focused test and static preflight

The reader test asserts both distinct rows and field order for the seed and writer-normalized forms. It rejects a count-two spelling with one altered direction token, a count-two file missing its second row, count three with two rows, present `SPRING2.INI`, and the existing truncated count-one row. This validates only native reader results; no GUI, page state, build, or test runner was exercised here.

`git diff --check` passed for the owned tracked source. The new focused test remains subject to root's CMake registration and build/CTest integration.

## Frozen files

| File | SHA-256 |
|---|---|
| `source/apps/moproj2mst/src/engine/simcontrol300_native_io.cpp` | `9A85BFE60CC34A6C33C49C0F5AD93612FA40A26CC15181FD1EC1CE8A9759F6FA` |
| `source/apps/moproj2mst/tests/mdt/test_simcontrol300_spring_reader.cpp` | `83758504FC4FE56CA99D9E9D6918B57070DB2814CF0CDBA4438722D0FC78BE9D` |

No shared header, page/list implementation, writer, CMake, build, GUI, or REA state was changed. Original runtime comparison remains pending.
