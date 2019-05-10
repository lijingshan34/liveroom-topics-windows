
// ZegoLiveRoomTopicsDlg.h : ͷ�ļ�
//

#pragma once


#include <vector>

#include "MediaPlayer/MediaPlayerUI/MediaPlayerDialog.h"
#include "MediaSideInfo/MediaSideInfoUI/MediaSideInfoDlg.h"
#include "MediaRecorder/MediaRecorderUI/MediaRecorderDlg.h"
#include "MediaSideInfo/MediaSideInfoUI/MediaSideInfoPannelDlg.h"
#include "ExternalVideoCapture/ExternalVideoCaptureUI/ZGExternalVideoCaptureDlg.h"
#include "ExternalVideoRender/ExternalVideoRenderUI/ExternalVideoRenderDlg.h"
#include "Publish/PublishUI/PublishDlg.h"
#include "ExternalVideoFilter/ExternalVideoFilterUI/ExternalVideoFilterDlg.h"
#include "Play/PlayUI/PlayDlg.h"

using std::vector;

// CZegoLiveRoomTopicsDlg �Ի���
class CZegoLiveRoomTopicsDlg : public CDialogEx
{
// ����
public:
	CZegoLiveRoomTopicsDlg(CWnd* pParent = NULL);	// ��׼���캯��
    ~CZegoLiveRoomTopicsDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ZEGOLIVEROOMTOPICS_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	//HICON m_hIcon;

    void ShowDlg(void * dlg);

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	//afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    MediaPlayerDialog * media_play_dlg_ptr_ = nullptr;    
    CMediaRecorderDlg * media_recorder_dlg_ptr_ = nullptr;
    MediaSideInfoPannelDlg * media_side_info_dlg_ptr_ = nullptr;
    CZGExternalVideoCaptureDlg * external_video_capture_dlg_ptr_ = nullptr;
    CExternalVideoRenderDlg * external_video_render_dlg_ptr_ = nullptr;
    CPublishDlg * publish_dlg_ptr_ = nullptr;
    CPlayDlg * play_dlg_ptr_ = nullptr;
    CExternalVideoFilterDlg * external_video_filter_ = nullptr;

    std::vector<void*> dlg_list_;

private:
    void InitTopicList();
    CListBox basic_topic_list_contronl_;
    CListBox advanced_topic_list_contronl_;
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLbnSelchangeListTopic();
public:
    afx_msg void OnLbnSelchangeListTopicBasic();
    afx_msg void OnLbnSelchangeListTopicAdvanced();
    afx_msg void OnBnClickedButtonDoc();
    afx_msg void OnBnClickedButtonCode();
    afx_msg void OnBnClickedButtonFaq();
};
