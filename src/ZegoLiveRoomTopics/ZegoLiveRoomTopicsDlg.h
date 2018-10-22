
// ZegoLiveRoomTopicsDlg.h : ͷ�ļ�
//

#pragma once

#include "MediaPlayer/MediaPlayerUI/MediaPlayerDialog.h"

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
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

    MediaPlayerDialog * media_play_dlg_pointer_;
private:
    void InitTopicList();
    CListBox topic_list_contronl_;
public:
    afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
    afx_msg void OnLbnSelchangeListTopic();
};
