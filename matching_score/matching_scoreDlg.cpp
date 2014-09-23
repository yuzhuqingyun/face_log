
// matching_scoreDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "matching_score.h"
#include "matching_scoreDlg.h"
#include "CvvImage.h"
#include "lbph.h"

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


// Cmatching_scoreDlg �Ի���




Cmatching_scoreDlg::Cmatching_scoreDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cmatching_scoreDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cmatching_scoreDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cmatching_scoreDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, &Cmatching_scoreDlg::OnBnClickedButtonTrain)
	ON_BN_CLICKED(IDC_BUTTON_PREDICT, &Cmatching_scoreDlg::OnBnClickedButtonPredict)
END_MESSAGE_MAP()


// Cmatching_scoreDlg ��Ϣ��������

BOOL Cmatching_scoreDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// ��������...���˵������ӵ�ϵͳ�˵��С�

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

	// TODO: �ڴ����Ӷ���ĳ�ʼ������

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cmatching_scoreDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// �����Ի���������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void Cmatching_scoreDlg::OnPaint()
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
HCURSOR Cmatching_scoreDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cmatching_scoreDlg::OnBnClickedButtonTrain()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CFileDialog fdlg(TRUE,"bmp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"ALL Files|*.bmp;*.tif;*.jpg||");
	if (fdlg.DoModal() != IDOK)
	{
		return;
	}
	CString stdImageName = fdlg.GetPathName();
	stdImage = imread(stdImageName.GetString(), 0);

	IplImage frame = stdImage;
	CDC *pDC = GetDlgItem(IDC_PICTURE_STD)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_PICTURE_STD)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(&frame);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);
}

void Cmatching_scoreDlg::OnBnClickedButtonPredict()
{
	// TODO: �ڴ����ӿؼ�֪ͨ�����������
	CFileDialog fdlg(TRUE,"bmp",NULL,OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,"ALL Files|*.bmp;*.tif;*.jpg||");
	if (fdlg.DoModal() != IDOK)
	{
		return;
	}
	CString testImageName = fdlg.GetPathName();
	testImage = imread(testImageName.GetString(), 0);

	//����LBP����
	Mat stdLBPH = ComputerLBPH(stdImage);
	Mat testLBPH = ComputerLBPH(testImage);
	double dist = CompareLBPH(stdLBPH, testLBPH);

	//��ʾͼ��
	IplImage frame = testImage;
	CDC *pDC = GetDlgItem(IDC_PICTURE_TEST)->GetDC();
	HDC hDC= pDC->GetSafeHdc();
	CRect rect;
	GetDlgItem(IDC_PICTURE_TEST)->GetClientRect(&rect);
	CvvImage cimg;
	cimg.CopyOf(&frame);
	cimg.DrawToHDC(hDC,&rect);
	ReleaseDC(pDC);

	CString dist_lbp;
	dist_lbp.Format("%f", dist);
	GetDlgItem(IDC_EDIT_LBP)->SetWindowText(dist_lbp);
}