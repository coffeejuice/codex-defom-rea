// Source contracts: wave3-editor-setup-explicit-state-source-v2.json and
// wave3-editor-constructor-prefix-contract.json. Literal values are original
// call arguments; unmodeled Qt defaults/propagation are never synthesized.
#include "m2m/engine/editor_setup.hpp"
#include "m2m/engine/view_manager.hpp"
#include "m2m/engine/simcontrol300_lifecycle.hpp"
#include "m2m/engine/simcontrol300_owner.hpp"
#include <algorithm>
#include <bit>
#include <iterator>
#include <stdexcept>
#include <utility>

namespace m2m::engine::simcontrol300 {
struct EditorSetupLifetime {};

void EditorSetupGuard::reset(EditorSetupState* owner) noexcept
{
    identity_ = owner;
    if (owner) lifetime_ = owner->lifetime_;
    else lifetime_.reset();
}

EditorSetupState* EditorSetupGuard::get() const noexcept
{
    return lifetime_.expired() ? nullptr : identity_;
}

namespace {
constexpr SetupControlIdentity identities[] = {
    {0x0, "gridLayout"},
    {0x8, "entryLstBx"},
    {0x10, "m_wgtstkSimCtrl"},
    {0x18, "Main"},
    {0x20, "gridLayout1"},
    {0x28, "mainFrameWidget"},
    {0x30, "gridLayout2"},
    {0x38, "gridLayout3"},
    {0x40, "unitBtnGrp"},
    {0x48, "gridLayout4"},
    {0x50, "m_rbtnEN"},
    {0x58, "m_rbtnSI"},
    {0x60, "GroupBox1"},
    {0x68, "verticalLayout_2"},
    {0x70, "modeDefomChckBx"},
    {0x78, "modeHtTransChckBx"},
    {0x80, "groupBox35"},
    {0x88, "gridLayout_22"},
    {0x90, "modeTransformChckBx"},
    {0xa0, "modeGrainChckBx"},
    {0xa8, "modeHeatChckBx"},
    {0xb8, "heatCmBx"},
    {0xc8, "modeDiffusionChckBx"},
    {0xd0, "m_btngrpSimType"},
    {0xd8, "gridLayout5"},
    {0xe0, "m_rbtnRolling"},
    {0xe8, "m_rbtnLagrang"},
    {0xf0, "m_rbtnMaching"},
    {0xf8, "m_btnStdStExtrusion"},
    {0x100, "m_rbtnRing"},
    {0x108, "m_btnALEExtrusion"},
    {0x110, "GroupBox8"},
    {0x118, "verticalLayout"},
    {0x120, "simTitleLbl"},
    {0x128, "simTitleLnEdt"},
    {0x130, "TextLabel1_2"},
    {0x138, "simNameLnEdt"},
    {0x140, "TextLabel1_3_5_3_5_2"},
    {0x148, "oprNoSpnBx"},
    {0x150, "lblMeshNo"},
    {0x158, "mshNoSpnBx"},
    {0x168, "GeoBtnGrp"},
    {0x170, "gridLayout6"},
    {0x178, "AxiSymRdBtn"},
    {0x180, "TorsionRdBtn"},
    {0x188, "PlStrnRdBtn"},
    {0x190, "PlaneStressRdBtn"},
    {0x198, "Stepping_Control"},
    {0x1a0, "gridLayout7"},
    {0x1a8, "StepIncTabWidget"},
    {0x1b0, "tab3"},
    {0x1b8, "gridLayout8"},
    {0x1c0, "m_gbStepIncControl"},
    {0x1c8, "gridLayout9"},
    {0x1d0, "wgtstkStepIncControl"},
    {0x1d8, "WgtPrimaryDisplacement"},
    {0x1e0, "gridLayout10"},
    {0x1e8, "m_grpbxDSMAX"},
    {0x1f0, "hboxLayout"},
    {0x1f8, "m_cmbbxDSMAX"},
    {0x200, "m_leDSMAX"},
    {0x208, "solStepDieUnitLbl"},
    {0x210, "m_pshbtnDSMAXDefine"},
    {0x218, "WgtPrimaryTime"},
    {0x220, "gridLayout11"},
    {0x228, "m_grpbxDTMAX"},
    {0x230, "gridLayout12"},
    {0x238, "solStepTimeUnitLbl"},
    {0x240, "m_pshbtnDTMAXDefine"},
    {0x248, "m_leDTMAX"},
    {0x250, "m_cmbbxDTMAX"},
    {0x258, "WgtPrimaryTemperature"},
    {0x260, "gridLayout13"},
    {0x268, "m_grpbxTempChange"},
    {0x270, "gridLayout14"},
    {0x278, "gridLayout15"},
    {0x280, "m_lbInitalTimeStep"},
    {0x288, "m_leInitalTimeStep"},
    {0x290, "gridLayout16"},
    {0x298, "solStepTempChangeUnitLbl"},
    {0x2a0, "m_leTempChangeMax"},
    {0x2a8, "TextLabel4_2_3_4_2_3_2"},
    {0x2b0, "gridLayout17"},
    {0x2b8, "TextLabel4_2_2_7_3_2_2_2"},
    {0x2c0, "m_leMinTimeTempChange"},
    {0x2c8, "m_leMaxTimeTempChange"},
    {0x2d0, "TextLabel4_2_2_5_4_2_2_2"},
    {0x2e0, "stepSolDefBtnGrp"},
    {0x2e8, "gridLayout18"},
    {0x2f0, "m_rdbtnDSMAX"},
    {0x2f8, "m_rdbtnDTMAX"},
    {0x300, "m_rdbtnDTempMAX"},
    {0x308, "m_gbDatabaseStepSaving"},
    {0x310, "gridLayout19"},
    {0x318, "m_rbtnDBStepSavingUser"},
    {0x320, "m_rbtnDBStepSavingSystem"},
    {0x330, "m_gbSubStepControl"},
    {0x338, "gridLayout20"},
    {0x340, "maxStrainWpLb"},
    {0x348, "maxStrainWpLnEdt"},
    {0x358, "wgtstkSubStepControl"},
    {0x360, "WStackPage1"},
    {0x368, "gridLayout21"},
    {0x370, "m_ckbxContactTimeSubStep"},
    {0x380, "WStackPage2"},
    {0x388, "gridLayout22"},
    {0x390, "m_gbPolygonLength"},
    {0x398, "gridLayout23"},
    {0x3a8, "m_gbPolygonLength3D_2_2_2_2"},
    {0x3b0, "gridLayout24"},
    {0x3b8, "maxPolyLengthUnitLbl_2_2_2_2"},
    {0x3c0, "maxPolyLengthLnEdt"},
    {0x3c8, "m_gbmaxCntctTimeLnEdt"},
    {0x3d0, "gridLayout25"},
    {0x3d8, "maxCntctTimelb_2"},
    {0x3e8, "m_gbPolygonLength3D_2_2_2_2_2"},
    {0x3f0, "gridLayout26"},
    {0x3f8, "maxCntctTimeLnEdt"},
    {0x400, "tab4"},
    {0x408, "gridLayout27"},
    {0x410, "m_gbTempStepControl"},
    {0x418, "gridLayout28"},
    {0x420, "gridLayout29"},
    {0x428, "maxTimeLb"},
    {0x430, "minTimeLnEdt"},
    {0x438, "maxTimeLnEdt"},
    {0x440, "minTimeLb"},
    {0x448, "maxTempLb"},
    {0x450, "maxTempLnEdt"},
    {0x460, "solStepMaxTempUnitLbl"},
    {0x468, "m_gbDefStepControl2D"},
    {0x470, "gridLayout30"},
    {0x478, "gridLayout31"},
    {0x480, "leMaximumDeformationTimeStep2D"},
    {0x488, "lblMinimumDeformationTimeStep2D"},
    {0x490, "leMinimumDeformationTimeStep2D"},
    {0x498, "lblMaximumDeformationTimeStep2D"},
    {0x4a0, "maxVolWp_2_2_2_2_2_2_2"},
    {0x4b0, "m_gbAdvSubStepControl"},
    {0x4b8, "gridLayout32"},
    {0x4c0, "maxSldErrLb"},
    {0x4c8, "maxSldErrLnEdt"},
    {0x4d8, "m_gbVolumeChange2D"},
    {0x4e0, "gridLayout33"},
    {0x4e8, "TextLabel4_2_2_4_2_2_2_2_2"},
    {0x4f0, "maxVolWp_3_2_2_2_2"},
    {0x4f8, "maxVolElemLnEdt"},
    {0x500, "maxVolWpLnEdt"},
    {0x510, "Stop"},
    {0x518, "gridLayout34"},
    {0x520, "stopTabWidget"},
    {0x528, "stopProcessParaTab"},
    {0x530, "gridLayout35"},
    {0x538, "GroupBox14"},
    {0x540, "gridLayout36"},
    {0x548, "processDurLnEdt"},
    {0x550, "processDurLbl"},
    {0x558, "processDurUnitLbl"},
    {0x560, "primDieDispLb"},
    {0x568, "primDieDispUnitLbl"},
    {0x570, "hboxLayout1"},
    {0x578, "textLabel1"},
    {0x580, "primDieDispXLnEdt"},
    {0x588, "textLabel1_2"},
    {0x590, "primDieDispYLnEdt"},
    {0x598, "m_lbPrimDieDispZ"},
    {0x5a0, "primDieDispZLnEdt"},
    {0x5a8, "GroupBox1_2_2_2"},
    {0x5b0, "gridLayout37"},
    {0x5b8, "minVlctyLb"},
    {0x5c0, "maxStrainElemLbl"},
    {0x5c8, "minVlctyUnitLbl"},
    {0x5d0, "maxLoadLb"},
    {0x5d8, "hboxLayout2"},
    {0x5e0, "textLabel1_3_2"},
    {0x5e8, "maxLoadXLnEdt"},
    {0x5f0, "textLabel1_2_3_2"},
    {0x5f8, "maxLoadYLnEdt"},
    {0x600, "m_lbMaxLoadZ"},
    {0x608, "maxLoadZLnEdt"},
    {0x610, "hboxLayout3"},
    {0x618, "textLabel1_3"},
    {0x620, "minVlctyXLnEdt"},
    {0x628, "textLabel1_2_3"},
    {0x630, "minVlctyYLnEdt"},
    {0x638, "m_lbMinVlctyZ"},
    {0x640, "minVlctyZLnEdt"},
    {0x648, "maxStrainElemLnEdt"},
    {0x650, "maxLoadUnitLbl"},
    {0x658, "maxStrainElemUnitLbl"},
    {0x668, "stopDieDistTab"},
    {0x670, "gridLayout38"},
    {0x678, "vboxLayout"},
    {0x680, "GroupBox9"},
    {0x688, "gridLayout39"},
    {0x690, "ref1BtnGrp"},
    {0x698, "gridLayout40"},
    {0x6a0, "m_rbtnNodeR1"},
    {0x6a8, "ref1NodeSpnBx"},
    {0x6b0, "m_rbtnCoordR1"},
    {0x6b8, "groupBox267"},
    {0x6c0, "hboxLayout4"},
    {0x6c8, "textLabel1_2_2_2_5_3"},
    {0x6d0, "ref1PtXLnEdt"},
    {0x6d8, "textLabel1_2_2_2_5"},
    {0x6e0, "ref1PtYLnEdt"},
    {0x6e8, "m_lbRef1PtZ"},
    {0x6f0, "ref1PtZLnEdt"},
    {0x6f8, "hboxLayout5"},
    {0x700, "m_lblRefObj1"},
    {0x708, "stopObj1CmBx"},
    {0x710, "btnPickRefObj1"},
    {0x718, "GroupBox9_2"},
    {0x720, "gridLayout41"},
    {0x728, "ref2BtnGrp"},
    {0x730, "gridLayout42"},
    {0x738, "m_rbtnNodeR2"},
    {0x740, "ref2NodeSpnBx"},
    {0x748, "m_rbtnCoordR2"},
    {0x750, "groupBox268"},
    {0x758, "hboxLayout6"},
    {0x760, "textLabel1_2_2_2_5_3_2"},
    {0x768, "ref2PtXLnEdt"},
    {0x770, "textLabel1_2_2_2_5_2"},
    {0x778, "ref2PtYLnEdt"},
    {0x780, "m_lbRef2PtZ"},
    {0x788, "ref2PtZLnEdt"},
    {0x790, "hboxLayout7"},
    {0x798, "m_lblRefObj2"},
    {0x7a0, "stopObj2CmBx"},
    {0x7a8, "btnPickRefObj2"},
    {0x7b0, "GroupBox17"},
    {0x7b8, "hboxLayout8"},
    {0x7c0, "stopMthdCmBx"},
    {0x7c8, "GroupBox16"},
    {0x7d0, "gridLayout43"},
    {0x7d8, "stopDistLnEdt"},
    {0x7e0, "stopDistUnitLbl"},
    {0x7e8, "dieDistInitBtn"},
    {0x7f8, "stopPlaneTab"},
    {0x800, "gridLayout44"},
    {0x808, "btnInitStopPlane"},
    {0x820, "m_grpbxReferencePts"},
    {0x828, "gridLayout45"},
    {0x830, "m_lblStopPlaneOrigin_2"},
    {0x838, "m_cmbxStopPlane"},
    {0x840, "hboxLayout9"},
    {0x848, "stopDirXLnEdt"},
    {0x850, "stopDirYLnEdt"},
    {0x858, "stopDirZLnEdt"},
    {0x860, "hboxLayout10"},
    {0x868, "m_lbXdir"},
    {0x870, "m_lbYdir"},
    {0x878, "m_lbZdir"},
    {0x880, "hboxLayout11"},
    {0x888, "stopPtXLnEdt"},
    {0x890, "stopPtYLnEdt"},
    {0x898, "stopPtZLnEdt"},
    {0x8a0, "m_lblStopPlaneObject"},
    {0x8a8, "m_lblStopPlaneOrigin"},
    {0x8b0, "m_lblStopPlaneVector"},
    {0x8b8, "Remesh_Criteria"},
    {0x8c0, "gridLayout46"},
    {0x8c8, "remeshingFrameWidget"},
    {0x8d0, "gridLayout47"},
    {0x8d8, "m_tabRemeshingCriteria"},
    {0x8e0, "tab5"},
    {0x8e8, "gridLayout48"},
    {0x8f0, "GroupBox19"},
    {0x8f8, "gridLayout49"},
    {0x900, "TextLabel2"},
    {0x908, "TextLabel4"},
    {0x910, "labelUnitRemeshTimeInc"},
    {0x918, "leRemeshTimeInc"},
    {0x920, "leRemeshStrokeInc"},
    {0x928, "TextLabel3"},
    {0x938, "spboxRemeshStepInc"},
    {0x940, "labelUnitRemeshStrokeInc"},
    {0x948, "lblRemeshInterferenceDepth2D"},
    {0x950, "leRemeshInterferenceDepth2D"},
    {0x958, "lblUnitRemeshInterferenceDepth2D"},
    {0x960, "btnGroupInterferenceDepth"},
    {0x968, "gridLayout50"},
    {0x970, "leRemeshInterferenceDepth"},
    {0x978, "m_rbtnIDB"},
    {0x988, "m_rbtnIDA"},
    {0x998, "labelUnitRemeshInterferenceDepth"},
    {0x9a0, "groupRemeshObjects"},
    {0x9a8, "hboxLayout12"},
    {0x9b0, "TextLabel1_7"},
    {0x9b8, "cmboxRemeshObjects"},
    {0x9c0, "btnPickRemeshingObject"},
    {0x9d0, "m_pageRemeshingMethod"},
    {0x9d8, "gridLayout51"},
    {0x9e0, "m_grpInternalElements"},
    {0x9e8, "gridLayout52"},
    {0x9f0, "m_btngrpInternalElementSize"},
    {0x9f8, "gridLayout53"},
    {0xa00, "m_rbtnNeighbor"},
    {0xa08, "m_rbtnInward"},
    {0xa10, "m_leInternalElementSizeScalingFactor"},
    {0xa28, "m_chboxSkipGoodInternalElements"},
    {0xa30, "m_btngrpRemeshingMethod"},
    {0xa38, "hboxLayout13"},
    {0xa40, "m_rbtnRemeshGlobal"},
    {0xa48, "m_rbtnRemeshLocal"},
    {0xa58, "Iteration"},
    {0xa60, "gridLayout54"},
    {0xa68, "iterationTabWidget"},
    {0xa70, "iterationGeneralTab"},
    {0xa78, "gridLayout_14"},
    {0xa80, "defSolBtnGrp"},
    {0xa88, "gridLayout_12"},
    {0xa90, "m_rbtnDefSolCG"},
    {0xa98, "layoutDefSolCG"},
    {0xaa0, "lbCG1"},
    {0xaa8, "cmBoxCGmethod"},
    {0xab0, "lbCG2"},
    {0xab8, "lineEdCGfillin"},
    {0xac0, "lbCG3"},
    {0xac8, "lineEdCGpartition"},
    {0xad0, "lbCG4"},
    {0xad8, "cmBoxCGdirectSolver"},
    {0xae0, "m_rbtnDefSolSkyline2D"},
    {0xae8, "m_rbtnDefSolGMRES"},
    {0xaf0, "m_rbtnDefSolSparse"},
    {0xaf8, "m_rbtnDefSolMUMPS"},
    {0xb00, "m_rbtnDefSolExplicit"},
    {0xb08, "gridLayout_9"},
    {0xb10, "lbExplicit1"},
    {0xb18, "lineEdDamping"},
    {0xb20, "lbExplicit2"},
    {0xb28, "lineEdMass"},
    {0xb30, "lbExplicit3"},
    {0xb38, "lineEdCycle"},
    {0xb40, "lbExplicit4"},
    {0xb48, "lineEdCurrentCycle"},
    {0xb50, "iterationMethodBtnGrp"},
    {0xb58, "gridLayout55"},
    {0xb60, "m_rbtnDefItrDirect"},
    {0xb68, "m_rbtnDefItrNR"},
    {0xb70, "m_rbtnDefItrBFGS2D"},
    {0xb88, "iterationAdvTab"},
    {0xb90, "gridLayout56"},
    {0xb98, "tempSolBtnGrp"},
    {0xba0, "gridLayout_8"},
    {0xba8, "m_rbtnTemSolSparse"},
    {0xbb0, "m_rbtnTemSolCG"},
    {0xbb8, "m_rbtnTemSolSkyline"},
    {0xbc0, "m_rbtnTemSolMUMPS"},
    {0xbc8, "m_rbtnTemSolExplicit"},
    {0xbd8, "tab_2"},
    {0xbe0, "gridLayout_11"},
    {0xbe8, "GroupBox10"},
    {0xbf0, "_2"},
    {0xbf8, "forceErrLb"},
    {0xc00, "vlctyErrLb"},
    {0xc08, "vlctyErrLnEdt"},
    {0xc10, "forceErrLnEdt"},
    {0xc28, "bandWidOptGrpBx_2"},
    {0xc30, "_4"},
    {0xc38, "bandWidOptDeformChkBx"},
    {0xc48, "gbMaximumIterationsperTimeStep"},
    {0xc50, "_3"},
    {0xc58, "lbMaximumIterationsperTimeStep"},
    {0xc60, "leMaximumIterationsperTimeStep"},
    {0xc68, "bandWidOptGrpBx"},
    {0xc70, "_5"},
    {0xc78, "bandWidOptTempChkBx"},
    {0xc88, "gbMaximumIterationsperHeatTransferTimeStep"},
    {0xc90, "_6"},
    {0xc98, "lbMaximumIterationsperHeatTransferTimeStep"},
    {0xca0, "leMaximumIterationsperHeatTransferTimeStep"},
    {0xca8, "Process_Conditions"},
    {0xcb0, "gridLayout57"},
    {0xcb8, "processCondTabWidget"},
    {0xcc0, "processCondHtTransTab"},
    {0xcc8, "gridLayout_13"},
    {0xcd0, "m_grpboxEnvTemp"},
    {0xcd8, "gridLayout58"},
    {0xce0, "envTempFunDefPshBtn"},
    {0xce8, "envTempBtnGrp"},
    {0xcf0, "gridLayout59"},
    {0xcf8, "envTempConsRdBtn"},
    {0xd00, "envTempConsLnEdt"},
    {0xd08, "envTempFunRdBtn"},
    {0xd18, "envTempConsUnitLbl"},
    {0xd38, "m_grpboxConvCoe"},
    {0xd40, "gridLayout60"},
    {0xd48, "convCoeBtnGrp"},
    {0xd50, "gridLayout61"},
    {0xd58, "onCoeConsRdBtn"},
    {0xd60, "convCoeConsLnEdt"},
    {0xd68, "convCoeConsUnitLbl"},
    {0xd70, "convCoeFunRdBtn"},
    {0xd80, "convCoeFunDefPshBtn"},
    {0xd98, "m_chbxViewFactorCalculation"},
    {0xda0, "viewFactorDefObjPshBtn"},
    {0xdb8, "m_chbxDiffBnd"},
    {0xdc8, "processCondDiffTab"},
    {0xdd0, "gridLayout62"},
    {0xdd8, "m_grpbxEnvAtom"},
    {0xde0, "gridLayout63"},
    {0xde8, "envAtmFunDefPshBtn"},
    {0xdf8, "envAtomBtnGrp"},
    {0xe00, "gridLayout64"},
    {0xe08, "envAtomConsRdBtn"},
    {0xe10, "hboxLayout14"},
    {0xe18, "envAtmLnEdt"},
    {0xe20, "envAtmUnitLbl"},
    {0xe28, "envAtomFunRdBtn"},
    {0xe40, "m_grpbxReaRateCoe"},
    {0xe48, "gridLayout65"},
    {0xe50, "reaRateCoeBtnGrp"},
    {0xe58, "gridLayout66"},
    {0xe60, "reaRateCoeLnEdt"},
    {0xe68, "reaRateCoeUnitLbl"},
    {0xe70, "reaRateCoeConsBdBtn"},
    {0xe78, "reaRateCoeConsBdBtn_3"},
    {0xe80, "reaRateCoeConsBdBtn_2"},
    {0xe88, "reaRateCoeConsBdBtn_4"},
    {0xe98, "reaRateCoeFunDefPshBtn"},
    {0xeb8, "inductionTab"},
    {0xec0, "gridLayout_23"},
    {0xec8, "mgntcPrmbltyLbl"},
    {0xed0, "mgntcPrmbltyLnEdt"},
    {0xed8, "mgntcPrmbltyUnitLbl"},
    {0xee0, "mgntcPrmtvtyLbl"},
    {0xee8, "mgntcPrmtvtyLnEdt"},
    {0xef0, "mgntcPrmtvtyUnitLbl"},
    {0xef8, "srcEngyRatioLbl"},
    {0xf00, "srcEngyRatioLnEdt"},
    {0xf10, "constantTab"},
    {0xf18, "gridLayout67"},
    {0xf28, "mshHtConvLbl"},
    {0xf30, "bltzmanRadConsLb"},
    {0xf40, "intfPntyConsLnEdt"},
    {0xf50, "timeIntFacLnEdt"},
    {0xf58, "bltzmanRadConsUnitLbl"},
    {0xf60, "bltzmanRadConsLnEdt"},
    {0xf68, "timeIntFacLb"},
    {0xf70, "mechHtConvLnEdt"},
    {0xf80, "intfPntyConsLb"},
    {0xf88, "timeTab"},
    {0xf90, "gridLayout_15"},
    {0xf98, "label_7"},
    {0xfa0, "tableBetweenT"},
    {0xfa8, "btnAddBetweenT"},
    {0xfb0, "btnDelBetweenT"},
    {0xfb8, "label_8"},
    {0xfc0, "tableBelowT"},
    {0xfc8, "btnAddBelowT"},
    {0xfd0, "btnDelBelowT"},
    {0xfd8, "label_9"},
    {0xfe0, "tableAboveT"},
    {0xfe8, "btnAddAboveT"},
    {0xff0, "btnDelAboveT"},
    {0xff8, "Advanced"},
    {0x1000, "gridLayout68"},
    {0x1008, "advTabWidget"},
    {0x1010, "varTab"},
    {0x1018, "gridLayout69"},
    {0x1020, "tmFunDataBtnGrp"},
    {0x1028, "gridLayout70"},
    {0x1030, "glbTmRdBtn"},
    {0x1038, "lcTmRdBtn"},
    {0x1040, "gridLayout71"},
    {0x1048, "TextLabel1_3_5_3_2_4"},
    {0x1060, "curLocTmUnitLbl"},
    {0x1068, "curLocTmLnEdt"},
    {0x1070, "curGlbTmLnEdt"},
    {0x1078, "TextLabel1_3_5_3_5"},
    {0x1088, "curGlbTmUnitLbl"},
    {0x10b0, "hboxLayout15"},
    {0x10b8, "TextLabel1_3_3_2_2_2"},
    {0x10c0, "m_cmbxPrmWorkPiece"},
    {0x10d0, "hboxLayout16"},
    {0x10d8, "m_chkbxTransKineAd"},
    {0x10e8, "errTolTab"},
    {0x10f0, "gridLayout_7"},
    {0x10f8, "GeoErrGroupBox"},
    {0x1100, "gridLayout_6"},
    {0x1108, "tanGeoErrLb"},
    {0x1110, "tanGeoErrLnEdt"},
    {0x1118, "tanGeoErrLnUnitLbl"},
    {0x1120, "normGeoErrLb"},
    {0x1128, "normGeoErrLnEdt"},
    {0x1130, "normGeoErrUnitLbl"},
    {0x1138, "GeoErrLb_3"},
    {0x1140, "GeoErrLnEdt_3"},
    {0x1148, "GeoErrUnitLbl_3"},
    {0x1150, "GeoErrLb_4"},
    {0x1158, "GeoErrLnEdt_4"},
    {0x1160, "GeoErrUnitLbl_4"},
    {0x1168, "m_btngrpContactError"},
    {0x1170, "gridLayout72"},
    {0x1178, "TextLabel1"},
    {0x1180, "ContactErrDistanceRdBtn"},
    {0x1188, "ContactErrDistantLb"},
    {0x1190, "ContactErrDistantLnEdt"},
    {0x1198, "m_lbAdvErrorDistIUnit"},
    {0x11a0, "ContactErrDefaultRdBtn"},
    {0x11b0, "usrDefTab"},
    {0x11b8, "gridLayout73"},
    {0x11c0, "usrDefTable"},
    {0x11c8, "pageNodeOscil"},
    {0x11d0, "gridLayout74"},
    {0x11d8, "GroupBox30_2_2"},
    {0x11e0, "gridLayout75"},
    {0x11e8, "TextLabel1_3_5_3_6_3_2"},
    {0x11f0, "TextLabel1_3_5_3_6_2_2_2"},
    {0x11f8, "SubstepsLockedSpnBx"},
    {0x1200, "NoOscSpnBx"},
    {0x1208, "ButtonGroup48_2"},
    {0x1210, "gridLayout76"},
    {0x1218, "TextLabel1_6"},
    {0x1220, "m_rdbtnTouchAfterCycling"},
    {0x1228, "m_rdbtnSeparateAfterCycling"},
    {0x1230, "TextLabel1_5_2"},
    {0x1238, "NoCyclesSpnBx"},
    {0x1248, "tab1"},
    {0x1250, "gridLayout77"},
    {0x1258, "m_brpboxOutputCntrl"},
    {0x1260, "gridLayout78"},
    {0x1268, "m_chkbxPlastic"},
    {0x1270, "m_chkbxElastic"},
    {0x1278, "m_chkbxCreep"},
    {0x1280, "m_chkbxTransform"},
    {0x1288, "m_chkbxThermVol"},
    {0x1290, "m_chkbxTransVol"},
    {0x1298, "m_chkbxTotal"},
    {0x12b8, "groupBox123"},
    {0x12c0, "gridLayout_5"},
    {0x12c8, "m_btngrpDamage"},
    {0x12d0, "gridLayout_2"},
    {0x12d8, "m_rbtnDamageElement"},
    {0x12e0, "m_rbtnDamageDual"},
    {0x12e8, "m_rbtnDamageInt"},
    {0x12f0, "m_btngrpStrain"},
    {0x12f8, "gridLayout_3"},
    {0x1300, "m_rbtnStrainElement"},
    {0x1308, "m_rbtnStrainDual"},
    {0x1310, "m_rbtnStrainInt"},
    {0x1318, "m_btngrpStress"},
    {0x1320, "gridLayout_4"},
    {0x1328, "m_rbtnStressElement"},
    {0x1330, "m_rbtnStressDual"},
    {0x1338, "m_rbtnStressInt"},
    {0x1348, "tab"},
    {0x1350, "gridLayout_10"},
    {0x1358, "label"},
    {0x1360, "cmBxFreqCNT"},
    {0x1368, "lb_FreqCNT_1"},
    {0x1370, "lineEdit_freqCNT"},
    {0x1378, "lb_FreqCNT_2"},
    {0x1380, "label_2"},
    {0x1388, "cmBxFreqTMP"},
    {0x1390, "lb_FreqTMP_1"},
    {0x1398, "lineEdit_freqTMP"},
    {0x13a0, "lb_FreqTMP_2"},
    {0x13a8, "label_3"},
    {0x13b0, "cmBxFreqDBW"},
    {0x13b8, "lb_FreqDBW_1"},
    {0x13c0, "lineEdit_freqDBW"},
    {0x13c8, "lb_FreqDBW_2"},
    {0x13d8, "label_4"},
    {0x13e0, "cmBxFreqMXW"},
    {0x13e8, "lb_FreqMXW_1"},
    {0x13f0, "lineEdit_freqMXW"},
    {0x13f8, "lb_FreqMXW_2"},
    {0x1400, "label_5"},
    {0x1408, "cmBxFreqVEW"},
    {0x1410, "lb_FreqVEW_1"},
    {0x1418, "lineEdit_freqVEW"},
    {0x1420, "lb_FreqVEW_2"},
    {0x1428, "label_6"},
    {0x1430, "cmBxFreqYLD"},
    {0x1438, "lb_FreqYLD_1"},
    {0x1440, "lineEdit_freqYLD"},
    {0x1448, "lb_FreqYLD_2"},
    {0x1458, "Control_Files"},
    {0x1460, "gridLayout79"},
    {0x1468, "datafileTabWidget"},
    {0x1470, "datafileCat1Tab"},
    {0x1478, "gridLayout80"},
    {0x1480, "m_chkbxSW2SP"},
    {0x14a8, "m_grpbxAngle"},
    {0x14b0, "hboxLayout17"},
    {0x14b8, "lblMaximumIterationsperTimeStep2D_2"},
    {0x14c0, "leDataNBC1"},
    {0x14c8, "lblUnitNBC1"},
    {0x14d8, "lblMaximumIterationsperTimeStep2D_2_2"},
    {0x14e0, "leDataNBC2"},
    {0x14e8, "lblUnitNBC2"},
    {0x14f0, "m_chkbxNBC"},
    {0x14f8, "groupBox72_2"},
    {0x1500, "hboxLayout18"},
    {0x1508, "lblMaximumIterationsperTimeStep2D_2_4"},
    {0x1510, "leSW2SP"},
    {0x1520, "datafileCat2Tab"},
    {0x1528, "gridLayout81"},
    {0x1538, "m_chkbxSTRETCH"},
    {0x1540, "groupBox72_2_2_2"},
    {0x1548, "gridLayout82"},
    {0x1550, "leSTRETCH1"},
    {0x1558, "lblMaximumIterationsperTimeStep2D_2_4_2_2"},
    {0x1560, "leSTRETCH2"},
    {0x1568, "lblMaximumIterationsperTimeStep2D_2_4_2_2_2"},
    {0x1570, "leSTRETCH3"},
    {0x1578, "lblMaximumIterationsperTimeStep2D_2_4_2_2_3"},
    {0x15a0, "groupBox72_2_2"},
    {0x15a8, "hboxLayout19"},
    {0x15b0, "lblMaximumIterationsperTimeStep2D_2_4_2"},
    {0x15b8, "leBWEIGHT"},
    {0x15c0, "m_chkbxBWEIGHT"},
    {0x15c8, "m_chbxStrainWeight"},
    {0x15e0, "tab2"},
    {0x15e8, "gridLayout83"},
    {0x15f0, "m_chkbxSPRING"},
    {0x1600, "groupBox40"},
    {0x1608, "gridLayout84"},
    {0x1610, "m_lbxSpingLoadDies"},
    {0x1618, "pushButton11_2"},
    {0x1620, "pushButton11"},
    {0x1628, "m_chbxSPRINGReversible"},
    {0x1630, "gridLayout85"},
    {0x1638, "m_leSPRINGStiff"},
    {0x1640, "lblMaximumIterationsperTimeStep2D_2_4_2_2_4_2"},
    {0x1648, "m_leSPRINGMaxDisp"},
    {0x1650, "cmbxSPRINGObjDie"},
    {0x1658, "lblMaximumIterationsperTimeStep2D_2_4_2_2_4_2_3_2"},
    {0x1660, "lblMaximumIterationsperTimeStep2D_2_4_2_2_4_2_3_3"},
    {0x1668, "lblMaximumIterationsperTimeStep2D_2_4_2_2_4_2_2"},
    {0x1670, "lblMaximumIterationsperTimeStep2D_2_4_2_2_4_2_3"},
    {0x1678, "lblMaximumIterationsperTimeStep2D_2_4_2_2_4_2_3_3_2"},
    {0x1680, "m_leSPRINGDampRatio2"},
    {0x1688, "cmbxSPRINGRelateObj"},
    {0x1690, "m_leSPRINGDampRatio1"},
    {0x1698, "lblMaximumIterationsperTimeStep2D_2_4_2_2_4"},
    {0x16a0, "m_leSPRINGPreLoad"},
    {0x16a8, "Step_Number"},
    {0x16b0, "gridLayout86"},
    {0x16b8, "SimulationStepBtnGrp_2"},
    {0x16c0, "gridLayout87"},
    {0x16c8, "m_cmbxPDie"},
    {0x16d0, "m_lbcmbxPDie"},
    {0x16e0, "SimulationStepBtnGrp"},
    {0x16e8, "gridLayout_21"},
    {0x16f0, "chkbxNSTART"},
    {0x16f8, "m_lbstartStepNoSpnBx"},
    {0x1700, "stackedWidgetStartStepNo"},
    {0x1708, "startStepNumberPage0"},
    {0x1710, "gridLayout_19"},
    {0x1718, "startStepNoSpnBx"},
    {0x1720, "startStepNumberPage1"},
    {0x1728, "gridLayout_20"},
    {0x1730, "lblAutoStartStepNo"},
    {0x1738, "m_lbstepTotalSpnBx"},
    {0x1740, "stepTotalSpnBx"},
    {0x1748, "m_lbstepIncSpnBx"},
    {0x1750, "stepIncSpnBx"},
    {0x1758, "page"},
    {0x1760, "gridLayout_16"},
    {0x1768, "groupBox"},
    {0x1770, "gridLayout_18"},
    {0x1778, "widget_2"},
    {0x1780, "horizontalLayout"},
    {0x1788, "m_tbPPVarAdd"},
    {0x1790, "m_tbPPVarDelete"},
    {0x17a0, "widget_3"},
    {0x17a8, "gridLayout_17"},
    {0x17b0, "m_lePPVarValue1"},
    {0x17b8, "m_cbPPVarSel"},
    {0x17c0, "m_lePPVarValue2"},
    {0x17d0, "widget_4"},
    {0x17d8, "horizontalLayout_2"},
    {0x17e0, "m_tbPPVarOpen"},
    {0x17e8, "m_tbPPVarSave"},
    {0x17f0, "label_10"},
    {0x17f8, "m_lePPVarFName"},
    {0x1800, "m_tvPPVarList"},
};

constexpr SetupScalarCall prefixCalls[] = {
    {0x80, SetupProperty::Enabled, 0, false, 0x140edbd8f},
    {0xb8, SetupProperty::Enabled, 0, false, 0x140edc676},
    {0x140, SetupProperty::Enabled, 1, false, 0x140ede0b4},
    {0x148, SetupProperty::Minimum, 1, false, 0x140ede2b5},
    {0x148, SetupProperty::Maximum, 9999, false, 0x140ede2cf},
    {0x148, SetupProperty::Value, 1, false, 0x140ede2e9},
    {0x158, SetupProperty::Minimum, 1, false, 0x140ede658},
    {0x158, SetupProperty::Maximum, 9999, false, 0x140ede672},
    {0x178, SetupProperty::Checked, 1, false, 0x140edeaf1},
    {0x238, SetupProperty::Enabled, 1, false, 0x140ee0d90},
    {0x2f0, SetupProperty::Checked, 0, false, 0x140ee30eb},
    {0x4e8, SetupProperty::Enabled, 1, false, 0x140ee8c7a},
    {0x4f0, SetupProperty::Enabled, 1, false, 0x140ee8e42},
    {0x4f8, SetupProperty::Enabled, 1, false, 0x140ee8fd4},
    {0x500, SetupProperty::Enabled, 1, false, 0x140ee914f},
    {0x6a0, SetupProperty::Checked, 1, false, 0x140eee062},
    {0x6a8, SetupProperty::Maximum, 999999, false, 0x140eee1dd},
    {0x6b8, SetupProperty::Enabled, 0, false, 0x140eee4bc},
    {0x710, SetupProperty::Checkable, 1, false, 0x140eef84a},
    {0x710, SetupProperty::Checked, 0, false, 0x140eef861},
    {0x738, SetupProperty::Checked, 1, false, 0x140ef0036},
    {0x740, SetupProperty::Maximum, 999999, false, 0x140ef01b1},
    {0x750, SetupProperty::Enabled, 0, false, 0x140ef0490},
    {0x7a8, SetupProperty::Checkable, 1, false, 0x140ef1652},
    {0x7a8, SetupProperty::Checked, 0, false, 0x140ef1669},
    {0x938, SetupProperty::Maximum, 1000000000, false, 0x140ef64a1},
    {0x988, SetupProperty::Checked, 1, false, 0x140ef7275},
    {0x9c0, SetupProperty::Checkable, 1, false, 0x140ef7e16},
    {0x9e0, SetupProperty::Enabled, 1, false, 0x140ef83ae},
    {0x9f0, SetupProperty::Enabled, 1, false, 0x140ef8613},
    {0xa00, SetupProperty::Enabled, 1, false, 0x140ef8878},
    {0xa00, SetupProperty::Checked, 1, false, 0x140ef888f},
    {0xa08, SetupProperty::Enabled, 1, false, 0x140ef8a07},
    {0xa10, SetupProperty::Enabled, 0, false, 0x140ef8b82},
    {0xa40, SetupProperty::Checked, 0, false, 0x140ef93ca},
    {0xa48, SetupProperty::Checked, 1, false, 0x140ef952d},
    {0xb18, SetupProperty::Enabled, 1, false, 0x140efb93f},
    {0xb28, SetupProperty::Enabled, 1, false, 0x140efbc40},
    {0xb38, SetupProperty::Enabled, 1, false, 0x140efbf44},
    {0xb48, SetupProperty::Enabled, 0, false, 0x140efc248},
    {0xc58, SetupProperty::Enabled, 1, false, 0x140eff056},
    {0xdf8, SetupProperty::Enabled, 1, false, 0x140f037bf},
    {0xfa0, SetupProperty::ColumnCount, 2, true, 0x140f08449},
    {0xfc0, SetupProperty::ColumnCount, 1, true, 0x140f08cb6},
    {0xfe0, SetupProperty::ColumnCount, 1, true, 0x140f09378},
    {0x1198, SetupProperty::Enabled, 1, false, 0x140f0e0e7},
    {0x11a0, SetupProperty::Checked, 1, false, 0x140f0e27c},
    {0x11e8, SetupProperty::Enabled, 1, false, 0x140f0eead},
    {0x1220, SetupProperty::Checked, 1, false, 0x140f0f904},
    {0x1298, SetupProperty::Enabled, 0, false, 0x140f10e0c},
    {0x1298, SetupProperty::Checked, 1, false, 0x140f10e23},
    {0x12d8, SetupProperty::Checked, 1, false, 0x140f1178c},
    {0x1300, SetupProperty::Checked, 1, false, 0x140f11e78},
    {0x1328, SetupProperty::Checked, 1, false, 0x140f12567},
    {0x14a8, SetupProperty::Enabled, 0, false, 0x140f16a68},
    {0x14f8, SetupProperty::Enabled, 0, false, 0x140f1780c},
    {0x1540, SetupProperty::Enabled, 0, false, 0x140f18418},
    {0x15a0, SetupProperty::Enabled, 0, false, 0x140f19432},
    {0x15f0, SetupProperty::Checked, 1, false, 0x140f1a1a8},
    {0x1718, SetupProperty::Minimum, -999999, false, 0x140f1d894},
    {0x1718, SetupProperty::Maximum, 999999, false, 0x140f1d8ae},
    {0x1718, SetupProperty::SingleStep, 1, false, 0x140f1d8c8},
    {0x1740, SetupProperty::Maximum, 999999, false, 0x140f1e145},
    {0x1740, SetupProperty::SingleStep, 10, false, 0x140f1e15f},
    {0x1750, SetupProperty::Minimum, 1, false, 0x140f1e4d3},
    {0x1750, SetupProperty::Maximum, 999999, false, 0x140f1e4ed},
    {0x1750, SetupProperty::SingleStep, 10, false, 0x140f1e507},
    {0x1800, SetupProperty::ColumnCount, 3, true, 0x140f2033d},
    {0x1800, SetupProperty::ColumnCount, 3, false, 0x140f20590},
};

constexpr SetupScalarCall finalIndexCalls[] = {
    {0x10, SetupProperty::CurrentIndex, 0, false, 0x140f21c8d},
    {0x1a8, SetupProperty::CurrentIndex, 0, false, 0x140f21ca4},
    {0x1d0, SetupProperty::CurrentIndex, 1, false, 0x140f21cbe},
    {0x520, SetupProperty::CurrentIndex, 0, false, 0x140f21cd5},
    {0x8d8, SetupProperty::CurrentIndex, 0, false, 0x140f21cec},
    {0xa68, SetupProperty::CurrentIndex, 0, false, 0x140f21d03},
    {0xcb8, SetupProperty::CurrentIndex, 0, false, 0x140f21d1a},
    {0x1008, SetupProperty::CurrentIndex, 0, false, 0x140f21d31},
    {0x1468, SetupProperty::CurrentIndex, 0, false, 0x140f21d48},
    {0x1700, SetupProperty::CurrentIndex, 0, false, 0x140f21d5f},
};

// All routes are toggled(bool) -> setEnabled(bool), AutoConnection0.
constexpr SetupInternalConnection internalConnections[] = {
    {0xa8, 0xb8},
    {0x6b0, 0x6b8},
    {0x78, 0x80},
    {0x14f0, 0x14a8},
    {0x6a0, 0x6a8},
    {0x738, 0x740},
    {0xa48, 0x9e0},
    {0x748, 0x750},
    {0xa08, 0xa10},
    {0x1480, 0x14f8},
    {0x1538, 0x1540},
    {0x15f0, 0x1600},
    {0x15f0, 0x1600},
    {0x15c0, 0x15a0},
};

constexpr SetupAutomaticSlot automaticSlots[] = {
    {0x16f0, "toggled(bool)", "on_chkbxNSTART_toggled(bool)"},
    {0xdb8, "stateChanged(int)", "on_m_chbxDiffBnd_stateChanged(int)"},
    {0x1360, "currentIndexChanged(int)", "on_cmBxFreqCNT_currentIndexChanged(int)"},
    {0x1388, "currentIndexChanged(int)", "on_cmBxFreqTMP_currentIndexChanged(int)"},
    {0x13b0, "currentIndexChanged(int)", "on_cmBxFreqDBW_currentIndexChanged(int)"},
    {0x13e0, "currentIndexChanged(int)", "on_cmBxFreqMXW_currentIndexChanged(int)"},
    {0x1408, "currentIndexChanged(int)", "on_cmBxFreqVEW_currentIndexChanged(int)"},
    {0x1430, "currentIndexChanged(int)", "on_cmBxFreqYLD_currentIndexChanged(int)"},
    {0x1370, "textEdited(QString)", "on_lineEdit_freqCNT_textEdited(QString)"},
    {0x1398, "textEdited(QString)", "on_lineEdit_freqTMP_textEdited(QString)"},
    {0x13c0, "textEdited(QString)", "on_lineEdit_freqDBW_textEdited(QString)"},
    {0x13f0, "textEdited(QString)", "on_lineEdit_freqMXW_textEdited(QString)"},
    {0x1418, "textEdited(QString)", "on_lineEdit_freqVEW_textEdited(QString)"},
    {0x1440, "textEdited(QString)", "on_lineEdit_freqYLD_textEdited(QString)"},
    {0xa90, "toggled(bool)", "on_m_rbtnDefSolCG_toggled(bool)"},
    {0xb00, "toggled(bool)", "on_m_rbtnDefSolExplicit_toggled(bool)"},
    {0xd98, "stateChanged(int)", "on_m_chbxViewFactorCalculation_stateChanged(int)"},
    {0xda0, "clicked()", "on_viewFactorDefObjPshBtn_clicked()"},
    {0xfa8, "clicked()", "on_btnAddBetweenT_clicked()"},
    {0xfb0, "clicked()", "on_btnDelBetweenT_clicked()"},
    {0xfc8, "clicked()", "on_btnAddBelowT_clicked()"},
    {0xfd0, "clicked()", "on_btnDelBelowT_clicked()"},
    {0xfe8, "clicked()", "on_btnAddAboveT_clicked()"},
    {0xff0, "clicked()", "on_btnDelAboveT_clicked()"},
    {0x1800, "cellPressed(int,int)", "on_m_tvPPVarList_cellPressed(int,int)"},
    {0x1800, "cellChanged(int,int)", "on_m_tvPPVarList_cellChanged(int,int)"},
    {0x1788, "clicked()", "on_m_tbPPVarAdd_clicked()"},
    {0x1790, "clicked()", "on_m_tbPPVarDelete_clicked()"},
    {0x17e0, "clicked()", "on_m_tbPPVarOpen_clicked()"},
    {0x17e8, "clicked()", "on_m_tbPPVarSave_clicked()"},
    {0x17b8, "currentIndexChanged(int)", "on_m_cbPPVarSel_currentIndexChanged(int)"},
    {0x17b0, "editingFinished()", "on_m_lePPVarValue1_editingFinished()"},
    {0x17c0, "editingFinished()", "on_m_lePPVarValue2_editingFinished()"},
};

// Original setupUi creates exactly these21 plain QComboBox(QWidget*) controls;
// no custom model or combo setters precede retranslation. Qt653906a0 installs
// QStandardItemModel(0,1); persistent index6710ccc0 is invalid, row6710ce60=-1.
constexpr SetupControlId freshComboControls[] = {
    0xb8, 0x1f8, 0x250, 0x708, 0x7a0, 0x7c0, 0x838, 0x9b8,
    0xaa8, 0xad8, 0x10c0, 0x1360, 0x1388, 0x13b0, 0x13e0,
    0x1408, 0x1430, 0x1650, 0x1688, 0x16c8, 0x17b8
};

// Original setupUi plain QSpinBox controls. QSpinBoxPrivate653fe140 establishes
// min0/max99/value0/step1 and its base clears wrapping; native setup has16
// numeric setter calls on these controls before translation/connections.
constexpr SetupControlId freshSpinControls[] = {
    0x148, 0x158, 0x6a8, 0x740, 0x938, 0x11f8,
    0x1200, 0x1238, 0x1718, 0x1740, 0x1750
};

// All consumed operands are ASCII UTF8; latin1 yields the same UTF16.
// Tables below preserve source statement order, not UI-offset order.
constexpr std::string_view comboText0[] = { "Resistance", "Induction", "Induction (BEM)" };
constexpr std::string_view comboText1[] = { "Constant", "f(time)", "f(stroke)" };
constexpr std::string_view comboText2[] = { "Constant", "f(time)", "f(stroke)" };
constexpr std::string_view comboText3[] = { "Distance", "X Distance", "Y Distance", "Z Distance" };
constexpr std::string_view comboText4[] = { "Old CG", "New CG", "Old and New" };
constexpr std::string_view comboText5[] = { "Spooles", "MUMPS" };
constexpr std::string_view comboText6[] = { "Auto", "Window", "User defined" };
constexpr std::string_view comboText7[] = { "Auto", "User defined" };
constexpr std::string_view comboText8[] = { "Auto", "User defined" };
constexpr std::string_view comboText9[] = { "Auto", "User defined" };
constexpr std::string_view comboText10[] = { "Auto", "User defined" };
constexpr std::string_view comboText11[] = { "Auto", "User defined" };
constexpr std::string_view comboText12[] = { "Time Between Temperatures", "Heating/Cooling Rate Between Temperatures", "Time To Reach Temperature (Heating)", "Time To Reach Temperature (Cooling)", "Time From Temperature To End (Heating)", "Time From Temperature To End (Cooling)", "Strain Above Temperature", "Strain Below Temperature", "Strain Above Strain Rate" };
constexpr std::string_view editText0[] = { "0" };
constexpr std::string_view editText1[] = { "0" };
constexpr std::string_view editText2[] = { "0" };
constexpr std::string_view editText3[] = { "0" };
constexpr std::string_view editText4[] = { "0" };
constexpr std::string_view editText5[] = { "0" };
constexpr std::string_view editText6[] = { "0" };
constexpr std::string_view editText7[] = { "0" };
constexpr std::string_view editText8[] = { "0" };
constexpr std::string_view editText9[] = { "0" };
constexpr std::string_view editText10[] = { "0" };
constexpr std::string_view editText11[] = { "0" };
constexpr std::string_view editText12[] = { "0.0" };
constexpr std::string_view editText13[] = { "1.0" };
constexpr std::string_view editText14[] = { "100" };
constexpr std::string_view editText15[] = { "0" };
struct ConsumedTranslationAction {
    SetupControlId control;
    bool combo;
    std::span<const std::string_view> sourceText;
};
constexpr ConsumedTranslationAction consumedTranslations[] = {
    {0xb8, true, comboText0}, // retranslation line375
    {0x1f8, true, comboText1}, // retranslation line731
    {0x250, true, comboText2}, // retranslation line869
    {0x6d0, false, editText0}, // retranslation line2219
    {0x6e0, false, editText1}, // retranslation line2241
    {0x6f0, false, editText2}, // retranslation line2263
    {0x768, false, editText3}, // retranslation line2378
    {0x778, false, editText4}, // retranslation line2400
    {0x788, false, editText5}, // retranslation line2422
    {0x7c0, true, comboText3}, // retranslation line2480
    {0x848, false, editText6}, // retranslation line2681
    {0x850, false, editText7}, // retranslation line2703
    {0x858, false, editText8}, // retranslation line2725
    {0x888, false, editText9}, // retranslation line2780
    {0x890, false, editText10}, // retranslation line2802
    {0x898, false, editText11}, // retranslation line2824
    {0xaa8, true, comboText4}, // retranslation line3311
    {0xad8, true, comboText5}, // retranslation line3387
    {0xb18, false, editText12}, // retranslation line3492
    {0xb28, false, editText13}, // retranslation line3514
    {0xb38, false, editText14}, // retranslation line3536
    {0xb48, false, editText15}, // retranslation line3558
    {0x1360, true, comboText6}, // retranslation line6012
    {0x1388, true, comboText7}, // retranslation line6143
    {0x13b0, true, comboText8}, // retranslation line6264
    {0x13e0, true, comboText9}, // retranslation line6385
    {0x1408, true, comboText10}, // retranslation line6506
    {0x1430, true, comboText11}, // retranslation line6627
    {0x17b8, true, comboText12}, // retranslation line7583
};
}

std::span<const SetupControlIdentity> editorSetupIdentities() noexcept { return identities; }
std::span<const SetupScalarCall> editorSetupPrefixCalls() noexcept { return prefixCalls; }
std::span<const SetupScalarCall> editorSetupFinalIndexCalls() noexcept { return finalIndexCalls; }
std::span<const SetupInternalConnection> editorSetupInternalConnections() noexcept { return internalConnections; }
std::span<const SetupAutomaticSlot> editorSetupAutomaticSlots() noexcept { return automaticSlots; }

EditorSetupState::EditorSetupState() : lifetime_(std::make_shared<EditorSetupLifetime>())
{
    controls_.reserve(std::size(identities));
    for (const auto identity : identities) {
        SetupControlState state;
        state.identity = identity;
        controls_.push_back(state);
    }
    for (const auto& call : prefixCalls) {
        auto& state = *mutableControl(call.control);
        switch (call.property) {
        case SetupProperty::Enabled: state.requestedEnabled = call.value != 0; break;
        case SetupProperty::Checked: state.requestedChecked = call.value != 0; break;
        case SetupProperty::Checkable: state.checkable = call.value != 0; break;
        case SetupProperty::Minimum: state.minimum = call.value; break;
        case SetupProperty::Maximum: state.maximum = call.value; break;
        case SetupProperty::Value: state.value = call.value; break;
        case SetupProperty::SingleStep: state.singleStep = call.value; break;
        case SetupProperty::ColumnCount:
            if (call.conditionalColumnMinimum) {
                state.columnCountLowerBound = std::max(state.columnCountLowerBound, call.value);
            } else {
                state.columnCount = call.value;
                state.columnCountLowerBound = call.value;
            }
            break;
        case SetupProperty::CurrentIndex: break; // later stage; never in prefixCalls
        }
    }
    // QtGui4_64 QAbstractButtonPrivate65370be0 clears checked bit1. Native
    // QRadioButton653f8a70 sets checkable/autoExclusive, retaining unchecked.
    // These two separate parent groups are ui9f0 and uia30; setup's explicit
    // setChecked selects Neighbor and Local without a conflicting later call.
    mutableControl(0xa00)->checked = true;
    mutableControl(0xa08)->checked = false;
    mutableControl(0xa40)->checked = false;
    mutableControl(0xa48)->checked = true;
}

EditorSetupState::~EditorSetupState() { lifetime_.reset(); }

NativeSetupPrefixReport EditorSetupState::beginNativePrefix(
    const StagedEditor300& factory, const NativeSetupObservation& context)
{
    // This entry is private to the bound factory, which validates the current
    // Project/Operation/GuiModel identities before reaching any native stores.
    if (factoryIdentity_ != &factory || context.receiver != &factory)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::IdentityMismatch};
    if (sealedForPages())
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::HistoricalState};
    if (context.qualification.empty())
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::MissingQualification};
    auto& state = nativePrefix_;
    auto widget = state.widget_.lock();
    if (!widget || !widget->nativeOwnerAlive() || widget->nativeIdentity() != &factory)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::WidgetOwnerUnavailable};
    if (state.report.attempted) {
        state.report.repeatedAttemptRejected = true;
        return state.report;
    }
    state.report.attempted = true;
    state.qualification = context.qualification;
    const auto stop = [&](NativeSetupPrefixStage stage, NativeSetupPrefixIssue issue) {
        state.report.stage = stage;
        state.report.issue = issue;
        return state.report;
    };
    if (context.objectName) {
        if (widget->objectName() && *widget->objectName() != *context.objectName)
            return stop(NativeSetupPrefixStage::ObjectName, NativeSetupPrefixIssue::ObservationMismatch);
        widget->observeObjectName(*context.objectName);
    }
    if (!widget->objectName())
        return stop(NativeSetupPrefixStage::ObjectName, NativeSetupPrefixIssue::UnknownObjectName);
    if (widget->objectName()->isEmpty()) {
        // 140ed98ce uses fromUtf8 for this ASCII literal; neither translation
        // nor the current C-string codec participates in this conversion.
        widget->assignObjectName(qt::L("SfPreSimControl300"));
        state.effects.push_back(NativeSetupPrefixEffect::ObjectNameAssigned);
        // 6712ae51 stores the QString before the declarative callback.
        if (!context.declarativeHookDataPresent)
            return stop(NativeSetupPrefixStage::ObjectNameHook, NativeSetupPrefixIssue::UnknownNameHook);
        if (*context.declarativeHookDataPresent)
            return stop(NativeSetupPrefixStage::ObjectNameHook, NativeSetupPrefixIssue::DeclarativeNameHook);
    }
    widget->assignAttributeBit(42, true); // 6506b5fb, before Created60 test
    state.effects.push_back(NativeSetupPrefixEffect::ResizeAttribute42Set);
    if (context.widgetCreated) {
        if (const auto known = widget->attributeBit(60); known && known != context.widgetCreated)
            return stop(NativeSetupPrefixStage::ResizeCreatedState, NativeSetupPrefixIssue::ObservationMismatch);
        widget->observeAttributeBit(60, *context.widgetCreated);
    }
    const auto created = widget->attributeBit(60);
    if (!created)
        return stop(NativeSetupPrefixStage::ResizeCreatedState, NativeSetupPrefixIssue::UnknownCreatedState);
    if (*created)
        return stop(NativeSetupPrefixStage::ResizeWindow, NativeSetupPrefixIssue::CreatedWidget);
    NativeSetupSizeLimits limits{0, 0, 0xffffff, 0xffffff};
    if (context.widgetExtra == NativeSetupWidgetExtra::Unknown ||
        (context.widgetExtra == NativeSetupWidgetExtra::Present && !context.sizeLimits))
        return stop(NativeSetupPrefixStage::ResizeConstraints, NativeSetupPrefixIssue::UnknownConstraints);
    if (context.widgetExtra == NativeSetupWidgetExtra::Present) limits = *context.sizeLimits;
    // QSize boundedTo(max), THEN expandedTo(min); inconsistent limits still
    // follow that exact order instead of std::clamp's precondition.
    const auto width = std::max(std::min(std::int32_t{477}, limits.maximumWidth), limits.minimumWidth);
    const auto height = std::max(std::min(std::int32_t{390}, limits.maximumHeight), limits.minimumHeight);
    if (context.geometryEndpoints) {
        if (widget->geometryEndpoints() && *widget->geometryEndpoints() != *context.geometryEndpoints)
            return stop(NativeSetupPrefixStage::ResizeGeometry, NativeSetupPrefixIssue::ObservationMismatch);
        widget->observeGeometryEndpoints(*context.geometryEndpoints);
    }
    if (!widget->geometryEndpoints())
        return stop(NativeSetupPrefixStage::ResizeGeometry, NativeSetupPrefixIssue::UnknownGeometry);
    auto rectangle = *widget->geometryEndpoints();
    // Native QRect::setSize uses 32-bit x1+width-1/y1+height-1. Unsigned
    // arithmetic and bit_cast retain that representation without host UB.
    rectangle[2] = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(rectangle[0]) +
        static_cast<std::uint32_t>(width) - std::uint32_t{1});
    rectangle[3] = std::bit_cast<std::int32_t>(static_cast<std::uint32_t>(rectangle[1]) +
        static_cast<std::uint32_t>(height) - std::uint32_t{1});
    widget->assignGeometryEndpoints(rectangle);
    state.effects.push_back(NativeSetupPrefixEffect::GeometrySizeAssigned);
    widget->assignAttributeBit(35, true); // uncreated tail6506b74f; no resize event
    state.effects.push_back(NativeSetupPrefixEffect::ResizeAttribute35Set);
    state.falseVariantConstructed = true;
    state.effects.push_back(NativeSetupPrefixEffect::FalseVariantConstructed);
    if (context.metaObject == NativeSetupMetaObject::Unknown)
        return stop(NativeSetupPrefixStage::PropertyMetaObject, NativeSetupPrefixIssue::UnknownMetaObject);
    if (context.metaObject != NativeSetupMetaObject::NativeStatic)
        return stop(NativeSetupPrefixStage::PropertyMetaObject, NativeSetupPrefixIssue::DynamicMetaObject);
    if (context.propertyTable == NativeSetupPropertyTable::NonemptyNativeTable)
        return stop(NativeSetupPrefixStage::PropertyTable, NativeSetupPrefixIssue::NonemptyPropertyTable);
    const auto& properties = widget->dynamicBooleanProperties();
    if ((properties.names && !properties.names->empty()) || (properties.values && !properties.values->empty()))
        return stop(NativeSetupPrefixStage::PropertyTable, NativeSetupPrefixIssue::NonemptyPropertyTable);
    if (context.propertyTable != NativeSetupPropertyTable::Unknown) {
        const bool allocated = context.propertyTable == NativeSetupPropertyTable::EmptyNativeTable;
        if (properties.extraDataAllocated && *properties.extraDataAllocated != allocated)
            return stop(NativeSetupPrefixStage::PropertyTable, NativeSetupPrefixIssue::ObservationMismatch);
        widget->observeEmptyDynamicBooleanProperties(allocated);
    }
    if (!properties.extraDataAllocated || !properties.names || !properties.values)
        return stop(NativeSetupPrefixStage::PropertyTable, NativeSetupPrefixIssue::UnknownPropertyTable);
    if (!*properties.extraDataAllocated) {
        widget->allocateDynamicBooleanProperties();
        state.effects.push_back(NativeSetupPrefixEffect::DynamicExtraDataAllocated);
    }
    // QObject::setProperty's negative-index branch writes the distinct name
    // and value lists before constructing/sending raw event0xaa. Its eventual
    // false return is ignored by setup; false does not mean storage failed.
    widget->appendDynamicPropertyName("sizeGripEnabled");
    state.effects.push_back(NativeSetupPrefixEffect::PropertyNameAppended);
    widget->appendDynamicPropertyBoolean(false);
    state.effects.push_back(NativeSetupPrefixEffect::PropertyValueAppended);
    state.event_ = WidgetEventState::dynamicPropertyChange("sizeGripEnabled");
    state.effects.push_back(NativeSetupPrefixEffect::PropertyEventConstructed);
    return stop(NativeSetupPrefixStage::PropertyNotification, NativeSetupPrefixIssue::NotificationContext);
}

