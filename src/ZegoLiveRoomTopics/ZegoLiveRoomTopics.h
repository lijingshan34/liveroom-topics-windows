
// ZegoLiveRoomTopics.h : PROJECT_NAME Ӧ�ó������ͷ�ļ�
//

#pragma once

#ifndef __AFXWIN_H__
	#error "�ڰ������ļ�֮ǰ������stdafx.h�������� PCH �ļ�"
#endif

#include "resource.h"		// ������
#include "MediaPlayer/MediaPlayerUI/resource.h"

// CZegoLiveRoomTopicsApp: 
// �йش����ʵ�֣������ ZegoLiveRoomTopics.cpp
//

class CZegoLiveRoomTopicsApp : public CWinApp
{
public:
	CZegoLiveRoomTopicsApp();

// ��д
public:
	virtual BOOL InitInstance();

// ʵ��

	DECLARE_MESSAGE_MAP()
};

extern CZegoLiveRoomTopicsApp theApp;