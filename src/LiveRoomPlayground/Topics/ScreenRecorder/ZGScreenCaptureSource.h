#pragma once

#include <vector>
#include <atomic>
#include <stdint.h>
#include <memory>
#include "ZegoLiveRoom/video_capture.h"
#include "Topics/ScreenRecorder/ScreenCapture/include/zego-screencapture.h"
using namespace AVE;
using std::vector;

// �ⲿ�ɼ�����Ƶ���ݽṹ
typedef struct ZGScreenVideoData
{
	// std::unique_ptr<uint8_t[]> data(new uint8_t[10]);
	// this will correctly call delete[]
	std::unique_ptr<uint8_t[] > data;      // �ⲿ�ɼ���video data���飬����new 10�����ȵ����飬std::unique_ptr<uint8_t[]> data(new uint8_t[10])��ָ���׵�ַdata.get()
	uint32_t len = 0;                      // ���ݳ���
	VideoCaptureFormat fomat;              // ��ʽ RGBA ����BGRA��
	unsigned long long reference_time = 0; // ʱ���

}ZGScreenVideoData;

class ZGScreenCaptureSource
{
public:
	ZGScreenCaptureSource();
	~ZGScreenCaptureSource();

	void OpenSource();
	void CloseSource();

	// ��ȡ���񵽵���Ļ����
	void OnGetVideoData(std::shared_ptr<ZGScreenVideoData> & video_data);

private:
	static void OnScreenCaptureData(const char *data, uint32_t length, const struct ZegoScreenCaptureVideoCaptureFormat *video_frame_format, uint64_t reference_time, uint32_t reference_time_scale, void *user_data);

	static void OnScreenCaptureError(enum ZegoScreenCaptureCaptureError error, void *data);

	// ������Ļ�ɼ�֡��
	int screen_capture_fps_ = 10;

	// ʹ���������
	bool scale_ = true;

	// ������Ļ�ɼ�����ֱ��ʿ�
	int width_ = 1920;

	// ������Ļ�ɼ�����ֱ��ʸ�
	int height_ = 1080;

	// ���ݻ���
	vector<std::shared_ptr<ZGScreenVideoData> > capture_video_data_vec_;

	// ��������С���ã����λ���
	const int MAX_VIDEO_BUF_LEN = 10;

	// д����λ��
	std::atomic<int> write_index_ = 0;

	// ������λ��
	std::atomic<int> read_index_ = 0;

	// �ɼ��Ƿ���
	bool have_start_captured_ = false;
};