NativeSetupPrefixReport EditorSetupState::continueNativePropertyNotification(const StagedEditor300& factory)
{
    auto& state = nativePrefix_;
    auto widget = state.widget_.lock();
    if (factoryIdentity_ != &factory || !widget || widget->nativeIdentity() != &factory)
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::IdentityMismatch};
    if (!widget->nativeOwnerAlive())
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::WidgetOwnerUnavailable};
    if (sealedForPages())
        return {NativeSetupPrefixStage::NotStarted, NativeSetupPrefixIssue::HistoricalState};
    if (state.report.stage == NativeSetupPrefixStage::PropertySetReturned) {
        state.report.repeatedNotificationRejected = true;
        return state.report;
    }
    if (!state.event_ || (state.report.stage != NativeSetupPrefixStage::PropertyNotification &&
        state.report.stage != NativeSetupPrefixStage::PropertyEventDestruction))
        return {state.report.stage, NativeSetupPrefixIssue::WrongStage};
    if (state.report.stage == NativeSetupPrefixStage::PropertyNotification) {
        auto runtime = widget->runtimeOwner();
        if (!runtime) {
            state.report.issue = NativeSetupPrefixIssue::RuntimeOwnerUnavailable;
            return state.report;
        }
        const auto& dispatch = state.dispatch_.resume(*runtime, widget, *state.event_);
        if (!dispatch.completed) {
            state.report.issue = NativeSetupPrefixIssue::NotificationContext;
            return state.report;
        }
        state.effects.push_back(NativeSetupPrefixEffect::PropertyNotificationReturned);
        state.report.stage = NativeSetupPrefixStage::PropertyEventDestruction;
    }
    // 6712e37a destroys the synchronous property event before AL=false. Its
    // independent name payload dies here; QObject property storage survives.
    state.event_->destroyStackEvent();
    state.effects.push_back(NativeSetupPrefixEffect::PropertyEventDestroyed);
    state.originalSetterResult = false;
    state.effects.push_back(NativeSetupPrefixEffect::DynamicPropertySetterReturned);
    state.report.stage = NativeSetupPrefixStage::PropertySetReturned;
    state.report.issue = NativeSetupPrefixIssue::None;
    return state.report;
}

