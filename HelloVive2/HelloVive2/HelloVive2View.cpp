
// HelloVive2View.cpp: CHelloVive2View 클래스의 구현
//

#include "stdafx.h"
// SHARED_HANDLERS는 미리 보기, 축소판 그림 및 검색 필터 처리기를 구현하는 ATL 프로젝트에서 정의할 수 있으며
// 해당 프로젝트와 문서 코드를 공유하도록 해 줍니다.
#ifndef SHARED_HANDLERS
#include "HelloVive2.h"
#endif

#include "HelloVive2Doc.h"
#include "HelloVive2View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif



// CHelloVive2View

IMPLEMENT_DYNCREATE(CHelloVive2View, CView)

BEGIN_MESSAGE_MAP(CHelloVive2View, CView)
	// 표준 인쇄 명령입니다.
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CView::OnFilePrintPreview)
	ON_WM_TIMER()
	ON_WM_DESTROY()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CHelloVive2View 생성/소멸

CHelloVive2View::CHelloVive2View() noexcept

{
	// TODO: 여기에 생성 코드를 추가합니다.
	m_cnt = 0;
	m_pHMD = NULL;
	m_mode = 0;
	m_unMaxTrackedDeviceCount = 0;



	m_iTrackedControllerCount = 0;
	m_iTrackedControllerCount_Last = -1;

	m_iValidPoseCount = 0;
	m_iValidPoseCount_Last = -1;

	m_strPoseClasses = "";
	// other initialization tasks are done in BInit
	memset(m_rDevClassChar, 0, sizeof(m_rDevClassChar));

}

CHelloVive2View::~CHelloVive2View()
{
}

BOOL CHelloVive2View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: CREATESTRUCT cs를 수정하여 여기에서
	//  Window 클래스 또는 스타일을 수정합니다.

	return CView::PreCreateWindow(cs);
}

// CHelloVive2View 그리기

void CHelloVive2View::OnDraw(CDC* /*pDC*/)
{
	CHelloVive2Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 여기에 원시 데이터에 대한 그리기 코드를 추가합니다.
}


// CHelloVive2View 인쇄

BOOL CHelloVive2View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 기본적인 준비
	return DoPreparePrinting(pInfo);
}

void CHelloVive2View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄하기 전에 추가 초기화 작업을 추가합니다.
}

void CHelloVive2View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 인쇄 후 정리 작업을 추가합니다.
}


// CHelloVive2View 진단

#ifdef _DEBUG
void CHelloVive2View::AssertValid() const
{
	CView::AssertValid();
}

void CHelloVive2View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CHelloVive2Doc* CHelloVive2View::GetDocument() const // 디버그되지 않은 버전은 인라인으로 지정됩니다.
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CHelloVive2Doc)));
	return (CHelloVive2Doc*)m_pDocument;
}
#endif //_DEBUG


// CHelloVive2View 메시지 처리기

#include <string>

#include <iostream>
void CHelloVive2View::OnInitialUpdate()
{
	CView::OnInitialUpdate();
	
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	SetTimer(0,10,NULL);
	//RedirectIOToConsole();
	CStringA str1 = "aa";

	m_info1.Create(str1, WS_CHILD|WS_VISIBLE, CRect(0, 0, 200, 30),this,0);
	m_info2.Create(str1, WS_CHILD | WS_VISIBLE, CRect(0, 0, 200, 30)+CPoint(0,50), this, 0);
	
	// Loading the SteamVR Runtime
	vr::EVRInitError eError = vr::VRInitError_None;
	m_pHMD = vr::VR_Init( &eError, vr::VRApplication_Scene );
	
	this->SetFocus();

}
bool CHelloVive2View::UpdateHMDMatrixPose()
{

	if (!m_pHMD)
		return false;

	vr::VRCompositor()->WaitGetPoses(m_rTrackedDevicePose, vr::k_unMaxTrackedDeviceCount, NULL, 0);
	m_iValidPoseCount = 0;
	m_strPoseClasses = "";
	m_unMaxTrackedDeviceCount = vr::k_unMaxTrackedDeviceCount;

	for (int nDevice = 0; nDevice < vr::k_unMaxTrackedDeviceCount; ++nDevice)
	{
		if (m_rTrackedDevicePose[nDevice].bPoseIsValid)
		{
			m_iValidPoseCount++;
			m_rmat4DevicePose[nDevice] = ConvertSteamVRMatrixToMatrix4(m_rTrackedDevicePose[nDevice].mDeviceToAbsoluteTracking);
			if (m_rDevClassChar[nDevice] == 0)
			{
				switch (m_pHMD->GetTrackedDeviceClass(nDevice))
				{
				case vr::TrackedDeviceClass_Controller:        m_rDevClassChar[nDevice] = 'C'; break;
				case vr::TrackedDeviceClass_HMD:               m_rDevClassChar[nDevice] = 'H'; break;
				case vr::TrackedDeviceClass_Invalid:           m_rDevClassChar[nDevice] = 'I'; break;
				case vr::TrackedDeviceClass_GenericTracker:    m_rDevClassChar[nDevice] = 'G'; break;
				case vr::TrackedDeviceClass_TrackingReference: m_rDevClassChar[nDevice] = 'T'; break;
				default:                                       m_rDevClassChar[nDevice] = '?'; break;
				}
			}
			m_strPoseClasses += m_rDevClassChar[nDevice];
		}
	}

	if (m_rTrackedDevicePose[vr::k_unTrackedDeviceIndex_Hmd].bPoseIsValid)
	{
		m_mat4HMDPose = m_rmat4DevicePose[vr::k_unTrackedDeviceIndex_Hmd];
		m_mat4HMDPose.invert();
	}


	return true;
}
void CHelloVive2View::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	if (nIDEvent == 0)
	{
		bool b1 = UpdateHMDMatrixPose();

		m_info1.dbg("%d\t%d\t%d\t%d", m_cnt, m_mode, b1, m_iValidPoseCount);//m_unMaxTrackedDeviceCount
		m_info2.dbg(m_strPoseClasses.c_str());
		m_cnt++;
	}
	CView::OnTimer(nIDEvent);
}


void CHelloVive2View::OnDestroy()
{
	CView::OnDestroy();
	KillTimer(0);
	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
}


void CHelloVive2View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 여기에 메시지 처리기 코드를 추가 및/또는 기본값을 호출합니다.
	
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}


BOOL CHelloVive2View::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 여기에 특수화된 코드를 추가 및/또는 기본 클래스를 호출합니다.
	if (pMsg->message == WM_KEYDOWN)
	{
		if (pMsg->wParam == VK_RETURN)
		{
			m_mode = 10;
		}
		else if (pMsg->wParam >= '0' && pMsg->wParam <= '9') 
		{
			m_mode = pMsg->wParam - '0';
		}
		//pMsg->wParam = VK_TAB;

		return true;
	}
	return CView::PreTranslateMessage(pMsg);
}

//-----------------------------------------------------------------------------
// Purpose: Converts a SteamVR matrix to our local matrix class
//-----------------------------------------------------------------------------
Matrix4 CHelloVive2View::ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose)
{
	Matrix4 matrixObj(
		matPose.m[0][0], matPose.m[1][0], matPose.m[2][0], 0.0,
		matPose.m[0][1], matPose.m[1][1], matPose.m[2][1], 0.0,
		matPose.m[0][2], matPose.m[1][2], matPose.m[2][2], 0.0,
		matPose.m[0][3], matPose.m[1][3], matPose.m[2][3], 1.0f
	);
	return matrixObj;
}
