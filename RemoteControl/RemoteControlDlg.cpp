
// RemoteControlDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "RemoteControl.h"
#include "RemoteControlDlg.h"
#include "afxdialogex.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif

typedef struct
{
	char	*title;           //列表的名称
	int		nWidth;			  //列表的宽度
}COLUMNSTRUCT;

COLUMNSTRUCT g_Column_Online_Data[] =
{
	{ "IP", 148 },
	{ "区域", 150 },
	{ "计算机名/备注", 160 },
	{ "操作系统", 128 },
	{ "CPU", 80 },
	{ "摄像头", 81 },
	{ "PING", 81 }
};

COLUMNSTRUCT g_Column_Message_Data[] =
{
	{ "信息类型", 68 },
	{ "时间", 100 },
	{ "信息内容", 660 }
};

//变量声明
int g_Column_Count_Message = 3; //日志列表的个数
int g_Column_Count_Online = 7; //主机列表的个数
int g_Column_Online_Width = 0;  //列总宽度
int g_Column_Message_Width = 0; //列总宽度

CIOCPServer *m_iocpServer = NULL;



  

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialogEx
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialogEx(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialogEx)
END_MESSAGE_MAP()


// CRemoteControlDlg 对话框



CRemoteControlDlg::CRemoteControlDlg(CWnd* pParent /*=NULL*/)
	: CDialogEx(CRemoteControlDlg::IDD, pParent)
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
	iCount = 0;
	
}

void CRemoteControlDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_ONLINE, m_CList_Online);
	DDX_Control(pDX, IDC_MESSAGE, m_CList_Message);
}

BEGIN_MESSAGE_MAP(CRemoteControlDlg, CDialogEx)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	ON_WM_SIZE()
	ON_NOTIFY(NM_RCLICK, IDC_ONLINE, &CRemoteControlDlg::OnNMRClickOnline)
	ON_COMMAND(ID_ONLINE_AUDIO, &CRemoteControlDlg::OnOnlineAudio)
	ON_COMMAND(ID_ONLINE_CMD, &CRemoteControlDlg::OnOnlineCmd)
	ON_COMMAND(ID_ONLINE_DESKTOP, &CRemoteControlDlg::OnOnlineDesktop)
	ON_COMMAND(ID_ONLINE_FILE, &CRemoteControlDlg::OnOnlineFile)
	ON_COMMAND(ID_ONLINE_PROCESS, &CRemoteControlDlg::OnOnlineProcess)
	ON_COMMAND(ID_ONLINE_REGEDIT, &CRemoteControlDlg::OnOnlineRegedit)
	ON_COMMAND(ID_ONLINE_Remote, &CRemoteControlDlg::OnOnlineRemote)
	ON_COMMAND(ID_ONLINE_SERVER, &CRemoteControlDlg::OnOnlineServer)
	ON_COMMAND(ID_ONLINE_VIDEO, &CRemoteControlDlg::OnOnlineVideo)
	ON_COMMAND(ID_ONLINE_WINDOW, &CRemoteControlDlg::OnOnlineWindow)
	ON_COMMAND(ID_ONLINE_DELETE, &CRemoteControlDlg::OnOnlineDelete)
	ON_COMMAND(ID_MAIN_ABOUT, &CRemoteControlDlg::OnMainAbout)
	ON_COMMAND(ID_MAIN_BUILD, &CRemoteControlDlg::OnMainBuild)
	ON_COMMAND(ID_MAIN_SET, &CRemoteControlDlg::OnMainSet)
	ON_COMMAND(ID_MAIN_CLOSE, &CRemoteControlDlg::OnMainClose)
	ON_MESSAGE(UM_ICONNOTIFY, (LRESULT(__thiscall CWnd::*)(WPARAM, LPARAM))OnIconNotify)
	ON_COMMAND(IDM_NOTIFY_CLOSE, &CRemoteControlDlg::OnNotifyClose)
	ON_COMMAND(IDM_NOTIFY_SHOW, &CRemoteControlDlg::OnNotifyShow)
	ON_WM_CLOSE()