bool EditorSetupState::bindFactoryOwner(const StagedEditor300& owner) noexcept
{
    if (factoryIdentity_ || sealedForPages()) return false;
    const auto widget = owner.widgetState();
    if (!widget || !widget->nativeOwnerAlive() || widget->nativeIdentity() != &owner) return false;
    factoryIdentity_ = &owner;
    nativePrefix_.widget_ = widget;
    return true;
}

bool EditorSetupState::destroyCallerFalseVariant(const StagedEditor300& factory)
{
    auto& state = nativePrefix_;
    const auto widget = state.widget_.lock();
    if (factoryIdentity_ != &factory || sealedForPages() || !widget || !widget->nativeOwnerAlive() ||
        widget->nativeIdentity() != &factory || state.report.stage != NativeSetupPrefixStage::PropertySetReturned ||
        !state.falseVariantConstructed || !state.originalSetterResult.has_value()) return false;
    if (!state.falseVariantDestroyed) {
        // 140ed9972 -> QVariant::~QVariant67138370. Exact metadata1 has no
        // shared bit and type<=7, so it returns without clear/free/callback.
        // This is the caller value, distinct from the already destroyed event.
        state.falseVariantDestroyed = true;
        state.effects.push_back(NativeSetupPrefixEffect::CallerFalseVariantDestroyed);
    }
    return true;
}

