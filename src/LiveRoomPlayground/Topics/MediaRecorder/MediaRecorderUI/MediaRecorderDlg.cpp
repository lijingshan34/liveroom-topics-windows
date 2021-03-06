
// MediaRecorderDlg.cpp: 实现文件
//

#include "stdafx.h"

#include<vector>

#include "MediaRecorderDlg.h"
#include "afxdialogex.h"

#include "AppSupport/ZGUtilTools.h"
#include "Topics/MediaRecorder/MediaRecorderUI/resource.h"

using std::vector;

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMediaRecorderDlg 对话框

CMediaRecorderDlg * CMediaRecorderDlg::CreateDlgInstance(CWnd * pParent)
{
    CMediaRecorderDlg * p = new CMediaRecorderDlg(pParent);
    p->Create(IDD_MEDIARECORDER_DIALOG, pParent);
    return p;
}

CMediaRecorderDlg::CMediaRecorderDlg(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_MEDIARECORDER_DIALOG, pParent)
    , record_status_(_T(""))
    , is_audio_only_check_(FALSE)
    , is_video_only_check_(FALSE)
    , save_file_path_(_T(""))
    , record_file_info_(_T(""))
{

}

void CMediaRecorderDlg::LoginAndPreview()
{
    demo_.StopRecord();
    demo_.InitDemoCallBack(this);
    demo_helper_.SetPublishStateObserver(std::bind(&CMediaRecorderDlg::OnPublishStateUpdate, this, std::placeholders::_1));
    demo_helper_.LoginAndPreview(GetDlgItem(IDC_LOCAL_VIDEO)->GetSafeHwnd());

    vector<unsigned int> control_id_list;
    control_id_list.push_back(IDC_AUDIO_ONLY_CHECK);
    control_id_list.push_back(IDC_VIDEO_ONLY_CHECK);
    control_id_list.push_back(IDC_PUBLISHING_CHECK);
    control_id_list.push_back(IDC_BOTH_CHECK);
    control_id_list.push_back(IDC_RADIO_FLV);
    control_id_list.push_back(IDC_RADIO_MP4);
    control_id_list.push_back(IDC_SET_PATH_BUTTON);
    control_id_list.push_back(IDC_START_RECORD);
    control_id_list.push_back(IDC_STOP_RECORD); 
    control_id_list.push_back(IDC_PLAY);
    
    for (auto nid : control_id_list)
    {
        GetDlgItem(nid)->EnableWindow(FALSE);
    }

}

void CMediaRecorderDlg::OnMediaRecorderFileStatus(const unsigned int duration, const unsigned int file_size)
{
    typedef struct UIDataStruct
    {
        string desc_record_file;
    } UIDataStruct;
    UIDataStruct ui_data;
    ui_data.desc_record_file = demo_.DescRecordFile(duration, file_size);

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {        
        record_file_info_ = CString(CStringA(ui_data.desc_record_file.c_str()).GetBuffer());

        UpdateData(FALSE);

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);
}

void CMediaRecorderDlg::OnPublishStateUpdate(string state)
{
    cur_publish_status_ = state;

    typedef struct UIDataStruct
    {
        CString str;
    } UIDataStruct;

    UIDataStruct ui_data;

    string cur_state = cur_publish_status_;
    if (cur_record_status_ != "")
    {
        cur_state += "  ,  ";
        cur_state += cur_record_status_;
    }

    CString str(CStringA(cur_state.c_str()));
    ui_data.str = str;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {
        record_status_ = ui_data.str;

        UpdateData(FALSE);

        if (cur_publish_status_ == "LoginOk")
        {
            vector<unsigned int> control_id_list;
            control_id_list.push_back(IDC_AUDIO_ONLY_CHECK);
            control_id_list.push_back(IDC_VIDEO_ONLY_CHECK);
            control_id_list.push_back(IDC_BOTH_CHECK);
            control_id_list.push_back(IDC_PUBLISHING_CHECK);
            control_id_list.push_back(IDC_RADIO_FLV);
            control_id_list.push_back(IDC_RADIO_MP4);
            control_id_list.push_back(IDC_SET_PATH_BUTTON);
            control_id_list.push_back(IDC_START_RECORD);
            //control_id_list.push_back(IDC_STOP_RECORD);
            //control_id_list.push_back(IDC_PLAY);
            for (auto nid : control_id_list)
            {
                GetDlgItem(nid)->EnableWindow(TRUE);
            }
            demo_helper_.StartPreview();
        }

        DestroyUIData(pdata);
    };

    PostMsgDataToUI(pdata);

}

