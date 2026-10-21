"""Assemble the retained native relation-property contract; no target execution."""
import hashlib
import json
from pathlib import Path
import re
import struct


def bind(path):
    p=Path(path);r={'path':p.as_posix(),'sha256':hashlib.sha256(p.read_bytes()).hexdigest()}
    if p.suffix=='.json':
        j=json.loads(p.read_text())
        if 'evidence_id'in j:r['evidence_id']=j['evidence_id']
    return r


def main():
    table=json.loads(Path('analysis/property-table-static-contract.json').read_text())
    byte=json.loads(Path('analysis/relation-property-byte-audit.json').read_text())
    ctor=json.loads(Path('analysis/volume-width-0x140333f60.json').read_text())['normalized_result']['pseudocode']
    ints={'0x0':0}
    for index,value in re.findall(r'param_1\[(0x[0-9a-f]+|[0-9]+)\] = (0x[0-9a-f]+|[0-9]+);',ctor):ints[hex(int(index,0)*4)]=int(value,0)
    doubles={}
    for index,value in re.findall(r'\*\(undefined8 \*\)\(param_1 \+ (0x[0-9a-f]+)\) = (0x[0-9a-f]+|[0-9]+);',ctor):
        offset=int(index,0)*4
        if offset in (0x770,0x778):continue
        bits=int(value,0);doubles[hex(offset)]={'uint64_hex':f'{bits:016x}','little_endian_hex':struct.pack('<Q',bits).hex(),'value':struct.unpack('<d',struct.pack('<Q',bits))[0]}
    assert len(ints)==13 and len(doubles)==39
    offsets=[0x78,0xf8,0x180,0x1f8,0x2c8,0x3f8,0x478,0x4f0,0x568,0x5e8,0x678,0x6f0]
    types=[44,55,179,48,50,54,0,0,0,0,109,110]
    profiles={}
    for type_id in sorted(set(types+[31,32,45,46,47,108])):
        profiles[str(type_id)]={'baseline':'type=argument, rank0, constant+0, selectors/counts0, width1, flag50/54/mode58/opaque60zero, all4arraypointersnull; cache68 establishednull by12d050',
            'ordered_int32_overrides':table['lifecycle']['type_specific_int32_writes'].get(str(type_id),[]),
            'allocation':'12fad0 normalizes counts/width and allocates; all selected initial profiles have zero counts and rank0width1 or higher rank withzero product, so initial arrays remainnull'}
    helpers={
        '0x140413ee0':{'format':'%i %i','destinations':['int32','int32']},
        '0x140413fb0':{'format':'%i %i %i','destinations':['int32','int32','int32'],'extra':'Fifth API argument is optional conversion-count pointer, not fourth parsed integer; callers passnull.'},
        '0x140414100':{'format':'%i %i %i %i','destinations':['int32']*4},
        '0x140414a40':{'format':'%Flf %Flf','destinations':['double','double']},
        '0x140414e40':{'format':'%Flf %Flf %Flf %Flf','destinations':['double']*4},
        '0x140414f50':{'format':'%Flf %Flf %Flf %Flf %Flf','destinations':['double']*5},
        '0x140415680':{'format':'%i %Flf %Flf','destinations':['int32','double','double']},
        '0x140415ff0':{'format':'%i %i %Flf %Flf','destinations':['int32','int32','double','double']},
        '0x140413de0':{'format':'%i','destinations':['int32'],'extra':'All selected calls third argument0: no blank-line retry.'},
    }
    for h in helpers.values():h.update(initialization='Zero every output destination before fgets, including persistent direct fields.',line='One fgets1024, comma-to-space replacement, scanf numeric prefix conversions.',status='1 only fgetsnull or scanf conversioncount==-1; zero or partial conversions return0. Caller may stage locals or expose direct zero/partial mutations.')
    headers={
        'generic_0_3':{'0':'rank0, constant=v1','1':'rank1, axis0code3, count0=trunc(v1)','2':'rank1, axis0code12, count0=trunc(v1)','3':'rank2, axiscodes3/12, counts=trunc(v1)/trunc(v2)','other':'No extra table writes after reset, or retained table unchanged when caller deliberately omits reset.'},
        'PHEQVF_0_7':{'0_or_4':'rank0, constant=v1','1_or_5':'rank1,axis0code3,count0=trunc(v1)','2_or_6':'rank1,axis0code12,count0=trunc(v1)','3_or_7':'rank2,axiscodes3/12,counts=trunc(v1)/trunc(v2)','other':'Reset scalarzero retained; rawselector still stored+5e0.'},
        'IPSNRG_0_3':{'0':'rank0,constant=v1','1':'rank1,axis0code3,count0=trunc(v1)','2':'rank1,axis0code23,count0=trunc(v1)','3':'rank2,axiscodes3/23,counts=trunc(v1)/trunc(v2)','other':'Reset scalarzero retained.'},
    }
    tttd={
        'entry':'413ee0 into local(mode,option). Onhelperstatus1 return2 beforepersistentoption/mode changes. Onsuccess option0/1/2 writesrelation+10 tosamevalue; everyotheroption retains+10.',
        'branches':[
            {'mode':1,'steps':['413fb0 locals(curve,width,field_f0);failure2 beforepersistentchanges.','414a40 locals(v1,v2);failure2 beforepersistentchanges.','curve0..3 writesrelation+14=curve and12fc70(table+78,type44+curve); unknowncurve doesneither andretainsoldtable/type/rank/mode.','Store+6c=width, table+78.width(+b8)=width,+f0=field_f0. Ifwidth==1 store double+70=v1 and+170=v2;otherwise store+170=v1,+178=v2.','413fb0 directly into tablecounts+94/+98/+9c; allthree zero/partial mutations persist evenfailure2.','41a800(FILE,table+78,0); mapstatus1to2.'], 'table':'0x78'},
            {'mode':2,'steps':['Store+14=4 BEFORE414100 locals(selector,n0,n1,n2). Headerfailure2 retainsmode4 butoldtable.','Resettable+1f8 type48, thenstorecounts+214/+218/+21c.','Ifselector!=2 return3 afterreset/countstores, beforepayloadallocation.','Storewidth+238=2 and41a800(table+1f8).'], 'table':'0x1f8'},
            {'mode':3,'steps':['Store+14=5. One fscanf with11 %Flf writes consecutive doubles+270..+2c0. Onlyoverallreturn==-1 returns2; partialpositive or0 conversions return0 and preserveunassignedpriorvalues. No line discard.'],'direct_doubles':[hex(0x270+8*i) for i in range(11)]},
            {'mode':4,'steps':['Store+14=6.414e40 directly overwriteszero/partial doubles+340/+348/+350/+358;failure2 beforecount/tablechanges.','413de0 localcount;failure2 keepscoefficients/mode butoldtable.','Resettable+2c8 type50;setcount0+2e4=count;41a800.'],'table':'0x2c8','direct_doubles':[hex(0x340+8*i) for i in range(4)]},
            {'mode':5,'steps':['Store+14=7. One fscanf with6 %Flf writes doubles+360..+388. Only-1 errors;partialpositive or0 succeeds, retainsunassignedpriorvalues, nodiscard.'],'direct_doubles':[hex(0x360+8*i) for i in range(6)]},
            {'mode':6,'steps':['Store+14=8.414e40 directly zeroes/parses doubles+390/+398/+3a0/+3a8. Statusmapped0/2;partialpersistentwrites retained.'],'direct_doubles':[hex(0x390+8*i) for i in range(4)]},
            {'mode':7,'steps':['Store+14=9. One fscanf with7 %Flf writes doubles+3b0..+3e0. Only-1 errors;partialpositive or0 succeeds, preservesunassignedvalues, nodiscard.'],'direct_doubles':[hex(0x3b0+8*i) for i in range(7)]},
            {'mode':8,'steps':['Store+14=10.413de0 localcount;failure2 beforetablemutation.','Resettable+3f8 type54;setcount0+414=count;41a800.'],'table':'0x3f8'},
            {'mode':9,'steps':['413ee0 locals(ignored,width);failure2 beforepersistentmode/tablechanges. Firstintegerisunused.','414a40 locals(v1,v2);failure2 beforepersistentmode/tablechanges.','Store+14=11;resettable+f8 type44 (constructorprofilewas55). Store+6c=width andtablewidth+138=width.','Ifwidth==1 store+70=v1,+170=v2; otherwise+3e8=v1,+3f0=v2. Do notupdate+f0 or+178.','413fb0 directly writescounts+114/+118/+11c;zero/partial stores survivefailure2.','41a800(table+f8).'],'table':'0xf8'},
            {'mode':[10,11,12,13],'steps':['Store+14=mode+2 BEFORE413de0 localwidth. Headerfailure2 retainspreviousvectorwidth/storage butnewmode.','Storetable+180.width(+1c0)=width; noreset;41a800(table+180).','Initialtype179 isrank0. Width<=1 (includingnegative) returns0 beforeallocation/normalization/reading andretainspriorphysicaldependentbuffer; width>1 reallocateszeroeddependent anddirectlyreadswidthscalars.'],'table':'0x180'},
            {'mode':'negative','steps':['Store+14=16;call484ab8(inputmode);store32-bit result+470;nootherread.','NativeMOV EAX,ECX;NEG EAX;TEST ECX,ECX;CMOVNS EAX,ECX meansINT_MIN staysINT_MIN, definedmodulo32 bitbehavior; noC++signed-overflow expression.']},
            {'mode':'0_or_positive_above13','steps':['Noadditionalbranchwrites/reads; successfulinitialheader returns0. Option+10 mayalreadyhavechanged.']},
        ],
    }
    routes=[
        {'id':135,'name':'TTTD','contract':tttd},
        {'id':136,'name':'TTTN','helper':'0x140413ab0','destination':'relation+0x18','capacity':81,'behavior':'Two fgets1024 attempts: discard first remainderline, second becomesname. Failureeitherread returns2 andpreservesoldname. Success stripsfirstLF, strncpy80, righttrimNUL/space/LF/CR overindices79..0 thenwritesNUL. Nameinitialonlybytes20/00proven; neverreadunknowninitialtail.'},
        {'id':137,'name':'PHASLH','helper':'0x140415680','table':'0x478','reset_type':31,'mapping':'generic_0_3','order':'Headerlocalzero/partial; status1 returns2 beforetablemutation. Onstatus0 reset12fc70 type31, applymapping,41a800.'},
        {'id':138,'name':'PHASVL','helper':'0x140415680','table':'0x4f0','reset_type':32,'mapping':'generic_0_3','order':'Headerfailurepreservestable; successresettype32,mapping,41a800.'},
        {'id':139,'name':'TRNSFP','helper':'0x140415ff0','table':'0x5e8','reset_type':None,'mapping':'generic_0_3','order':'Headerlocals(enable,selector,v1,v2). Failure1 returns2 withflag/tableunchanged. Enable==0 stores+5e4=0 andreturns0 withouttablemutation/payload. Enable!=0 stores+5e4=1, appliesmappingWITHOUTRESET,then41a800. Unknownselectorretainsalltablemetadata andstillreadsretainedtable.'},
        {'id':140,'name':'PHEQVF','helper':'0x140415680','table':'0x568','reset_type':108,'mapping':'PHEQVF_0_7','order':'Headerfailurepreservestable/+5e0. Successresettype108;store rawselector+5e0;applymapping;41a800.'},
        {'id':141,'name':'COARSN','helper':'0x140414f50','table':'0x678','reset_type':109,'order':'Fivezeroedoutputs: firsttwo directlyrelationdouble+668/+670; third/fourth/fifthlocals. Headerfailure2 retainsdirectzero/partial firsttwo, butflag+660/tableunchanged. Successstore+660=1;truncate third toselector;resettype109;selector0 setsconstant=fourth;selector1 rank1/axis3/counttrunc(fourth);other resetzeroscalar. Fifthignored. Then41a800.'},
        {'id':142,'name':'IPSNRG','helper':'0x140415680','table':'0x6f0','reset_type':110,'mapping':'IPSNRG_0_3','order':'Headerfailurepreservestable. Successresettype110;mapping;41a800.'},
        {'id':143,'name':'VARSEL','helper':'0x140413de0','destination':'relation+0x768 int32','order':'Readzeroedlocalwithskip0, unconditionallystore+768 evenhelperfailure;return0or2.'},
    ]
    sources=['analysis/volume-width-0x140383120.json','analysis/volume-width-0x140333f60.json','analysis/relation-import-static-contract.json','analysis/property-table-static-contract.json','analysis/property-table-byte-audit.json','analysis/relation-property-byte-audit.json','analysis/model-0x140413ee0.json','analysis/geometry-import-0x140413fb0.json','analysis/custom-field-0x140414100.json','analysis/time-controls-0x140414a40.json','analysis/model-0x140413de0.json','analysis/model-0x140413ab0.json','analysis/model-0x140415680.json']
    sources += ['analysis/relation-property-'+a+'.json' for a in ['0x140414e40','0x140414f50','0x140415ff0','0x140484ab8']]
    sources += [x['path'] for x in table['source_bindings']]
    sources += [x['path'] for x in byte['bindings']]
    sources += [__file__]
    result={'schema_version':1,'status':'STATIC_SELECTED_NINE_HANDLER_CONTRACT_COMPLETE_RUNTIME_PENDING','target':bind('source/DEF_PRE_64.exe'),
        'scope':'All nine category5 keywords135..143 including everyTTTDbranch, sharedtable lifecycle/reader operations, exactsideeffects andpartialcommits. Scientificinterpolation/use oftablevalues isnot inferred.',
        'common_dispatch':{'function':'0x140383120','abi':'RCXroot,RDXFILE*,R8DkeywordID','pair':'One fscanf(%i %i) into uninitializedlocals. Return==-1→2 beforelookup. Onlyfullyassignedtwo-int32selector domain defined here; conversions0/1 mayuseuninitializedsecond/first IDs andmustnotfabricatevalues.','lookup':'33a040(root+1358,key0,key1) exact ordered signedpair. Missingpair339df0 appendsdefaultrecord, thenassignskey0/+4; no keypositivityfilter. Creationpersistsafterpayloadfailure.','effects':'Unlike134PHASTF,135..143 do notwriteactive+0c/mode+8, do notcall1d6450 recount or1d0fc0 refresh. Cachedroot13f4/component51/52stayunchanged.','status':'Commonlocalstatus0→0,nonzero→2; TTTDmode2 invalidselector returns3 explicitly. SharedEOFselectorreturns2.','dimensions':'No2D/3Dgate inselectedcases.'},
        'record_layout':{'initialized_int32':ints,'initialized_double_bits':doubles,'name':{'offset':'0x18','capacity':81,'initialized_prefix_hex':'2000','unwritten_tail':'Do not inferinitialzeroes beyondterminator.'},'next':'0x770 initializednull','owner':'0x778 constructorargument','tables':[{'offset':hex(o),'initial_type':t} for o,t in zip(offsets,types)],'padding':'Unwrittenalignment/padding andtablepadding44/5c/70 remainunspecified.'},
        'type_profiles':profiles,'line_helpers':helpers,'header_mappings':headers,'routes':routes,
        'shared_payload_41a800':table['payload_41a800'],
        'retained_storage_cases':['TRNSFP neverresets:scalar rank0width1 earlyreturn preservesoldaxis/dependentpointers,physicalcapacities andcounts beyondrank. Unknownselector canreadexistingtable.','TTTDvector10..13 changeswidthwithoutreset; width<=1 returnsbeforeclamping/allocation. Logicalwidth maynegativewhileoldphysicalvector remainsallocated.','Whenallocationdoesrun,12fc00 clampsnegativecounts0/width<1to1.12f480 usesCURRENTcountmetadata todecidefrees: anyzero-count/non-nullaxis suppressesallfrees, thennullsall4pointers. Newaxesallocatedonlybelowrank;counts aboverankremainretained. Nativeleakpossible andmustnotbe mistakenforaliasownership inownedmodel.','Transient preallocation snapshots mustread independentlytrackedphysicalextent, notnewlogicalcount/product. Newlogicalcount mayexceedoldcapacity before41a800 reallocates.'],
        'probe_continuations':{'source':'analysis/relation-property-byte-audit.json cases[].calls','all_selected_header_payload_returns':'No directincomingbranches tolineheader/41a800 returns; exactcalls/bytes/ABIcontextbound.','shared_reset_join':'0x140383498 hasfour directincomingbranches andisnot unconditionalproof oftype47reset. Useactualresetframe orbeforecallproof.','shared_payload_join':'0x14041abc8 issharedrank0/1 completion andrank2/3freecontinuation; emitfreeonlyif temporarywasactuallyallocated.','allocation_scope':'Trackonly knownselectedtable oractiveexacttable; neverevaluateboundedselectedmetadata onglobalunselected12fad0 calls. Selectedconstructortableshavenullinitialstorage.','staging':'Rank2/3temporary ismallocuninitialized. Recordonlyassignedslots; completeconversion0 duringtemporary scan isoutsideboundeddefineddomain. EOFpreserveszerodependent andleaksnative temporary; do notpretendcommit/free.'},
        'numeric_domain':['Signed32pairkeys,mode/options anddirectintdestinations includeINT_MIN/MAX. NEG32 absolutevalue explicitlywrapsINT_MIN.','Finitebinary64 sourcevalues andsigned32truncationrange beforeC++casts; nativeCVTTSD2SI invalidconversion returnnot generalized.','Checkint32allocationproducts/sizeextents; no OOM/overflow behavior equivalence claim.','Combined11/6/7real fscanf intoinitializedpersistentcoefficients acceptsconversion0 andpartialpositivecounts; noinitializedtemporary assumption isneeded for thoseblocks.'],
        'remaining_unknowns':['Scientifictableinterpolation/solverconsumers andsave/exportformat areoutside thisimportbehaviorcontract.','Incompletepairselector usesuninitializedlocals; allocationfailures, signedcountproductoverflow, nonfinite/range CRT conversion andborrowed/cacheownership notestablished.','Fulltablemetadata54/60/cache68 scientific meaning remainsunnamed; resetpreservation behavior isknown.','NativeEOF scratchleaks/resourceusage differfromownedC++cleanup andremainreported.'],
        'source_bindings':[bind(p) for p in dict.fromkeys(sources)]}
    Path('analysis/relation-property-static-contract.json').write_text(json.dumps(result,indent=2)+'\n')
    print('saved',len(routes),'routes',len(ints),'integers',len(doubles),'doubles',len(result['source_bindings']),'bindings')


if __name__=='__main__':main()
