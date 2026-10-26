# builds s1-01/ledger.tsv from the decompiled function list (dc/*.c) and the roles below
import pickle, re, glob, collections
P = 'C:/Users/alext/Projects/deform-rea-work/gui_mo/DEF_GUI_MO_64.exe'
refs, fs, fe = pickle.load(open(P + '.xref.pkl', 'rb'))
B = 0x140000000
size = {a + B: b - a for a, b in zip(fs, fe)}
addrs = set()
for f in glob.glob('dc/*.c'):
    addrs |= {int(x, 16) for x in re.findall(r'^// ===== FUN_([0-9a-f]+)', open(f, encoding='utf-8').read(), re.M)}
R = {}
S = 'spec'
def r(st, role, *a):
    for x in a:
        R[x] = (role, st)

r(S, 'OpenProject: mode by suffix, P+0x18, flags +0x55..57', 0x14008a9b0)
r(S, 'ReadProjectFile: QFile + QDomDocument::setContent', 0x14008afa0)
r(S, 'WalkProject: root children dispatch', 0x14008b410)
r(S, 'ParseOperationList', 0x14008cb50); r(S, 'ParseOperation', 0x14008ccf0)
r(S, 'ParseItemList (MoItem)', 0x14008e290); r(S, 'ParseMoItem', 0x14008e3d0)
r(S, 'ParseSubProject index>=1 (DbName only)', 0x14008f200)
r(S, 'ParseSubProject full (on activation)', 0x14008f530)
r(S, 'ParseDiagramItem', 0x140091f00); r(S, 'ParseConnectors', 0x140092ff0)
r(S, 'ParseConnections/ItemIndex', 0x140093140); r(S, 'ParseItemData', 0x140091db0)
r(S, 'WriteBlob hex+qUncompress to file (overlay in DLL)', 0x1400937b0)
r(S, 'ParseObjPropertiesKey', 0x140093ab0); r(S, 'ParseObjects', 0x140093c30)
r(S, 'ParseProperties', 0x140094f70); r(S, 'ApplyProperty (recursive)', 0x1400952c0)
r(S, 'ParseActionKeywords', 0x140095cc0); r(S, 'ParseNSTART', 0x140095e40)
r(S, 'ParseUserDefinedKey', 0x140096010); r(S, 'ParseRestartKey', 0x1400961d0)
r(S, 'ParseKeywordText TLOC/OBJPOS/INICTC/GENCTC/state vars', 0x140096390)
r(S, 'KEY tokenizer (twin of DEF_PRE reader)', 0x140906e40)
r(S, 'tokenizer replace exponent char', 0x140906790); r(S, 'tokenizer toupper', 0x140906880)
r(S, 'tokenizer state init', 0x140906740)
r(S, 'SVSCAL array reader', 0x1400abab0); r(S, 'SVSCAL set', 0x1400aba20)
r(S, 'operation factory, legacy Type mapping', 0x140046cc0)
r(S, 'unique operation name', 0x140047630); r(S, 'name exists in OprManager', 0x14006d040)
r(S, 'registry lookup with error message', 0x140047c00); r(S, 'registry find', 0x140070650)
r(S, 'registry contains', 0x140070920); r(S, 'registry build (all operation classes)', 0x14006dba0)
r(S, 'registry insert', 0x14006f940)
r(S, 'OprManager create item vt+0x170', 0x1400468e0); r(S, 'SfMoOprItem ctor', 0x140045c30)
r(S, 'SfMoOprItem init', 0x140045d10); r(S, 'create SfOperationInfo', 0x140045f10)
r(S, 'SfOperationInfo ctor', 0x14004fcd0); r(S, 'SfOperationInfo ctor from source', 0x14004fe20)
r(S, 'SfOperationInfo defaults', 0x140050550); r(S, 'SfOperationInfo copy', 0x1400500b0)
r(S, 'set operation id, task dir, mkpath', 0x140050820); r(S, 'mkpath task dir', 0x140051190)
r(S, 'set task dir rel', 0x140051160); r(S, 'OPI reset ptrs/ids', 0x140051250)
r(S, 'InterObject block init', 0x1400651f0); r(S, 'OBJPOS record ctor', 0x1400658e0)
r(S, 'OBJ ctor', 0x14004d160); r(S, 'OBJ defaults', 0x14004d450); r(S, 'OBJ set slot no', 0x14004d520)
r(S, 'project ctor', 0x140089880); r(S, 'set path + DB path', 0x14008a470)
r(S, 'default DB path', 0x1400a4cc0); r(S, 'SetPrimaryDb', 0x1400a4da0)
r(S, 'set unit + globals', 0x1400a2150); r(S, 'unit globals (engine)', 0x140917d60)
r(S, 'FOR003 rename offer (UI, skip in DLL)', 0x1400a5290)
r(S, 'SUB ctor (open)', 0x1400b5470); r(S, 'SUB ctor (GUI new sub-project)', 0x1400b5680)
r(S, 'activate sub-project', 0x1400a7250); r(S, 'activate + parse sub-project', 0x1400a7b90)
r(S, 'find i-th Sub_Project element', 0x1400a78e0)
r(S, 'flow-chart order for MST', 0x1400b6630); r(S, 'group items sorted', 0x140085190)
r(S, 'non-group items in a group range, sorted', 0x140085470); r(S, 'connector chain', 0x140084ad0)
r(S, 'chain number of op id (groups share)', 0x140085800); r(S, 'predecessor id', 0x140086720)
r(S, 'find item by id', 0x140086820); r(S, 'compare by cell', 0x140084a50)
r(S, 'compare by group begin', 0x140085110); r(S, 'model successor', 0x14014d540)
r(S, 'model predecessor', 0x14014d620); r(S, 'model append item', 0x14014cf20)
r(S, 'MoItem list add vt+0x198', 0x14006c9c0)
for a, k in [(0x140068e60, 'Object'), (0x140042850, 'Material'), (0x1400bd910, 'Lubricant'), (0x1400bef30, 'Equipment')]:
    r(S, 'KEY path from id: ' + k, a)
