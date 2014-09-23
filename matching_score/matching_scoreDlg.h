
// matching_scoreDlg.h : 头文件
//

#pragma once
#include "opencv2\opencv.hpp"

// Cmatching_scoreDlg 对话框
class Cmatching_scoreDlg : public CDialog
{
// 构造
public:
	Cmatching_scoreDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_MATCHING_SCORE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持

public:
	cv::Mat stdImage, testImage;
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
};
