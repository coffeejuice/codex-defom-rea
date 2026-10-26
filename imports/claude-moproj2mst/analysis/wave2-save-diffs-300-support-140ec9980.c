
void FUN_140ec9980(undefined8 param_1,undefined8 param_2)

{
  char cVar1;
  undefined8 uVar2;
  undefined1 auStack_138 [32];
  undefined1 auStack_118 [8];
  undefined4 auStack_110 [6];
  undefined1 auStack_f8 [208];
  undefined4 uStack_28;
  undefined8 uStack_20;
  ulonglong uStack_18;
  
  uStack_20 = 0xfffffffffffffffe;
  uStack_18 = uRam0000000143b5e030 ^ (ulonglong)auStack_138;
  auStack_110[0] = 0;
  cVar1 = _isEmpty_QString__QEBA_NXZ(param_2);
  if (cVar1 == '\0') {
    _toLocal8Bit_QString__QEBA_AVQByteArray__XZ(param_2,auStack_118);
    uVar2 = _data_QByteArray__QEAAPEADXZ(auStack_118);
    func_0x00014146fec0(auStack_f8,uVar2);
    FUN_14146dfd4(auStack_f8,&UNK_142b14f84,auStack_110);
    uStack_28 = auStack_110[0];
    __1QByteArray__QEAA_XZ(auStack_118);
  }
  func_0x00014146df10(uStack_18 ^ (ulonglong)auStack_138);
  return;
}

