
// quality_assessment.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cquality_assessmentApp:
// �йش����ʵ�֣������ quality_assessment.cpp
//

class Cquality_assessmentApp : public CWinAppEx
{
public:
	Cquality_assessmentApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cquality_assessmentApp theApp;