for a, k in [(0x1400a4510, 'Objects'), (0x1400a4640, 'Materials'), (0x1400a48a0, 'Lubricants'),
             (0x1400a4770, 'Equipments'), (0x1400a43e0, 'Operations'), (0x1400a49d0, 'DoeStudies')]:
    r(S, 'project subdir ' + k, a)
r(S, 'mkpath project subdirs (side effect)', 0x14008c630); r(S, 'mkpath with message box', 0x14081afb0)
r(S, 'task path SimCtrl.KEY', 0x140059290); r(S, 'task path InterObject.KEY', 0x14005a690)
r(S, 'task path ObjProperties%05d.KEY', 0x14005eb10); r(S, 'taskDirAbs', 0x140050df0)
r(S, 'material type from string', 0x140041eb0); r(S, 'material is mixture', 0x140041e70)
r(S, 'material phase list', 0x1400421b0)
r(S, 'set object id in slot', 0x14005fc80); r(S, 'get/create OBJ in slot', 0x140061250)
r(S, 'create OBJ', 0x140061020); r(S, 'new OBJPOS record', 0x140061e30)
r(S, 'qUncompress wrapper', 0x1400ab710); r(S, 'absolutePath of file', 0x140819590)
r(S, 'endsWithSuffix', 0x140818b50); r(S, 'changeSuffix', 0x140818d50)
r(S, 'strip leading dot', 0x140818e20); r(S, 'completeBaseName', 0x140819090)
r(S, 'path+sep+base+.ext cleaned', 0x1408192e0); r(S, 'projectDir(P)', 0x1400a4220)

