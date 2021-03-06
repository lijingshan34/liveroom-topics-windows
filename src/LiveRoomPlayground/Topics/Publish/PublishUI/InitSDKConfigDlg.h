#pragma once


// InitSDKConfigDlg 对话框

#include "Publish/PublishDemo.h"


class InitSDKConfigDlgEvent
{
public:
    virtual void OnInitSDKButtonEvent() = 0;

};

class InitSDKConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(InitSDKConfigDlg)

public:
	InitSDKConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~InitSDKConfigDlg();

    void SetEventCallBack(InitSDKConfigDlgEvent * cb);
    InitSDKConfig GetConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUBLISH_DIALOG };
#endif


    virtual BOOL OnInitDialog() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
    BOOL SetTipText(UINT id, NMHDR *pTTTStruct, LRESULT *pResult);
	DECLARE_MESSAGE_MAP()


    bool CheckParams();
    InitSDKConfigDlgEvent * dlg_event_cb_ = nullptr;

    unsigned int appid_ = 0;
    CString app_sign_;
    CString user_id_;
    CString user_name_;


    InitSDKConfig init_sdk_config_;


    virtual void OnOK() override;


    virtual void OnCancel() override;

public:
    afx_msg void OnBnClickedButtonPublishTopicInitsdk();
};
