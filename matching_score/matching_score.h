
// matching_score.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cmatching_scoreApp:
// �йش����ʵ�֣������ matching_score.cpp
//

class Cmatching_scoreApp : public CWinAppEx
{
public:
	Cmatching_scoreApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cmatching_scoreApp theApp;