END_MESSAGE_MAP()



//所有关于socket的处理都要经过这个函数
void CALLBACK CRemoteControlDlg::NotifyProc(LPVOID lpParam, ClientContext *pContext, UINT nCode)
{
	
		switch (nCode)
		{
		case NC_CLIENT_CONNECT:
			break;
		case NC_CLIENT_DISCONNECT:
			//g_pConnectView->PostMessage(WM_REMOVEFROMLIST, 0, (LPARAM)pContext);
			break;
		case NC_TRANSMIT:
			break;
		case NC_RECEIVE:
			//ProcessReceive(pContext);        //这里是有数据到来，但没有完全接受
			break;
		case NC_RECEIVE_COMPLETE:
			//ProcessReceiveComplete(pContext);       //这里是完全接收发送来的数据 跟进    ProcessReceiveComplete
			break;
		}
	
	
}

void CRemoteControlDlg::Activate(UINT nPort, UINT nMaxConnections)
{
	CString		str;

	if (m_iocpServer != NULL)
	{
		m_iocpServer->Shutdown();
		delete m_iocpServer;

	}
	m_iocpServer = new CIOCPServer;

	////lang2.1_8
	// 开启IPCP服务器 最大连接  端口     查看NotifyProc回调函数  函数定义
	if (m_iocpServer->Initialize(NotifyProc, NULL, 100000, nPort))
	{

		char hostname[256];
		gethostname(hostname, sizeof(hostname));
		HOSTENT *host = gethostbyname(hostname);
		if (host != NULL)
		{
			for (int i = 0;; i++)
			{
				str += inet_ntoa(*(IN_ADDR*)host->h_addr_list[i]);
				if (host->h_addr_list[i] + host->h_length >= host->h_name)
					break;
				str += "/";
			}
		}

		str.Format("监听端口: %d成功", nPort);
		ShowMessage(true, str);
	}
	else
	{
		str.Format("监听端口: %d失败", nPort);
		ShowMessage(false, str);
	}

	//m_wndStatusBar.SetPaneText(3, "连接: 0");
}

// CRemoteControlDlg 消息处理程序

BOOL CRemoteControlDlg::OnInitDialog()
{
	CDialogEx::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		BOOL bNameValid;
		CString strAboutMenu;
		bNameValid = strAboutMenu.LoadString(IDD_ABOUTBOX);
		ASSERT(bNameValid);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。  当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	// TODO:  在此添加额外的初始化代码
	CreatStatusBar();//创建状态栏
	InitList();//初始化列表信息
	Activate(2000, 9999);
	test();//测试上线
	


	HMENU hmenu;
	hmenu = LoadMenu(NULL, MAKEINTRESOURCE(IDR_MENU_MAIN));  //载入菜单资源
	::SetMenu(this->GetSafeHwnd(), hmenu);                  //为窗口设置菜单
	::DrawMenuBar(this->GetSafeHwnd());                    //显示菜单
	AddNotify();

	

	CRect rect;
	GetWindowRect(&rect);
	rect.bottom += 20;
	MoveWindow(rect);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CRemoteControlDlg::OnSysCommand(UINT nID, LPARAM lParam)
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

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。  对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CRemoteControlDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialogEx::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CRemoteControlDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}



