// CDlgimage.cpp: 구현 파일
//

#include "pch.h"
#include "gLim_Task.h"
#include "afxdialogex.h"
#include "CDlgimage.h"
#include "gLim_TaskDlg.h"
#include <iostream>

using namespace std;

#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// CDlgimage 대화 상자

IMPLEMENT_DYNAMIC(CDlgimage, CDialogEx)

CDlgimage::CDlgimage(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_CDlgimage, pParent)
{
	m_pParent = pParent;
}

CDlgimage::~CDlgimage()
{
}

void CDlgimage::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CDlgimage, CDialogEx)
	ON_BN_CLICKED(IDC_BTN_UP_PARENT, &CDlgimage::OnBnClickedBtnUpParent)
	ON_WM_PAINT()
	ON_WM_MOUSEACTIVATE()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()


// CDlgimage 메시지 처리기

#include "gLim_TaskDlg.h"
void CDlgimage::OnBnClickedBtnUpParent()
{
	static int n = 100;
	CgLimTaskDlg* pDlg = (CgLimTaskDlg*)m_pParent;
	pDlg->callFunc(n++);
	
}


BOOL CDlgimage::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	MoveWindow(0, 0, 640, 480); // 다이얼로그 크기 조정
	InitImage();                // 이미지 초기화


	return TRUE; // 포커스 설정 완료
	// 예외: OCX 속성 페이지는 FALSE를 반환해야 합니다.
}


//void CDlgimage::OnPaint()
//{
//	CPaintDC dc(this); // device context for painting
//	// TODO: 여기에 메시지 처리기 코드를 추가합니다.
//	// 그리기 메시지에 대해서는 CDialogEx::OnPaint()을(를) 호출하지 마십시오.
//	if (m_image) {
//		m_image.Draw(dc, 0, 0);
//	}
//}

//void CDlgimage::OnPaint()
//{
//	CPaintDC dc(this); // 그리기를 위한 DC
//	if (m_image) {
//		m_image.Draw(dc, 0, 0); // 이미지를 다이얼로그에 그림
//	}
//
//	// 좌표 표시
//	CString clickMsg, moveMsg;
//	clickMsg.Format(_T("저장 좌표: (%d, %d)"), m_clickPoint.x, m_clickPoint.y);
//	moveMsg.Format(_T("현재 좌표: (%d, %d)"), m_currentPoint.x, m_currentPoint.y);
//
//	// 좌표를 다이얼로그 상단에 표시
//	dc.TextOut(10, 10, clickMsg);
//	dc.TextOut(10, 30, moveMsg);
//}

void CDlgimage::OnPaint()
{
	CPaintDC dc(this);
	if (m_image) {
		m_image.Draw(dc, 0, 0);
	}
}





void CDlgimage::InitImage()
{
	int nWidth = 640;  // 이미지 폭
	int nHeight = 480; // 이미지 높이
	int nBpp = 8;      // 비트 깊이 (8비트 회색조)

	m_image.Create(nWidth, -nHeight, nBpp);
	if (nBpp == 8) {
		static RGBQUAD rgb[256];
		for (int i = 0; i < 256; i++) {
			rgb[i].rgbRed = rgb[i].rgbGreen = rgb[i].rgbBlue = i;
		}
		m_image.SetColorTable(0, 256, rgb);
	}

	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	memset(fm, 0xff, nWidth * nHeight);
}


int CDlgimage::OnMouseActivate(CWnd* pDesktopWnd, UINT nHitTest, UINT message)
{
	return CDialogEx::OnMouseActivate(pDesktopWnd, nHitTest, message);
}

void CDlgimage::OnMouseMove(UINT nFlags, CPoint point)
{
	// 마우스 이동 좌표 업데이트
	m_currentPoint = point;

	// 화면 갱신 요청
	//Invalidate();

	CDialogEx::OnMouseMove(nFlags, point);
}

void CDlgimage::OnLButtonDown(UINT nFlags, CPoint point)
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();
	int radius = 10; // 기본 반지름 값 설정
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	// 클릭한 좌표가 이미지 크기를 벗어났는지 확인
	if (point.x < 0 || point.y < 0 || point.x > nWidth || point.y > nHeight) {
		AfxMessageBox(_T("클릭한 좌표가 이미지 영역을 벗어났습니다."));
	}
	else {
		// 부모 다이얼로그에서 반지름 값 가져오기
		CgLimTaskDlg* pParent = static_cast<CgLimTaskDlg*>(m_pParent); // 부모 다이얼로그 캐스팅

		if (pParent != nullptr) {
			if (pParent->m_pDlgImage == this && (pParent->m_nRadius > 0) && (pParent->m_CirWidth > 0)) {
				// 클릭 좌표 저장 (최대 3개)
				if (pParent->m_clickPoints.size() < 100) {
					pParent->m_clickPoints.push_back(point);
					radius = pParent->m_nRadius;

					cout << "X: " << point.x << ", Y: " << point.y << endl;
					cout << "└" << "X: " << nWidth << ", Y: " << nHeight << endl;

					if (radius > 0) {
						DrawCircle(fm, point.x, point.y, radius, 0); // 클릭 지점에 원 그리기
					}
				}
				else {
					AfxMessageBox(_T("3개의 클릭 좌표가 이미 저장되었습니다."));
				}
			}
			else if (pParent->m_pDlgImage != this) {
				AfxMessageBox(_T("해당 다이얼로그는 결과를 나타내는 창입니다."));
			}
			else {
				AfxMessageBox(_T("먼저 반지름 및 정원 폭 값을 입력해주세요.\nex) Value > 0"));
			}
		}
		else {
			AfxMessageBox(_T("부모 다이얼로그가 유효하지 않습니다."));
		}
	}

	CDialogEx::OnLButtonDown(nFlags, point);
}








void CDlgimage::DrawCircle(unsigned char*, int x, int y, int radius, int color)
{
	CgLimTaskDlg* pParent = static_cast<CgLimTaskDlg*>(m_pParent); // 부모 다이얼로그 캐스팅
	int nCenterX = x +radius;
	int nCenterY = y +radius;

	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	for (int j = y; j <= y+radius*2; j++) {
		for (int i = x; i <= x+radius*x; i++) {
			if(isinCircle(i,j, nCenterX, nCenterY, radius))
				fm[(j - radius) * nPitch + (i - radius)] = color; // 검은색 픽셀
		}
	}

	pParent->m_pDlgImage->Invalidate(); // 화면 갱신
}


bool CDlgimage::isinCircle(int i, int j, int nCenterX, int nCenterY, int radius) {
	bool bRet = false;

	double dx = i - nCenterX;
	double dy = j - nCenterY;
	double dDist = dx * dx + dy * dy;

	if (dDist < radius * radius) {
		bRet = true;
	}
	return bRet;
}