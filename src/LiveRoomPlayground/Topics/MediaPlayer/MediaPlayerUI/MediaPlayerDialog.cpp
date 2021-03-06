// MediaPlayerDialog.cpp: 实现文件
//

#include "stdafx.h"
#include "ZegoLiveRoomTopics.h"
#include "MediaPlayerDialog.h"
#include "afxdialogex.h"

#include "AppSupport/ZGUtilTools.h"
#include "AppSupport/ZGLog.h"
#include "AppSupport/ZGManager.h"
#include "AppSupport/ZGHelper.h"

#include "Topics/MediaPlayer/ZGMediaPlayerDemoHelper.h"
#include "Topics/MediaPlayer/MediaPlayerUI/resource.h"


// MediaPlayerDialog 对话框

IMPLEMENT_DYNAMIC(MediaPlayerDialog, CDialogEx)

MediaPlayerDialog::MediaPlayerDialog(CWnd* pParent /*=nullptr*/)
    : CDialogEx(IDD_MEDIAPLAYER_DIALOG, pParent),
      cur_media_max_duration_(0)
{
}

MediaPlayerDialog::~MediaPlayerDialog()
{
}

MediaPlayerDialog * MediaPlayerDialog::CreateDlgInstance(CWnd * pParent)
{
    MediaPlayerDialog * p = new MediaPlayerDialog(pParent);
    p->Create(IDD_MEDIAPLAYER_DIALOG, pParent);
    return p;
}

void MediaPlayerDialog::StartMediaPlayer()
{
    media_player_.InitMediaPlayer();
    media_player_.InitPlayerStateCallBack(this);
}

BEGIN_MESSAGE_MAP(MediaPlayerDialog, CDialogEx)
    ON_MESSAGE(UI_CALLBACK_MSG, &MediaPlayerDialog::OnUICallbackMsg)
    ON_WM_HSCROLL()
    ON_BN_CLICKED(IDC_BUTTON_PLAY, &MediaPlayerDialog::OnBnClickedButtonPlay)
    ON_BN_CLICKED(IDC_BUTTON_STOP, &MediaPlayerDialog::OnBnClickedButtonStop)
    ON_BN_CLICKED(IDC_BUTTON_PAUSE, &MediaPlayerDialog::OnBnClickedButtonPause)
    ON_BN_CLICKED(IDC_BUTTON_Resume, &MediaPlayerDialog::OnBnClickedButtonResume)
    ON_CBN_SELCHANGE(IDC_COMBO_PLAY_TYPE, &MediaPlayerDialog::OnSelPlayAudioTypeChange)
    ON_BN_CLICKED(IDC_CHECK_MIC, &MediaPlayerDialog::OnBnClickedCheckMic)
END_MESSAGE_MAP()



void MediaPlayerDialog::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);

    DDX_Control(pDX, IDC_SLIDER_PLAY_PROGRESS, play_progress_slider_control_);
    DDX_Control(pDX, IDC_SLIDER_VOLUME, volume_slider_control_);
}

void MediaPlayerDialog::PostNcDestroy()
{
    CDialogEx::PostNcDestroy();
    delete this;
}

void MediaPlayerDialog::OnCancel()
{
    CDialogEx::DestroyWindow();
}

BOOL MediaPlayerDialog::OnInitDialog()
{
    CDialogEx::OnInitDialog();

    InitControls();

    return TRUE;
}

void MediaPlayerDialog::OnPlayerState(string play_state)
{
    UpdatePlayStatus(play_state);
}

void MediaPlayerDialog::OnPlayerProgress(long current, long max, string desc)
{
    cur_media_max_duration_ = max;
    UpdatePlayProgressSliderStatus(current*1.0f/max);
    UpdatePlayerProgress(desc);
}

void MediaPlayerDialog::OnPlayerStop()
{

}

