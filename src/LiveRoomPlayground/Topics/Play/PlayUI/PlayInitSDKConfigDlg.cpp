// PlayInitSDKConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "PlayInitSDKConfigDlg.h"
#include "afxdialogex.h"

#include "AppSupport/ZGUtilTools.h"
#include "AppSupport/ZGHelper.h"

#include "Play/PlayUI/resource.h"

// PlayInitSDKConfigDlg 对话框

IMPLEMENT_DYNAMIC(PlayInitSDKConfigDlg, CDialogEx)

PlayInitSDKConfigDlg::PlayInitSDKConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLAY_INITSDK_CONFIG_DLG, pParent)
{
    user_id_ = CString(CStringA(ZGHelper::Instance()->GetDeviceUUID().c_str()));
    user_id_ += "1";

    user_name_ = CString(CStringA(ZGHelper::Instance()->GetMachineName().c_str()));
    user_name_ += "1";

    appid_ = 0;
    app_sign_ = "";
}

PlayInitSDKConfigDlg::~PlayInitSDKConfigDlg()
{
}

void PlayInitSDKConfigDlg::SetEventCallBack(PlayInitSDKConfigDlgEvent * cb)
{
    dlg_event_cb_ = cb;
}

PlayInitSDKConfig PlayInitSDKConfigDlg::GetConfig()
{
    return init_sdk_config_;
}

BOOL PlayInitSDKConfigDlg::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    EnableToolTips(TRUE);

    LoadPngToControl((CStatic*)GetDlgItem(IDC_STATIC_PLAY_USER_ID_MARK), IDB_PNG_WH, 0, 2, 20, 20);
    LoadPngToControl((CStatic*)GetDlgItem(IDC_STATIC_PLAY_APP_ID_MARK), IDB_PNG_WH, 0, 2, 20, 20);
    LoadPngToControl((CStatic*)GetDlgItem(IDC_STATIC_PLAY_APP_SIGN_MARK), IDB_PNG_WH, 0, 2, 20, 20);

    ((CButton*)GetDlgItem(IDC_RADIO_PLAY_TOPIC_TESTENV))->SetCheck(TRUE);

    return TRUE;
}

void PlayInitSDKConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);

    DDX_Text(pDX, IDC_EDIT_PLAY_TOPIC_APPID, appid_);
    DDX_Text(pDX, IDC_EDIT_PLAY_TOPIC_APPSIGN, app_sign_);
    DDX_Text(pDX, IDC_EDIT_PLAY_TOPIC_USERID, user_id_);

}


BOOL PlayInitSDKConfigDlg::SetTipText(UINT id, NMHDR *pTTTStruct, LRESULT *pResult)
{
    TOOLTIPTEXT *pTTT = (TOOLTIPTEXT *)pTTTStruct;
    UINT nID = pTTTStruct->idFrom;   //得到相应窗口ID，有可能是HWND   

    //表明nID是否为HWND
    if (pTTT->uFlags & TTF_IDISHWND)
    {
        //从HWND得到ID值，当然你也可以通过HWND值来判断
        nID = ::GetDlgCtrlID((HWND)nID);
        if (NULL == nID)
            return FALSE;

        switch (nID)
        {
        case(IDC_STATIC_PLAY_USER_ID_MARK):
            pTTT->lpszText = _T("!!!必须保证userID的唯一性。可与app业务后台账号进行关联，userID还能便于zego技术支持帮忙查找定位线上问题，建议定义一个有意义的userID");
            break;
        case(IDC_STATIC_PLAY_APP_ID_MARK):
            pTTT->lpszText = _T("AppID 和 AppSign 由 Zego 分配给各 App。为了安全考虑，建议将 AppSign 存储在 App 的业务后台，使用时从后台获取。");
            break;
        case(IDC_STATIC_PLAY_APP_SIGN_MARK):
            pTTT->lpszText = _T("AppID 和 AppSign 由 Zego 分配给各 App。为了安全考虑，建议将 AppSign 存储在 App 的业务后台，使用时从后台获取。");
            break;
        default:
            break;
        }

        return TRUE;
    }
    return FALSE;
}

bool PlayInitSDKConfigDlg::CheckParams()
{
    if (user_id_ == _T(""))
    {
        MessageBox(_T("请输入userID"));
        return false;
    }

    if (appid_ == 0)
    {
        MessageBox(_T("请输入AppID"));
        return false;
    }

    if (app_sign_ == _T(""))
    {
        MessageBox(_T("请输入AppSign"));
        return false;
    }


    return true;
}

void PlayInitSDKConfigDlg::OnOK()
{

}

void PlayInitSDKConfigDlg::OnCancel()
{

}

BEGIN_MESSAGE_MAP(PlayInitSDKConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_TOPIC_INITSDK, &PlayInitSDKConfigDlg::OnBnClickedButtonPlayTopicInitsdk)
    ON_NOTIFY_EX(TTN_NEEDTEXT, 0, SetTipText)
END_MESSAGE_MAP()


// PlayInitSDKConfigDlg 消息处理程序


void PlayInitSDKConfigDlg::OnBnClickedButtonPlayTopicInitsdk()
{
    if (!UpdateData(true))
    {
        return;
    }
    if (!CheckParams())
    {
        return;
    }

    init_sdk_config_.is_test_env = ((CButton*)GetDlgItem(IDC_RADIO_PLAY_TOPIC_TESTENV))->GetCheck();

    init_sdk_config_.app_id = appid_;
    init_sdk_config_.app_sign = AppSignStrToVecData(app_sign_);
    init_sdk_config_.user_id = CStringA(user_id_).GetBuffer();
    init_sdk_config_.user_name = CStringA(user_name_).GetBuffer();

    if (dlg_event_cb_ != nullptr)
    {
        dlg_event_cb_->OnInitSDKButtonEvent();
    }

}
