#pragma once

#include <memory>
#include <string>
#include <thread>
#include <atomic>
#include <mutex>

#include "ZGScreenCaptureDevice.h"
#include "ZGScreenCaptureSource.h"

using std::string;


class ZGScreenCaptureManager
{
public:
	ZGScreenCaptureManager();
	~ZGScreenCaptureManager();

	void StartCapture();
	void StopCapture();

protected:
	// ��ʼ��
	void InitManager();

	// ��ʼ�Ӳɼ�Դ�ж�ȡ���ݸ��豸
	void StartScreenCaptureThread();
	// ֹͣ�Ӳɼ�Դ�ж�ȡ���ݸ��豸
	void StopScreenCaptureThread();
	// �ⲿ�ɼ��̺߳���
	void CaptureVideoDataThread();

	time_t GetTimeStamp();

private:
	// �ⲿ�ɼ��߳�
	std::thread capture_thread_;

	// �ⲿ�Ƿ�ֹͣ
	std::atomic<bool> stoped_ = true;

	std::shared_ptr<ZGScreenCaptureDevice> device_;
	std::shared_ptr<ZGScreenCaptureSource> source_;

	// �ⲿ�ɼ�֡�����úͿ���
	int capture_fps_ = 10;
	std::mutex external_source_mutex_;

};