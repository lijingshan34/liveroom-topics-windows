// PublishConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "ZegoLiveRoomTopics.h"
#include "PublishConfigDlg.h"
#include "afxdialogex.h"

#include "Publish/PublishUI/Resource.h"


// PublishConfigDlg 对话框

IMPLEMENT_DYNAMIC(PublishConfigDlg, CDialogEx)

PublishConfigDlg::PublishConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PUBLISH_CONFIG_DLG, pParent)
{

}

PublishConfigDlg::~PublishConfigDlg()
{
}

void PublishConfigDlg::SetEventCallBack(PublishConfigDlgEvent * cb)
{
    event_cb_ = cb;
}

PublishConfig PublishConfigDlg::GetConfig()
{
    return publish_config_;
}

void PublishConfigDlg::InitDlgData()
{
    show_base_fun_ = false;
    ((CButton*)GetDlgItem(IDC_BUTTON_PUBLISH_TOPIC_BASE_FUN))->SetWindowText(_T("常用功能>>"));
}

void PublishConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PUBLISH_TOPIC_STREAM_ID, stream_id_);
}


void PublishConfigDlg::OnOK()
{
}

void PublishConfigDlg::OnCancel()
{
}

BEGIN_MESSAGE_MAP(PublishConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_PUBLISH_TOPIC_START_PUBLISH, &PublishConfigDlg::OnBnClickedButtonPublishTopicStartPublish)
    ON_BN_CLICKED(IDC_BUTTON_PUBLISH_TOPIC_BASE_FUN, &PublishConfigDlg::OnBnClickedButtonPublishTopicBaseFun)
END_MESSAGE_MAP()


// PublishConfigDlg 消息处理程序


void PublishConfigDlg::OnBnClickedButtonPublishTopicStartPublish()
{
    UpdateData(TRUE);
    
    if (stream_id_ == _T(""))
    {
        MessageBox(_T("请输入streamID"));
        return;
    }

    publish_config_.stream_id = CStringA(stream_id_).GetBuffer();
    publish_config_.publish_flag = ZEGO::AV::ZEGO_JOIN_PUBLISH;
    publish_config_.live_title = "";
    publish_config_.publish_param = "";

    if (event_cb_ != nullptr)
    {
        event_cb_->OnPublishButtonEvent();
    }
}

void PublishConfigDlg::OnBnClickedButtonPublishTopicBaseFun()
{
    if (!show_base_fun_)
    {
        ((CButton*)GetDlgItem(IDC_BUTTON_PUBLISH_TOPIC_BASE_FUN))->SetWindowText(_T("常用功能<<"));
        show_base_fun_ = true;
    }
    else {
        ((CButton*)GetDlgItem(IDC_BUTTON_PUBLISH_TOPIC_BASE_FUN))->SetWindowText(_T("常用功能>>"));
        show_base_fun_ = false;
    }

    if (event_cb_ != nullptr)
    {
        event_cb_->OnBaseFunctionButtonEvent(show_base_fun_);
    }

}
