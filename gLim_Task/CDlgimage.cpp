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
	: CDialogEx(IDD_CDlgimage, pParent), 
	m_isDragging(false),
	m_draggedPointIndex(-1) // 초기화
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
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CDlgimage 메시지 처리기
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


void CDlgimage::OnPaint()
{
	CPaintDC dc(this);
	if (m_image) {
		m_image.Draw(dc, 0, 0);
	}
	DrawOverlays();
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
	if (m_isDragging) {
		CgLimTaskDlg* pParent = static_cast<CgLimTaskDlg*>(m_pParent);
		if (pParent != nullptr && validimgPos(point.x, point.y)) {
			// 드래그 중인 클릭 지점 좌표 업데이트
			pParent->m_clickPoints[m_draggedPointIndex].point = point;

			// 정원 및 원 다시 그리기
			pParent->RedrawAll();
		}
	}

	CDialogEx::OnMouseMove(nFlags, point);
}


void CDlgimage::OnLButtonDown(UINT nFlags, CPoint point)
{
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	// 좌표가 이미지 영역에 유효한지 확인
	if (!validimgPos(point.x, point.y)) {
		AfxMessageBox(_T("클릭한 좌표가 이미지 영역을 벗어났습니다."));
		return; // 유효하지 않으면 함수 종료
	}

	// 부모 다이얼로그에서 반지름 값 가져오기
	CgLimTaskDlg* pParent = static_cast<CgLimTaskDlg*>(m_pParent); // 부모 다이얼로그 캐스팅

	if (pParent != nullptr) {
		if (pParent->m_pDlgImage == this && (pParent->m_nRadius > 0) && (pParent->m_CirWidth > 0)) {
			if (pParent->m_clickPoints.size() < 3) { // 클릭 지점이 3개 미만인 경우
				// PointData 생성 및 추가
				PointData data;
				data.point = point;
				data.radius = pParent->m_nRadius; // 현재 반지름 값 저장
				pParent->m_clickPoints.push_back(data);

				if (data.radius > 0) {
					DrawCircle(fm, point.x, point.y, data.radius, 0); // 클릭 지점에 원 그리기
				}
				pParent->m_pDlgImage->Invalidate(); // 화면 갱신
			}
			else {
				// 클릭된 좌표가 기존 점 근처인지 확인하여 드래그 시작
				for (size_t i = 0; i < pParent->m_clickPoints.size(); ++i) {
					const auto& clickPoint = pParent->m_clickPoints[i];
					int recognitionRadius = max(7, clickPoint.radius); // 반지름 기반 인식 범위 설정

					// 클릭 지점이 원의 인식 범위 내에 있는지 확인
					int dx = point.x - clickPoint.point.x;
					int dy = point.y - clickPoint.point.y;
					if (dx * dx + dy * dy <= recognitionRadius * recognitionRadius) {
						m_isDragging = true; // 드래그 시작
						m_draggedPointIndex = static_cast<int>(i); // 드래그 중인 점의 인덱스 저장
						return;
					}
				}
			}

			// 클릭 지점이 3개인 경우 정원 및 원 다시 그리기
			if (pParent->m_clickPoints.size() == 3) {
				pParent->RedrawAll();
				pParent->m_pDlgImage->Invalidate();
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

	CDialogEx::OnLButtonDown(nFlags, point);
}


void CDlgimage::OnLButtonUp(UINT nFlags, CPoint point)
{
	if (m_isDragging) {
		m_isDragging = false;    // 드래그 종료
		m_draggedPointIndex = -1; // 인덱스 초기화

		CgLimTaskDlg* pParent = static_cast<CgLimTaskDlg*>(m_pParent);
		if (pParent != nullptr) {
			// 최종 상태에서 정원 다시 그리기
			pParent->RedrawAll();
		}
	}

	CDialogEx::OnLButtonUp(nFlags, point);
}





void CDlgimage::DrawCircle(unsigned char*, int x, int y, int radius, int color)
{
	if (m_image.IsNull()) {
		AfxMessageBox(_T("이미지가 초기화되지 않았습니다."));
		return;
	}
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();
	int nPitch = m_image.GetPitch();
	unsigned char* fm = (unsigned char*)m_image.GetBits();

	int nCenterX = x; // 클릭한 좌표를 중심으로 설정
	int nCenterY = y;

	// 원 그리기
	for (int j = nCenterY - radius; j <= nCenterY + radius; j++) {
		for (int i = nCenterX - radius; i <= nCenterX + radius; i++) {
			// 원 내부의 픽셀인지 확인
			if (isinCircle(i, j, nCenterX, nCenterY, radius)) {
				// 유효한 이미지 좌표인지 확인
				if (validimgPos(i, j)) {
					// 중심을 기준으로 메모리 위치 계산
					fm[j * nPitch + i] = color;
				}
			}
		}
	}
	CopyImage();
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



bool CDlgimage::validimgPos(int x, int y)
{
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight();

	return (x >= 0 && x < nWidth && y >= 0 && y < nHeight);
}


void CDlgimage::ClearImage()
{
	if (m_image==NULL) {
		return; // 이미지가 없는 경우 바로 리턴
	}

	// 이미지 크기 가져오기
	int nWidth = m_image.GetWidth();
	int nHeight = m_image.GetHeight(); 
	int nPitch = m_image.GetPitch();

	// 이미지 메모리 초기화 (하얀색으로 설정) 
	unsigned char* pBits = (unsigned char*)m_image.GetBits();
	memset(pBits, 0xFF, nHeight * nPitch);

	Invalidate(); // 화면 갱신
}

void CDlgimage::CopyImage()
{
	CgLimTaskDlg* pParent = static_cast<CgLimTaskDlg*>(m_pParent);
	if (!pParent || !pParent->m_pDlgImage_Result) {
		AfxMessageBox(_T("부모 다이얼로그 또는 결과 다이얼로그가 유효하지 않습니다."));
		return;
	}

	// m_image를 m_pDlgImage_Result로 메모리 복사
	HDC srcDC = m_image.GetDC();
	HDC destDC = pParent->m_pDlgImage_Result->m_image.GetDC();

	int width = m_image.GetWidth();
	int height = m_image.GetHeight();

	BitBlt(destDC, 0, 0, width, height, srcDC, 0, 0, SRCCOPY);

	m_image.ReleaseDC();
	pParent->m_pDlgImage_Result->m_image.ReleaseDC();

	// 데이터 기반 추가 그리기
	DrawOverlays();

	// 결과 창 갱신
	pParent->m_pDlgImage_Result->Invalidate();
}

void CDlgimage::DrawOverlays()
{
	CgLimTaskDlg* pParent = static_cast<CgLimTaskDlg*>(m_pParent);
	if (!pParent) return;

	CClientDC dc(pParent->m_pDlgImage_Result);

	// 점선 스타일 설정
	CPen dottedPen(PS_DOT, 1, RGB(0x00, 0x80, 0x80)); // 파란색 점선
	CPen* pOldPen = dc.SelectObject(&dottedPen);

	// 텍스트 배경을 투명하게 설정
	dc.SetBkMode(TRANSPARENT);

	// 텍스트 색상 설정
	dc.SetTextColor(RGB(0x80, 0xA0, 0xff));


	CRect clientRect;
	pParent->m_pDlgImage_Result->GetClientRect(&clientRect);

	int textOffsetY = 0; // 화면 밖에 표시될 텍스트의 Y 위치
	const int textPadding = 20; // 텍스트 간 간격

	// 1. 클릭된 점(PointData)들 간에 점선 및 텍스트 표시
	for (size_t i = 0; i < pParent->m_clickPoints.size(); ++i) {
		const auto& pointData = pParent->m_clickPoints[i];
		CString text;

		// 원 좌표와 반지름 텍스트 생성
		text.Format(_T("%d_(%d, %d)\nR=%d"), static_cast<int>(i + 1), pointData.point.x, pointData.point.y, pointData.radius);

		// 점선으로 다른 클릭된 점과 연결
		if (i > 0) {
			dc.MoveTo(pParent->m_clickPoints[i - 1].point);
			dc.LineTo(pointData.point);
		}

		// 원 중심 좌표가 화면 안에 있는지 확인
		if (clientRect.PtInRect(pointData.point)) {
			// 중앙 정렬하여 텍스트 출력
			CRect textRect(pointData.point.x - 50, pointData.point.y + 5, pointData.point.x + 50, pointData.point.y + 50);
			dc.DrawText(text, textRect, DT_CENTER | DT_WORDBREAK);
		}
		else {
			// 화면 밖으로 나가면 왼쪽 위에 차례대로 표시
			CRect textRect(5, textOffsetY, clientRect.Width() - 5, textOffsetY + textPadding * 2);
			dc.DrawText(text, textRect, DT_LEFT | DT_WORDBREAK);
			textOffsetY += textPadding * 2;
		}
	}

	// 마지막 점과 첫 번째 점을 연결 (삼각형 닫기)
	if (pParent->m_clickPoints.size() == 3) {
		dc.MoveTo(pParent->m_clickPoints[2].point);
		dc.LineTo(pParent->m_clickPoints[0].point);
	}

	// 2. 정원의 중심(m_circleCenters) 표시
	if (pParent->m_clickPoints.size() == 3) {
		const CPoint& center = pParent->m_circleCenters;
		CString centerText;
		centerText.Format(_T("Center\n(%d, %d)"), center.x, center.y);

		// 정원의 중심 좌표가 화면 안에 있는지 확인
		if (clientRect.PtInRect(center)) {
			// 중앙 정렬하여 텍스트 출력
			CRect textRect(center.x - 50, center.y + 10, center.x + 50, center.y + 50);
			dc.DrawText(centerText, textRect, DT_CENTER | DT_WORDBREAK);
		}
		else {
			// 화면 밖으로 나가면 왼쪽 위에 차례대로 표시
			CRect textRect(5, textOffsetY, clientRect.Width() - 5, textOffsetY + textPadding * 2);
			dc.DrawText(centerText, textRect, DT_LEFT | DT_WORDBREAK);
			textOffsetY += textPadding * 2;
		}
	}

	// 원래 펜 복원
	dc.SelectObject(pOldPen);
}