SetupControlState* EditorSetupState::mutableControl(SetupControlId id) noexcept
{
    const auto it = std::find_if(controls_.begin(), controls_.end(),
        [id](const auto& state) { return state.identity.id == id; });
    return it == controls_.end() ? nullptr : &*it;
}

const SetupControlState* EditorSetupState::control(SetupControlId id) const noexcept
{
    return constructorControl(id);
}

const SetupControlState* EditorSetupState::constructorControl(SetupControlId id) const noexcept
{
    const auto it = std::find_if(controls_.begin(), controls_.end(),
        [id](const auto& state) { return state.identity.id == id; });
    return it == controls_.end() ? nullptr : &*it;
}

SetupAdvanceResult EditorSetupState::bindComboModels(SetupComboModelProfile profile)
{
    if (sealedForPages()) return SetupAdvanceResult::HistoricalState;
    if (progress_ != SetupProgress::Prefix ||
        comboModelProfile_ != SetupComboModelProfile::Unknown)
        return SetupAdvanceResult::WrongStage;
    if (profile == SetupComboModelProfile::Unknown)
        return SetupAdvanceResult::UnknownComboModel;
    if (profile != SetupComboModelProfile::FreshDefaultQtModel)
        return SetupAdvanceResult::UnsupportedComboModel;
    for (const auto id : freshComboControls) {
        auto& state = *mutableControl(id);
        state.comboItems.emplace();
        state.comboCurrentIndex = -1;
    }
    comboModelProfile_ = profile;
    return SetupAdvanceResult::Applied;
}

