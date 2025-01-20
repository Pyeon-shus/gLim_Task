
// gLim_TaskDlg.cpp: 구현 파일
//

#include "pch.h"
#include "framework.h"
#include "gLim_Task.h"
#include "gLim_TaskDlg.h"
#include "afxdialogex.h"
#include <iostream>
#include <thread>
#include <chrono>
#include <ctime>

using namespace std;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#pragma comment(linker, "/entry:WinMainCRTStartup /subsystem:console")

// 응용 프로그램 정보에 사용되는 CAboutDlg 대화 상자입니다.

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 대화 상자 데이터입니다.
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 지원입니다.

// 구현입니다.
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CgLimTaskDlg 대화 상자



CgLimTaskDlg::CgLimTaskDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_GLIM_TASK_DIALOG, pParent)
	, m_pDlgImage(nullptr)         // 멤버 변수 초기화
	, m_pDlgImage_Result(nullptr)  // 멤버 변수 초기화
	, m_nRadius(0)
	, m_CirWidth(0)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CgLimTaskDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_RAD, m_nRadius);
	DDX_Text(pDX, IDC_CIRCLE_WID, m_CirWidth);
}

BEGIN_MESSAGE_MAP(CgLimTaskDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_DESTROY()
	ON_EN_CHANGE(IDC_RAD, &CgLimTaskDlg::OnEnChangeRad)
	ON_EN_CHANGE(IDC_CIRCLE_WID, &CgLimTaskDlg::OnEnChangeCircleWid)
	ON_BN_CLICKED(IDC_BTN_RAND_MOV, &CgLimTaskDlg::OnBnClickedBtnRandMov)
	ON_BN_CLICKED(IDC_BTN_RESET, &CgLimTaskDlg::OnBnClickedBtnReset)
	ON_BN_CLICKED(IDC_BTN_SET, &CgLimTaskDlg::OnBnClickedBtnSet)
	ON_BN_CLICKED(IDC_BTN_PROCESS, &CgLimTaskDlg::OnBnClickedBtnProcess)
	ON_BN_CLICKED(IDC_BTN_RAND_MOV2, &CgLimTaskDlg::OnBnClickedBtnRandMov2)
END_MESSAGE_MAP()


// CgLimTaskDlg 메시지 처리기

BOOL CgLimTaskDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 시스템 메뉴에 "정보..." 메뉴 항목을 추가합니다.

	// IDM_ABOUTBOX는 시스템 명령 범위에 있어야 합니다.
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != nullptr)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 이 대화 상자의 아이콘을 설정합니다.  응용 프로그램의 주 창이 대화 상자가 아닐 경우에는
	//  프레임워크가 이 작업을 자동으로 수행합니다.
	SetIcon(m_hIcon, TRUE);			// 큰 아이콘을 설정합니다.
	SetIcon(m_hIcon, FALSE);		// 작은 아이콘을 설정합니다.

	SetDlgItemText(IDC_RAD, _T("10")); // 기본 반지름 값을 10으로 설정

	// TODO: 여기에 추가 초기화 작업을 추가합니다.
	MoveWindow(0, 0, 640*2, 800);
	m_pDlgImage = new CDlgimage(this);
	m_pDlgImage->Create(IDD_CDlgimage, this);
	m_pDlgImage->ShowWindow(SW_SHOW);
	m_pDlgImage->MoveWindow(0, 0, 640, 480);

	m_pDlgImage_Result = new CDlgimage(this);
	m_pDlgImage_Result->Create(IDD_CDlgimage, this);
	m_pDlgImage_Result->ShowWindow(SW_SHOW);
	m_pDlgImage_Result->MoveWindow(640, 0, 640, 480);

	return TRUE;  // 포커스를 컨트롤에 설정하지 않으면 TRUE를 반환합니다.
}

void CgLimTaskDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// 대화 상자에 최소화 단추를 추가할 경우 아이콘을 그리려면
//  아래 코드가 필요합니다.  문서/뷰 모델을 사용하는 MFC 애플리케이션의 경우에는
//  프레임워크에서 이 작업을 자동으로 수행합니다.

void CgLimTaskDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 그리기를 위한 디바이스 컨텍스트입니다.

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 클라이언트 사각형에서 아이콘을 가운데에 맞춥니다.
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 아이콘을 그립니다.
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

// 사용자가 최소화된 창을 끄는 동안에 커서가 표시되도록 시스템에서
//  이 함수를 호출합니다.
HCURSOR CgLimTaskDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CgLimTaskDlg::OnDestroy()
{
	CDialogEx::OnDestroy();

	if (m_pDlgImage != nullptr)
	{
		delete m_pDlgImage;
		m_pDlgImage = nullptr; // 포인터 초기화
	}

	if (m_pDlgImage_Result != nullptr)
	{
		delete m_pDlgImage_Result;
		m_pDlgImage_Result = nullptr; // 포인터 초기화
	}
}


