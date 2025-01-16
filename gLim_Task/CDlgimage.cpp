// CDlgimage.cpp: 구현 파일
//

#include "pch.h"
#include "gLim_Task.h"
#include "afxdialogex.h"
#include "CDlgimage.h"


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
END_MESSAGE_MAP()


// CDlgimage 메시지 처리기

#include "gLim_TaskDlg.h"
void CDlgimage::OnBnClickedBtnUpParent()
{
	static int n = 100;
	CgLimTaskDlg* pDlg = (CgLimTaskDlg*)m_pParent;
	pDlg->callFunc(n++);
	
}
