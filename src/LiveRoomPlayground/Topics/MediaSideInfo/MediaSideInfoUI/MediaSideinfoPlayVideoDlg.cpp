// CMediaSideinfoPlayVideoDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ZegoLiveRoomTopics.h"
#include "MediaSideinfoPlayVideoDlg.h"
#include "afxdialogex.h"

#include "AppSupport/ZGUtilTools.h"
#include "MediaSideInfo/MediaSideInfoUI/resource.h"

CMediaSideinfoPlayVideoDlg * CMediaSideinfoPlayVideoDlg::CreateDlgInstance(CWnd * pParent)
{
    CMediaSideinfoPlayVideoDlg * p = new CMediaSideinfoPlayVideoDlg(pParent);
    p->Create(IDD_MEDIASIDEINFO_PLAY, pParent);
    return p;
}

// CMediaSideinfoPlayVideoDlg 对话框

IMPLEMENT_DYNAMIC(CMediaSideinfoPlayVideoDlg, CDialogEx)

CMediaSideinfoPlayVideoDlg::CMediaSideinfoPlayVideoDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEDIASIDEINFO_PLAY, pParent)
{

}

CMediaSideinfoPlayVideoDlg::~CMediaSideinfoPlayVideoDlg()
{
}

BOOL CMediaSideinfoPlayVideoDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    GetDlgItem(IDC_APPID)->SetWindowText(_T(""));
    ((CButton*)GetDlgItem(IDC_RADIO_STREAM_ID))->SetCheck(TRUE);
    ((CButton*)GetDlgItem(IDC_CHECK_TEST_ENV))->SetCheck(TRUE);
    is_url_ = false;

    return TRUE;
}

void CMediaSideinfoPlayVideoDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_APPID, appid_);
    DDX_Text(pDX, IDC_SIGN, sign_);
    DDX_Text(pDX, IDC_ROOMID, room_id_);
    DDX_Text(pDX, IDC_ROOMNAME, room_name_);
    DDX_Text(pDX, IDC_STREAMID_OR_URL, stream_id_);
    DDX_Text(pDX, IDC_PLAYPARAMS, play_params_);
    DDX_Control(pDX, IDC_SIDEINFO_LIST, recv_list_);
}


BEGIN_MESSAGE_MAP(CMediaSideinfoPlayVideoDlg, CDialogEx)
    ON_MESSAGE(UI_CALLBACK_MSG, &CMediaSideinfoPlayVideoDlg::OnUICallbackMsg)
    ON_BN_CLICKED(IDC_PLAY, &CMediaSideinfoPlayVideoDlg::OnBnClickedPlay)
    ON_BN_CLICKED(IDC_STOP, &CMediaSideinfoPlayVideoDlg::OnBnClickedStop)
    ON_BN_CLICKED(IDC_CLEAR, &CMediaSideinfoPlayVideoDlg::OnBnClickedClear)
    ON_BN_CLICKED(IDC_RADIO_STREAM_ID, &CMediaSideinfoPlayVideoDlg::OnBnClickedRadioStreamId)
    ON_BN_CLICKED(IDC_RADIO_URL, &CMediaSideinfoPlayVideoDlg::OnBnClickedRadioUrl)
END_MESSAGE_MAP()


// CMediaSideinfoPlayVideoDlg 消息处理程序


bool CMediaSideinfoPlayVideoDlg::CheckParams()
{
    if (appid_ == 0)
    {
        MessageBox(_T("请输入AppID"));
        return false;
    }

    if (sign_ == _T(""))
    {
        MessageBox(_T("请输入Sign"));
        return false;
    }

    if (room_id_ == _T(""))
    {
        MessageBox(_T("请输入RoomID"));
        return false;
    }

    if (stream_id_ == _T(""))
    {
        MessageBox(_T("请输入StreamId"));
        return false;
    }


    return true;
}

