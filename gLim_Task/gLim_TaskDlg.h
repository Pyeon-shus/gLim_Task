
// gLim_TaskDlg.h: 헤더 파일
//

#pragma once

#include "CDlgimage.h"
#include <vector>

using namespace std;

// CgLimTaskDlg 대화 상자
class CgLimTaskDlg : public CDialogEx
{
// 생성입니다.
public:
	CgLimTaskDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	
	CDlgimage* m_pDlgImage;
	CDlgimage* m_pDlgImage_Result;
	vector<CPoint> m_clickPoints;

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_GLIM_TASK_DIALOG };
#endif

	protected:
	
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 지원입니다.


// 구현입니다.
protected:
	HICON m_hIcon;

	// 생성된 메시지 맵 함수
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDestroy();

	void callFunc(int n);
	afx_msg void OnEnChangeRad();
	afx_msg void OnEnChangeCircleWid();
	afx_msg void OnBnClickedBtnRandMov();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnSet();
	int m_nRadius;
	int m_CirWidth;
};