void CRemoteControlDlg::OnSize(UINT nType, int cx, int cy)
{
	CDialogEx::OnSize(nType, cx, cy);
	if (m_CList_Online.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 10;       //列表的左坐标
		rc.top = 0;       //列表的上坐标
		rc.right = cx - 10;  //列表的右坐标
		rc.bottom = cy - 160;  //列表的下坐标
		m_CList_Online.MoveWindow(rc);

		  		
		
		double dcx = cx;     //对话框的总宽度

		for (int i = 0; i < g_Column_Count_Online; i++){      //遍历每一个列
			double dd = g_Column_Online_Data[i].nWidth;       //得到当前列的宽度
			dd /= g_Column_Online_Width+20;                      //看一看当前宽度占总长度的几分之几
			dd *= dcx;                                        //用原来的长度乘以所占的几分之几得到当前的宽度
			int lenth = dd;                                   //转换为int 类型
			m_CList_Online.SetColumnWidth(i, (lenth));        //设置当前的宽度
		}
	}
	if (m_CList_Message.m_hWnd != NULL)
	{
		CRect rc;
		rc.left = 10;        //列表的左坐标
		rc.top = cy - 156;    //列表的上坐标
		rc.right = cx - 10;    //列表的右坐标
		rc.bottom = cy - 16;  //列表的下坐标
		m_CList_Message.MoveWindow(rc);

		
		
		double dcx = cx;
		for (int i = 0; i < g_Column_Count_Message; i++){     //遍历每一个列
			double dd = g_Column_Message_Data[i].nWidth;     //得到当前列的宽度
			dd /= g_Column_Message_Width+20;                    //看一看当前宽度占总长度的几分之几
			dd *= dcx;                                       //用原来的长度乘以所占的几分之几得到当前的宽度
			int lenth = dd;                                   //转换为int 类型
			m_CList_Message.SetColumnWidth(i, (lenth));        //设置当前的宽度
		}
	}
	if (m_wndStatusBar.m_hWnd != NULL){    //当对话框大小改变时 状态条大小也随之改变
		CRect rc;
		rc.top = cy - 20;
		rc.left = 0;
		rc.right = cx;
		rc.bottom = cy;
		m_wndStatusBar.MoveWindow(rc);
		m_wndStatusBar.SetPaneInfo(0, m_wndStatusBar.GetItemID(0), SBPS_POPOUT, cx - 10);
	}
	// TODO:  在此处添加消息处理程序代码
}


// 初始化列表
int CRemoteControlDlg::InitList()
{

	m_CList_Online.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	m_CList_Message.SetExtendedStyle(LVS_EX_FULLROWSELECT);
	
	for (int i = 0; i < g_Column_Count_Online; i++)
	{
		m_CList_Online.InsertColumn(i, g_Column_Online_Data[i].title, LVCFMT_CENTER, g_Column_Online_Data[i].nWidth);
		g_Column_Online_Width += g_Column_Online_Data[i].nWidth;       //得到总宽度
	}
	for (int i = 0; i < g_Column_Count_Message; i++)
	{
		m_CList_Message.InsertColumn(i, g_Column_Message_Data[i].title, LVCFMT_CENTER, g_Column_Message_Data[i].nWidth);
		g_Column_Message_Width += g_Column_Message_Data[i].nWidth;       //得到总宽度
	}

	return 0;
}


// 添加上线主机信息
void CRemoteControlDlg::AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing)
{
	m_CList_Online.InsertItem(0, strIP);           //默认为0行  这样所有插入的新列都在最上面
	m_CList_Online.SetItemText(0, ONLINELIST_ADDR, strAddr);      //设置列的显示字符   这里 ONLINELIST_ADDR等 为第二节课中的枚举类型 用这样的方法
	m_CList_Online.SetItemText(0, ONLINELIST_COMPUTER_NAME, strPCName); //解决问题会避免以后扩展时的冲突
	m_CList_Online.SetItemText(0, ONLINELIST_OS, strOS);
	m_CList_Online.SetItemText(0, ONLINELIST_CPU, strCPU);
	m_CList_Online.SetItemText(0, ONLINELIST_VIDEO, strVideo);
	m_CList_Online.SetItemText(0, ONLINELIST_PING, strPing);
	ShowMessage(true, strIP + "主机上线");
}

