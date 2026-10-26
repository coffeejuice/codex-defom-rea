"""mbprops.py <case_dir> <spec.json-key>: rewrite the MultiBlow property tree of a GUI-saved case.

Spec (in mb_specs below): scalar overrides by property name, and heats = [[blow, ...], ...] where a
blow is a dict of overrides for BlowEnergy, BlowEfficiency, BlowDwellTime, BlowReheat, BlowFlip,
BlowRotAngle, BlowRotSelfCenter, BlowRotCenterX/Y/Z, BlowRotAxisX/Y/Z (suffix = blow index j).
"""
import os, re, sys

BLOW = [("BlowEnergy", "double", "100"), ("BlowEfficiency", "double", "1"), ("BlowDwellTime", "double", "1"),
        ("BlowReheat", "bool", "false"), ("BlowFlip", "bool", "false"), ("BlowRotAngle", "double", "0"),
        ("BlowRotSelfCenter", "bool", "true"), ("BlowRotCenterX", "double", "0"), ("BlowRotCenterY", "double", "0"),
        ("BlowRotCenterZ", "double", "0"), ("BlowRotAxisX", "double", "0"), ("BlowRotAxisY", "double", "0"),
        ("BlowRotAxisZ", "double", "0")]


def prop(ind, name, typ, val):
    p = " " * ind
    return ("%s<Property>\n%s <Name>%s</Name>\n%s <TypeName>%s</TypeName>\n%s <Value>%s</Value>\n%s</Property>\n"
            % (p, p, name, p, typ, p, val, p))


FE_BLOW = BLOW[:5]


def heats_xml(heats, temps, fields=BLOW):
    out = ""
    for i, blows in enumerate(heats, 1):
        out += "     <Property>\n      <Name>Heat%d</Name>\n      <TypeName>group</TypeName>\n" % i
        out += prop(6, "Temperature%d" % i, "double", temps[i - 1])
        out += prop(6, "NumBlows%d" % i, "int", str(len(blows)))
        for j, b in enumerate(blows, 1):
            out += "      <Property>\n       <Name>Blow%d</Name>\n       <TypeName>group</TypeName>\n" % j
            for n, t, v in fields:
                out += prop(7, "%s%d" % (n, j), t, b.get(n, v))
            out += "      </Property>\n"
        out += "     </Property>\n"
    return out


def apply(path, scal, heats, temps, type_prefix="<Type>SfOprMultiBlow", fields=BLOW):
    s = open(path, encoding="utf-8").read()
    i = s.index(type_prefix)
    j = s.index("</Operation>", i)
    blk = s[i:j]
    for name, val in scal.items():
        blk, n = re.subn(r"(<Name>%s</Name>\n\s*<TypeName>[^<]*</TypeName>\n\s*<Value>)[^<]*(</Value>)" % re.escape(name),
                         lambda m: m.group(1) + val + m.group(2), blk)
        if n != 1:
            sys.exit("property %s found %d times" % (name, n))
    blk, n = re.subn(r"(<Name>NumHeats</Name>\n\s*<TypeName>int</TypeName>\n\s*<Value>)\d+", lambda m: m.group(1) + str(len(heats)), blk)
    assert n == 1
    a = blk.index("     <Property>\n      <Name>Heat1</Name>")
    nxt = [m.start() + 1 for m in re.finditer(r"\n     <Property>\n      <Name>(\w+)</Name>", blk[a:]) if not re.match(r"Heat\d+$", m.group(1))]
    close = blk.index("\n    </Property>\n", a) - a + 1
    end = a + min(nxt + [close])   # the first sibling after the last HeatN group, or the parent's close
    blk = blk[:a] + heats_xml(heats, temps, fields) + blk[end:]
    s = s[:i] + blk + s[j:]
    open(path, "w", encoding="utf-8", newline="\n").write(s)