SetupComboModelProfile EditorSetupState::comboModelProfile() const noexcept
{
    return comboModelProfile_;
}

SetupAdvanceResult EditorSetupState::bindSpinControls(SetupSpinProfile profile)
{
    if (sealedForPages()) return SetupAdvanceResult::HistoricalState;
    if (progress_ != SetupProgress::Prefix || spinProfile_ != SetupSpinProfile::Unknown)
        return SetupAdvanceResult::WrongStage;
    if (profile == SetupSpinProfile::Unknown)
        return SetupAdvanceResult::UnknownSpinControl;
    if (profile != SetupSpinProfile::FreshNative)
        return SetupAdvanceResult::UnsupportedSpinControl;
    for (const auto id : freshSpinControls)
        mutableControl(id)->spin = SetupSpinState{0, 99, 0, 1};
    for (const auto& call : prefixCalls) {
        auto& control = *mutableControl(call.control);
        if (!control.spin) continue;
        auto& spin = *control.spin;
        switch (call.property) {
        case SetupProperty::Minimum:
            //653feb60 -> private range653779f0 -> bound653777e0.
            spin.minimum = call.value;
            spin.maximum = std::max(spin.maximum, spin.minimum);
            spin.value = std::clamp(spin.value, spin.minimum, spin.maximum);
            break;
        case SetupProperty::Maximum:
            //653febf0 adjusts minimum downward before bounding stored value.
            spin.maximum = call.value;
            spin.minimum = std::min(spin.minimum, spin.maximum);
            spin.value = std::clamp(spin.value, spin.minimum, spin.maximum);
            break;
        case SetupProperty::Value:
            spin.value = std::clamp(call.value, spin.minimum, spin.maximum);
            break;
        case SetupProperty::SingleStep:
            if (call.value >= 0) spin.singleStep = call.value; //653feaf0
            break;
        default: break;
        }
    }
    spinProfile_ = profile;
    return SetupAdvanceResult::Applied;
}

