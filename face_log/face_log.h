
// face_log.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cface_logApp:
// �йش����ʵ�֣������ face_log.cpp
//

class Cface_logApp : public CWinAppEx
{
public:
	Cface_logApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cface_logApp theApp;