void CMediaRecorderDlg::OnMediaRecordStateUpdate(string record_state)
{
    cur_record_status_ = record_state;

    typedef struct UIDataStruct
    {
        CString str;
    } UIDataStruct;

    UIDataStruct ui_data;

    string cur_state = cur_publish_status_;
    if (cur_record_status_ != "")
    {
        cur_state += "  ,  ";
        cur_state += cur_record_status_;
    }

    CString str(CStringA(cur_state.c_str()));
    ui_data.str = str;

    PostUIData * pdata = CreateUIData();
    pdata->cb_in_ui_fun = [ui_data, pdata, this]()->void
    {

        record_status_ = ui_data.str;

        UpdateData(FALSE);

        if (cur_record_status_ == "Recording")
        {
            vector<unsigned int> control_id_list;
            control_id_list.push_back(IDC_AUDIO_ONLY_CHECK);
            control_id_list.push_back(IDC_VIDEO_ONLY_CHECK);
            control_id_list.push_back(IDC_PUBLISHING_CHECK);
            control_id_list.push_back(IDC_BOTH_CHECK);
            control_id_list.push_back(IDC_RADIO_FLV);
            control_id_list.push_back(IDC_RADIO_MP4);
            control_id_list.push_back(IDC_SET_PATH_BUTTON);
            control_id_list.push_back(IDC_START_RECORD);
            //control_id_list.push_back(IDC_STOP_RECORD);
            control_id_list.push_back(IDC_PLAY);
            for (auto nid : control_id_list)
            {
                GetDlgItem(nid)->EnableWindow(FALSE);
            }

            control_id_list.clear();
            control_id_list.push_back(IDC_STOP_RECORD);
            for (auto nid : control_id_list)
            {
                GetDlgItem(nid)->EnableWindow(TRUE);
            }

        }else if (cur_record_status_ == "Stoped Record")
        {
            vector<unsigned int> control_id_list;
            control_id_list.push_back(IDC_AUDIO_ONLY_CHECK);
            control_id_list.push_back(IDC_VIDEO_ONLY_CHECK);
            control_id_list.push_back(IDC_PUBLISHING_CHECK);
            control_id_list.push_back(IDC_BOTH_CHECK);
            control_id_list.push_back(IDC_RADIO_FLV);
            control_id_list.push_back(IDC_RADIO_MP4);
            control_id_list.push_back(IDC_SET_PATH_BUTTON);
            control_id_list.push_back(IDC_START_RECORD);
            //control_id_list.push_back(IDC_STOP_RECORD);
            control_id_list.push_back(IDC_PLAY);
            for (auto nid : control_id_list)
            {
                GetDlgItem(nid)->EnableWindow(TRUE);
            }

            control_id_list.clear();
            control_id_list.push_back(IDC_STOP_RECORD);
            for (auto nid : control_id_list)
            {
                GetDlgItem(nid)->EnableWindow(FALSE);
            }
        }

        DestroyUIData(pdata);
    };


    PostMsgDataToUI(pdata);
}

void CMediaRecorderDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Text(pDX, IDC_RECORD_STATUS, record_status_);
    DDX_Check(pDX, IDC_AUDIO_ONLY_CHECK, is_audio_only_check_);
    DDX_Check(pDX, IDC_VIDEO_ONLY_CHECK, is_video_only_check_);
    DDX_Check(pDX, IDC_BOTH_CHECK, is_both_check_);
    DDX_Check(pDX, IDC_RADIO_FLV, is_flv_check_);
    DDX_Check(pDX, IDC_RADIO_MP4, is_mp4_check_);
    DDX_Text(pDX, IDC_STORAGE_PATH, save_file_path_);
    DDX_Text(pDX, IDC_RECORD_FILE_STATUS, record_file_info_);
}

BEGIN_MESSAGE_MAP(CMediaRecorderDlg, CDialogEx)
    ON_MESSAGE(UI_CALLBACK_MSG, &CMediaRecorderDlg::OnUICallbackMsg)
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_BN_CLICKED(IDC_SET_PATH_BUTTON, &CMediaRecorderDlg::OnBnClickedSetPathButton)
    ON_BN_CLICKED(IDC_START_RECORD, &CMediaRecorderDlg::OnBnClickedStartRecord)
    ON_BN_CLICKED(IDC_STOP_RECORD, &CMediaRecorderDlg::OnBnClickedStopRecord)
    ON_BN_CLICKED(IDC_PLAY, &CMediaRecorderDlg::OnBnClickedPlay)
    ON_BN_CLICKED(IDC_AUDIO_ONLY_CHECK, &CMediaRecorderDlg::OnBnClickedAudioOnlyCheck)
    ON_BN_CLICKED(IDC_VIDEO_ONLY_CHECK, &CMediaRecorderDlg::OnBnClickedVideoOnlyCheck)
    ON_BN_CLICKED(IDC_PUBLISHING_CHECK, &CMediaRecorderDlg::OnBnClickedPublishingCheck)
    ON_BN_CLICKED(IDC_RADIO_FLV, &CMediaRecorderDlg::OnBnClickedRadioFlv)
    ON_BN_CLICKED(IDC_RADIO_MP4, &CMediaRecorderDlg::OnBnClickedRadioMp4)
    ON_BN_CLICKED(IDC_BOTH_CHECK, &CMediaRecorderDlg::OnBnClickedBothCheck)
