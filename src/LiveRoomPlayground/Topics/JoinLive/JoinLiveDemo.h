#pragma once

#include <vector>
#include <string>
#include <functional>
#include <map>
#include <mutex>

#include "ZegoLiveRoom/LiveRoom.h"
#include "ZegoLiveRoom/LiveRoomCallback-Publisher.h"
#include "ZegoLiveRoom/LiveRoomCallBack.h"
#include "ZegoLiveRoom/LiveRoom-Player.h"
#include "ZegoLiveRoom/LiveRoomCallback-Player.h"

#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGUIOperator.h"

#include <windows.h>

using std::string;
using std::map;

using namespace ZEGO;
using ZEGO::COMMON::ZegoPublishingStreamInfo;
using LIVEROOM::ZegoPublishQuality;
using COMMON::ZegoStreamUpdateType;
using LIVEROOM::ZegoPlayQuality;

typedef struct ZegoJoinLiveUser
{
    string user_id;
    string user_name;
    bool operator < (ZegoJoinLiveUser const& z) const
    {
        if (user_id < z.user_id) {
            return true;
        }
        return false;
    }
    ZegoJoinLiveUser()
    {
        user_id = "";
        user_name = "";
    }

}ZegoJoinLiveUser;

enum ZGJoinLiveRoomStatus
{
    kZGJoinLiveStatus_None = 0,             // δ��ʼ��ʼ״̬
    kZGJoinLiveStatus_Starting_Init_SDK,    // ��ʼ��ʼ��sdk
    kZGJoinLiveStatus_Init_SDK_OK,          // sdk��ʼ���ɹ�
    kZGJoinLiveStatus_Starting_Login_Room,  // ��ʼ��½����
    kZGJoinLiveStatus_Login_OK,             // ��½����ɹ�
    kZGJoinLiveStatus_Starting_Publishing,  // ��ʼ���� 
    kZGJoinLiveStatus_Publishing,           // �����ɹ�
    kZGJoinLiveStatus_Stoped_Publishing     // ֹͣ����
};

// �����¼�¼�
enum ZGJoinLiveLoginEvent {
    kZGJoinLiveLoginEventLogin,           // ��ʼ��¼����
    kZGJoinLiveLoginEventLoginSuccess,    // ��¼����ɹ�
    kZGJoinLiveLoginEventLoginFailed,     // ��¼����ʧ��
    kZGJoinLiveLoginEventLogout,          // �˳���¼
    kZGJoinLiveLoginEventTempBroke,       // ����������ʱ�Ͽ�
    kZGJoinLiveLoginEventReconnect,       // �������ӻָ�
    kZGJoinLiveLoginEventDisconnect,      // ������ʱ��ֹͣ�����Ͽ���������
    kZGJoinLiveLoginEventKickOut,         // ���߳����䣬�Ͽ���������
};

// �����¼״̬
enum ZGJoinLiveStatus
{
    kZGJoinLiveLoginStatusLogout,     // δ��¼
    kZGJoinLiveLoginStatusStartLogin, // ��ʼ��¼����
    kZGJoinLiveLoginStatusLogin,      // �ѵ�¼
    kZGJoinLiveLoginStatusTempBroken, // ��ʱ�Ͽ���������
};

// ����������ֹԭ��
enum ZGJoinLiveReconnectStopReason {
    kZGJoinLiveReconnectStopReasonLogout, // �ֶ�����leaveRoom��ֹ����
    kZGJoinLiveReconnectStopReasonKickout,// ���������߳���ֹ����
    kZGJoinLiveReconnectStopReasonTimeout,// ����timeout
};

class JoinLiveCallBack
{
public:
    virtual void OnLoginEvent(ZGJoinLiveLoginEvent event, ZGJoinLiveStatus status) = 0;
    virtual void OnUserJoin(ZegoJoinLiveUser user) {}
    virtual void OnUserLeave(ZegoJoinLiveUser user) {}
};


