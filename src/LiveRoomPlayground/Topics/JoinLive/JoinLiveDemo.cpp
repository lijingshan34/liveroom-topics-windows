#include "stdafx.h"
#include "JoinLiveDemo.h"

#include <algorithm>
#include <thread>

#include "AppSupport/ZGHelper.h"
#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGConfigHelper.h"


// 190*190
const int kResolutionWidth = 190;
const int kResolutionHeight = 190;
const int kBitrate = 250000;//250kbps

JoinLiveDemo::JoinLiveDemo()
{
}

JoinLiveDemo::~JoinLiveDemo()
{
}

void JoinLiveDemo::SetDemoCallBack(JoinLiveCallBack * cb)
{
    join_live_cb_ = cb;
}

void JoinLiveDemo::SetLocalVideoView(HWND view, LIVEROOM::ZegoVideoViewMode view_mode /*= LIVEROOM::ZegoVideoViewModeScaleAspectFit*/)
{
    local_video_view_ = view;
    local_view_mode = view_mode;
    
}

void JoinLiveDemo::SetRemoteVideoView(HWND view, ZegoJoinLiveUser for_user, LIVEROOM::ZegoVideoViewMode view_mode /*= LIVEROOM::ZegoVideoViewModeScaleAspectFit*/)
{
    PlayConfig config;
    if (GetPlayConfigByUser(for_user, config))
    {
        config.view_hwnd = view;
        config.view_mode = view_mode;
        UpdatePlayConfigByUser(for_user, config);

        LIVEROOM::StartPlayingStream(config.stream_id.c_str(), config.view_hwnd);
        LIVEROOM::SetViewMode(config.view_mode, config.stream_id.c_str());

    }
    else {
        PlayConfig play_config;
        play_config.view_hwnd = view;
        play_config.view_mode = view_mode;

        std::lock_guard<std::mutex> lock(play_config_map_mutex_);
        play_map_info_.insert(std::make_pair(for_user, play_config));
    }
}


void JoinLiveDemo::PrintLogToView(const char * format, ...)
{
    if (view_log_ != nullptr)
    {
        char log_buf[1024] = { 0 };
        va_list la;
        va_start(la, format);
        vsprintf_s(log_buf, format, la);
        va_end(la);

        view_log_->PrintToView(log_buf);
    }
}


void JoinLiveDemo::LoginRoom(string room_id, bool need_publish_after_logined)
{
    // 1.init sdk        ��ʼ��sdk
    // 2.login room      ��¼����
    // 3.start publish   ��ʼ����
    // 4.on stream added �յ��������¼�
    // 5.start playing   ��ʼ����

    room_id_ = room_id;
    logined_need_publish_ = need_publish_after_logined;

    bool is_test_env = true;
    PrintLogToView("ʹ��%s����", is_test_env ? "���Ի���":"��ʽ����");

    LIVEROOM::SetUseTestEnv(is_test_env);

    string user_id = ZGHelper::Instance()->GetRandomUUID().c_str();
    string user_name = ZGHelper::Instance()->GetMachineName().c_str();

    PrintLogToView("����userid = %s��username = %s", user_id.c_str(), user_name.c_str());
    LIVEROOM::SetUser(user_id.c_str(), user_name.c_str());

    // ���ù��ڿ��Դ������䣬���������û��б����
    LIVEROOM::SetRoomConfig(true, true);

    LIVEROOM::SetRoomCallback(this);             // ����ص�ע��
    LIVEROOM::SetLivePublisherCallback(this);    // �����ص�ע�� 
    LIVEROOM::SetLivePlayerCallback(this);       // �����ص�ע��
    LIVEROOM::SetDeviceStateCallback(this);      // �豸�¼��ص�ע��

    PrintLogToView("ִ�г�ʼ��");
    if (!ZGManagerInstance()->InitSdk())
    {
        UpdateLoginEvent(kZGJoinLiveLoginEventLoginFailed);
    }

}