SetupSpinProfile EditorSetupState::spinProfile() const noexcept
{
    return spinProfile_;
}

SetupAdvanceResult EditorSetupState::bindStoppingControls(SetupStoppingProfile profile)
{
    if (sealedForPages()) return SetupAdvanceResult::HistoricalState;
    if (progress_ != SetupProgress::Prefix || stoppingProfile_ != SetupStoppingProfile::Unknown)
        return SetupAdvanceResult::WrongStage;
    if (profile == SetupStoppingProfile::Unknown)
        return SetupAdvanceResult::UnknownStoppingControl;
    if (profile != SetupStoppingProfile::FreshNativeTree)
        return SetupAdvanceResult::UnsupportedStoppingControl;
    // Original plain radio pairs, parents690/728. This explicit fresh tree
    // profile excludes inherited disables and external constructor callbacks.
    for (const auto id : {0x6a0, 0x738}) mutableControl(id)->checked = true;
    for (const auto id : {0x6b0, 0x748}) mutableControl(id)->checked = false;
    for (const auto id : {0x6a0, 0x6a8, 0x738, 0x740}) mutableControl(id)->enabled = true;
    mutableControl(0x6b8)->enabled = false; // explicit140eee4bc
    mutableControl(0x750)->enabled = false; // explicit140ef0490
    stoppingProfile_ = profile;
    return SetupAdvanceResult::Applied;
}