acc = [0x140050ac0, 0x140050b10, 0x140050b20, 0x140050bc0, 0x140050d00, 0x140050910, 0x140051110, 0x140051230,
       0x140054e90, 0x140054ff0, 0x140055030, 0x140055050, 0x140055150, 0x1400550d0, 0x14005a300, 0x14005a2a0,
       0x14005a2e0, 0x14005f0f0, 0x14005f150, 0x14005f1c0, 0x14005f230, 0x140064740, 0x14005f770, 0x14005f820,
       0x14005fa80, 0x14005fb30, 0x14005fc50, 0x14005fc10, 0x140061220, 0x1400612d0, 0x140061310, 0x140060640,
       0x140060690, 0x1400606c0, 0x140060600, 0x140062070, 0x1400620a0, 0x1405ef320, 0x1405ef2b0, 0x1405ef280,
       0x1405f8d90, 0x1405f8d60, 0x1405f8dc0, 0x1400a2180, 0x140c0df50, 0x140ceed20, 0x14008a500, 0x14008a4b0,
       0x1400a4ee0, 0x1400a7360, 0x1400a74f0, 0x1400a7390, 0x1400a73c0, 0x1400a75a0, 0x1400a7670, 0x1400b5a70,
       0x1400b5a90, 0x1400b5c60, 0x140047dc0, 0x140047950, 0x1400a58b0, 0x1400a58f0, 0x1400a5930, 0x1400a5970,
       0x1400a59b0, 0x1400a5890, 0x140060030, 0x1400690a0, 0x140045f90, 0x14006b8a0, 0x140065790, 0x14004df00,
       0x140068b40, 0x1400666e0, 0x1400667b0, 0x140050a50, 0x1400509d0, 0x1400b5f40, 0x140045e80, 0x14004c900,
       0x14006c210, 0x1400a7760, 0x140047910, 0x14004bd60, 0x14006cdd0, 0x14005f0f0, 0x1400135a0]
for a in acc:
    R.setdefault(a, ('accessor (spec section 10)', S))
setters = [0x14004ca30, 0x1407b5170, 0x1400ab6f0, 0x140050930, 0x140050a90, 0x140050c80, 0x140050d50, 0x14004c9a0,
           0x1400511f0, 0x14005a350, 0x14005a2c0, 0x140062ed0, 0x140062e60, 0x140064c60, 0x140064760, 0x1400abc90,
           0x140065270, 0x1400ab7c0, 0x1400ab7e0, 0x1404ce5b0, 0x14004d580, 0x14004d700, 0x14004d780, 0x14004d7d0,
           0x14004db20, 0x14004dc20, 0x14004deb0, 0x14004df20, 0x14004df60, 0x14004dfa0, 0x14004dfe0, 0x14004e020,
           0x14004e0a0, 0x14004e0e0, 0x14004e120, 0x14004e160, 0x14004e810, 0x1400abc40, 0x1400abc60, 0x1400ab800,
           0x1400ab820, 0x1400ab840, 0x1400ab860, 0x1400ab880, 0x1400ab8a0, 0x1400ab8c0, 0x141432bc0, 0x1412e2450,
           0x1400ab8e0, 0x1400ab900, 0x1416cab50, 0x141632700, 0x1400ab920, 0x141163320, 0x1400ab940, 0x140bb1000,
           0x1400ab960, 0x1400ab980, 0x1400aba00, 0x1400ab9a0, 0x1400ab9c0, 0x140414430, 0x1400ab9e0, 0x1400a4150,
           0x1400a41f0, 0x1400b9f10, 0x14006ba30, 0x14006b8e0, 0x14006bb40, 0x14006b9c0, 0x14006b9f0, 0x14006baf0,
           0x14004d730, 0x14004db50, 0x14004ded0, 0x14004e1c0, 0x14004e210, 0x14005fa60, 0x14005a370]
for a in setters:
    R.setdefault(a, ('field setter/getter (spec sections 5-9)', S))
