
// BomberManOnline.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "afxwinappex.h"
#include "afxdialogex.h"
#include "BomberManOnline.h"
#include "MainFrm.h"


#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CBomberManOnlineApp

BEGIN_MESSAGE_MAP(CBomberManOnlineApp, CWinAppEx)
	ON_COMMAND(ID_APP_ABOUT, &CBomberManOnlineApp::OnAppAbout)
END_MESSAGE_MAP()


// CBomberManOnlineApp 构造

CBomberManOnlineApp::CBomberManOnlineApp()
{
	m_bHiColorIcons = TRUE;

	// TODO: 将以下应用程序 ID 字符串替换为唯一的 ID 字符串；建议的字符串格式
	//为 CompanyName.ProductName.SubProduct.VersionInformation
	SetAppID(_T("BomberManOnline.AppID.NoVersion"));

	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}

// 唯一的一个 CBomberManOnlineApp 对象

CBomberManOnlineApp theApp;


// CBomberManOnlineApp 初始化

BOOL CBomberManOnlineApp::InitInstance()
{
	CWinAppEx::InitInstance();


	EnableTaskbarInteraction(FALSE);

	// 使用 RichEdit 控件需要  AfxInitRichEdit2()	
	// AfxInitRichEdit2();

	// 标准初始化
	// 如果未使用这些功能并希望减小
	// 最终可执行文件的大小，则应移除下列
	// 不需要的特定初始化例程
	// 更改用于存储设置的注册表项
	// TODO: 应适当修改该字符串，
	// 例如修改为公司或组织名
	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	InitContextMenuManager();

	InitKeyboardManager();

	InitTooltipManager();
	CMFCToolTipInfo ttParams;
	ttParams.m_bVislManagerTheme = TRUE;
	theApp.GetTooltipManager()->SetTooltipParams(AFX_TOOLTIP_TYPE_ALL,
		RUNTIME_CLASS(CMFCToolTipCtrl), &ttParams);

	// 若要创建主窗口，此代码将创建新的框架窗口
	// 对象，然后将其设置为应用程序的主窗口对象
	CMainFrame* pFrame = new CMainFrame;
	if (!pFrame)
		return FALSE;
	m_pMainWnd = pFrame;
	// 创建并加载框架及其资源
	pFrame->LoadFrame(IDR_MAINFRAME,
		WS_OVERLAPPEDWINDOW | FWS_ADDTOTITLE, NULL,
		NULL);

	//设置窗口大小
	m_pMainWnd->SetWindowPos(CWnd::FromHandle(HWND_NOTOPMOST),90,20,WINDOW_WIDTH + 11, WINDOW_HEIGHT + 32,
		SWP_FRAMECHANGED|SWP_SHOWWINDOW);
	m_pMainWnd->ShowWindow(SW_SHOW);
	m_pMainWnd->UpdateWindow();

	return TRUE;
}

int CBomberManOnlineApp::ExitInstance()
{
	//TODO: 处理可能已添加的附加资源
	return CWinAppEx::ExitInstance();
}

// CBomberManOnlineApp 消息处理程序


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

// 用于运行对话框的应用程序命令
void CBomberManOnlineApp::OnAppAbout()
{
	CAboutDlg aboutDlg;
	aboutDlg.DoModal();
}

// CBomberManOnlineApp 自定义加载/保存方法

void CBomberManOnlineApp::PreLoadState()
{
	BOOL bNameValid;
	CString strName;
	bNameValid = strName.LoadString(IDS_EDIT_MENU);
	ASSERT(bNameValid);
	GetContextMenuManager()->AddMenu(strName, IDR_POPUP_EDIT);
}

void CBomberManOnlineApp::LoadCustomState()
{
}

void CBomberManOnlineApp::SaveCustomState()
{
}

// CBomberManOnlineApp 消息处理程序



