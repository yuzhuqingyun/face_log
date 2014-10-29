
// face_detectDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "face_detect.h"
#include "face_detectDlg.h"
#include "face_detect_lib.h"
#include "CvvImage.h"
#include <iostream>

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// Cface_detectDlg 对话框




Cface_detectDlg::Cface_detectDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cface_detectDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cface_detectDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cface_detectDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_LOAD, &Cface_detectDlg::OnBnClickedButtonLoad)
	ON_BN_CLICKED(IDC_BUTTON_DETECT, &Cface_detectDlg::OnBnClickedButtonDetect)
END_MESSAGE_MAP()


// Cface_detectDlg 消息处理程序

BOOL Cface_detectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
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

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void Cface_detectDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cface_detectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR Cface_detectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cface_detectDlg::OnBnClickedButtonLoad()
{
	// TODO: 在此添加控件通知处理程序代码
	CFileDialog fdlg(TRUE,"bmp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"ALL Files|*.bmp;*.tif;*.jpg||");
	if (fdlg.DoModal() != IDOK)
	{
		return;
	}
	CString testImageName = fdlg.GetPathName();
	image = imread(testImageName.GetString(), 1);
	if (image.empty())
	{
		return;
	}
}

void Cface_detectDlg::OnBnClickedButtonDetect()
{
	// TODO: 在此添加控件通知处理程序代码
	//ASM定位
	Mat asmFace = GetASMFace(image, 0);
	//显示图像
	IplImage frame = asmFace;
	CDC *pDC = GetDlgItem(IDC_PICTURE_ASM)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_PICTURE_ASM)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(&frame);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);

	//opencv人脸检测
	Mat opencvFace = GetOpencvFace(image);
	//显示图像
	IplImage frame1 = opencvFace;
	CDC *pDC1 = GetDlgItem(IDC_PICTURE_OPENCVFACE)->GetDC();
	HDC hDC1= pDC1->GetSafeHdc();
	CRect rect1;
	GetDlgItem(IDC_PICTURE_OPENCVFACE)->GetClientRect(&rect1);
	CvvImage cimg1;
	cimg.CopyOf(&frame1);
	cimg.DrawToHDC(hDC1,&rect1);
	ReleaseDC(pDC1);
}