class JoinLiveDemo :
    public LIVEROOM::ILivePublisherCallback,
    public LIVEROOM::ILivePlayerCallback,
    public LIVEROOM::IRoomCallback,
    public AV::IZegoDeviceStateCallback
{
public:

    JoinLiveDemo();
    ~JoinLiveDemo();

    void SetDemoCallBack(JoinLiveCallBack * cb);

    void SetLocalVideoView(HWND view, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void SetRemoteVideoView(HWND view, ZegoJoinLiveUser for_user, LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit);

    void LoginRoom(string room_id, bool need_publish_after_logined = false);

    void StartPublish();

    void LogoutRoom();

    // ���������ڵľ����Ϊ�����߳�ʹ��
    void InitMainHwnd(HWND hwnd);

    void SetLogCallBack(ILogToView *log_cb);

protected:
    
    void PrintLogToView(const char * format, ...);

    virtual void OnPlayStateUpdate(int stateCode, const char* pszStreamID) override;


    virtual void OnPlayQualityUpdate(const char* pszStreamID, ZegoPlayQuality playQuality) override;


    virtual void OnVideoSizeChanged(const char* pStreamID, int nWidth, int nHeight) override;


    virtual void OnAudioDeviceStateChanged(AV::AudioDeviceType deviceType, AV::DeviceInfo *deviceInfo, AV::DeviceState state) override;


    virtual void OnDeviceError(const char* deviceName, int errorCode) override;


    virtual void OnInitSDK(int nError) override;


    virtual void OnLoginRoom(int errorCode, const char *pszRoomID, const COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount) override;


    virtual void OnLogoutRoom(int errorCode, const char *pszRoomID) override;


    virtual void OnKickOut(int reason, const char *pszRoomID) override;


    virtual void OnDisconnect(int errorCode, const char *pszRoomID) override;


    virtual void OnReconnect(int errorCode, const char *pszRoomID) override;


    virtual void OnTempBroken(int errorCode, const char *pszRoomID) override;


    virtual void OnStreamUpdated(ZegoStreamUpdateType type, COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;


    virtual void OnStreamExtraInfoUpdated(COMMON::ZegoStreamInfo *pStreamInfo, unsigned int streamCount, const char *pszRoomID) override;


    virtual void OnCustomCommand(int errorCode, int requestSeq, const char *pszRoomID) override;


    virtual void OnRecvCustomCommand(const char *pszUserId, const char *pszUserName, const char *pszContent, const char *pszRoomID) override;


    virtual void OnPublishStateUpdate(int stateCode, const char* pszStreamID, const ZegoPublishingStreamInfo& oStreamInfo) override;


    virtual void OnPublishQualityUpdate(const char* pszStreamID, ZegoPublishQuality publishQuality) override;


    virtual void OnCaptureVideoSizeChanged(int nWidth, int nHeight) override;


    virtual void OnCaptureVideoFirstFrame() override;


protected:

    HWND main_hwnd_ = nullptr;
    HWND local_video_view_ = nullptr;
    LIVEROOM::ZegoVideoViewMode local_view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit;

    string room_id_ = "";

    bool have_published_ = false;

    // ��������
    typedef struct PlayConfig
    {
        HWND view_hwnd = nullptr;
        std::string stream_id;
        std::string play_param;

        LIVEROOM::ZegoVideoViewMode view_mode = LIVEROOM::ZegoVideoViewModeScaleAspectFit;

    }PlayConfig;

    // �����������б�
    std::map<ZegoJoinLiveUser, PlayConfig> play_map_info_;

    std::mutex play_config_map_mutex_;

    ZGJoinLiveRoomStatus cur_status_ = kZGJoinLiveStatus_None;

    ZGJoinLiveStatus login_status_ = kZGJoinLiveLoginStatusLogout;

    // ��¼��ʼ���������ʱ���
    int64_t start_retry_login_room_timestamp_ = -1;

    // �Զ����Ե�½
    bool auto_retry_login_room_ = true;

    // ����������ʱ���ã���λ���룬0Ϊ����ʱ
    int retry_login_room_timeout_ms_ = 1200000;

    // ʧ������ʱ��������λ����
    int retry_interval_ms_ = 1000;

    void ProcessStreamAdded(string stream_id, string user_id, string user_name, string room_id);
    void ProcessStreamDeleted(string stream_id, string user_id, string user_name, string room_id);

    void UpdatePlayConfigByUser(ZegoJoinLiveUser user, const PlayConfig& config);
    void RemovePlayConfigByStreamId(string stream_id);
    bool GetPlayConfigByUser(ZegoJoinLiveUser user, PlayConfig& config);

    void UpdateLoginEvent(ZGJoinLiveLoginEvent event);
    bool RetryLoginRoom();
    void StopRetryRoomReconnect(ZGJoinLiveReconnectStopReason reason);

    JoinLiveCallBack * join_live_cb_ = nullptr;

    ILogToView * view_log_ = nullptr;

    bool logined_need_publish_ = false;
};

