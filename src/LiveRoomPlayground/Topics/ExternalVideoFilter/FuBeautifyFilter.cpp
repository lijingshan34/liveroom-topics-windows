#include "stdafx.h"
#include "FuBeautifyFilter.h"

#include <string>
#include <vector>
#include <fstream>
#include <thread>

#include "funama.h"


// ��ο��������ӣ������ȡ���յ�֤���ļ� authpack.h ���ŵ���Ŀ¼ libs\fusdk\Windows\include ��
// 
// https://github.com/Faceunity/FULivePC/blob/master/docs/Windows_Nama_SDK_%E9%9B%86%E6%88%90%E6%8C%87%E5%AF%BC%E6%96%87%E6%A1%A3.md
// 
// ��ȡ֤��:
// ����绰 0571-88069272
// �����ʼ��� marketing@faceunity.com ������ѯ��
// 
#include "authpack.h"

#include "AppSupport/ZGUtilTools.h"

#include <windows.h>
#pragma comment(lib,"opengl32.lib")
#pragma comment(lib,"glu32.lib")


FuBeautifyFilter::FuBeautifyFilter()
{
}


FuBeautifyFilter::~FuBeautifyFilter()
{

}

size_t FileSize(std::ifstream& file)
{
    std::streampos oldPos = file.tellg();
    file.seekg(0, std::ios::beg);
    std::streampos beg = file.tellg();
    file.seekg(0, std::ios::end);
    std::streampos end = file.tellg();
    file.seekg(oldPos, std::ios::beg);
    return static_cast<size_t>(end - beg);
}

bool LoadBundle(const std::string& filepath, std::vector<char>& data)
{
    std::ifstream fin(filepath, std::ios::binary);
    if (false == fin.good())
    {
        fin.close();
        return false;
    }
    size_t size = FileSize(fin);
    if (0 == size)
    {
        fin.close();
        return false;
    }
    data.resize(size);
    fin.read(reinterpret_cast<char*>(&data[0]), size);

    fin.close();
    return true;
}

bool FuBeautifyFilter::InitFilter()
{
    if (!InitOpenGL())
    {
        return false;
    }

    if (!InitFuSdk())
    {
        return false;
    }

    if (!LoadFuResource())
    {
        return false;
    }

    // ����Ĭ�����յȼ�
    UpdateFilterLevel(5);

    return true;
}

void FuBeautifyFilter::FilterProcessI420Data(unsigned char * data, int frame_len, int frame_w, int frame_h)
{

}

void FuBeautifyFilter::UpdateFilterLevel(int level)
{
    level = level + 1;// [1, 10]

    // ��ɫ��⿪�أ�0Ϊ�أ�1Ϊ��
    fuItemSetParamd(beauty_handles_, "skin_detect", 1);
    // blur_level: ĥƤ�̶ȣ�ȡֵ��Χ0.0-6.0��Ĭ��6.0
    fuItemSetParamd(beauty_handles_, "blur_level", level);

    // ���� color_level ȡֵ��Χ 0.0-1.0,0.0Ϊ��Ч����1.0Ϊ���Ч����Ĭ��ֵ0.2
    fuItemSetParamd(beauty_handles_, "color_level", level * 1.0 / 10.0);

    // ���� red_level ȡֵ��Χ 0.0-1.0,0.0Ϊ��Ч����1.0Ϊ���Ч����Ĭ��ֵ0.5
    fuItemSetParamd(beauty_handles_, "red_level", level * 1.0 / 10.0);

    // ���� eye_bright   ȡֵ��Χ 0.0-1.0,0.0Ϊ��Ч����1.0Ϊ���Ч����Ĭ��ֵ1.0
    fuItemSetParamd(beauty_handles_, "eye_bright", level * 1.0 / 10.0);

    // ���� tooth_whiten   ȡֵ��Χ 0.0-1.0,0.0Ϊ��Ч����1.0Ϊ���Ч����Ĭ��ֵ1.0
    fuItemSetParamd(beauty_handles_, "tooth_whiten", level * 1.0 / 10.0);
}

void FuBeautifyFilter::FilterProcessRGBAData(unsigned char * data, int frame_len, int frame_w, int frame_h)
{
    fuRenderItemsEx2(FU_FORMAT_RGBA_BUFFER, reinterpret_cast<int*>(data), FU_FORMAT_RGBA_BUFFER, reinterpret_cast<int*>(data), frame_w, frame_h, frame_id_++, handles_.data(), handles_.size(), NAMA_RENDER_FEATURE_FULL, NULL);
}

bool FuBeautifyFilter::InitFuSdk()
{
    // ��ʼ��fu sdk
    std::vector<char> v3data;
    LoadBundle(GetExePath() + "/Res/v3.bundle", v3data);
    int result_code = fuSetup(reinterpret_cast<float*>(&v3data[0]), v3data.size(), NULL, g_auth_package, sizeof(g_auth_package));
    if (result_code == 0)
    {
        return false;
    }
    return true;
}

bool FuBeautifyFilter::LoadFuResource()
{
    //��ȡ���յ���    
    std::vector<char> propData;
    if (false == LoadBundle(GetExePath() + "/Res/face_beautification.bundle", propData))
    {
        return false;
    }

    beauty_handles_ = fuCreateItemFromPackage(&propData[0], propData.size());
    handles_.push_back(beauty_handles_);
    return true;
}

bool FuBeautifyFilter::InitOpenGL()
{
    PIXELFORMATDESCRIPTOR pfd = {
        sizeof(PIXELFORMATDESCRIPTOR),
        1u,
        PFD_SUPPORT_OPENGL | PFD_DOUBLEBUFFER | PFD_DRAW_TO_WINDOW,
        PFD_TYPE_RGBA,
        32u,
        0u, 0u, 0u, 0u, 0u, 0u,
        8u,
        0u,
        0u,
        0u, 0u, 0u, 0u,
        24u,
        8u,
        0u,
        PFD_MAIN_PLANE,
        0u,
        0u, 0u };

    HWND hw = CreateWindowExA(
        0, "EDIT", "", ES_READONLY,
        0, 0, 1, 1,
        NULL, NULL,
        GetModuleHandleA(NULL), NULL);

    HDC hgldc = GetDC(hw);
    int spf = ChoosePixelFormat(hgldc, &pfd);
    if (!spf)
    {
        return false;
    }
    int ret = SetPixelFormat(hgldc, spf, &pfd);

    HGLRC hglrc = wglCreateContext(hgldc);
    wglMakeCurrent(hgldc, hglrc);

    if (hglrc != nullptr)
    {
        //OpenGL�����ĵ�ַ�����4��������һ����Ϊ��ֵ����OpenGL�����ǿ��õ�
        //�˺������Nama SDK���ö���������context
        //����ͻ������������ƴ����������OpenGL context����ô��ȷ������Nama�ӿ�ʱһֱ��ͬһ��context
        if (wglGetProcAddress("glGenFramebuffersARB") != nullptr
            || wglGetProcAddress("glGenFramebuffersOES") != nullptr
            || wglGetProcAddress("glGenFramebuffersEXT") != nullptr
            || wglGetProcAddress("glGenFramebuffers") != nullptr)
        {
            return true;
        }
        else {
            return false;
        }
    }
    else {
        return false;
    }
}
