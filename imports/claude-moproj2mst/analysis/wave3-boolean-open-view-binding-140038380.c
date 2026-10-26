
void FUN_140038380(undefined8 param_1,char param_2)

{
  char cVar1;
  int iVar2;
  undefined8 uVar3;
  longlong lVar4;
  longlong *plVar5;
  int iStack_88;
  undefined1 auStack_80 [8];
  longlong *plStack_78;
  longlong lStack_70;
  undefined1 auStack_68 [8];
  longlong *plStack_60;
  undefined4 auStack_58 [2];
  undefined8 uStack_50;
  undefined1 auStack_48 [8];
  longlong *plStack_40;
  undefined8 uStack_38;
  undefined1 auStack_30 [8];
  undefined1 *puStack_28;
  undefined8 uStack_20;
  undefined8 uStack_18;
  longlong lStack_10;
  
  uStack_20 = 0xfffffffffffffffe;
  uVar3 = FUN_1400133a0(param_1);
  lVar4 = FUN_140045780(uVar3);
  if ((lVar4 != 0) && (cVar1 = func_0x00014003f670(lVar4), cVar1 == '\0')) {
    plVar5 = (longlong *)FUN_140012fa0(param_1);
    lVar4 = FUN_1400a7390(lVar4);
    if ((plVar5 != (longlong *)0x0) && (lVar4 != 0)) {
      lVar4 = func_0x0001400b5a70(lVar4);
      if (param_2 != '\0') {
        (**(code **)(*(longlong *)(lVar4 + 0x10) + 0x160))(lVar4 + 0x10,auStack_80);
        iVar2 = FUN_140004ae0(auStack_80);
        for (iStack_88 = 0; iStack_88 < iVar2; iStack_88 = iStack_88 + 1) {
          plStack_78 = (longlong *)FUN_140160c10(auStack_80,iStack_88);
          plStack_78 = (longlong *)*plStack_78;
          if (plStack_78 != (longlong *)0x0) {
            auStack_58[0] = (**(code **)(*plStack_78 + 0x28))(plStack_78);
            plStack_60 = (longlong *)(**(code **)(*plStack_78 + 8))(plStack_78);
            (**(code **)(*plStack_60 + 0x38))(plStack_60,auStack_68);
            uStack_50 = FUN_14001d7d0(param_1);
            lStack_70 = FUN_140047dc0(uStack_50,auStack_58);
            if (lStack_70 != 0) {
              FUN_140050d00(lStack_70,auStack_48);
              cVar1 = __9QString__QEBA_NAEBV0__Z(auStack_48,auStack_68);
              if (cVar1 != '\0') {
                plStack_40 = (longlong *)func_0x0001400135a0(param_1);
                puStack_28 = auStack_30;
                uStack_18 = __0QString__QEAA_AEBV0__Z(puStack_28,auStack_48);
                uStack_38 = (**(code **)(*plStack_40 + 8))(plStack_40,uStack_18);
                (**(code **)*plStack_78)(plStack_78,uStack_38);
              }
              __1QString__QEAA_XZ(auStack_48);
            }
            __1QString__QEAA_XZ(auStack_68);
          }
        }
        FUN_1402081a0(auStack_80);
      }
      if (lVar4 == 0) {
        lStack_10 = 0;
      }
      else {
        lStack_10 = lVar4 + 0x10;
      }
      (**(code **)(*plVar5 + 0x1b8))(plVar5,lStack_10);
    }
  }
  return;
}