void JoinLiveDemo::StartPublish()
{
    if (have_published_)
    {
        return;
    }

    have_published_ = true;

    PrintLogToView("��ʼԤ��");

    // ���òɼ��ֱ��ʺͱ���ֱ���
    PrintLogToView("���òɼ��ֱ��ʺͱ���ֱ���,%dx%d", kResolutionWidth, kResolutionHeight);
    ZGConfigHelperInstance()->SetPublishResolution(kResolutionWidth, kResolutionHeight);

    LIVEROOM::SetPreviewView(local_video_view_);
    LIVEROOM::StartPreview();

    string stream_id = ZGHelper::Instance()->GetRandomUUID().c_str();
    string live_title = "";
    string publish_param = "";
    AV::ZegoPublishFlag publish_flag = AV::ZEGO_JOIN_PUBLISH;// ����ʽ

    PrintLogToView("���� id = %s", stream_id.c_str());

    PrintLogToView("��ʼ����");

    ZGConfigHelperInstance()->SetVideoBitrate(kBitrate);

    PrintLogToView("��������Ϊ:%d bps", kBitrate);

    LIVEROOM::StartPublishing(live_title.c_str(), stream_id.c_str(), publish_flag, publish_param.c_str());
}

void JoinLiveDemo::InitMainHwnd(HWND hwnd)
{
    main_hwnd_ = hwnd;
}

void JoinLiveDemo::SetLogCallBack(ILogToView *log_cb)
{
    view_log_ = log_cb;
}

void JoinLiveDemo::OnPlayStateUpdate(int stateCode, const char* pszStreamID)
{
    if (stateCode == 0)
    {
        PrintLogToView("�����ɹ�");
    }
    else {
        PrintLogToView("����״̬���£�����ʧ�ܣ�״̬�� = %d",stateCode);
    }
}

void JoinLiveDemo::OnPlayQualityUpdate(const char* pszStreamID, ZegoPlayQuality playQuality)
{
    
}

void JoinLiveDemo::OnVideoSizeChanged(const char* pStreamID, int nWidth, int nHeight)
{
    
}

void JoinLiveDemo::OnAudioDeviceStateChanged(AV::AudioDeviceType deviceType, AV::DeviceInfo *deviceInfo, AV::DeviceState state)
{
    
}

void JoinLiveDemo::OnDeviceError(const char* deviceName, int errorCode)
{
    if (errorCode != 0)
    {
        string name = deviceName;
        if (name == "camera")
        {
            PrintLogToView("������ͷ����������������ͷ�Ƿ�ռ��");
        }else if (name == "microphone")
        {
            PrintLogToView("����˷�ʧ��");
        }else if (name == "speaker")
        {
            PrintLogToView("��������ʧ��");
        }
    }   
}

void JoinLiveDemo::LogoutRoom()
{
    have_published_ = false;
    LIVEROOM::StopPublishing();
    LIVEROOM::LogoutRoom();
    UpdateLoginEvent(kZGJoinLiveLoginEventLogout);
}

void JoinLiveDemo::OnInitSDK(int nError)
{
    if (nError == 0)
    {
        PrintLogToView("��ʼ��sdk�ɹ�");

        // ��ʼ���ɹ������߳�Ȼ���ڿ�ʼ��¼����
        PostUIData * pdata = CreateUIData();
        pdata->cb_in_ui_fun = [pdata, this]()->void
        {
            PrintLogToView("��ʼ��¼��room id = %s", room_id_.c_str());

            UpdateLoginEvent(kZGJoinLiveLoginEventLogin);
            start_retry_login_room_timestamp_ = -1;

            COMMON::ZegoRoomRole role = logined_need_publish_ ? COMMON::Anchor: COMMON::Audience;
            PrintLogToView("��%s��ݵ�¼", role == COMMON::Anchor ? "����":"����");
            if (!LIVEROOM::LoginRoom(room_id_.c_str(), role))
            {
                UpdateLoginEvent(kZGJoinLiveLoginEventLoginFailed);
            }

            DestroyUIData(pdata);
        };

        GlobalPostMsgDataToUI(main_hwnd_, pdata);
    }
    else 
    {
        PrintLogToView("��ʼ��ʧ�ܣ�������Ϊ��%d", nError);
        UpdateLoginEvent(kZGJoinLiveLoginEventLoginFailed);
    }
}