void CgLimTaskDlg::callFunc(int n) {
	cout << n << endl;
}


void CgLimTaskDlg::OnEnChangeRad()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CgLimTaskDlg::OnEnChangeCircleWid()
{
	// TODO:  RICHEDIT 컨트롤인 경우, 이 컨트롤은
	// CDialogEx::OnInitDialog() 함수를 재지정 
	//하고 마스크에 OR 연산하여 설정된 ENM_CHANGE 플래그를 지정하여 CRichEditCtrl().SetEventMask()를 호출하지 않으면
	// ENM_CHANGE가 있으면 마스크에 ORed를 플래그합니다.

	// TODO:  여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CgLimTaskDlg::OnBnClickedBtnRandMov()
{
	// 클릭 지점이 3개인지 확인
	if (m_clickPoints.size() != 3) {
		AfxMessageBox(_T("좌표가 3개가 아닙니다. 클릭 좌표를 추가해 주세요."));
		return;
	}

	// 랜덤 이동 작업을 비동기로 실행
	thread(&CgLimTaskDlg::PerformRandomMovement, this, 1).detach();
}




void CgLimTaskDlg::OnBnClickedBtnReset()
{

	// m_pDlgImage 초기화
	if (m_pDlgImage != nullptr) {
		m_pDlgImage->ClearImage(); // m_pDlgImage 초기화
	}

	// 좌표값 초기화
	if (!m_clickPoints.empty()) {
		m_clickPoints.clear(); // 좌표 저장 벡터 초기화
	}

	// 화면과 좌표 값 초기화 메시지 출력
	AfxMessageBox(_T("화면과 좌표 값이 초기화되었습니다."));
}






void CgLimTaskDlg::OnBnClickedBtnSet()
{
	UpdateData(TRUE);
	// 컨트롤 ID 유효성 확인
	if (GetDlgItem(IDC_RAD) == nullptr) {
		AfxMessageBox(_T("IDC_RAD 컨트롤이 존재하지 않습니다."));
		return;
	}
	if (GetDlgItem(IDC_CIRCLE_WID) == nullptr) {
		AfxMessageBox(_T("IDC_CIRCLE_WID 컨트롤이 존재하지 않습니다."));
		return;
	}

	// 이후 기존 코드 실행
	CString strRadius, strCirWid;
	GetDlgItemText(IDC_RAD, strRadius);
	GetDlgItemText(IDC_CIRCLE_WID, strCirWid);

	int nRadius = _ttoi(strRadius);
	int nCirWidth = _ttoi(strCirWid);

	if (nRadius > 0 && nCirWidth > 0) {
		AfxMessageBox(_T("반지름 및 정원 폭 값이 설정되었습니다."));
		nRadius = _ttoi(strRadius);
		cout << m_nRadius << endl;
		nCirWidth = _ttoi(strCirWid);
		cout << m_CirWidth << endl;
	}
	else {
		AfxMessageBox(_T("유효한 값들을 입력하세요.\nex) value > 0"));
	}
}

void CgLimTaskDlg::PrintClickData()
{
	for (const auto& data : m_clickPoints) {
		cout << "X: " << data.point.x << ", Y: " << data.point.y << ", R: " << data.radius << endl;
	}
}


bool CgLimTaskDlg::CalculateCircle(const CPoint& p1, const CPoint& p2, const CPoint& p3, CPoint& center, int& radius)
{
	double x1 = p1.x, y1 = p1.y;
	double x2 = p2.x, y2 = p2.y;
	double x3 = p3.x, y3 = p3.y;

	// 행렬식 계산
	double det = (x1 * (y2 - y3)) - (y1 * (x2 - x3)) + ((x2 * y3) - (x3 * y2));
	if (fabs(det) < 1e-6) {
		return false; // 점이 거의 일직선인 경우 계산 실패
	}

	// 중심 계산
	double A = x1 * x1 + y1 * y1;
	double B = x2 * x2 + y2 * y2;
	double C = x3 * x3 + y3 * y3;

	double D = x1 * (y2 - y3) + x2 * (y3 - y1) + x3 * (y1 - y2);
	double E = A * (y3 - y2) + B * (y1 - y3) + C * (y2 - y1);
	double F = A * (x2 - x3) + B * (x3 - x1) + C * (x1 - x2);

	double centerX = -E / (2 * D);
	double centerY = -F / (2 * D);

	// 소수점 보정 및 정수 변환
	center.x = static_cast<int>(round(centerX));
	center.y = static_cast<int>(round(centerY));

	// 정원의 중심 좌표를 벡터에 저장
	m_circleCenters = center;

	// 반지름 계산 (모든 점에서 중심까지의 거리 평균)
	double r1 = sqrt((centerX - x1) * (centerX - x1) + (centerY - y1) * (centerY - y1));
	double r2 = sqrt((centerX - x2) * (centerX - x2) + (centerY - y2) * (centerY - y2));
	double r3 = sqrt((centerX - x3) * (centerX - x3) + (centerY - y3) * (centerY - y3));

	radius = static_cast<int>(round((r1 + r2 + r3) / 3)); // 평균 반지름 사용

	return true;
}



void CgLimTaskDlg::DrawEnclosingCircle()
{
	if (m_clickPoints.size() != 3) {
		return; // 정원을 그리기 위한 점이 부족하면 리턴
	}

	const auto& points = m_clickPoints;
	CPoint center;
	int radius;

	// 원의 중심과 반지름 계산
	if (!CalculateCircle(points[0].point, points[1].point, points[2].point, center, radius)) {
		return; // 점들이 일직선상에 있으면 리턴
	}

	int outerRadius = radius + m_CirWidth / 2;
	int innerRadius = radius - m_CirWidth / 2;

	// 큰 원 (회색)
	if (m_pDlgImage) {
		m_pDlgImage->DrawCircle(nullptr, center.x, center.y, outerRadius, 128); // 128 = 회색
	}

	// 작은 원 (하얀색)
	if (innerRadius > 0 && m_pDlgImage) {
		m_pDlgImage->DrawCircle(nullptr, center.x, center.y, innerRadius, 255); // 255 = 하얀색
	}
}

void CgLimTaskDlg::RedrawAll()
{
	// 이미지 초기화 (하얀색 배경)
	if (m_pDlgImage != nullptr) {
		m_pDlgImage->ClearImage();
	}

	// 정원을 먼저 그림
	if (m_clickPoints.size() == 3) {
		DrawEnclosingCircle();
	}

	// 3개의 원을 그림
	for (const auto& data : m_clickPoints) {
		if (m_pDlgImage != nullptr) {
			m_pDlgImage->DrawCircle(nullptr, data.point.x, data.point.y, data.radius, 0); // 검은색 원
		}
	}

	// 화면 갱신
	if (m_pDlgImage != nullptr) {
		m_pDlgImage->Invalidate();
	}
}




void CgLimTaskDlg::OnBnClickedBtnProcess()
{
	// TODO: 여기에 컨트롤 알림 처리기 코드를 추가합니다.
}


void CgLimTaskDlg::MoveClickPointsRandomly()
{
	if (m_clickPoints.empty()) {
		return;
	}

	int nWidth = m_pDlgImage->m_image.GetWidth();
	int nHeight = m_pDlgImage->m_image.GetHeight();

	// 클릭 지점 이동
	for (auto& point : m_clickPoints) {
		point.point.x = rand() % (nWidth-PADDING);
		point.point.y = rand() % (nHeight-PADDING);
	}

	// 화면 갱신
	RedrawAll();
	m_pDlgImage->Invalidate();
}

void CgLimTaskDlg::PerformRandomMovement(int ett)
{
	int iterations = ett; // 총 10번 반복
	int delayMs = 500;   // 초당 2회 (500ms 간격)

	for (int i = 0; i < iterations; ++i) {
		MoveClickPointsRandomly(); // 클릭 지점 랜덤 이동 수행

		// 정원의 중심 좌표 출력
		cout << "Iteration " << i + 1 << ":\n";
		cout << "Click Points:\n";

		// 클릭 지점 출력
		for (const auto& point : m_clickPoints) {
			cout << "  X: " << point.point.x << ", Y: " << point.point.y << ", Radius: " << point.radius << endl;
		}

		// 저장된 정원의 중심 좌표 출력
		cout << "Circle Center:\n";
		cout << "  Center X: " << m_circleCenters.x << ", Center Y: " << m_circleCenters.y << endl;


		// 화면 갱신
		RedrawAll();

		// 500ms 대기 (UI 멈춤 방지)
		std::this_thread::sleep_for(std::chrono::milliseconds(delayMs));
	}

	// 작업 완료 메시지 출력
	AfxMessageBox(_T("랜덤 이동 작업이 완료되었습니다."));
}



void CgLimTaskDlg::OnBnClickedBtnRandMov2()
{
	// 클릭 지점이 3개인지 확인
	if (m_clickPoints.size() != 3) {
		AfxMessageBox(_T("좌표가 3개가 아닙니다. 클릭 좌표를 추가해 주세요."));
		return;
	}

	// 랜덤 이동 작업을 비동기로 실행
	thread(&CgLimTaskDlg::PerformRandomMovement, this, 10).detach();

}
