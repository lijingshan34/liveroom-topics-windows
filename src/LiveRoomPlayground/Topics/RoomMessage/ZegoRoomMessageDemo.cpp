#include "stdafx.h"
#include "ZegoRoomMessageDemo.h"


ZegoRoomMessageDemo::ZegoRoomMessageDemo()
{
}


ZegoRoomMessageDemo::~ZegoRoomMessageDemo()
{
}

void ZegoRoomMessageDemo::LoginRoom(string user_id)
{
	bool is_test_env = true;
	ZGLog("ʹ��%s����", is_test_env ? "���Ի���" : "��ʽ����");

	LIVEROOM::SetUseTestEnv(is_test_env);

	string user_name = user_id;

	ZGLog("����userid = %s��username = %s", user_id.c_str(), user_name.c_str());
	LIVEROOM::SetUser(user_id.c_str(), user_name.c_str());

	// ���ù��ڿ��Դ������䣬���������û��б����
	LIVEROOM::SetRoomConfig(true, true);

	LIVEROOM::SetRoomCallback(this);
	LIVEROOM::SetIMCallback(this);
	
	ZGLog("ִ�г�ʼ��");
	if (!ZGManagerInstance()->InitSdk())
	{
		ZGLog("��ʼ��ʧ��");
	}
}

void ZegoRoomMessageDemo::SendCustomMessage(string peer_id, string content)
{
	ROOM::ZegoUser *user = new ROOM::ZegoUser();
	strcpy(user->szUserId, peer_id.c_str());
	strcpy(user->szUserName, peer_id.c_str());	
	LIVEROOM::SendCustomCommand(user, 1, content.c_str());
}

void ZegoRoomMessageDemo::SendRoomMessage(string content)
{
	LIVEROOM::SendRoomMessageEx(ROOM::Text, ROOM::Chat, content.c_str());
}

void ZegoRoomMessageDemo::SendBigRoomMessage(string content)
{
	LIVEROOM::SendBigRoomMessage(ROOM::Text, ROOM::Chat, content.c_str());
}

void ZegoRoomMessageDemo::InitMainHwnd(HWND hwnd)
{
	main_hwnd_ = hwnd;
}

void ZegoRoomMessageDemo::OnLoginRoom(int errorCode, const char *pszRoomID, const COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
	if (errorCode == 0)
	{
		ZGLog("��¼�ɹ�");
	}
	else
	{
		ZGLog("��¼ʧ��");
	}
}

void ZegoRoomMessageDemo::OnInitSDK(int nError)
{
	if (nError == 0)
	{
		ZGLog("��ʼ��sdk�ɹ�");

		
		// ��ʼ���ɹ������߳�Ȼ���ڿ�ʼ��¼����
		PostUIData * pdata = CreateUIData();
		pdata->cb_in_ui_fun = [pdata, this]()->void
		{
			ZGLog("��ʼ��¼��room id = %s", room_id_.c_str());

			if (!LIVEROOM::LoginRoom(room_id_.c_str(), COMMON::Anchor))
			{
				ZGLog("��¼ʧ��");
			}

			DestroyUIData(pdata);
		};

		GlobalPostMsgDataToUI(main_hwnd_, pdata);
	}
	else
	{
		ZGLog("��ʼ��ʧ�ܣ�������Ϊ��%d", nError);
	}
}

void ZegoRoomMessageDemo::OnKickOut(int reason, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnDisconnect(int errorCode, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
	ZGLog("OnCustomCommand: errorCode=%d, requestSeq=%d, roomId=%s", errorCode, requestSeq, pszRoomID);
}

void ZegoRoomMessageDemo::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
	ZGLog("OnRecvCustomCommand: pszUserId=%s, pszUserName=%s, pszContent=%s,pszRoomID=%s", pszUserId, pszUserName, pszContent, pszRoomID);
}

void ZegoRoomMessageDemo::OnStreamUpdated(COMMON::ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{

}

void ZegoRoomMessageDemo::OnSendRoomMessage(int errorCode, const char *pszRoomID, int sendSeq, unsigned long long messageId)
{
	ZGLog("OnSendRoomMessage: errorCode=%d, pszRoomID=%s, sendSeq=%d, messageId=%ull", errorCode, pszRoomID, sendSeq, messageId);
}

void ZegoRoomMessageDemo::OnRecvRoomMessage(ROOM::ZegoRoomMessage *pMessageInfo, unsigned int messageCount, const char *pszRoomID)
{
	ZGLog("OnRecvRoomMessage: messageCount=%d, pszRoomID=%s", messageCount, pszRoomID);
}

void ZegoRoomMessageDemo::OnUserUpdate(const COMMON::ZegoUserInfo *pUserInfo, unsigned int userCount, COMMON::ZegoUserUpdateType type)
{

}

void ZegoRoomMessageDemo::OnSendBigRoomMessage(int errorCode, const char *pszRoomID, int sendSeq, const char *pszmessageID)
{
	ZGLog("OnSendBigRoomMessage: errorCode=%d, pszRoomID=%s, sendSeq=%d, messageId=%s", errorCode, pszRoomID, sendSeq, pszmessageID);
}

void ZegoRoomMessageDemo::OnRecvBigRoomMessage(ROOM::ZegoBigRoomMessage *pMessageInfo, unsigned int messageCount, const char *pszRoomID)
{
	ZGLog("OnRecvBigRoomMessage: messageCount=%d, pszRoomID=%s", messageCount, pszRoomID);
}
