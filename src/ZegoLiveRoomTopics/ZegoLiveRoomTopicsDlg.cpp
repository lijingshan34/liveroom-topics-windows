
// ZegoLiveRoomTopicsDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "ZegoLiveRoomTopics.h"
#include "ZegoLiveRoomTopicsDlg.h"
#include "afxdialogex.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TOPIC_MIDIA_PLAYER      _T("MediaPlayer")
#define TOPIC_MEDIA_SIDE_INFO   _T("Media Side Info")


// ����Ӧ�ó��򡰹��ڡ��˵���� CAboutDlg �Ի���

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// �Ի�������
#ifdef AFX_DESIGN_TIME
	enum { IDD = IDD_ABOUTBOX };
#endif

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

// ʵ��
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(IDD_ABOUTBOX)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CZegoLiveRoomTopicsDlg �Ի���



CZegoLiveRoomTopicsDlg::CZegoLiveRoomTopicsDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(IDD_ZEGOLIVEROOMTOPICS_DIALOG, pParent),
    media_play_dlg_ptr_(nullptr),
    media_side_info_dlg_ptr_(nullptr)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

CZegoLiveRoomTopicsDlg::~CZegoLiveRoomTopicsDlg()
{
}

void CZegoLiveRoomTopicsDlg::DoDataExchange(CDataExchange* pDX)
{
    CDialogEx::DoDataExchange(pDX);
    DDX_Control(pDX, IDC_LIST_TOPIC, topic_list_contronl_);
}

BEGIN_MESSAGE_MAP(CZegoLiveRoomTopicsDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
    ON_WM_LBUTTONDOWN()
    ON_LBN_SELCHANGE(IDC_LIST_TOPIC, &CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopic)
END_MESSAGE_MAP()


// CZegoLiveRoomTopicsDlg ��Ϣ�������

BOOL CZegoLiveRoomTopicsDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// ��������...���˵�����ӵ�ϵͳ�˵��С�

	// IDM_ABOUTBOX ������ϵͳ���Χ�ڡ�
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDS_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// ���ô˶Ի����ͼ�ꡣ  ��Ӧ�ó��������ڲ��ǶԻ���ʱ����ܽ��Զ�
	//  ִ�д˲���
	SetIcon(m_hIcon, TRUE);			// ���ô�ͼ��
	SetIcon(m_hIcon, FALSE);		// ����Сͼ��

	// TODO: �ڴ���Ӷ���ĳ�ʼ������

    InitTopicList();

	return TRUE;  // ���ǽ��������õ��ؼ������򷵻� TRUE
}

void CZegoLiveRoomTopicsDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else
	{
		CDialogEx::OnSysCommand(nID, lParam);
	}
}

// �����Ի��������С����ť������Ҫ����Ĵ���
//  �����Ƹ�ͼ�ꡣ  ����ʹ���ĵ�/��ͼģ�͵� MFC Ӧ�ó���
//  �⽫�ɿ���Զ���ɡ�

void CZegoLiveRoomTopicsDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // ���ڻ��Ƶ��豸������

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// ʹͼ���ڹ����������о���
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// ����ͼ��
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//���û��϶���С������ʱϵͳ���ô˺���ȡ�ù��
//��ʾ��
HCURSOR CZegoLiveRoomTopicsDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}

void CZegoLiveRoomTopicsDlg::InitTopicList()
{
    topic_list_contronl_.AddString(TOPIC_MIDIA_PLAYER);
    topic_list_contronl_.AddString(TOPIC_MEDIA_SIDE_INFO);
}

void CZegoLiveRoomTopicsDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
    CDialogEx::OnLButtonDown(nFlags, point);
}

void CZegoLiveRoomTopicsDlg::ShowDlg(void * dlg)
{
    for (auto p : dlg_list_)
    {
        if (p == dlg)
        {
            ((CDialogEx*)p)->ShowWindow(SW_SHOW);
        }
        else 
        {
            ((CDialogEx*)p)->ShowWindow(SW_HIDE);
        }
    }
}

void CZegoLiveRoomTopicsDlg::OnLbnSelchangeListTopic()
{
    int cur_sel = topic_list_contronl_.GetCurSel();
    if (cur_sel >= 0)
    {
        CString str;
        topic_list_contronl_.GetText(cur_sel, str);
        //MessageBox(str);
        if (str == TOPIC_MIDIA_PLAYER)
        {
            if (media_play_dlg_ptr_ == nullptr)
            {
                media_play_dlg_ptr_ = new MediaPlayerDialog();
                media_play_dlg_ptr_->Create(IDD_MEDIAPLAYER_DIALOG, this);
                dlg_list_.push_back(media_play_dlg_ptr_);
            }

            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            media_play_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(media_play_dlg_ptr_);
            media_play_dlg_ptr_->StartMediaPlayer();
        }
        else if (str == TOPIC_MEDIA_SIDE_INFO)
        {
            if (media_side_info_dlg_ptr_ == nullptr)
            {
                media_side_info_dlg_ptr_ = new CMediaSideInfoDlg();
                media_side_info_dlg_ptr_->Create(IDD_MEDIASIDEINFO_DIALOG, this);
                dlg_list_.push_back(media_side_info_dlg_ptr_);
            }

            CRect rect;
            GetDlgItem(IDC_PANEL)->GetWindowRect(rect);
            ScreenToClient(&rect);
            media_side_info_dlg_ptr_->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
            ShowDlg(media_side_info_dlg_ptr_);
            media_side_info_dlg_ptr_->LoginRoom();
        }
    }
}

