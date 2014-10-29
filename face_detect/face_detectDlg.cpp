
// face_detectDlg.cpp : ʵ���ļ�
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


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// �Ի�������
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
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


// Cface_detectDlg �Ի���




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


// Cface_detectDlg ��Ϣ�������

BOOL Cface_detectDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
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

	// ���ô˶Ի����ͼ�ꡣ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
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

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cface_detectDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR Cface_detectDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cface_detectDlg::OnBnClickedButtonLoad()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
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
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//ASM��λ
	Mat asmFace = GetASMFace(image, 0);
	//��ʾͼ��
	IplImage frame = asmFace;
	CDC *pDC = GetDlgItem(IDC_PICTURE_ASM)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_PICTURE_ASM)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(&frame);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);

	//opencv�������
	Mat opencvFace = GetOpencvFace(image);
	//��ʾͼ��
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