void JoinLiveDemo::OnLoginRoom(int errorCode, const char *pszRoomID, const  COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount)
{
    if (errorCode == 0)
    {
        start_retry_login_room_timestamp_ = -1;

        PrintLogToView("��¼�ɹ�");
        UpdateLoginEvent(kZGJoinLiveLoginEventLoginSuccess);

        if (logined_need_publish_)
        {
            // ��¼����ɹ�����Ҫ���������߳��ڿ�ʼ����
            PostUIData * pdata = CreateUIData();
            pdata->cb_in_ui_fun = [pdata, this]()->void
            {
                StartPublish();

                DestroyUIData(pdata);
            };

            GlobalPostMsgDataToUI(main_hwnd_, pdata);
        }

        for (int i = 0; i < streamCount; ++i)
        {
            PrintLogToView("index:%d, szStreamId=%s, szUserId = %s", i, pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId);

            ProcessStreamAdded(pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId, pStreamInfo[i].szUserName, pszRoomID);
        }

    }
    else {
        PrintLogToView("��¼����ʧ�ܣ�������Ϊ��%d", errorCode);
        UpdateLoginEvent(kZGJoinLiveLoginEventLoginFailed);
        if (auto_retry_login_room_)
        {
            std::thread([this]() {
                std::this_thread::sleep_for(std::chrono::milliseconds(retry_interval_ms_));
                RetryLoginRoom();
            }).detach();
        }
    }
}

void JoinLiveDemo::OnLogoutRoom(int errorCode, const char *pszRoomID)
{
    PrintLogToView("���˳���¼");
}

void JoinLiveDemo::OnKickOut(int reason, const char *pszRoomID, const char* pszCustomReason/*=""*/)
{
    PrintLogToView("���ߵ��ߣ�reason = %d", reason);
    UpdateLoginEvent(kZGJoinLiveLoginEventKickOut);
}

void JoinLiveDemo::OnDisconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("�ѵ���");
    UpdateLoginEvent(kZGJoinLiveLoginEventDisconnect);
}

void JoinLiveDemo::OnReconnect(int errorCode, const char *pszRoomID)
{
    PrintLogToView("�������Ϸ�����");
    UpdateLoginEvent(kZGJoinLiveLoginEventReconnect);
}

void JoinLiveDemo::OnTempBroken(int errorCode, const char *pszRoomID)
{
    PrintLogToView("OnTempBroken����ʱ���ߣ�SDK�ڲ����ڽ��������������� = %d", errorCode);
    UpdateLoginEvent(kZGJoinLiveLoginEventTempBroke);
}

void JoinLiveDemo::OnStreamUpdated(ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    PrintLogToView("OnStreamUpdated, %s, streamCount = %d, pszRoomID = %s", type == COMMON::StreamAdded ? "������" : "��ɾ��", streamCount, pszRoomID);

    for (int i = 0; i < streamCount; ++i)
    {
        PrintLogToView("index:%d , szStreamId = %s, ", i, pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId);
        // ��������
        if (type == COMMON::StreamAdded)
        {
            ProcessStreamAdded(pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId, pStreamInfo[i].szUserName, pszRoomID);
        }
        // ��ɾ����֪ͨ
        else if (type == COMMON::StreamDeleted) {

            ProcessStreamDeleted(pStreamInfo[i].szStreamId, pStreamInfo[i].szUserId, pStreamInfo[i].szUserName, pszRoomID);
        }
    }
}

void JoinLiveDemo::OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID)
{
    
}

void JoinLiveDemo::OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID)
{
    
}

void JoinLiveDemo::OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID)
{
    
}

void JoinLiveDemo::OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo)
{
    if (stateCode == 0)
    {
        PrintLogToView("�����ɹ�");
    }
    else {
        PrintLogToView("����״̬����£�����ʧ�ܣ�״̬�� = %d", stateCode);
        have_published_ = false;
    }
}

void JoinLiveDemo::OnPublishQualityUpdate(const char* pszStreamID, ZegoPublishQuality publishQuality)
{
    
}

