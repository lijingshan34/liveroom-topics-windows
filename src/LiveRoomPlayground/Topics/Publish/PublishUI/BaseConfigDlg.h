#pragma once


#include "AppSupport/ZGConfigHelper.h"


class BaseConfigDlgEvent
{
public:
    virtual void OnResolutionSelectChanged(int w, int h) = 0;
    virtual void OnBitRateSelectChaned(int bit_rate) = 0;
    virtual void OnFpsSelectChanged(int fps) = 0;
    virtual void OnViewModeChaned(int view_mode_index) = 0;
    virtual void OnCaptureCameraMirrorChanged(bool enable_mirror) = 0;
	virtual void OnActivateVideoPublishStreamChanged(bool activated) = 0;
	virtual void OnActivateAudioPublishStreamChanged(bool activated) = 0;
    virtual void OnVideoDeviceSelectChanged(std::string device_id) = 0;
    virtual void OnMicDeviceSelectChanged(std::string device_id) = 0;
    virtual void OnSpeakerDeviceSelectChanged(std::string device_id) = 0;
};

class BaseConfigDlg : public CDialogEx
{
	DECLARE_DYNAMIC(BaseConfigDlg)

public:
	BaseConfigDlg(CWnd* pParent = nullptr);   // 标准构造函数
	virtual ~BaseConfigDlg();
    void SetEventCallBack(BaseConfigDlgEvent * cb);

    void InitDefaultConfig();

// 对话框数据
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_BASE_FUN_DLG };
#endif


    virtual BOOL OnInitDialog() override;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()

    CComboBox resolution_combo_box_;
    CComboBox bitrate_combo_box_;
    CComboBox framte_rate_combo_box_;
    CComboBox view_mode_combo_box_;
    CComboBox camera_combo_box_;
    CComboBox mic_combo_box_;
    CComboBox speaker_combo_box_;

    std::vector<VideoDeviceInfo> video_device_list_;
    int cur_vieo_devie_index_ = 0;

    std::vector<AudioDeviceInfo> mic_device_list_;
    int cur_mic_devie_index_ = 0;

    std::vector<AudioDeviceInfo> speaker_device_list_;
    int cur_speaker_devie_index_ = 0;

    BaseConfigDlgEvent * event_cb_ = nullptr;

    virtual void OnOK() override;


    virtual void OnCancel() override;

public:
    afx_msg void OnCbnSelchangeComboPublishTopicResolution();
    afx_msg void OnCbnSelchangeComboPublishTopicBitrate();
    afx_msg void OnCbnSelchangeComboPublishTopicFrameRate();
    afx_msg void OnCbnSelchangeComboPublishTopicViewMode();
    afx_msg void OnBnClickedCheck();
    afx_msg void OnCbnSelchangeComboPublishTopicVideoList();
    afx_msg void OnCbnSelchangeComboPublishTopicMicList();
    afx_msg void OnCbnSelchangeComboPublishTopicSpeaker();
	afx_msg void OnBnClickedCheckActivateaudiopublishstream();
	afx_msg void OnBnClickedCheckActivatevideopublishstream();
};
