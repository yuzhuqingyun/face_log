
// quality_assessmentDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "quality_assessment.h"
#include "quality_assessmentDlg.h"
#include <fstream>
#include "quality_lib.h"
#include "VisualizationTool.h"

using namespace std;;

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


// Cquality_assessmentDlg �Ի���




Cquality_assessmentDlg::Cquality_assessmentDlg(CWnd* pParent /*=NULL*/)
	: CDialog(Cquality_assessmentDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void Cquality_assessmentDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(Cquality_assessmentDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_TRAIN, &Cquality_assessmentDlg::OnBnClickedButtonTrain)
	ON_BN_CLICKED(IDC_BUTTON_PREDICT, &Cquality_assessmentDlg::OnBnClickedButtonPredict)
	ON_BN_CLICKED(IDC_BUTTON_SHOW, &Cquality_assessmentDlg::OnBnClickedButtonShow)
END_MESSAGE_MAP()


// Cquality_assessmentDlg ��Ϣ�������

BOOL Cquality_assessmentDlg::OnInitDialog()
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
	isSaveModel = 0;
	maxFace = 100;	//�����ȡ�����ͼƬ
	ntrainPerson = 2;
	nnumPerPerson = 10;

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void Cquality_assessmentDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

void Cquality_assessmentDlg::OnPaint()
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
HCURSOR Cquality_assessmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cquality_assessmentDlg::OnBnClickedButtonTrain()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	if (!isSaveModel)
	{
		string strDirectory = "G:\\Picture\\CAS-PEAL-R1\\quality_assessment\\";
		string strNormalTxt = strDirectory + "normal_image.txt";	//��׼ͼ·��
		string strLogTxt = strDirectory + "image_log.txt";	//ͼƬ����·��
		vector<string> vnormalPath, vlogPath;
		vector<Vec4f> vnormalPoint, vlogPoint;
		vector<Face> vnormalFaces, vlogFaces;
		//��txt�л�ȡͼƬ·������������
		ReadTxt(strNormalTxt, vnormalPath, vnormalPoint, ' ');	
		ReadTxt(strLogTxt, vlogPath, vlogPoint, ' ');
		//��ͼƬ·����������������ϢFace
		for (size_t i = 0; i<maxFace/*vnormalPath.size()*/; i++)
		{
			Face normalFace;
			GetFaceFromFilename(vnormalPath[i], vnormalPoint[i], normalFace);
			vnormalFaces.push_back(normalFace);
		}
		for (size_t j = 0; j<maxFace/*vlogPath.size()*/; j++)
		{
			Face logFace;
			GetFaceFromFilename(vlogPath[j], vlogPoint[j], logFace);
			vlogFaces.push_back(logFace);
		}

		//��logͼƬ�������ҵ���Ӧ��normalͼ
		//vector<Face> vlastNormalFace;
		//vector<vector<Face>> vlastLogFace;
		for (size_t m=0; m<vnormalFaces.size();m++)
		{
			vector<Face> vtempFace;
			for (size_t n=0; n<vlogFaces.size(); n++)
			{
				if (vnormalFaces[m].id == vlogFaces[n].id)	//�����ͬһ����
				{
					vtempFace.push_back(vlogFaces[n]);
				}
			}
			if (!vtempFace.empty())		//normalͼ�ܹ���logͼ�������ҵ���Ӧ
			{
				vlastNormalFace.push_back(vnormalFaces[m]);
				vlastLogFace.push_back(vtempFace);
			}
		}
		if (vlastNormalFace.empty() || vlastLogFace.empty() || (vlastNormalFace.size() != vlastLogFace.size()))
		{
			string error_message = "�޷�������׼ͼ��ͼ�����е�ӳ��";
			CV_Error(CV_StsBadArg, error_message);
		}

		int totalImage = 0;	//ѵ��ͼƬ����
		//����ÿ��ͼ������
		for (size_t i=0; i<vlastLogFace.size(); i++)
		{
			for (size_t j=0; j<vlastLogFace[i].size(); j++)
			{

				vlastLogFace[i][j].quality = ComputerQuality(vlastLogFace[i][j]);	//����

				////��������������ת��Ϊ������
				//Mat matOfQuality;
				//if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality))
				//{
				//	return -1;
				//}
				totalImage++;
			}
		}

		//��һ����������ָ��
		for (size_t i=0; i<vlastLogFace.size(); i++)
		{
			NormalizeQuality(vlastLogFace[i]);
		}
		//����ÿ��ͼ��ƥ���
		for (size_t i=0; i<vlastLogFace.size(); i++)
		{
			for (size_t j=0; j<vlastLogFace[i].size(); j++)
			{
				Mat stdLbph = ComputerLBPH(vlastNormalFace[i].face);
				Mat dstLbph = ComputerLBPH(vlastLogFace[i][j].face);
				//imshow("normal", vlastNormalFace[i].face);
				//imshow("compare", vlastLogFace[i][j].face);
				//waitKey(0)

				vlastLogFace[i][j].value = CompareLBPH(stdLbph, dstLbph);
			}
		}

		//�õ�ѵ������
		int num = 0;
		int nnumOfQuality = 11;
		Mat trainData = Mat::zeros(totalImage, nnumOfQuality, CV_64FC1);
		Mat labelsData = Mat::zeros(totalImage, 1, CV_64FC1);
		for (size_t i=0; i<ntrainPerson/*vlastLogFace.size()*/; i++)	//ntrainPerson��������ѵ��
		{
			for (size_t j=0; j</*nnumPerPerson*/vlastLogFace[i].size(); j++)		//ÿ����nnumPerPerson��ͼ����ѵ��
			{
				//��������������ת��Ϊ������
				Mat matOfQuality = trainData.row(num);
				if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality))
				{
					return ;
				}
				labelsData.at<double>(num) = vlastLogFace[i][j].value;	//ƥ���
				num++;

			}
		}
		//FileStorage fs("train.xml", FileStorage::WRITE);
		//fs<<"trainData"<<trainData;
		//fs<<"labelsData"<<labelsData;
		//fs.release();

		//SVM�ع�
		CvSVMParams param;  
		param.svm_type = CvSVM::EPS_SVR;  
		param.kernel_type = CvSVM::RBF;  
		param.C = 5;  
		param.p = 1e-3;  
		param.gamma = 0.1;  

		//ѵ��
		//CvSVM regresser;  
		trainData.convertTo(trainData, CV_32F);	//CV_64F����
		labelsData.convertTo(labelsData, CV_32F);
		regresser.train(trainData, labelsData, Mat(), Mat(), param);  
		regresser.save("quality_model.txt");

		//// predict the responses of the samples and show them  
		//for (float i = 0; i < 10; i+=0.23f)  
		//{  
		//	Mat sample(1,1, CV_32FC1);  
		//	sample.at<float>(0, 0) = static_cast<float>(i);  

		//	float response = regresser.predict(sample);  
		//	//cout<<response<<endl;  
		//	float x = (sample.at<float>(0, 0) - X_shift) * X_ratio;  
		//	float y = static_cast<float>(height) - (response - Y_shift) * Y_ratio;  
		//	circle(canvas, Point2f(x, y), 3, Scalar(0,255,0), -1);  
		//}  

		//imshow("predict", canvas);  

		//return 1;

		isSaveModel = 1;
	}
	else
	{
		//CvSVM regresser; 
		regresser.load("quality_model.txt");	
	}
	AfxMessageBox("ѵ���ɹ���");
}

