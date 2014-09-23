
// matching_scoreDlg.h : ͷ�ļ�
//

#pragma once
#include "opencv2\opencv.hpp"

// Cmatching_scoreDlg �Ի���
class Cmatching_scoreDlg : public CDialog
{
// ����
public:
	Cmatching_scoreDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MATCHING_SCORE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��

public:
	cv::Mat stdImage, testImage;
// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonTrain();
	afx_msg void OnBnClickedButtonPredict();
};