void MediaPlayerDialog::OnPublishState(string pub_state)
{
    typedef struct UIDataStruct
    {
        string status;
    } UIDataStruct;
    
    UIDataStruct ui_data;
    ui_data.status = pub_state;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {

        CString str(CStringA(ui_data.status.c_str()));
        viewer_url_edit_contronl_->SetWindowTextW(str);

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

void MediaPlayerDialog::OnPlayerError(int ec)
{

}

void MediaPlayerDialog::OnBnClickedButtonPlay()
{
    int cur_sel = media_file_list_control_->GetCurSel();
    if (cur_sel < 0)
    {
        MessageBox(_T("请选择要播放的文件"));
        return;
    }
    MediaFileInfo *pData = (MediaFileInfo*)media_file_list_control_->GetItemData(cur_sel);

    cur_media_file_url_ = pData->url;

    media_player_.SetVideoView(GetDlgItem(IDC_VIDEO_CONTROL)->GetSafeHwnd());

    bool bRepeat = repeat_checkbox_button_->GetCheck();
    bool bMic = mic_checkbox_button_->GetCheck();
    ZGLog("play , %s ,repeat = %d, bMic = %d", cur_media_file_url_.c_str(), bRepeat, bMic);
    ZGManagerInstance()->EnableMic(bMic);
    media_player_.StartPlaying(cur_media_file_url_.c_str(), bRepeat);

}

void MediaPlayerDialog::OnBnClickedCheckMic()
{
    bool bMic = mic_checkbox_button_->GetCheck();
    ZGManagerInstance()->EnableMic(bMic);
}


void MediaPlayerDialog::OnBnClickedButtonStop()
{
    media_player_.Stop();
}

void MediaPlayerDialog::OnBnClickedButtonPause()
{
    media_player_.Pause();
}

void MediaPlayerDialog::OnBnClickedButtonResume()
{
    media_player_.Resume();
}

void MediaPlayerDialog::UpdatePlayStatus(string str_status)
{
    typedef struct UIDataStruct
    {
        string status;
    } UIDataStruct;
    
    UIDataStruct ui_data;
    ui_data.status = str_status;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata,this]()->void
    {
        play_status_control_->SetWindowText(CString(CStringA(ui_data.status.c_str())));
        if (ui_data.status == "Playing: Begin")
        {
            play_audio_type_combobox_contronl_->ResetContent();
            int audio_stream_count = media_player_.GetAudioStreamCount();
            ZGLog("as count %d", audio_stream_count);
            for (int i = 0; i < audio_stream_count; ++i)
            {
                CString str;
                str.Format(_T("%d"), i);
                play_audio_type_combobox_contronl_->AddString(str);
            }
        }

        DestroyUIData(pdata);
    };


    PostMsgDataToUI(pdata);
}

void MediaPlayerDialog::UpdatePlayerProgress(string str_progress)
{
    typedef struct UIDataStruct
    {
        string str;
    } UIDataStruct;

    UIDataStruct ui_data;
    ui_data.str = str_progress;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {
        play_progress_status_control_->SetWindowText(CString(CStringA(ui_data.str.c_str())));

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

void MediaPlayerDialog::UpdatePlayProgressSliderStatus(float percent)
{
    typedef struct UIDataStruct
    {
        float percent;
    } UIDataStruct;
    
    UIDataStruct ui_data;
    ui_data.percent = percent;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {
        int pos = ui_data.percent * 100;
        if (pos >= 95)
        {
            play_progress_slider_control_.SetPos(pos);
        }
        play_progress_slider_control_.SetPos(pos);

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

void MediaPlayerDialog::InitControls()
{
    media_file_list_control_        = (CListBox*)GetDlgItem(IDC_LIST_MEDIAFILE);
    play_status_control_            = (CStatic *)GetDlgItem(IDC_PLAY_STATUS);
    play_progress_status_control_   = (CStatic *)GetDlgItem(IDC_PLAY_PROGRESS_INFO);
    repeat_checkbox_button_         = (CButton *)GetDlgItem(IDC_CHECK_REPEAT);
    mic_checkbox_button_            = (CButton *)GetDlgItem(IDC_CHECK_MIC);
    play_audio_type_combobox_contronl_ = (CComboBox*)GetDlgItem(IDC_COMBO_PLAY_TYPE);
    viewer_url_edit_contronl_          = (CEdit*)GetDlgItem(IDC_EDIT_VIEWER_URL);

    play_progress_slider_control_.SetRange(0, 100);
    play_progress_slider_control_.SetPos(0);
    play_progress_slider_control_.OnSliderPosChange(std::bind(&MediaPlayerDialog::ProcessPlaySliderPosChange, this, std::placeholders::_1));

    volume_slider_control_.SetRange(0, 100);
    volume_slider_control_.SetPos(50);
    volume_slider_control_.OnSliderPosChange(std::bind(&MediaPlayerDialog::ProcessVolumeSliderPosChange, this, std::placeholders::_1));

    ZGMediaPlayerDemoHelper helper;
    vector<MediaFileInfo> file_list = helper.GetMediaList();
    for (vector<MediaFileInfo>::iterator it = file_list.begin(); it != file_list.end(); ++it)
    {
        string str_title = helper.GetTitleForItem(*it);
        media_file_list_control_->AddString(CString(CStringA(str_title.c_str())));
        MediaFileInfo *pData = new MediaFileInfo();
        *pData = *it;
        media_file_list_control_->SetItemDataPtr(media_file_list_control_->GetCount() - 1, (void*)pData);
    }
    media_file_list_control_->SetCurSel(0);

    media_player_.InitMainHwnd(GetSafeHwnd());
}

void MediaPlayerDialog::ProcessPlaySliderPosChange(int nPos)
{
    //media_player_.SeekTo(nPos);
    long cur_set_duration = cur_media_max_duration_ * nPos / 100.0f;
    ZGLog("ProcessPlaySliderPosChange %d , set duration = %ld ", nPos, cur_set_duration);
    media_player_.SeekTo(cur_set_duration);
}

void MediaPlayerDialog::ProcessVolumeSliderPosChange(int nPos)
{
    ZGLog("set volume %d", nPos);
    media_player_.SetVolume(nPos);
}

void MediaPlayerDialog::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
    CDialogEx::OnHScroll(nSBCode, nPos, pScrollBar);

    if ((CSliderCtrl*)pScrollBar == &play_progress_slider_control_)
    {
        if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
        {
            //ZGLog("on hsroll %d , nSBCode = %d ", nPos, nSBCode);
            ProcessPlaySliderPosChange(nPos);
        }
    }
    else if ((CSliderCtrl*)pScrollBar == &volume_slider_control_)
    {
        if (nSBCode == SB_THUMBPOSITION || nSBCode == SB_THUMBTRACK)
        {
            //ZGLog("on hsroll %d , nSBCode = %d ", nPos, nSBCode);
            ProcessVolumeSliderPosChange(nPos);
        }
    }
}

void MediaPlayerDialog::OnSelPlayAudioTypeChange()
{
    int cur_sel_index = play_audio_type_combobox_contronl_->GetCurSel();
    ZGLog("select audio track index %d ", cur_sel_index);
    media_player_.SetAudioStream(cur_sel_index);
}

afx_msg LRESULT MediaPlayerDialog::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}

