
// CDlgimage.h: 헤더 파일
//

#pragma once
#include "afxdialogex.h"
#include <vector>
#include <direct.h>

using namespace std;


// CDlgimage 대화 상자

class CDlgimage : public CDialogEx
{
	DECLARE_DYNAMIC(CDlgimage)

public:
	CDlgimage(CWnd* pParent = nullptr);   // 표준 생성자입니다.
	virtual ~CDlgimage();

	void DrawCircle(unsigned char* fm, int x, int y, int radius, int color = 0);
	bool isinCircle(int i, int j, int nCenterX, int nCenterY, int radius);
	bool validimgPos(int x, int y);
	void ClearImage();
	void CopyImage();
	void DrawOverlays();

	CImage m_image;
	CWnd* m_pParent;

	bool m_isDragging; // 드래그 여부
	int m_draggedPointIndex; // 드래그 중인 클릭 지점의 인덱스


// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_CDlgimage };
#endif

protected:
	CPoint m_currentPoint;    // 실시간 마우스 이동 좌표
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.
	
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedBtnUpParent();
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	void InitImage();
	afx_msg int OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
};
