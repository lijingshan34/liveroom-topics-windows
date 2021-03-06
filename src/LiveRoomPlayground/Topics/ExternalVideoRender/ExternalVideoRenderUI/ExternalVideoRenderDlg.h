
// ExternalVideoRenderDlg.h: 头文件
//

#pragma once

#include "Topics/ExternalVideoRender/ZGExternalVideoRenderDemo.h"
#include "Topics/ExternalVideoRender/ZGExternalVideoRenderDemoHelper.h"
#include "Topics/ExternalVideoRender/ZGVideoRender.h"

#include "AppSupport/ZGUIOperator.h"

// CExternalVideoRenderDlg 对话框
class CExternalVideoRenderDlg : public CDialogEx
{
// 构造
public:
    static CExternalVideoRenderDlg * CreateDlgInstance(CWnd * pParent);
    
// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_EXTERNALVIDEORENDER_DIALOG };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持
    void PostNcDestroy()
    {
        CDialogEx::PostNcDestroy();
        delete this;
    }

// 实现
protected:
    CExternalVideoRenderDlg(CWnd* pParent = nullptr);	// 标准构造函数

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()


    void OnLocalVideoUpdate(string stream_id, const unsigned char *pData, int dataLen, int width, int height);

    void OnRemoteVideoUpdate(string stream_id, const unsigned char *pData, int dataLen, int width, int height);

    void OnStatusUpdate(std::string status);

    ZGExternalVideoRenderDemo demo_;
    ZGExternalVideoRenderDemoHelper helper_;

    ZGVideoRender local_video_render_;
    ZGVideoRender remote_video_render_;

    ZEGO::EXTERNAL_RENDER::VideoRenderType cur_set_external_render_type_ = ZEGO::EXTERNAL_RENDER::VIDEO_RENDER_TYPE_RGB;
    ZEGO::EXTERNAL_RENDER::VideoRenderType cur_use_external_render_type_ = ZEGO::EXTERNAL_RENDER::VIDEO_RENDER_TYPE_RGB;

public:
    afx_msg void OnBnClickedButtonPublish();
    afx_msg void OnBnClickedButtonPlay();
    afx_msg LRESULT OnUICallbackMsg(WPARAM wParam, LPARAM lParam);
	afx_msg void OnBnClickedVideoRenderTypeNone();
	afx_msg void OnBnClickedVideoRenderTypeRgb();
	afx_msg void OnBnClickedExternalVideoRenderTypeYuv();
	afx_msg void OnBnClickedVideoRenderTypeExternalInternalYuv();
	afx_msg void OnBnClickedVideoRenderTypeExternalInternalRgb();
};
