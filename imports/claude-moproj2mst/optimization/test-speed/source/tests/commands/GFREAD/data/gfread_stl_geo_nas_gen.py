# Writes the synthetic STL / GEO / NASTRAN inputs of the gfread_{stl,geo,nas}_* cases.
# Usage: python -I gfread_stl_geo_nas_gen.py <this data folder>
import struct, os, sys
D = sys.argv[1]
def w(name, text, mode='w'):
    with open(os.path.join(D, name), mode, newline='' if mode == 'w' else None) as f:
        f.write(text)

P1=(0,0,0); P2=(1,0,0); P3=(0,1,0); P4=(1,1,0); P5=(0.5,0.5,1)
P4n=(1.000005,0.999995,0)       # within 1e-5 of P4 -> merged
P1d=(0.000003,0,0)              # within tol of P1 -> degenerate triangle
P6=(1.00002,0,0)                # 2e-5 from P2 -> separate node
A=(0.2,0.0,0.5); B=(0.200015,0.0,0.5); C=(0.200008,0.0,0.5)  # C within tol of A and B, nearer B
Q1=(0.3,0.2,0.5); Q2=(0.25,0.3,0.5)
tris = [
  (P1,P2,P3),        # T1
  (P2,P4,P3),        # T2 consistent with T1
  (P4n,P3,P5),       # T3 inconsistent with T2 -> reversed; P4n merges into P4
  (P1,P1d,P5),       # T4 degenerate -> dropped
  (P2,P6,P4),        # T5 P6 separate node
  (A,B,Q1),          # T6 second patch
  (Q1,Q2,C),         # T7 C -> A (first match), consistent with T6? edge Q1->... 
]
def ascii(tris, name='test'):
    s = 'solid %s\n' % name
    for t in tris:
        s += '  facet normal 0.0 0.0 1.0\n    outer loop\n'
        for p in t:
            s += '      vertex %.7g %.7g %.7g\n' % p
        s += '    endloop\n  endfacet\n'
    s += 'endsolid %s\n' % name
    return s
w('stl_ascii_merge.stl', ascii(tris))
def binary(tris, extra=b''):
    b = bytearray(b'binary test header'.ljust(80, b' '))
    b += struct.pack('<I', len(tris))
    for t in tris:
        b += struct.pack('<3f', 0.0, 0.0, 1.0)
        for p in t: b += struct.pack('<3f', *p)
        b += b'\x00\x00'
    return bytes(b) + extra
w('stl_binary_merge.stl', binary(tris, b'\x01\x02\x03\x04\x05\x06\x07\x08\x09\x0a'), 'wb')
w('stl_bigbounds.stl', ascii([(P1,P2,(0,2e10,0)), (P2,P4,P3)]))
w('stl_alldegen.stl', ascii([(P1,P1d,(0,0.000004,0)), (P2,P2,P3)]))

# 3D GEO
w('geo3d_small.geo', '''6
1 0.0 0.0 0.0
2 1.0 0.0 0.0
3 1.0 1.0 0.0
4 0.0 1.0 0.0
5 0.0 0.0 1.0
6 0.1 0.0 1.0
5
1 1 2 3 4
2 1 3 2 0
3 1 4 3 0
4 2 6 5 0
5 1 2 6 1
''')
w('geo3d_badcheck.geo', '''4
1 0.0 0.0
2 1.0 0.0 0.0
3 1.0 1.0 0.0
4 0.0 1.0 0.0
1
1 1 2 3 4
''')
w('geo3d_noelem.geo', '''3
1 0.0 0.0 0.0
2 1.0 0.0 0.0
3 1.0 1.0 0.0
0
''')
# 2D GEO
w('geo2d_multi.geo', '''2
3 7
1 3.0 5.0 0.0
* a comment line
2 2.0 5.0 0.1
3 2.0 1.0 0.2
2
2 -1
1 9.0 9.0 9.0
2 8.0 8.0 8.0
1
2 3
1 7.0 7.0
2 6.0 6.0
2
2 4
1 1.5 2.5,3.5
2 1.0D0 2.0d0 // comment
3.0
''')
w('geo2d_topo.geo', '''2
2 5
1 3.0 5.0 0.0
2 2.0 1.0 0.0
2
2 6
1 2.0 1.0 0.0
2 3.0 5.0 0.0
99
* topology follows
2 2 1 1
* Vertex Data:
1 2 3.0 5.0
1
2
2 2 2.0 1.0
1
2
* Edge Data:
1 1 1 2
1
2 1 2 1
1
* Loop Data:
1 2 1
1
2
1
* Face Data:
1 1 0
1
''')
# NASTRAN
def f8(*fields):
    return ''.join(('%-8s' % x)[:8] for x in fields)
grid = lambda i,x,y,z: f8('GRID', str(i), '', x, y, z) + '\n'
w('nas_tet_fixed.nas', '$ fixed-field CTETRA test\nBEGIN BULK\n' +
  grid(1,'0.0','0.0','0.0') + grid(2,'0.1','0.0','0.0') + grid(3,'0.0','1.0','0.0') +
  grid(4,'0.0','0.0','1.0') + grid(5,'1.0-3','2.5E-1','1.0') +
  '$ CTETRA in a comment is skipped\n' +
  f8('CTETRA','1','1','1','2','3','4') + '\n' + f8('CTETRA','2','1','2','3','4','5') + '\nENDDATA\n')
w('nas_tet_free.nas', '$ free-field\nGRID,1,,0.0,0.0,0.0\nGRID,2,,0.1,0.0,0.0\nGRID,3,,0.0,1.0,0.0\nGRID,4,,0.0,0.0,1.0\nGRID,5,,1.0-3,2.5E-1,1.0\nCTETRA,1,1,1,2,3,4\nCTETRA,2,1,2,3,4,5\nENDDATA\n')
hexg = [(0,0,0),(1,0,0),(1,1,0),(0,1,0),(0,0,1),(1,0,1),(1,1,1),(0,1,0.9)]
s = 'BEGIN BULK\n' + ''.join(grid(i+1, *['%.1f' % c for c in p]) for i,p in enumerate(hexg))
s += f8('CHEXA','1','1','1','2','3','4','5','6') + '+CH1\n' + f8('+CH1','7','8') + '\nENDDATA\n'
w('nas_hex.nas', s)
w('nas_tria.nas', 'BEGIN BULK\n' + grid(1,'0.0','0.0','0.0') + grid(2,'0.1','0.0','0.0') +
  grid(3,'0.0','1.0','0.0') + grid(4,'1.0','1.0','0.0') +
  f8('CTRIA3','1','1','1','2','3') + '\n' + f8('CTRIA3','2','1','2','4','3') + '\nENDDATA\n')
w('nas_none.nas', 'BEGIN BULK\n' + grid(1,'0.0','0.0','0.0') + 'CQUAD4  1       1       1       2       3       4\nENDDATA\n')
# extra cases
s = ascii([(P1,P2,(0,2e10,0)), (P2,P4,P3)]).replace('endsolid test', 'endsolid')
w('stl_bigbounds_noname.stl', s)
X1=(0.500496,0.5,0); X2=(0.500504,0.5,0)
w('stl_bucket_edge.stl', ascii([(P1,X1,P3), (X2,P2,P4), (P1,P2,X2), (P3,X1,P4)], 'flat'))