ctors = [0x140044a90, 0x140044c80, 0x140044d20, 0x140044dc0, 0x140044e60, 0x140044f00, 0x140068ba0, 0x1400425a0,
         0x1400bd680, 0x1400beca0, 0x140045fe0, 0x14006c120, 0x140068df0, 0x1400427e0, 0x1400bd8a0, 0x1400beec0,
         0x1400688f0, 0x140041be0, 0x1400bd580, 0x1400beba0, 0x14006b6a0, 0x14006b7c0, 0x1400689a0, 0x140041c90,
         0x1400bd610, 0x1400bec30, 0x14004e260, 0x140066860, 0x140070f10, 0x1400711a0, 0x1400c4890, 0x1412f10d0,
         0x140046960, 0x1400653e0]
for a in ctors:
    R.setdefault(a, ('model object ctor (spec 9)', S))
for a, cls in [(0x14013ab80, 'ShapeOptim'), (0x1401c24d0, 'Convert2Dto3D'), (0x14029a880, 'DiestressBM2D'),
               (0x14029c6c0, 'DiestressBM3D'), (0x1402b4fb0, 'Diestress'), (0x140600a50, 'MultiBlow2D'),
               (0x140601480, 'MultiBlow'), (0x140750140, 'Cogging'), (0x140c043e0, 'Forming2D'),
               (0x140c05bc0, 'Forming'), (0x140c65420, 'HTFurnace'), (0x140e22470, 'CopyMirror'),
               (0x140e2df30, 'DoeOutput'), (0x1411ae690, 'HeatTreat2D'), (0x1411b0ca0, 'HeatTreat'),
               (0x1411de9b0, 'Boolean'), (0x1412ad930, 'Report'), (0x1413474d0, 'Pre2D'), (0x1413489d0, 'Pre'),
               (0x14134d2e0, 'Cycle'), (0x141352020, 'Group'), (0x1413530d0, 'HeatTransfer2D'),
               (0x141353ec0, 'HeatTransfer'), (0x141362050, 'Distortion'), (0x1413e57a0, 'HeatTransferExpress2D'),
               (0x1413e6060, 'HeatTransferExpress'), (0x1414126f0, 'FormingExpress2D'), (0x141413000, 'FormingExpress')]:
    R[a] = ('SfOpr%s init vt+0x180 (registry key)' % cls, S)
for a, cls in [(0x1400707b0, 'DiestressBM2D'), (0x140070810, 'DiestressBM3D'), (0x141361f70, 'Distortion'), (0x1411de8d0, 'Boolean')]:
    R[a] = ('SfOpr%s ctor' % cls, S)
R[0x1400b3190] = ('express/distortion class name from wizard (not open)', 'out-of-scope')
for a, d in [(0x140760430, 'Cogging PROPS +0xc0'), (0x140761c90, 'Cogging MDT load'),
             (0x140761d90, 'Cogging.MDT reader (s1-03)'), (0x140761580, 'Cogging.MDT name'),
             (0x1411e09d0, 'Boolean PROPS +0xc0'), (0x1411e1cd0, 'Boolean BWZ load'),
             (0x1411e1dd0, 'Boolean.BWZ reader (s1-04)'), (0x1411e0a00, 'Boolean.BWZ name'),
             (0x141402060, 'Express PROPS +0xc0'), (0x1413f8280, 'Express FWZ load'),
             (0x1413f83e0, 'Forming.FWZ reader (s1-05)'), (0x1413f7230, 'Forming.FWZ name')]:
    R[a] = ('open-time class hook, spec 6.4: ' + d, 'out-of-scope')
R[0x14134ec40] = ('Cycle PROPS +0xc0: Cycle.CWZ WZD_CYCLE_COUNT', S)
R[0x14134ee90] = ('Cycle set count', S)
R[0x140cac130] = ('PROPS +0xc0 default (no-op)', S)
R[0x1405f8420] = ('qt_metacast SfOprBaseProperties', 'library')
for a, d in [(0x140131680, 'MST header'), (0x1401313c0, 'MST top'), (0x1401318b0, 'MST main project'),
             (0x140131a50, 'MST sub-project'), (0x140131df0, 'MST operation walk'), (0x140132100, 'MST op call'),
             (0x1401321a0, 'MST DOE op call'), (0x140131d10, 'MST trailer'), (0x1400a20d0, 'Save MST entry'),
             (0x1400a50a0, 'MST path')]:
    R[a] = (d + ' (read for accessors; spec in s1-02)', 'out-of-scope')