void CMediaSideinfoPlayVideoDlg::OnBnClickedPlay()
{
    UpdateData(TRUE);
    if (!CheckParams())
    {
        return;
    }

    HelperConfig config;
    config.app_id = appid_;
    // sign_ 格式为 0x01,0x02,0x03,0x05... 的字符串
    config.sign = AppSignStrToVecData(sign_);

    config.room_id = CStringA(room_id_).GetBuffer();
    config.room_name = CStringA(room_name_).GetBuffer();
    config.stream_id = CStringA(stream_id_).GetBuffer();
    config.play_params = CStringA(play_params_).GetBuffer();
    config.video_view = GetDlgItem(IDC_PLAY_VIDEO_PANNEL)->GetSafeHwnd();

    // 设置接收媒体次要信息的接口
    demo_.SetMediaSideInfoCallBack(std::bind(&CMediaSideinfoPlayVideoDlg::OnRecvMediaSideInfo, this, std::placeholders::_1, std::placeholders::_2, std::placeholders::_3));    
    // 设置状态回调接口
    demo_helper_.status_cb_ = std::bind(&CMediaSideinfoPlayVideoDlg::OnStatusUpdate, this, std::placeholders::_1);
    // 设置初始化sdk、登陆房间、拉流配置
    demo_helper_.InitSetLoginAndPlayConfig(config);
    // 登陆房间
    demo_helper_.LoginByConfig(((CButton*)GetDlgItem(IDC_CHECK_TEST_ENV))->GetCheck());

    std::vector<unsigned int> control_id_list;
    control_id_list.push_back(IDC_APPID);
    control_id_list.push_back(IDC_SIGN);
    control_id_list.push_back(IDC_ROOMID);
    control_id_list.push_back(IDC_ROOMNAME);
    control_id_list.push_back(IDC_STREAMID);
    control_id_list.push_back(IDC_PLAYPARAMS);
    control_id_list.push_back(IDC_PLAY);

    for (auto control_id : control_id_list)
    {
        GetDlgItem(control_id)->EnableWindow(FALSE);
    }
}


void CMediaSideinfoPlayVideoDlg::OnBnClickedStop()
{
    demo_helper_.StopPlay();

    std::vector<unsigned int> control_id_list;
    control_id_list.push_back(IDC_APPID);
    control_id_list.push_back(IDC_SIGN);
    control_id_list.push_back(IDC_ROOMID);
    control_id_list.push_back(IDC_ROOMNAME);
    control_id_list.push_back(IDC_STREAMID);
    control_id_list.push_back(IDC_PLAYPARAMS);
    control_id_list.push_back(IDC_PLAY);

    for (auto control_id : control_id_list)
    {
        GetDlgItem(control_id)->EnableWindow(TRUE);
    }
}

// 状态处理函数
void CMediaSideinfoPlayVideoDlg::OnStatusUpdate(MediaSideInfoStatus status)
{
    typedef struct UIDataStruct
    {
        MediaSideInfoStatus s;
    } UIDataStruct;

    UIDataStruct ui_data;
    ui_data.s = status;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {

        string status_str = demo_helper_.DescOfStatus(ui_data.s);
        string login_status_str = CStringA(status_str.c_str());

        if (login_status_str == "LoginOk")
        {
            // 要在初始化sdk后，拉流前调用
            // 开启接收媒体次要信息
            demo_.InitSideInfoForPlay();
            // 拉流播放
            demo_helper_.PlayStreamByConfig(!is_url_);
        }

        GetDlgItem(IDC_SIDEINFO_PLAY_STATUS)->SetWindowText(CString(CStringA(login_status_str.c_str())));


        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

// 接收媒体次要信息处理函数
void CMediaSideinfoPlayVideoDlg::OnRecvMediaSideInfo(uint8_t * data, int len, string stream_id)
{
    typedef struct UIDataStruct
    {
        string str;
        string stream_id;

    } UIDataStruct;

    UIDataStruct ui_data;
    ui_data.str = string((char *)data, len);
    ui_data.stream_id = stream_id;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {
        string recv_data = ui_data.str;
        demo_helper_.AddRecvMsg(recv_data);

        recv_list_.AddString(CString(CStringA(recv_data.c_str())));

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

void CMediaSideinfoPlayVideoDlg::OnBnClickedClear()
{
    recv_list_.ResetContent();
}


void CMediaSideinfoPlayVideoDlg::OnBnClickedRadioStreamId()
{
    is_url_ = false;
}


void CMediaSideinfoPlayVideoDlg::OnBnClickedRadioUrl()
{
    is_url_ = true;
}

afx_msg LRESULT CMediaSideinfoPlayVideoDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}




