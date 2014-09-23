
// quality_assessmentDlg.cpp : 实现文件
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


// Cquality_assessmentDlg 对话框




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


// Cquality_assessmentDlg 消息处理程序

BOOL Cquality_assessmentDlg::OnInitDialog()
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
	isSaveModel = 0;
	maxFace = 100;	//允许读取的最多图片
	ntrainPerson = 2;
	nnumPerPerson = 10;

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void Cquality_assessmentDlg::OnPaint()
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
HCURSOR Cquality_assessmentDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void Cquality_assessmentDlg::OnBnClickedButtonTrain()
{
	// TODO: 在此添加控件通知处理程序代码
	if (!isSaveModel)
	{
		string strDirectory = "G:\\Picture\\CAS-PEAL-R1\\quality_assessment\\";
		string strNormalTxt = strDirectory + "normal_image.txt";	//标准图路径
		string strLogTxt = strDirectory + "image_log.txt";	//图片长廊路径
		vector<string> vnormalPath, vlogPath;
		vector<Vec4f> vnormalPoint, vlogPoint;
		vector<Face> vnormalFaces, vlogFaces;
		//从txt中获取图片路径和两眼坐标
		ReadTxt(strNormalTxt, vnormalPath, vnormalPoint, ' ');	
		ReadTxt(strLogTxt, vlogPath, vlogPoint, ' ');
		//从图片路径和坐标获得人脸信息Face
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

		//从log图片序列中找到对应的normal图
		//vector<Face> vlastNormalFace;
		//vector<vector<Face>> vlastLogFace;
		for (size_t m=0; m<vnormalFaces.size();m++)
		{
			vector<Face> vtempFace;
			for (size_t n=0; n<vlogFaces.size(); n++)
			{
				if (vnormalFaces[m].id == vlogFaces[n].id)	//如果是同一个人
				{
					vtempFace.push_back(vlogFaces[n]);
				}
			}
			if (!vtempFace.empty())		//normal图能够在log图序列中找到对应
			{
				vlastNormalFace.push_back(vnormalFaces[m]);
				vlastLogFace.push_back(vtempFace);
			}
		}
		if (vlastNormalFace.empty() || vlastLogFace.empty() || (vlastNormalFace.size() != vlastLogFace.size()))
		{
			string error_message = "无法建立标准图和图像序列的映射";
			CV_Error(CV_StsBadArg, error_message);
		}

		int totalImage = 0;	//训练图片总数
		//计算每幅图的质量
		for (size_t i=0; i<vlastLogFace.size(); i++)
		{
			for (size_t j=0; j<vlastLogFace[i].size(); j++)
			{

				vlastLogFace[i][j].quality = ComputerQuality(vlastLogFace[i][j]);	//质量

				////将各个评估质量转化为行向量
				//Mat matOfQuality;
				//if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality))
				//{
				//	return -1;
				//}
				totalImage++;
			}
		}

		//归一化各个质量指标
		for (size_t i=0; i<vlastLogFace.size(); i++)
		{
			NormalizeQuality(vlastLogFace[i]);
		}
		//计算每幅图的匹配度
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

		//得到训练矩阵
		int num = 0;
		int nnumOfQuality = 11;
		Mat trainData = Mat::zeros(totalImage, nnumOfQuality, CV_64FC1);
		Mat labelsData = Mat::zeros(totalImage, 1, CV_64FC1);
		for (size_t i=0; i<ntrainPerson/*vlastLogFace.size()*/; i++)	//ntrainPerson个人用于训练
		{
			for (size_t j=0; j</*nnumPerPerson*/vlastLogFace[i].size(); j++)		//每个人nnumPerPerson张图用于训练
			{
				//将各个评估质量转化为行向量
				Mat matOfQuality = trainData.row(num);
				if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality))
				{
					return ;
				}
				labelsData.at<double>(num) = vlastLogFace[i][j].value;	//匹配度
				num++;

			}
		}
		//FileStorage fs("train.xml", FileStorage::WRITE);
		//fs<<"trainData"<<trainData;
		//fs<<"labelsData"<<labelsData;
		//fs.release();

		//SVM回归
		CvSVMParams param;  
		param.svm_type = CvSVM::EPS_SVR;  
		param.kernel_type = CvSVM::RBF;  
		param.C = 5;  
		param.p = 1e-3;  
		param.gamma = 0.1;  

		//训练
		//CvSVM regresser;  
		trainData.convertTo(trainData, CV_32F);	//CV_64F报错
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
	AfxMessageBox("训练成功！");
}

void Cquality_assessmentDlg::OnBnClickedButtonPredict()
{
	// TODO: 在此添加控件通知处理程序代码
	//按真实匹配度排序
	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		sort(vlastLogFace[i].begin(), vlastLogFace[i].end(), greater<Face>());
	}
	////显示

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
		Mat showImage(160*3, 120*10, CV_8UC3);	//显示大图
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			//将各个评估质量转化为行向量
			Mat matOfQuality ;
			if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality))
			{
				return ;
			}
			matOfQuality.convertTo(matOfQuality, CV_32F);

			//为图片打上标记
			Mat image = imread(vlastLogFace[i][j].filename, 1);
			resize(image, image, Size(120, 160));
			char words[50];
			sprintf(words, "%s%.3f", "normal: ", vlastLogFace[i][j].value);
			putText(image, words, Point( 4,10),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 0, 0));
			double testLabel = regresser.predict(matOfQuality);	//预测结果
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
	////按预测分数排序

	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			//将各个评估质量转化为行向量
			Mat matOfQuality1 ;
			if (QualityAsRowMatrix(vlastLogFace[i][j].quality, matOfQuality1))
			{
				return ;
			}
			matOfQuality1.convertTo(matOfQuality1, CV_32F);
			double testLabel = regresser.predict(matOfQuality1);	//预测结果
			vlastLogFace[i][j].value = testLabel;
		}
	}
	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		sort(vlastLogFace[i].begin(), vlastLogFace[i].end(), greater<Face>());
	}

	for (size_t i=ntrainPerson; i<vlastNormalFace.size(); i++)
	{
		Mat showImage(160*3, 120*10, CV_8UC3);	//显示大图
		for (size_t j=0; j<vlastLogFace[i].size(); j++)
		{
			//为图片打上标记
			Mat image = imread(vlastLogFace[i][j].filename, 1);
			resize(image, image, Size(120, 160));
			char words[50];
			sprintf(words, "%s%.3f", "predict: ", vlastLogFace[i][j].value);
			putText(image, words, Point( 4,10),CV_FONT_HERSHEY_COMPLEX, 0.4, Scalar(255, 0, 0));
			//double testLabel = regresser.predict(matOfQuality);	//预测结果
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
	// TODO: 在此添加控件通知处理程序代码
	Mat image = imread("G:\\Picture\\Lena.jpg", 1);
	Mat gray;
	cvtColor(image , gray, CV_BGR2GRAY);
	VisualizationTool::showImageHistogram("histogram", image);
	VisualizationTool::showImageColorDistribution("ColorDistribution", image);
	VisualizationTool::imageSC("imageSc", gray);
	//VisualizationTool::ShowArrayHistogram("arrayHistogram", image);
	waitKey(0);
}
