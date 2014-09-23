
// quality_assessmentDlg.h : 头文件
//

#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "train.h"
#include "quality.h"

using namespace cv;

// Cquality_assessmentDlg 对话框
class Cquality_assessmentDlg : public CDialog
{
// 构造
public:
	Cquality_assessmentDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_QUALITY_ASSESSMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTrain();
	afx_msg void OnBnClickedButtonPredict();

	vector<Face> vlastNormalFace;	//标准图
	vector<vector<Face>> vlastLogFace;	//测试图
	bool isSaveModel;
	CvSVM regresser; 
	int maxFace;	//允许读取的最多图片
	int ntrainPerson;	//用于训练人数
	int nnumPerPerson;
	afx_msg void OnBnClickedButtonShow();
};