//添加日志消息的处理:
void CRemoteControlDlg::ShowMessage(bool bIsOK, CString strMsg)
{
	CString strIsOK, strTime;
	CTime t = CTime::GetCurrentTime();
	strTime = t.Format("%H:%M:%S");
	if (bIsOK)
	{
		strIsOK = "执行成功";
	}
	else{
		strIsOK = "执行失败";
	}
	m_CList_Message.InsertItem(0, strIsOK);
	m_CList_Message.SetItemText(0, 1, strTime);
	m_CList_Message.SetItemText(0, 2, strMsg);

	CString strStatusMsg;
	if (strMsg.Find("上线") > 0)         //处理上线还是下线消息
	{
		iCount++;
	}
	else if (strMsg.Find("下线") > 0)
	{
		iCount--;
	}
	else if (strMsg.Find("断开") > 0)
	{
		iCount--;
	}
	iCount = (iCount <= 0 ? 0 : iCount);         //防止iCount 有-1的情况
	strStatusMsg.Format("有%d个主机在线", iCount);
	m_wndStatusBar.SetPaneText(0, strStatusMsg, TRUE);   //在状态条上显示文字
}


// 伪造的上线信息，仅测试使用
void CRemoteControlDlg::test()
{
	ShowMessage(true, "软件初始化成功...");
	AddList("192.168.0.7", "本机局域网", "root", "Windows7", "3.2GHZ", "有", "10");
	AddList("192.168.0.8", "本机局域网", "root", "Windows7", "3.2GHZ", "有", "10");
	AddList("192.168.0.9", "本机局域网", "root", "Windows7", "3.2GHZ", "有", "10");
	
}


void CRemoteControlDlg::OnNMRClickOnline(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMITEMACTIVATE pNMItemActivate = reinterpret_cast<LPNMITEMACTIVATE>(pNMHDR);
	// TODO:  在此添加控件通知处理程序代码
	
	CMenu	popup;                  //声明一个菜单变量
	popup.LoadMenu(IDR_LIST);       //载入菜单资源
	CMenu*	pM = popup.GetSubMenu(0);  //得到菜单项
	CPoint	p;
	GetCursorPos(&p);                    //得到鼠标指针的位置
	int	count = pM->GetMenuItemCount();       //得到菜单的个数
	if (m_CList_Online.GetSelectedCount() == 0)  //如果没有选中列表中的条目
	{
		//	for (int i = 0; i < count - 2 ; i++)       //遍历每一个菜单
		//	{
		//		pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);  //该项变灰
		//	}
		//	//pM->EnableMenuItem(count - 1, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);

		//}

		//// 全选
		//if (m_CList_Online.GetItemCount() > 0)    //列表中的条目项大于0     
		//	pM->EnableMenuItem(count - 2, MF_BYPOSITION | MF_ENABLED);    //激活倒数第二个菜单 
		//else
		//	pM->EnableMenuItem(count - 2, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);  //否则变灰
		for (int i = 0; i < count; i++)       //遍历每一个菜单
		{
			pM->EnableMenuItem(i, MF_BYPOSITION | MF_DISABLED | MF_GRAYED);  //该项变灰
		}
	}

		pM->TrackPopupMenu(TPM_LEFTALIGN, p.x, p.y, this);        //在指定位置显示菜单

		*pResult = 0;
	
}


void CRemoteControlDlg::OnOnlineAudio()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("语音管理");
}


void CRemoteControlDlg::OnOnlineCmd()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("终端管理");
}


void CRemoteControlDlg::OnOnlineDesktop()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("桌面管理");
}


void CRemoteControlDlg::OnOnlineFile()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("文件管理");
}


void CRemoteControlDlg::OnOnlineProcess()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("进程管理");
}


void CRemoteControlDlg::OnOnlineRegedit()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("注册表管理");
}


void CRemoteControlDlg::OnOnlineRemote()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("远程管理");
}


void CRemoteControlDlg::OnOnlineServer()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("服务管理");
}


void CRemoteControlDlg::OnOnlineVideo()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("s视频管理");
}


void CRemoteControlDlg::OnOnlineWindow()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("窗口管理");
}


