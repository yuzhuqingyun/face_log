
// face_detect.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������


// Cface_detectApp:
// �йش����ʵ�֣������ face_detect.cpp
//

class Cface_detectApp : public CWinAppEx
{
public:
	Cface_detectApp();

// ��д
	public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern Cface_detectApp theApp;