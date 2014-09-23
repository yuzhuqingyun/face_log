
// quality_assessmentDlg.h : ͷ�ļ�
//

#pragma once
#include "opencv2\core\core.hpp"
#include "opencv2\highgui\highgui.hpp"
#include "opencv2\opencv.hpp"
#include "train.h"
#include "quality.h"

using namespace cv;

// Cquality_assessmentDlg �Ի���
class Cquality_assessmentDlg : public CDialog
{
// ����
public:
	Cquality_assessmentDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_QUALITY_ASSESSMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


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

	vector<Face> vlastNormalFace;	//��׼ͼ
	vector<vector<Face>> vlastLogFace;	//����ͼ
	bool isSaveModel;
	CvSVM regresser; 
	int maxFace;	//�����ȡ�����ͼƬ
	int ntrainPerson;	//����ѵ������
	int nnumPerPerson;
	afx_msg void OnBnClickedButtonShow();
};