void JoinLiveDemo::OnCaptureVideoSizeChanged(int nWidth, int nHeight)
{
    PrintLogToView("�ɼ���Ƶ֡��С�仯��width = %d, height = %d", nWidth, nHeight);
}

void JoinLiveDemo::OnCaptureVideoFirstFrame()
{
    PrintLogToView("�ɼ���Ƶ��֡����");
}


void JoinLiveDemo::UpdateLoginEvent(ZGJoinLiveLoginEvent event)
{
    ZGJoinLiveStatus status = login_status_;

    switch (status) {
    case kZGJoinLiveLoginStatusLogout: {
        switch (event) {
        case kZGJoinLiveLoginEventLogin:
            status = kZGJoinLiveLoginStatusStartLogin;
            break;
        case kZGJoinLiveLoginEventLogout:
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    case kZGJoinLiveLoginStatusStartLogin: {
        switch (event) {
        case kZGJoinLiveLoginEventLoginSuccess:
            status = kZGJoinLiveLoginStatusLogin;
            break;
        case kZGJoinLiveLoginEventLogout:
        case kZGJoinLiveLoginEventLoginFailed:
            status = kZGJoinLiveLoginStatusLogout;
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    case kZGJoinLiveLoginStatusLogin: {
        switch (event) {
        case kZGJoinLiveLoginEventTempBroke:
            status = kZGJoinLiveLoginStatusTempBroken;
            break;
        case kZGJoinLiveLoginEventLogout:
        case kZGJoinLiveLoginEventKickOut:
        case kZGJoinLiveLoginEventDisconnect:
            status = kZGJoinLiveLoginStatusLogout;
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    case kZGJoinLiveLoginStatusTempBroken: {
        switch (event) {
        case kZGJoinLiveLoginEventTempBroke:
            break;
        case kZGJoinLiveLoginEventReconnect:
            status = kZGJoinLiveLoginStatusLogin;
            break;
        case kZGJoinLiveLoginEventLogout:
        case kZGJoinLiveLoginEventKickOut:
        case kZGJoinLiveLoginEventDisconnect:
            status = kZGJoinLiveLoginStatusLogout;
            break;
        default:
            PrintLogToView("ZegoRoomManager error event with current status");
            break;
        }
        break;
    }
    }

    login_status_ = status;

    if (join_live_cb_ != nullptr)
    {
        join_live_cb_->OnLoginEvent(event, login_status_);
    }
}

bool JoinLiveDemo::RetryLoginRoom()
{
    if (start_retry_login_room_timestamp_ == -1)
    {
        start_retry_login_room_timestamp_ = ZGHelperInstance()->GetCurTimeStampMs();
    }
    else if (retry_login_room_timeout_ms_ != 0)
    {
        if (ZGHelperInstance()->GetCurTimeStampMs() - start_retry_login_room_timestamp_ > retry_login_room_timeout_ms_)
        {
            PrintLogToView("������ʱ,��������");
            StopRetryRoomReconnect(kZGJoinLiveReconnectStopReasonTimeout);
            return false;
        }
    }
    PrintLogToView("ҵ���������...");

    PrintLogToView("room id = %s", room_id_.c_str());
    UpdateLoginEvent(kZGJoinLiveLoginEventLogin);
    BOOL ret = LIVEROOM::LoginRoom(room_id_.c_str(), COMMON::Audience);
    if (!ret)
    {
        UpdateLoginEvent(kZGJoinLiveLoginEventLoginFailed);
    }
    return ret;
}

void JoinLiveDemo::StopRetryRoomReconnect(ZGJoinLiveReconnectStopReason reason)
{
    if (auto_retry_login_room_)
    {
//         if (on_stop_room_reconnect_ != nullptr)
//         {
//             on_stop_room_reconnect_(reason);
//         }
    }
    auto_retry_login_room_ = false;
}

void JoinLiveDemo::ProcessStreamAdded(string stream_id, string user_id, string user_name, string room_id)
{
    PlayConfig config;
    ZegoJoinLiveUser for_user;

    for_user.user_id = user_id;
    for_user.user_name = user_name;

    if (GetPlayConfigByUser(for_user, config))
    {
        config.stream_id = stream_id;

        UpdatePlayConfigByUser(for_user, config);

        // �Ѿ�Ϊ���û�������ͼʱֱ�������û�����
        if (config.view_hwnd != nullptr)
        {
            // ���߳�Ȼ���ڿ�ʼ����
            PostUIData * pdata = CreateUIData();
            pdata->cb_in_ui_fun = [pdata, config, this]()->void
            {
                LIVEROOM::StartPlayingStream(config.stream_id.c_str(), config.view_hwnd);
                LIVEROOM::SetViewMode(config.view_mode, config.stream_id.c_str());

                DestroyUIData(pdata);
            };

            GlobalPostMsgDataToUI(main_hwnd_, pdata);

        }
    }
    else {
        PlayConfig play_config;
        ZegoJoinLiveUser user;

        user.user_id = user_id;
        user.user_name = user_name;
        play_config.stream_id = stream_id;

        std::lock_guard<std::mutex> lock(play_config_map_mutex_);
        play_map_info_.insert(std::make_pair(user, play_config));
    }

    if (join_live_cb_ != nullptr)
    {
        join_live_cb_->OnUserJoin(for_user);
    }
}

void JoinLiveDemo::ProcessStreamDeleted(string stream_id, string user_id, string user_name, string room_id)
{
    RemovePlayConfigByStreamId(stream_id);

    ZegoJoinLiveUser for_user;
    for_user.user_id = user_id;
    for_user.user_name = user_name;

    if (join_live_cb_ != nullptr)
    {
        join_live_cb_->OnUserLeave(for_user);
    }
}

void JoinLiveDemo::UpdatePlayConfigByUser(ZegoJoinLiveUser user, const PlayConfig& config)
{
    std::lock_guard<std::mutex> lock(play_config_map_mutex_);

    std::map<ZegoJoinLiveUser, PlayConfig>::iterator find_iter = std::find_if(play_map_info_.begin(), play_map_info_.end(), [&](const std::pair<ZegoJoinLiveUser, PlayConfig> &item) {
        if (item.first.user_id == user.user_id && item.first.user_name == user.user_name)
        {
            return true;
        }
        else {
            return false;
        }
    });
    if (find_iter != play_map_info_.end())
    {
        find_iter->second = config;
    }
}

void JoinLiveDemo::RemovePlayConfigByStreamId(string stream_id)
{
    std::lock_guard<std::mutex> lock(play_config_map_mutex_);

    std::map<ZegoJoinLiveUser, PlayConfig>::iterator find_iter = std::find_if(play_map_info_.begin(), play_map_info_.end(), [&](const std::pair<ZegoJoinLiveUser, PlayConfig> &item) {
        if (item.second.stream_id == stream_id)
        {
            return true;
        }
        else {
            return false;
        }
    });
    if (find_iter != play_map_info_.end())
    {
        PlayConfig need_stop_play_config;
        need_stop_play_config = find_iter->second;

        // ���߳�Ȼ����ֹͣ����
        PostUIData * pdata = CreateUIData();
        pdata->cb_in_ui_fun = [pdata, need_stop_play_config, this]()->void
        {
            PrintLogToView("ֹͣ����, stream id = %s", need_stop_play_config.stream_id.c_str());
            
            LIVEROOM::StopPlayingStream(need_stop_play_config.stream_id.c_str());

            DestroyUIData(pdata);
        };

        GlobalPostMsgDataToUI(main_hwnd_, pdata);

        play_map_info_.erase(find_iter);
    }
}


bool JoinLiveDemo::GetPlayConfigByUser(ZegoJoinLiveUser user, PlayConfig& config)
{
    std::lock_guard<std::mutex> lock(play_config_map_mutex_);

    std::map<ZegoJoinLiveUser, PlayConfig>::iterator find_iter = std::find_if(play_map_info_.begin(), play_map_info_.end(), [&](const std::pair<ZegoJoinLiveUser, PlayConfig> &item) {
        if (item.first.user_id == user.user_id && item.first.user_name == user.user_name)
        {
            return true;
        }
        else {
            return false;
        }
    });
    if (find_iter != play_map_info_.end())
    {
        config = find_iter->second;
        return true;
    }
    else {
        return false;
    }
}