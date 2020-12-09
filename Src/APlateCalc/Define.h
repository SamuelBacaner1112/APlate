// This is a part of the Hangil IT Program Source.
// Copyright (C) 1998- (c) Hangil IT Corporation
// All rights reserved.
//

//// Element 결과값 종류
//#define  ELE_SHEAR_2	0       // Shear 2-2
//#define  ELE_MOMENT3	1       // Moment 2-2
//#define  ELE_AXFORCE	2       // Axial Force
//#define  ELE_SHEAR_3	3       // Shear 3-3
//#define  ELE_MOMENT2	4       // Moment 3-3
//#define  ELE_AXTORSN	5       // Axial Torsion
#define  JOINT_REACTION		6   // joint reaction
#define  JOINT_DISPLACEMENT	7   // joint disp

//////////////////////////////////////////////////////////////////////////
// m_nExtraInfo
#define	BEFORE_COMPOSITE_ANALYSIS		0	// 합성전 - 강재거더 + 구속콘크리트
#define BEFORE_TURN_COMPOSITE_ANALYSIS	1	// 합성전 - 180도 터닝 + 슬래브(+가로보)
#define AFTER_COMPOSITE_ANALYSIS		2	// 합성후
#define LIVE_LOAD_ANALYSIS				3	// 활하중
#define SETTLEMENT_ANALYSIS				4	// 지점 침하

#define	RAILFLOOR_GEN					6	// 바닥판 일반구간
#define	RAILFLOOR_POLE					7	// 바닥판 전차선주 설치구간
#define CROSS_ANALYSIS					8	// 횡분배 해석
#define HORIZONTAL_LOAD_ANALYSIS		9	// 풍하중 및 수평하중 해석
#define	INFLUENCE_ANALYSIS				10	// 영향선 해석
//////////////////////////////////////////////////////////////////////////
#define SECTION_STRESS					0	// 단면 응력
//
//const DWORD VIEW_COORDINATE				=	0;
//const DWORD VIEW_COLORINDEX				=	1;
//const DWORD VIEW_JOINT					=	2;
//const DWORD VIEW_JOINT_NUMBER			=	3;
//const DWORD VIEW_JOINT_DISPLACEMENT		=	4;
//const DWORD VIEW_JOINT_REACTION			=	5;
//const DWORD VIEW_ELEMENT				=	6;
//const DWORD VIEW_ELEMENT_NUMBER			=	7;
//const DWORD VIEW_ELEMENT_DISPLACEMENT	=	8;
//const DWORD VIEW_EDGE_CUT				=	9;
//const DWORD VIEW_ELEMENT_FORCE			=  10;
//const DWORD VIEW_ELEMENT_FORCEVALUE		=  11;
//const DWORD VIEW_REACTION				=  12;
//const DWORD VIEW_RENDER_RESTRAINT		=  13;

//const DWORD ENABLE_MOUSEPICKING			=  14;

//////////////////////////////////////////////////////////////////////////
// 횡방향 NodeType
const long LEFT_END_NODE			= 0x0001;
const long LEFT_CENTER_NODE			= 0x0002;
const long SLAB_NODE				= 0x0004;
const long GIRDER_NODE				= 0x0008;
const long RIGHT_CENTER_NODE		= 0x0010;
const long RIGHT_END_NODE			= 0x0020;
const long STRINGER_NODE			= 0x0040;
const long SHOE_NODE				= 0x0080;

// 종방향 3D Frame bMain(BYTE) - Element Type
const long FRAME_GIRDER_ELEMENT		= 0x0001;
const long FRAME_CROSS_ELEMENT		= 0x0002;
const long FRAME_STRINGER_ELEMENT	= 0x0004;
const long FRAME_SHOE_ELEMENT		= 0x0008;
//////////////////////////////////////////////////////////////////////////
// 종방향 3D Frame m_bMain(BYTE)
const long FRAME_GIRDER_NODE		= 0x0001;
const long FRAME_CROSS_NODE			= 0x0002;
const long FRAME_SHOE_CENT_NODE		= 0x0004;
const long FRAME_SHOE_LEFT_NODE		= 0x0008;
const long FRAME_SHOE_RIGHT_NODE	= 0x0010;
//////////////////////////////////////////////////////////////////////////

// m_LongAttrib
const long ELEMENT_TYPE			= 0;
const long ELEMENT_TU_POS_NEG	= 1;
const long JOINT_TYPE			= 0;
const long JOINT_GIR			= 1;

const long POSITIVE			= 0;
const long NEGATIVE			= 1;

enum {POS_GIRDER=0, POS_CROSSBEAM=1, POS_STRINGER=2, POS_OUTCROSSBEAM=3, POS_SLAB=4, POS_EIVAR=5, POS_SHOE=6};	// CFrameSection::m_nFlag
enum {SEC_BU=0, SEC_TU=1, SEC_BL=2, SEC_TL=3, SEC_HW=4, SEC_TW=5};	// CFrameSection::m_dJewon[10]

const long FRAME_SEG_SIZE	 = 4;
