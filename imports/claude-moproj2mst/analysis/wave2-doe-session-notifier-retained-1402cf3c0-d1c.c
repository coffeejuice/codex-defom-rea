// ===== FUN_1402cf3c0 @ 1402cf3c0

void FUN_1402cf3c0(longlong param_1,undefined8 param_2,undefined1 *param_3,undefined1 *param_4,
                  undefined1 *param_5,undefined1 *param_6,undefined1 *param_7,undefined1 *param_8,
                  undefined1 *param_9,undefined1 *param_10,undefined1 *param_11,undefined1 *param_12
                  )

{
  undefined8 uVar1;
  longlong *plVar2;
  longlong lVar3;
  undefined8 uVar4;
  undefined1 *puVar5;
  undefined1 *puVar6;
  int iStack_120;
  int iStack_11c;
  longlong lStack_118;
  undefined8 *puStack_30;
  
  uStack_48 = 0xfffffffffffffffe;
  if (*(char *)(param_1 + 0x141) == '\0') {
    *(undefined1 *)(param_1 + 0x90) = 0;
    uStack_40 = __0QString__QEAA_PEBD_Z(auStack_100,param_2);
    uStack_38 = uStack_40;
    uVar1 = FUN_1402d03f0(param_1 + 0x88,uStack_40);
    FUN_1402ef070(auStack_128,uVar1);
    iStack_11c = FUN_140004ae0(auStack_128);
    for (iStack_120 = 0; iStack_120 < iStack_11c; iStack_120 = iStack_120 + 1) {
      plVar2 = (longlong *)FUN_1402eee10(auStack_128,iStack_120);
      lStack_118 = *plVar2;
      lVar3 = func_0x000140c1aef0(lStack_118);
      if (lVar3 == 0) {
        FUN_1408188f0(auStack_128,iStack_120);
        iStack_11c = iStack_11c + -1;
        iStack_120 = iStack_120 + -1;
        if (iStack_120 < -1) {
          iStack_120 = -1;
        }
        if (iStack_11c < 1) break;
      }
      else {
        puStack_30 = (undefined8 *)func_0x000140c1aef0(lStack_118);
        uStack_108 = (**(code **)*puStack_30)(puStack_30);
        puVar5 = param_12;
        puVar6 = auStack_58;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_11;
        puVar6 = auStack_68;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_10;
        puVar6 = auStack_78;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_9;
        puVar6 = auStack_88;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_8;
        puVar6 = auStack_98;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_7;
        puVar6 = auStack_a8;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_6;
        puVar6 = auStack_b8;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_5;
        puVar6 = auStack_c8;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_4;
        puVar6 = auStack_d8;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        puVar5 = param_3;
        puVar6 = auStack_e8;
        for (lVar3 = 0x10; lVar3 != 0; lVar3 = lVar3 + -1) {
          *puVar6 = *puVar5;
          puVar5 = puVar5 + 1;
          puVar6 = puVar6 + 1;
        }
        uStack_28 = _toAscii_QString__QEBA_AVQByteArray__XZ(lStack_118 + 8,auStack_f8);
        uStack_20 = uStack_28;
        uVar1 = _data_QByteArray__QEAAPEADXZ(uStack_28);
        uVar4 = func_0x000140c1aef0(lStack_118);
        uStack_10c = _invokeMethod_QMetaObject__SA_NPEAVQObject__PEBDW4ConnectionType_Qt__VQGenericArgument__333333333_Z
                               (uVar4,uVar1,1,auStack_e8,auStack_d8,auStack_c8,auStack_b8,auStack_a8
                                ,auStack_98,auStack_88,auStack_78,auStack_68,auStack_58);
        __1QByteArray__QEAA_XZ(auStack_f8);
      }
    }
    FUN_1402081a0(auStack_128);
  }
  return;
}


