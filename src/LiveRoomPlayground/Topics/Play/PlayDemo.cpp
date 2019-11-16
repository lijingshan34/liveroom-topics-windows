#include "stdafx.h"
#include "PlayDemo.h"


#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUtilTools.h"

using std::string;

PlayDemo::PlayDemo()
{
}


PlayDemo::~PlayDemo()
{
}


void PlayDemo::SetViewLogCallBack(ILogToView * log_cb)
{
    view_log_ = log_cb;
}

void PlayDemo::SetStateUpdateObserver(ZGPlayStatusCallBackType cb)
{
    status_cb_ = cb;
}

void PlayDemo::SetRealDataRefreshObserver(ZGPlayRealDataRefreshCallBackType cb)
{
    real_data_refresh_cb_ = cb;
}

bool PlayDemo::InitSDK(PlayInitSDKConfig c)
{
    UpdateStatus(kZGPlayStatus_Starting_Init_SDK);

    PrintLogToView("����SDK������%s", c.is_test_env ? "ʹ�ò��Ի���" : "ʹ����ʽ����");
    LIVEROOM::SetUseTestEnv(c.is_test_env);

    PrintLogToView("����userid = %s��username = %s", c.user_id.c_str(), c.user_name.c_str());
    LIVEROOM::SetUser(c.user_id.c_str(), c.user_name.c_str());
    LIVEROOM::SetLivePlayerCallback(this);
    LIVEROOM::SetRoomCallback(this);
    LIVEROOM::SetLiveEventCallback(this);

    cur_real_data_.bit_rate_bps_per_second = 0;
    cur_real_data_.frame_rate = 0;
    cur_real_data_.resolution_h = 0;
    cur_real_data_.resolution_w = 0;
    cur_real_data_.room_id = "";
    cur_real_data_.stream_id = "";

	/*
	PrintLogToView("ǿ�ƴӷ���������");
	LIVEROOM::SetConfig("prefer_play_ultra_source=1");
	*/

    PrintLogToView("ִ�г�ʼ��");
    return ZGManagerInstance()->InitSdk(c.app_id, c.app_sign.data(), c.app_sign.size());
}

bool PlayDemo::LoginRoom(PlayLoginRomConfig c)
{
    UpdateStatus(kZGPlayStatus_Starting_Login_Room);

    cur_real_data_.room_id = c.room_id;
    
    PrintLogToView("room id = %s", c.room_id.c_str());

    return LIVEROOM::LoginRoom(c.room_id.c_str(), c.room_role);
}

bool PlayDemo::StartPlay(PlayConfig c)
{
    cur_real_data_.stream_id = c.stream_id;
    PrintLogToView("���� id = %s", c.stream_id.c_str());
    return LIVEROOM::StartPlayingStream(c.stream_id.c_str(), c.view_hwnd, c.play_param.c_str());
}

void PlayDemo::OnRemoteCameraStatusUpdate(const char* pStreamID, int nStatus, int nReason)
{
    PrintLogToView("�Զ�����ͷ״̬ stream id = %s, status = %d", pStreamID, nStatus);
}

void PlayDemo::OnRemoteMicStatusUpdate(const char* pStreamID, int nStatus, int nReason)
{
    PrintLogToView("�Զ���˷�״̬ stream id = %s, status = %d", pStreamID, nStatus);
}

std::string PlayDemo::DescOfStatus(ZGPlayStatus status)
{
    static string status_des_array[] = {
    "None",
    "Starting_Init_SDK",
    "Init_SDK_OK",
    "Starting_Login_Room",
    "Login_OK",
    "Starting_Playing",
    "Playing",
    "Stoped_Playing"
    };

    return status_des_array[status];
}

void PlayDemo::OnAVKitEvent(int event, AV::EventInfo* pInfo)
{
    
}

void PlayDemo::OnInitSDK(int nError)
{
    if (nError == 0)
    {
        UpdateStatus(kZGPlayStatus_Init_SDK_OK);
    }
    else {
        UpdateStatus(kZGPlayStatus_None);
        PrintLogToView("��ʼ��ʧ�ܣ�������Ϊ��%d", nError);
    }
}

void PlayDemo::OnLogoutRoom(int errorCode, const char *pszRoomID)
{
    UpdateStatus(kZGPlayStatus_None);
}

void PlayDemo::OnReconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("�������Ϸ�����");
}

void PlayDemo::OnTempBroken(int errorCode, const char *pszRoomID)
{
    PrintLogToView("OnTempBroken����ʱ����֪ͨ�������� = %d", errorCode);
}

void PlayDemo::OnLoginRoom(int errorCode, const char *pszRoomID, const ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode == 0)
    {
        UpdateStatus(kZGPlayStatus_Login_OK);
    }
    else {
        UpdateStatus(kZGPlayStatus_None);
        PrintLogToView("��¼����ʧ�ܣ�������Ϊ��%d", errorCode);
    }
}

void PlayDemo::OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason/*=""*/)
{
    PrintLogToView("���ߵ��ߣ�reason = %d", reason);
}

void PlayDemo::OnDisconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("�ѵ���");
}

void PlayDemo::OnStreamUpdated(ZegoStreamUpdateType type, ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    PrintLogToView("������֪ͨ��%s", type == COMMON::StreamAdded ? "������":"��ɾ��");
}

void PlayDemo::OnStreamExtraInfoUpdated(ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    
}

void PlayDemo::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
    
}

void PlayDemo::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
    
}

void PlayDemo::OnPlayStateUpdate(int stateCode, const char* pszStreamID)
{
    if (stateCode == 0)
    {
        UpdateStatus(kZGPlayStatus_Playing);
    }
    else {
        UpdateStatus(kZGPlayStatus_None);
        PrintLogToView("����״̬���£�״̬��Ϊ:%d",stateCode);
    }
}

void PlayDemo::OnPlayQualityUpdate(const char* pszStreamID, ZegoPlayQuality playQuality)
{
    if (real_data_refresh_cb_ != nullptr)
    {
        cur_real_data_.bit_rate_bps_per_second = playQuality.kbps;
        cur_real_data_.frame_rate = playQuality.fps;

        real_data_refresh_cb_(cur_real_data_);
    }
}

void PlayDemo::OnVideoSizeChanged(const char* pStreamID, int nWidth, int nHeight)
{
    cur_real_data_.resolution_w = nWidth;
    cur_real_data_.resolution_h = nHeight;

    PrintLogToView("��Ƶ֡��С�仯: width = %d, height = %d", nWidth, nHeight);
}

void PlayDemo::OnRecvRemoteAudioFirstFrame(const char* pStreamID)
{
	PrintLogToView("�յ���Ƶ��֡: streamId= %s", pStreamID);
}

void PlayDemo::UpdateStatus(ZGPlayStatus s)
{
    cur_status_ = s;

    if (status_cb_ != nullptr)
    {
        status_cb_(cur_status_);
    }
}

void PlayDemo::PrintLogToView(const char * format, ...)
{
    char log_buf[1024] = { 0 };
    va_list la;
    va_start(la, format);
    vsprintf_s(log_buf, format, la);
    va_end(la);

    if (view_log_ != nullptr)
    {
        view_log_->PrintToView(log_buf);
    }
}
