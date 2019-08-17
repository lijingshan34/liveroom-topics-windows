//
// VideoFilterProcessBase.h
//
// Copyright 2019�� Zego. All rights reserved.
// 

#ifndef VideoFilterProcessBase_h__
#define VideoFilterProcessBase_h__

// ��Ƶ�˾�������
class VideoFilterProcessBase
{
public:
    VideoFilterProcessBase();
    virtual ~VideoFilterProcessBase();

    // ��ʼ���˾�
    virtual bool InitFilter(){ return false;};

    // �˾�����YUV I420 ��Ƶ����
    virtual void FilterProcessI420Data(unsigned char * data, int frame_len, int frame_w, int frame_h) {}

    // �˾�����RGB����
    virtual void FilterProcessRGBAData(unsigned char * data, int frame_len, int frame_w, int frame_h) {}

    // ˢ���˾�����ȼ����������յȼ�
    virtual void UpdateFilterLevel(int level){}

};

#endif // VideoFilterProcessBase_h__