void CRemoteControlDlg::OnOnlineDelete()
{
	// TODO:  在此添加命令处理程序代码
	CString strIP;
	int iSelect = m_CList_Online.GetSelectionMark();
	strIP = m_CList_Online.GetItemText(iSelect, ONLINELIST_IP);
	m_CList_Online.DeleteItem(iSelect);
	strIP += " 主机断开连接";
	ShowMessage(true, strIP);
}


void CRemoteControlDlg::OnMainAbout()
{
	// TODO:  在此添加命令处理程序代码
	CAboutDlg dlgAbout;
	dlgAbout.DoModal();
}


void CRemoteControlDlg::OnMainBuild()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("生成服务端");
}


void CRemoteControlDlg::OnMainSet()
{
	// TODO:  在此添加命令处理程序代码
	MessageBox("参数设置");
}


void CRemoteControlDlg::OnMainClose()
{
	// TODO:  在此添加命令处理程序代码
	PostMessage(WM_CLOSE, 0, 0);
}


//创建字符ID的数组
static UINT indicators[] =
{
	IDS_STATUSBAR_STRING
};

// 创建状态栏
void CRemoteControlDlg::CreatStatusBar()
{
	if (!m_wndStatusBar.Create(this) ||
		!m_wndStatusBar.SetIndicators(indicators,
		sizeof(indicators) / sizeof(UINT)))                    //创建状态条并设置字符资源的ID
	{
		TRACE0("Failed to create status bar\n");
		return;      // fail to create
	}
	CRect rc;
	::GetWindowRect(m_wndStatusBar.m_hWnd, rc);
	m_wndStatusBar.MoveWindow(rc);                              //移动状态条到指定位置

}


// 添加托盘图标
void CRemoteControlDlg::AddNotify()
{
	nid.cbSize = sizeof(nid);     //大小赋值
	nid.hWnd = m_hWnd;           //父窗口
	nid.uID = IDR_MAINFRAME;     //icon  ID
	nid.uFlags = NIF_MESSAGE | NIF_ICON | NIF_TIP;   //托盘所拥有的状态
	nid.uCallbackMessage = UM_ICONNOTIFY;            //回调消息
	nid.hIcon = m_hIcon;                            //icon 变量
	CString str = "PCRemote远程协助软件.........";       //气泡提示
	lstrcpyn(nid.szTip, (LPCSTR)str, sizeof(nid.szTip) / sizeof(nid.szTip[0]));
	
	Shell_NotifyIcon(NIM_ADD, &nid);   //显示托盘
}

void CRemoteControlDlg::OnIconNotify(WPARAM wParam, LPARAM lParam)
{
	switch ((UINT)lParam)
	{
	case WM_LBUTTONDOWN: // click or dbclick left button on icon
	case WM_LBUTTONDBLCLK: // should show desktop
		if (!IsWindowVisible())
			ShowWindow(SW_SHOW);
		else
			ShowWindow(SW_HIDE);
		break;
	case WM_RBUTTONDOWN: // click right button, show menu
		CMenu menu;
		menu.LoadMenu(IDR_MENU_NOTIFY);
		CPoint point;
		GetCursorPos(&point);
		SetForegroundWindow();
		menu.GetSubMenu(0)->TrackPopupMenu(
			TPM_LEFTBUTTON | TPM_RIGHTBUTTON,
			point.x, point.y, this, NULL);
		PostMessage(WM_USER, 0, 0);
		break;
	}
}


void CRemoteControlDlg::OnNotifyClose()
{
	// TODO:  在此添加命令处理程序代码
	PostQuitMessage(0);
}


void CRemoteControlDlg::OnNotifyShow()
{
	// TODO:  在此添加命令处理程序代码
	ShowWindow(SW_SHOW);
}


void CRemoteControlDlg::OnClose()
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	Shell_NotifyIcon(NIM_DELETE, &nid); //销毁图标
	CDialogEx::OnClose();
}
