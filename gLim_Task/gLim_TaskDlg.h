
// gLim_TaskDlg.h: 헤더 파일
//

#pragma once

#include "CDlgimage.h"
#include <vector>

#define PADDING		10
#define C_GRAY		0x80
#define C_WITHE		0xFF
#define REPEAT		10
#define DELAY		500

#define PATH_1 _T("C:\\Process")
#define PATH_2 _T("C:\\Process\\ResultImage.bmp")


using namespace std;

struct PointData {
	CPoint point{ 0, 0 }; // 클릭 좌표를 (0, 0)으로 초기화
	int radius{ 0 };      // 반지름을 0으로 초기화
};

// CgLimTaskDlg 대화 상자
class CgLimTaskDlg : public CDialogEx
{
// 생성입니다.
public:
	CgLimTaskDlg(CWnd* pParent = nullptr);	// 표준 생성자입니다.
	
	CDlgimage* m_pDlgImage;
	CDlgimage* m_pDlgImage_Result;
	vector<PointData> m_clickPoints;
	CPoint m_circleCenters;
	int m_nRadius;
	int m_CirWidth;

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
	void PrintClickData();
	bool CalculateCircle(const CPoint& p1, const CPoint& p2, const CPoint& p3, CPoint& center, int& radius);
	void DrawEnclosingCircle();
	void RedrawAll();
	void MoveClickPointsRandomly();
	void PerformRandomMovement();
	void SaveImage();
	void Repeat_SaveImage(int i);


	afx_msg void OnEnChangeRad();
	afx_msg void OnEnChangeCircleWid();
	afx_msg void OnBnClickedBtnRandMov();
	afx_msg void OnBnClickedBtnReset();
	afx_msg void OnBnClickedBtnSet();
	afx_msg void OnBnClickedBtnRandMov2();
	afx_msg void OnBnClickedBtnImgSave();
};