END_MESSAGE_MAP()


// CMediaRecorderDlg 消息处理程序

BOOL CMediaRecorderDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// TODO: 在此添加额外的初始化代码

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMediaRecorderDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

	}
	else
	{
		CDialogEx::OnPaint();
	}
}

void CMediaRecorderDlg::OnBnClickedSetPathButton()
{
    UpdateData(TRUE);

    CString filter = _T("文件 (*.flv; *.mp4;)");
    CString defaultDir = _T("D:\\");
    CString default_file_name = _T("record.mp4");
    if (is_flv_check_)
    {
        filter = _T("文件 (*.flv;)|*.flv;|");
        default_file_name = _T("record.flv");
    }else if (is_mp4_check_)
    {
        filter = _T("文件 (*.mp4;)|*.mp4;|");
        default_file_name = _T("record.mp4");
    }

    BOOL isOpen = FALSE;

    CString filePath(CStringA(config_.record_file_path.c_str()).GetBuffer());

    CFileDialog openFileDlg(isOpen, defaultDir, default_file_name, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT, filter, NULL);
    openFileDlg.GetOFN().lpstrInitialDir = filePath;
    INT_PTR result = openFileDlg.DoModal();
    
    if (result == IDOK) {
        filePath = openFileDlg.GetPathName();
    }
    save_file_path_ = filePath;
    config_.record_file_path = CStringA(save_file_path_.GetBuffer());
    UpdateData(FALSE);
}

void CMediaRecorderDlg::OnBnClickedStartRecord()
{
    record_file_info_ = "";

    UpdateData(FALSE);
    
    if (is_flv_check_)
    {
        config_.record_format = MEDIA_RECORDER::ZEGO_MEDIA_RECORD_FLV;
    }
    else {
        config_.record_format = MEDIA_RECORDER::ZEGO_MEDIA_RECORD_MP4;
    }

    if (is_audio_only_check_)
    {
        config_.record_type = MEDIA_RECORDER::ZEGO_MEDIA_RECORD_AUDIO;
    }else if (is_video_only_check_)
    {
        config_.record_type = MEDIA_RECORDER::ZEGO_MEDIA_RECORD_VIDEO;
    }
    else {
        config_.record_type = MEDIA_RECORDER::ZEGO_MEDIA_RECORD_BOTH;
    }

    config_.record_file_path = CStringA(save_file_path_.GetBuffer());
    if (config_.record_file_path == "")
    {
        MessageBox(_T("invalid path"));
        return;
    }
    
    demo_.StartRecord(config_);
}

void CMediaRecorderDlg::OnBnClickedStopRecord()
{
    demo_.StopRecord();
}

void CMediaRecorderDlg::OnBnClickedRadioFlv()
{
    OnBnClickedSetPathButton();
}

void CMediaRecorderDlg::OnBnClickedRadioMp4()
{
    OnBnClickedSetPathButton();
}

void CMediaRecorderDlg::OnBnClickedPlay()
{
    ShellExecuteA(this->GetSafeHwnd(), "open", config_.record_file_path.c_str(), NULL, NULL, SW_SHOW);
}

void CMediaRecorderDlg::OnBnClickedAudioOnlyCheck()
{
    UpdateData(TRUE);
    if (is_audio_only_check_)
    {
        is_video_only_check_ = FALSE;
        is_both_check_ = FALSE;
    }

    if (!is_audio_only_check_ && !is_video_only_check_)
    {
        is_both_check_ = TRUE;
    }
    UpdateData(FALSE);
}

void CMediaRecorderDlg::OnBnClickedVideoOnlyCheck()
{
    UpdateData(TRUE);
    if (is_video_only_check_)
    {
        is_audio_only_check_ = FALSE;
        is_both_check_ = FALSE;
    }
    if (!is_audio_only_check_ && !is_video_only_check_)
    {
        is_both_check_ = TRUE;
    }
    UpdateData(FALSE);
}

void CMediaRecorderDlg::OnBnClickedBothCheck()
{
    UpdateData(TRUE);
    is_both_check_ = TRUE;

    if (is_both_check_)
    {
        is_audio_only_check_ = FALSE;
        is_video_only_check_ = FALSE;
        UpdateData(FALSE);
    }
}

void CMediaRecorderDlg::OnBnClickedPublishingCheck()
{
    CButton * pub_check = (CButton *)GetDlgItem(IDC_PUBLISHING_CHECK);

    if (pub_check->GetCheck())
    {
        demo_helper_.StartingPublishing();
    }
    else {
        demo_helper_.StopPublishing();
    }
}

afx_msg LRESULT CMediaRecorderDlg::OnUICallbackMsg(WPARAM wParam, LPARAM lParam)
{
    PostUIData * pdata = (PostUIData *)wParam;
    if (pdata != nullptr && pdata->cb_in_ui_fun)
    {
        pdata->cb_in_ui_fun();
    }
    return 0;
}




