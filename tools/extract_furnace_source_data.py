"""Extract bounded furnace constants from retained REA evidence, without eval."""
import hashlib
import json
from pathlib import Path
import re
import struct


def main():
    target='f78a2a5c3ebb3c1affca3d2d0ea31f0ed802f6fe1e9083168681502ac6cea794'
    layout_path=Path('analysis/environment-default-layout-bytes.json')
    factor_path=Path('analysis/environment-unit-factor-bytes.json')
    source_path=Path('analysis/configuration-0x140375e90.json')
    records=[json.loads(p.read_bytes())for p in [layout_path,factor_path,source_path]]
    assert all(r['subject']['digest']['sha256']==target for r in records)
    layout,factors=records[:2]
    for r,count in [(layout,400),(factors,704)]:
        assert r['normalized_result']['complete'] and r['normalized_result']['returned_bytes']==count
    values=struct.unpack('<100i',bytes.fromhex(layout['normalized_result']['bytes_hex']))
    rows=[list(values[i:i+5])for i in range(0,100,5)]
    bits=struct.unpack('<88Q',bytes.fromhex(factors['normalized_result']['bytes_hex']))
    axis_path=Path('analysis/environment-axis-unit-bytes.json')
    axis_record=json.loads(axis_path.read_bytes())
    assert axis_record['subject']['digest']['sha256']==target
    assert axis_record['normalized_result']['complete']
    axis_units=struct.unpack('<33i',bytes.fromhex(axis_record['normalized_result']['bytes_hex']))
    source=records[2]['normalized_result']['pseudocode']

    def close(start):
        depth=1;i=start+1
        while depth:
            if source[i]=='{':depth+=1
            elif source[i]=='}':depth-=1
            i+=1
        return i

    writes=[]
    for index in range(1,11):
        match=re.search(r'local_24\[0\] != '+str(index)+r'\)',source)
        start=source.index('{',match.end());end=close(start)
        tail=re.match(r'\s*else\s*\{',source[end:]);assert tail
        begin=end+tail.end()-1;part=source[begin+1:close(begin)-1]
        for match in re.finditer(r'FUN_140130800\(local_10,(0x[0-9a-f]+|0),(\d+),0,0,([^;]+)\);',part):
            value,row,column=match.groups()
            literal=re.fullmatch(r'(0x[0-9a-f]+|\d+)',column.strip())
            subtract=re.fullmatch(r'(0x[0-9a-f]+|\d+) - \*\(int\s*\*\)\(local_10 \+ 0x40\)',column.strip())
            assert literal or subtract,(index,column)
            col=int((literal or subtract).group(1),0)-(rows[index-1][4] if subtract else 0)
            writes.append([index,int(row),col,int(value,0)])
    assert len(writes)==source.count('FUN_140130800(')==40,len(writes)
    text=['// Generated from retained REA data; see furnace-source-extraction.json.',
          'constexpr std::array<std::array<int, 5>, 20> default_layout{{']
    text+=['    {'+', '.join(map(str,row))+'},'for row in rows]
    text+=['}};','constexpr std::array<std::array<std::uint64_t, 2>, 44> unit_factors{{']
    text+=['    {0x%016xULL, 0x%016xULL},'%(bits[i],bits[i+1])for i in range(0,88,2)]
    text+=['}};','constexpr std::array<int, 33> axis_unit_codes{'+', '.join(map(str,axis_units))+'};',
           'struct DefaultWrite { int parameter, row, column; std::uint64_t bits; };',
           'constexpr std::array<DefaultWrite, 40> default_writes{{']
    text+=['    {%d, %d, %d, 0x%016xULL},'%tuple(v)for v in writes]
    text+=['}};','']
    output=Path('src/recovered/furnace_source_data.inc');output.write_text('\n'.join(text),encoding='utf-8',newline='\n')
    proof=dict(status='EXTRACTED_SOURCE_CONSTANTS',target_sha256=target,sources=[dict(path=str(p).replace('\\','/'),sha256=hashlib.sha256(p.read_bytes()).hexdigest(),evidence_id=r['evidence_id'])for p,r in zip([layout_path,factor_path,source_path,axis_path],records+[axis_record])],layout_rows=rows,default_writes=writes,output=dict(path=str(output).replace('\\','/'),sha256=hashlib.sha256(output.read_bytes()).hexdigest()),qualification='Source/data extraction only. Profile/setter lifetime, unit algorithms and original-process comparisons are separate obligations. No captured model data seeds this file.')
    Path('analysis/furnace-source-extraction.json').write_text(json.dumps(proof,indent=2)+'\n',encoding='utf-8',newline='\n')
    print(json.dumps(dict(layout_rows=len(rows),factor_rows=44,default_writes=len(writes))))


if __name__=='__main__':main()
