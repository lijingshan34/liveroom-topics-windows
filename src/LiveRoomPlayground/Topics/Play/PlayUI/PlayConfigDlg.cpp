// PlayConfigDlg.cpp: 实现文件
//

#include "stdafx.h"
#include "PlayConfigDlg.h"
#include "afxdialogex.h"

#include "Play/PlayUI/resource.h"

// PlayConfigDlg 对话框

IMPLEMENT_DYNAMIC(PlayConfigDlg, CDialogEx)

PlayConfigDlg::PlayConfigDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_PLAY_CONFIG_DLG, pParent)
{
    stream_id_ = "";
}

PlayConfigDlg::~PlayConfigDlg()
{
}

void PlayConfigDlg::SetEventCallBack(PlayConfigDlgEvent * cb)
{
    event_cb_ = cb;
}

PlayConfig PlayConfigDlg::GetConfig()
{
    return play_config_;
}

void PlayConfigDlg::InitDlgData()
{
    show_base_fun_ = false;
    ((CButton*)GetDlgItem(IDC_BUTTON_PLAY_TOPIC_BASE_FUN))->SetWindowText(_T("常用功能>>"));
}

void PlayConfigDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_EDIT_PLAY_TOPIC_STREAM_ID, stream_id_);
}


BEGIN_MESSAGE_MAP(PlayConfigDlg, CDialogEx)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_TOPIC_START_PLAY, &PlayConfigDlg::OnBnClickedButtonPlayTopicStartPlay)
    ON_BN_CLICKED(IDC_BUTTON_PLAY_TOPIC_BASE_FUN, &PlayConfigDlg::OnBnClickedButtonPlayTopicBaseFun)
END_MESSAGE_MAP()


void PlayConfigDlg::OnBnClickedButtonPlayTopicStartPlay()
{
    UpdateData(TRUE);

    if (stream_id_ == _T(""))
    {
        MessageBox(_T("请输入streamID"));
        return;
    }

    play_config_.stream_id = CStringA(stream_id_).GetBuffer();
    play_config_.play_param = "";

    if (event_cb_ != nullptr)
    {
        event_cb_->OnPlayButtonEvent();
    }
}


void PlayConfigDlg::OnBnClickedButtonPlayTopicBaseFun()
{
    if (!show_base_fun_)
    {
        ((CButton*)GetDlgItem(IDC_BUTTON_PLAY_TOPIC_BASE_FUN))->SetWindowText(_T("常用功能<<"));
        show_base_fun_ = true;
    }
    else {
        ((CButton*)GetDlgItem(IDC_BUTTON_PLAY_TOPIC_BASE_FUN))->SetWindowText(_T("常用功能>>"));
        show_base_fun_ = false;
    }

    if (event_cb_ != nullptr)
    {
        event_cb_->OnBaseFunctionButtonEvent(show_base_fun_);
    }
}
