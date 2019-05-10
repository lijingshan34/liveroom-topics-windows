//
// BeautifyFilter.h
//
// Copyright 2019�� Zego. All rights reserved.
// 

#ifndef BeautifyFilter_h__
#define BeautifyFilter_h__

#include "VideoFilterProcessBase.h"

class TILFilter;

// tencent ���սӿ�ʵ��
class BeautifyFilter : public VideoFilterProcessBase
{
public:
    BeautifyFilter();
    ~BeautifyFilter();

    // ��ʼ�������˾�
    virtual bool InitFilter() override;

    // ���մ���YUV I420����
    virtual void FilterProcessI420Data(unsigned char * data, int frame_len, int frame_w, int frame_h) override;

    // �������յȼ�����Χ[0-9]
    virtual void UpdateFilterLevel(int level) override;

protected:

    TILFilter* beauty_filter_ = nullptr;
    bool inited_ = false;

};

#endif // BeautifyFilter_h__


