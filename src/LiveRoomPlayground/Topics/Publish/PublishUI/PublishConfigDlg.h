#pragma once


// PublishConfigDlg 对话框

#include "Publish/PublishDemo.h"

class PublishConfigDlgEvent
{
public:
    virtual void OnPublishButtonEvent() = 0;
    virtual void OnBaseFunctionButtonEvent(bool show) = 0;
};

class PublishConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(PublishConfigDlg)

public:
	PublishConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~PublishConfigDlg();

    void SetEventCallBack(PublishConfigDlgEvent * cb);

    PublishConfig GetConfig();

    void InitDlgData();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_PUBLISH_CONFIG_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    PublishConfigDlgEvent * event_cb_ = nullptr;

    PublishConfig publish_config_;

    CString stream_id_;
    bool show_base_fun_ = false;

    virtual void OnOK() override;


    virtual void OnCancel() override;

public:
    afx_msg void OnBnClickedButtonPublishTopicStartPublish();
    afx_msg void OnBnClickedButtonPublishTopicBaseFun();
};
