
void FUN_140170430(longlong *param_1,longlong *param_2)

{
  undefined4 uVar1;
  undefined8 uVar2;
  undefined1 *puVar3;
  longlong lVar4;
  longlong lVar5;
  undefined1 *puVar6;
  longlong *plStack_78;
  undefined1 auStack_70 [8];
  undefined1 auStack_68 [16];
  undefined1 auStack_58 [16];
  undefined1 auStack_48 [16];
  undefined1 auStack_38 [16];
  longlong lStack_28;
  longlong lStack_20;
  
  *(undefined1 *)(param_1[5] + 0x19e) = 1;
  if (*(longlong *)(param_1[5] + 0x18) != 0) {
    uVar2 = _viewport_QAbstractScrollArea__QEBAPEAVQWidget__XZ(param_1);
    puVar3 = (undefined1 *)_geometry_QWidget__QEBAAEBVQRect__XZ(uVar2);
    puVar6 = auStack_38;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar6 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar6 = puVar6 + 1;
    }
    puVar3 = auStack_38;
    puVar6 = auStack_68;
    for (lVar4 = 0x10; lVar4 != 0; lVar4 = lVar4 + -1) {
      *puVar6 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar6 = puVar6 + 1;
    }
    _center_QRect__QEBA_AVQPoint__XZ(auStack_68,auStack_70);
    _mapToScene_QGraphicsView__QEBA_AVQPointF__AEBVQPoint___Z(param_1,auStack_58,auStack_70);
    lVar4 = param_1[5];
    puVar3 = auStack_58;
    puVar6 = auStack_48;
    for (lVar5 = 0x10; lVar5 != 0; lVar5 = lVar5 + -1) {
      *puVar6 = *puVar3;
      puVar3 = puVar3 + 1;
      puVar6 = puVar6 + 1;
    }
    (**(code **)(**(longlong **)(param_1[5] + 0x18) + 0x110))
              (*(undefined8 *)(param_1[5] + 0x18),auStack_48,*(undefined4 *)(lVar4 + 0x54));
    (**(code **)(**(longlong **)(param_1[5] + 0x18) + 8))(*(undefined8 *)(param_1[5] + 0x18),0);
  }
  *(longlong **)(param_1[5] + 0x18) = param_2;
  if (param_2 != (longlong *)0x0) {
    (**(code **)(*param_2 + 8))(param_2,param_1);
  }
  FUN_140cbc920(param_1[5] + 0x40,0);
  (**(code **)(*param_1 + 0x2b8))(param_1,0);
  *(longlong **)(param_1[5] + 0x18) = param_2;
  if (param_2 != (longlong *)0x0) {
    (**(code **)(*param_2 + 8))(param_2,param_1);
  }
  (**(code **)(*param_1 + 0x1c8))(param_1);
  (**(code **)(*param_1 + 0x548))(param_1);
  *(undefined1 *)(param_1[5] + 0x19e) = 0;
  plStack_78 = (longlong *)0x0;
  if (param_2 == (longlong *)0x0) {
    FUN_140cbc920(param_1[5] + 0x40,0);
    *(undefined8 *)(param_1[5] + 0x48) = 0;
  }
  else {
    plStack_78 = (longlong *)(**(code **)(*param_2 + 0x100))(param_2);
    if (plStack_78 != (longlong *)0x0) {
      uVar2 = (**(code **)(*param_1 + 0x1d8))(param_1,plStack_78);
      FUN_140cbc920(param_1[5] + 0x40,uVar2);
      uVar2 = (**(code **)(*param_1 + 0x1f0))(param_1,plStack_78);
      *(undefined8 *)(param_1[5] + 0x48) = uVar2;
      lVar4 = func_0x000140c1aef0(param_1[5] + 0x40);
      if (lVar4 != 0) {
        lStack_28 = func_0x000140c1aef0(param_1[5] + 0x40);
        if (lStack_28 == 0) {
          lStack_20 = 0;
        }
        else {
          lStack_20 = lStack_28 + 0x10;
        }
        (**(code **)(*param_1 + 0x338))(param_1,lStack_20);
      }
      if (*(longlong *)(param_1[5] + 0x48) != 0) {
        (**(code **)(*param_1 + 0x340))(param_1,*(undefined8 *)(param_1[5] + 0x48));
      }
    }
  }
  if (plStack_78 != (longlong *)0x0) {
    uVar1 = (**(code **)(*plStack_78 + 0x28))(plStack_78);
    (**(code **)(**(longlong **)(param_1[5] + 0x18) + 0xf8))
              (*(undefined8 *)(param_1[5] + 0x18),uVar1);
  }
  if (*(longlong *)(param_1[5] + 0x18) != 0) {
    (**(code **)(**(longlong **)(param_1[5] + 0x18) + 0x168))(*(undefined8 *)(param_1[5] + 0x18));
  }
  if (param_2 != (longlong *)0x0) {
    _singleShot_QTimer__SAXHPEAVQObject__PEBD_Z(1000,param_1,&UNK_1421d7e18);
  }
  return;
}