SetupStoppingProfile EditorSetupState::stoppingProfile() const noexcept
{
    return stoppingProfile_;
}

SetupAdvanceResult EditorSetupState::translateConsumedControls(SetupTranslationProfile profile)
{
    if (sealedForPages()) return SetupAdvanceResult::HistoricalState;
    if (progress_ != SetupProgress::Prefix) return SetupAdvanceResult::WrongStage;
    if (profile != SetupTranslationProfile::NoActiveTranslator)
        return SetupAdvanceResult::UnknownTranslation;
    if (comboModelProfile_ != SetupComboModelProfile::FreshDefaultQtModel)
        return SetupAdvanceResult::UnknownComboModel;
    progress_ = SetupProgress::TranslatingConsumedControls;
    // On allocation failure the reached state remains owned and this method
    // cannot replay the prefix/earlier actions. Native allocator failure details
    // are not claimed by the consumed-control projection.
    for (const auto& action : consumedTranslations) {
        auto& state = *mutableControl(action.control);
        if (action.combo) {
            state.comboItems.emplace(); // original clear precedes translations
            state.comboCurrentIndex = -1;
            std::vector<qt::String> evaluated;
            evaluated.reserve(action.sourceText.size());
            // Original QString arguments evaluate in reverse insertion order.
            for (auto it = action.sourceText.rbegin(); it != action.sourceText.rend(); ++it)
                evaluated.push_back(qt::String::latin1(*it));
            std::reverse(evaluated.begin(), evaluated.end());
            *state.comboItems = std::move(evaluated);
            // Bound fresh root/default model: rowsInserted6538e2c0 sees
            // start0, end+1==count and invalid current index; at6538e39f it
            // selects0 through6538c150. Editor connections do not yet exist.
            if (!state.comboItems->empty()) state.comboCurrentIndex = 0;
        } else {
            state.text = qt::String::latin1(action.sourceText.front());
        }
        ++translatedActions_;
    }
    progress_ = SetupProgress::ConsumedControlsTranslated;
    translationProfile_ = profile;
    return SetupAdvanceResult::Applied;
}