for a, d in [(0x1405f5bf0, 'OB vt+0x190 dim (base)'), (0x140e329d0, 'OB vt+0x190 DoeOutput'), (0x14134dc50, 'OB vt+0x190 Cycle')]:
    R[a] = (d, S)
for a, d in [(0x140051340, 'lazy Open operation (UI select)'), (0x140051c30, 'lazy open, new project message'),
             (0x140047f80, 'select operation, lazy open'), (0x1405ef430, 'OB vt+0x58 load op KEYs into engine'),
             (0x140064cb0, 'copy material ids from engine'), (0x140064d40, 'copy material id per object'),
             (0x1405ef3a0, 'OB widget'), (0x1405ef960, 'OB info string'), (0x1405ef9d0, 'OB check DB'),
             (0x140c7b720, 'engine model reset'), (0x140c7b2d0, 'engine model +0x750'), (0x1400c3950, 'event pump')]:
    R[a] = (d + ' (Q4)', 'out-of-scope')
for a, d in [(0x14001ca30, 'File>Open dispatcher (licence-type dialogs not followed)'), (0x1400a6a00, 'GUI new sub-project'),
             (0x1400a9be0, 'template open .motmpl'), (0x1400aa000, 'template walk'), (0x1400a89e0, 'main window helper'),
             (0x1400aa960, 'post-open: register OB names into P+0x60 (Q6)'), (0x140082710, 'model set active item'),
             (0x140047af0, 'set active op id'), (0x1400b5e30, 'active item id'), (0x1400b5e90, 'set active item'),
             (0x140075050, 'connect items (+labels, Q5)'), (0x140073900, 'model create item'),
             (0x140044bf0, 'delete P'), (0x140054f40, 'OPI model +0x160'),
             (0x14004f8c0, 'fragment: NRECVL/ERECVL emitter (s1-02)'), (0x140068c10, 'fragment'),
             (0x1400d7fc0, 'info +0x18 getter'), (0x1400a4f30, 'exists(P+0x30)'), (0x14008a550, 'empty string'),
             (0x140cdd4f0, 'M dir'), (0x140cdd620, 'M base'), (0x140c79390, 'create data root M'),
             (0x14002a9b0, 'window: set current sub-project'), (0x1400a70b0, 'remove sub-project'),
             (0x1400a7a00 if False else 0x140047a00, 'current op id')]:
    R.setdefault(a, (d, 'out-of-scope'))

for a, d in [(0x1400600a0, 'object item for slot (MST closure; s1-02 to detail)'), (0x140062e80, 'OPI getter (MST closure; s1-02 to detail)'),
             (0x140065560, 'int +0x1c == 2 test (MST closure; s1-02)'), (0x140065800, 'dim == 2 test (MST closure; s1-02)'),
             (0x14006fb40, 'registry map1 find'), (0x14006fbd0, 'registry map1 op'), (0x14006ff90, 'registry map1 op'),
             (0x140068880, 'item colour (UI)'), (0x1400ab770, 'DOE study path getter'), (0x1400bc640, 'DOE getter'),
             (0x1400bc700, 'DOE helper'), (0x1400bc870, 'dtor'), (0x1400bcd30, 'dtor'), (0x1400bcdb0, 'DOE helper')]:
    R[a] = (d, 'out-of-scope')
R[0x140353770] = ('property tree root list', S)
out = []
for a in sorted(addrs | set(R)):
    role, st = R.get(a, (None, None))
    if role is None:
        role, st = 'Qt/QList/QString/QMap helper', 'library'
    out.append('%x\t%s\t%s\t%s' % (a, ('0x%x' % size[a]) if a in size else '?', role, st))
open('ledger.tsv', 'w', encoding='utf-8').write('address\tsize\trole\tstatus\n' + '\n'.join(out) + '\n')
print(collections.Counter(l.split('\t')[3] for l in out))