void Cquality_assessmentDlg::OnBnClickedButtonPredict()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//����ʵƥ�������
	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		sort(vlastLogFace[i].begin(), vlastLogFace[i].end(), greater<Face>());
	}
	////��ʾ

	//for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	//{
	//	Mat showImage()
	//	for (size_t j=0; j<vlastLogFace[i].size(); j++)
	//	{
	//		Mat showImage = imread(vlastLogFace[i][j].filename, 1);
	//		imshow("normal", showImage);
	//		waitKey(0);
	//	}
	//}
	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		Mat showImage(160*3, 120*10, CV_8UC3);	//��ʾ��ͼ
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			//��������������ת��Ϊ������
			Mat matOfQuality ;
			if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality))
			{
				return ;
			}
			matOfQuality.convertTo(matOfQuality, CV_32F);

			//ΪͼƬ���ϱ��
			Mat image = imread(vlastLogFace[i][j].filename, 1);
			resize(image, image, Size(120, 160));
			char words[50];
			sprintf(words, "%s%.3f", "normal: ", vlastLogFace[i][j].value);
			putText(image, words, Point( 4,10),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 0, 0));
			double testLabel = regresser.predict(matOfQuality);	//Ԥ����
			char words2[50];
			sprintf(words2, "%s%.3f", "predict: ", testLabel);
			putText(image, words2, Point( 4,20),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 0, 0));
			Rect rect = Rect((int)(120*(j%10)), (int)(160*(j/10)), 120, 160);
			//showImage(rect) = image.clone();
			image.copyTo(showImage(rect));
			//vlastLogFace[i][j].value = testLabel;
		}
		imshow("result_normal", showImage);
		waitKey(0);
	}
	////��Ԥ���������

	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			//��������������ת��Ϊ������
			Mat matOfQuality1 ;
			if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality1))
			{
				return ;
			}
			matOfQuality1.convertTo(matOfQuality1, CV_32F);
			double testLabel = regresser.predict(matOfQuality1);	//Ԥ����
			vlastLogFace[i][j].value = testLabel;
		}
	}
	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		sort(vlastLogFace[i].begin(), vlastLogFace[i].end(), greater<Face>());
	}

	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		Mat showImage(160*3, 120*10, CV_8UC3);	//��ʾ��ͼ
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			//ΪͼƬ���ϱ��
			Mat image = imread(vlastLogFace[i][j].filename, 1);
			resize(image, image, Size(120, 160));
			char words[50];
			sprintf(words, "%s%.3f", "predict: ", vlastLogFace[i][j].value);
			putText(image, words, Point( 4,10),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 0, 0));
			//double testLabel = regresser.predict(matOfQuality);	//Ԥ����
			//char words2[50];
			//sprintf(words2, "%s%.3f", "predict: ", testLabel);
			//putText(image, words2, Point( 4,20),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 0, 0));
			Rect rect = Rect((int)(120*(j%10)), (int)(160*(j/10)), 120, 160);
			//showImage(rect) = image.clone();
			image.copyTo(showImage(rect));
			//vlastLogFace[i][j].value = testLabel;
		}
		imshow("result_predict", showImage);
		waitKey(0);
	}
}

void Cquality_assessmentDlg::OnBnClickedButtonShow()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	Mat image = imread("G:\\Picture\\Lena.jpg", 1);
	Mat gray;
	cvtColor(image , gray, CV_BGR2GRAY);
	VisualizationTool::showImageHistogram("histogram", image);
	VisualizationTool::showImageColorDistribution("ColorDistribution", image);
	VisualizationTool::imageSC("imageSc", gray);
	//VisualizationTool::ShowArrayHistogram("arrayHistogram", image);
	waitKey(0);
}
