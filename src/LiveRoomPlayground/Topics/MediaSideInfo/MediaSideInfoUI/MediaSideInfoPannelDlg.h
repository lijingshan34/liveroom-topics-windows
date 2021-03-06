#pragma once

#include "Topics/MediaSideInfo/MediaSideInfoUI/MediaSideInfoDlg.h"
#include "Topics/MediaSideInfo/MediaSideInfoUI/MediaSideinfoPlayVideoDlg.h"

// MediaSideInfoPannelDlg 对话框

class MediaSideInfoPannelDlg : public CDialogEx
{
	DECLARE_DYNAMIC(MediaSideInfoPannelDlg)

public:

    static MediaSideInfoPannelDlg * CreateDlgInstance(CWnd * pParent);

	virtual ~MediaSideInfoPannelDlg();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_MEDIASIDEINFO_PANNEL };
#endif


    virtual BOOL OnInitDialog() override;

private:
    CMediaSideInfoDlg * media_side_info_dlg_ptr_ = nullptr;
    CMediaSideinfoPlayVideoDlg * media_side_info_play_only_ = nullptr;

protected:
    
    MediaSideInfoPannelDlg(CWnd* pParent = nullptr);   // 标准构造函数

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    afx_msg void OnBnClickedSendrecv();
    afx_msg void OnBnClickedRecv();
};
