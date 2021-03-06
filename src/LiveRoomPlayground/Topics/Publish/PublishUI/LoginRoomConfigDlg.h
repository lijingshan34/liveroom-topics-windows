#pragma once

#include "Publish/PublishDemo.h"

// LoginRoomConfigDlg 对话框


class LoginRoomConfigDlgEvent
{
public:
    virtual void OnLoginRoomButtonEvent() = 0;

};

class LoginRoomConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(LoginRoomConfigDlg)

public:
	LoginRoomConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~LoginRoomConfigDlg();

    void SetEventCallBack(LoginRoomConfigDlgEvent * cb);

    LoginRomConfig GetConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_LOGINROOM_CONFIG_DLG };
#endif

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    CString room_id_;

    LoginRoomConfigDlgEvent * event_cb_ = nullptr;
    LoginRomConfig login_room_config_;

    virtual void OnOK() override;


    virtual void OnCancel() override;

public:
    afx_msg void OnBnClickedButtonPublishTopicLoginroom();
};