SetupProgress EditorSetupState::progress() const noexcept { return progress_; }
std::size_t EditorSetupState::consumedTranslationActionsCompleted() const noexcept
{
    return translatedActions_;
}

bool EditorSetupState::installInternalConnectionProjection()
{
    if (sealedForPages()) return false;
    if (progress_ != SetupProgress::ConsumedControlsTranslated) return false;
    internalConnectionsInstalled_ = true;
    progress_ = SetupProgress::InternalConnections;
    // QObject::connect registers future delivery; it does not synchronize the
    // receiver with the sender's current checked state.
    return true;
}

bool EditorSetupState::internalConnectionProjectionInstalled() const noexcept
{
    return internalConnectionsInstalled_;
}

SetupAdvanceResult EditorSetupState::recordFinalIndexProjection()
{
    if (sealedForPages()) return SetupAdvanceResult::HistoricalState;
    if (progress_ != SetupProgress::InternalConnections) return SetupAdvanceResult::WrongStage;
    for (const auto& call : finalIndexCalls)
        mutableControl(call.control)->requestedCurrentIndex = call.value;
    progress_ = SetupProgress::FinalIndicesRecorded;
    return SetupAdvanceResult::Applied;
}

SetupAdvanceResult EditorSetupState::recordAutomaticSlotProjection()
{
    if (sealedForPages()) return SetupAdvanceResult::HistoricalState;
    if (progress_ != SetupProgress::FinalIndicesRecorded) return SetupAdvanceResult::WrongStage;
    // Retain the reached plan, not a fabricated count of native connections.
    progress_ = SetupProgress::AutomaticSlotsRecorded;
    return SetupAdvanceResult::Applied;
}

std::vector<SetupEnableDelivery> EditorSetupState::receiveInternalToggled(
    SetupControlId sender, bool value)
{
    if (sealedForPages())
        throw std::logic_error("EditorSetupState is historical; deliver live toggles to its page owner");
    std::vector<SetupEnableDelivery> deliveries;
    if (!internalConnectionsInstalled_) return deliveries;
    for (const auto& connection : internalConnections) {
        if (connection.sender != sender) continue;
        auto& receiver = *mutableControl(connection.receiver);
        receiver.requestedEnabled = value;
        if (receiver.enabled) receiver.enabled = value;
        deliveries.push_back({sender, connection.receiver, value});
    }
    return deliveries;
}

SetupPageHandoffReport EditorSetupState::handoffToPages(const StagedEditor300& factory,
    InitialEditorSession& destination, StoppingLabelProfile labelProfile)
{
    const auto refuse = [](SetupPageHandoffIssue issue, SetupControlId id = 0) {
        return SetupPageHandoffReport{issue, id};
    };
    if (sealedForPages()) return refuse(SetupPageHandoffIssue::AlreadySealed);
    if (factoryIdentity_ != &factory) return refuse(SetupPageHandoffIssue::WrongFactory);
    if (factory.partialPageState() != &destination)
        return refuse(SetupPageHandoffIssue::WrongDestination);
    if (factory.construction().state != Editor300ConstructorState::PageConstruction)
        return refuse(SetupPageHandoffIssue::WrongStage);
    if (destination.constructionStarted || destination.pageStateComplete ||
        destination.sourceModel || destination.sourceControl || destination.sourceRemesh)
        return refuse(SetupPageHandoffIssue::DestinationStarted);
    if (comboModelProfile_ != SetupComboModelProfile::FreshDefaultQtModel)
        return refuse(SetupPageHandoffIssue::UnknownComboModel);
    if (spinProfile_ != SetupSpinProfile::FreshNative)
        return refuse(SetupPageHandoffIssue::UnknownSpinControl);
    if (stoppingProfile_ != SetupStoppingProfile::FreshNativeTree)
        return refuse(SetupPageHandoffIssue::UnknownStoppingControl);
    if (translationProfile_ != SetupTranslationProfile::NoActiveTranslator)
        return refuse(SetupPageHandoffIssue::UnknownTranslation);
    if (progress_ != SetupProgress::AutomaticSlotsRecorded)
        return refuse(SetupPageHandoffIssue::WrongStage);
    if (labelProfile != StoppingLabelProfile::AsciiNoActiveTranslator)
        return refuse(SetupPageHandoffIssue::UnknownLabelProfile);

    // Prepare all values before changing the destination or sealing history.
    StoppingWidgetState widgets;
    widgets.labelProfile = labelProfile;
    constexpr SetupControlId radio[2][2]{{0x6a0, 0x6b0}, {0x738, 0x748}};
    constexpr SetupControlId spin[2]{0x6a8, 0x740}, group[2]{0x6b8, 0x750};
    constexpr SetupControlId selector[2]{0x708, 0x7a0};
    constexpr SetupControlId text[2][3]{{0x6d0, 0x6e0, 0x6f0}, {0x768, 0x778, 0x788}};
    for (int side = 0; side < 2; ++side) {
        auto& reference = widgets.references[side];
        const auto* node = control(radio[side][0]);
        const auto* coord = control(radio[side][1]);
        const auto* value = control(spin[side]);
        const auto* coords = control(group[side]);
        const auto* choice = control(selector[side]);
        if (!node || !node->checked || !node->enabled)
            return refuse(SetupPageHandoffIssue::UnresolvedControl, radio[side][0]);
        if (!coord || !coord->checked)
            return refuse(SetupPageHandoffIssue::UnresolvedControl, radio[side][1]);
        if (!value || !value->spin || !value->enabled || value->spin->minimum != 0 ||
            value->spin->maximum != 999999 || value->spin->value < 0 || value->spin->value > 999999)
            return refuse(SetupPageHandoffIssue::UnresolvedControl, spin[side]);
        if (!coords || !coords->enabled)
            return refuse(SetupPageHandoffIssue::UnresolvedControl, group[side]);
        if (!choice || !choice->comboItems || !choice->comboCurrentIndex ||
            !choice->comboItems->empty() || *choice->comboCurrentIndex != -1)
            return refuse(SetupPageHandoffIssue::UnresolvedControl, selector[side]);
        reference.selectedObject = 0; // empty native selector, before first fill
        reference.nodeChecked = *node->checked;
        reference.coordinateChecked = *coord->checked;
        reference.nodeEnabled = *node->enabled;
        reference.nodeSpinEnabled = *value->enabled;
        reference.coordinatesEnabled = *coords->enabled;
        reference.node = value->spin->value;
        for (int axis = 0; axis < 3; ++axis) {
            const auto* coordinate = control(text[side][axis]);
            if (!coordinate || !coordinate->text)
                return refuse(SetupPageHandoffIssue::UnresolvedControl, text[side][axis]);
            reference.coordinates[axis] = *coordinate->text;
        }
    }
    const auto* mode = control(0x7c0);
    if (!mode || !mode->comboItems || !mode->comboCurrentIndex ||
        *mode->comboCurrentIndex < 0 || *mode->comboCurrentIndex > 3 ||
        mode->comboItems->size() != 4)
        return refuse(SetupPageHandoffIssue::UnresolvedControl, 0x7c0);
    std::array<qt::String, 3> rotationVector, rotationCenter;
    constexpr SetupControlId rotation[2][3]{{0x848, 0x850, 0x858}, {0x888, 0x890, 0x898}};
    for (int axis = 0; axis < 3; ++axis) {
        for (int side = 0; side < 2; ++side) {
            const auto* coordinate = control(rotation[side][axis]);
            if (!coordinate || !coordinate->text)
                return refuse(SetupPageHandoffIssue::UnresolvedControl, rotation[side][axis]);
            (side == 0 ? rotationVector : rotationCenter)[axis] = *coordinate->text;
        }
    }
    auto& stopping = destination.pages.stopping;
    stopping.widgets = std::move(widgets);
    stopping.rotationVector = std::move(rotationVector);
    stopping.rotationCenter = std::move(rotationCenter);
    stopping.mdsModeCombo = *mode->comboCurrentIndex;
    stopping.referenceChoices.clear();
    stopping.referenceFillOrder = {-1, -1};
    pageOwnerIdentity_ = &destination;
    return {};
}

} // namespace m2m::engine::simcontrol300
