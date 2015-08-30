
// RemoteControlDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "include/IOCPServer.h"


// CRemoteControlDlg 对话框
class CRemoteControlDlg : public CDialogEx
{
// 构造
public:
	CRemoteControlDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_REMOTECONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	CStatusBar  m_wndStatusBar;              //状态条
	int iCount;							 //在线主机数
	NOTIFYICONDATA nid;					//托盘图标

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// 上线主机
	CListCtrl m_CList_Online;
	// 日志
	CListCtrl m_CList_Message;
	// 初始化列表
	int InitList();
	// 添加上线主机信息
	void AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing);
	void ShowMessage(bool bIsOK, CString strMsg);
	// 伪造的上线信息，仅测试使用
	void test();
	afx_msg void OnNMRClickOnline(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnOnlineAudio();
	afx_msg void OnOnlineCmd();
	afx_msg void OnOnlineDesktop();
	afx_msg void OnOnlineFile();
	afx_msg void OnOnlineProcess();
	afx_msg void OnOnlineRegedit();
	afx_msg void OnOnlineRemote();
	afx_msg void OnOnlineServer();
	afx_msg void OnOnlineVideo();
	afx_msg void OnOnlineWindow();
	afx_msg void OnOnlineDelete();
	afx_msg void OnMainAbout();
	afx_msg void OnMainBuild();
	afx_msg void OnMainSet();
	afx_msg void OnMainClose();

	//自定义消息
	afx_msg void OnIconNotify(WPARAM wParam, LPARAM lParam);
	// 创建状态栏
	void CreatStatusBar();
	// 添加托盘图标
	void AddNotify();
	afx_msg void OnNotifyClose();
	afx_msg void OnNotifyShow();
	afx_msg void OnClose();
protected:
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	void CRemoteControlDlg::Activate(UINT nPort, UINT nMaxConnections);
};
