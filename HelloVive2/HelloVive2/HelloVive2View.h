
// HelloVive2View.h: CHelloVive2View 클래스의 인터페이스
//

#pragma once

//#include "../lib/guicon.h"
#include "../lib/ddStatic.h"
#include "../lib/ddEdit.h"

#include "../lib/openvr/headers/openvr.h"
#include "../lib/openvr/shared/Matrices.h"

class CHelloVive2View : public CView
{
protected: // serialization에서만 만들어집니다.
	CHelloVive2View() noexcept;
	DECLARE_DYNCREATE(CHelloVive2View)

	// 특성입니다.
public:
	CHelloVive2Doc* GetDocument() const;

	ddStatic m_info1;
	ddStatic m_info2;
	ddStatic m_info3;


	ddEdit m_edit1; //HMD
	ddEdit m_edit2;

	void ConvertToString(Matrix4 a, CString& buf);
	// 작업입니다.
public:

	int m_cnt;
	int m_mode;
	int m_unMaxTrackedDeviceCount;
	vr::IVRSystem *m_pHMD;
	int m_iTrackedControllerCount;
	int m_iTrackedControllerCount_Last;
	int m_iValidPoseCount;
	int m_iValidPoseCount_Last;
	std::string m_strPoseClasses;
	char m_rDevClassChar[vr::k_unMaxTrackedDeviceCount];   // for each device, a character representing its class

	//
	int m_n_generic_tracker;
	int m_n_head;


	vr::TrackedDevicePose_t m_rTrackedDevicePose[vr::k_unMaxTrackedDeviceCount];
	Matrix4 m_rmat4DevicePose[vr::k_unMaxTrackedDeviceCount];

	bool UpdateHMDMatrixPose();
	Matrix4 ConvertSteamVRMatrixToMatrix4(const vr::HmdMatrix34_t &matPose);

	Matrix4 m_mat4HMDPose;
	Matrix4 m_mat4eyePosLeft;
	Matrix4 m_mat4eyePosRight;

	Matrix4 m_mat4ProjectionCenter;
	Matrix4 m_mat4ProjectionLeft;
	Matrix4 m_mat4ProjectionRight;


	// 재정의입니다.
public:
	virtual void OnDraw(CDC* pDC);  // 이 뷰를 그리기 위해 재정의되었습니다.
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

	// 구현입니다.
public:
	virtual ~CHelloVive2View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

	// 생성된 메시지 맵 함수
protected:
	DECLARE_MESSAGE_MAP()
public:
	virtual void OnInitialUpdate();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnDestroy();
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};

#ifndef _DEBUG  // HelloVive2View.cpp의 디버그 버전
inline CHelloVive2Doc* CHelloVive2View::GetDocument() const
{
	return reinterpret_cast<CHelloVive2Doc*>(m_pDocument);
}
#endif

