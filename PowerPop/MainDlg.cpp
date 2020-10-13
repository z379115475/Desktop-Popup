// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "resource.h"
#include "Utility.h"
#include "BrowserView.h"
#include "aboutdlg.h"
#include "Kernel.h"
#include "Kbutton.h"
#include "MainDlg.h"

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CWindow::IsDialogMessage(pMsg);
}

BOOL CMainDlg::OnIdle()
{
	UIUpdateChildWindows();
	return FALSE;
}

LRESULT CMainDlg::OnInitDialog(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	// center the dialog on the screen
	CenterWindow();

	// set icons
	//HICON hIcon = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXICON), ::GetSystemMetrics(SM_CYICON));
	//SetIcon(hIcon, TRUE);
	m_hIconSmall = AtlLoadIconImage(IDR_MAINFRAME, LR_DEFAULTCOLOR, ::GetSystemMetrics(SM_CXSMICON), ::GetSystemMetrics(SM_CYSMICON));
	SetIcon(m_hIconSmall, FALSE);

	m_Title = TEXT("影视");
	DWORD dwStyle = GetStyle();		//获取旧样式  
	DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	dwNewStyle &= dwStyle;			//按位与将旧样式去掉  
	SetWindowLong(GWL_STYLE, dwNewStyle);		//设置成新的样式  
	DWORD dwExStyle = GetExStyle();				//获取旧扩展样式  
	DWORD dwNewExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
	dwNewExStyle &= dwExStyle;					//按位与将旧扩展样式去掉  
	SetWindowLong(GWL_EXSTYLE, dwNewExStyle);	//设置新的扩展样式  
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);//告诉windows：我的样式改变了，窗口位置和大小保持原来不变！
	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);		// 在任务栏隐藏

	CRect rect;
	GetClientRect(rect);
	m_CloseBtn.OnInitDialog(uMsg, wParam, lParam, bHandled);
	m_CloseBtn.SubclassWindow(GetDlgItem(IDC_CLOSE_BTN), rect);
	TString url;
	_S_OK(CKernel::GetInstance()->GetConfigUrl(url));
	
	rect.top += 24;
	rect.left += 1;
	m_IEView.Create(m_hWnd, rect, url, WS_CHILD | WS_VISIBLE | WS_CLIPSIBLINGS | WS_CLIPCHILDREN);
	// register object for message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->AddMessageFilter(this);
	pLoop->AddIdleHandler(this);

	UIAddChildWindowContainer(m_hWnd);

	return TRUE;
}

LRESULT CMainDlg::OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	// unregister message filtering and idle updates
	CMessageLoop* pLoop = _Module.GetMessageLoop();
	ATLASSERT(pLoop != NULL);
	pLoop->RemoveMessageFilter(this);
	pLoop->RemoveIdleHandler(this);

	return 0;
}

LRESULT CMainDlg::OnCancel(WORD /*wNotifyCode*/, WORD wID, HWND /*hWndCtl*/, BOOL& /*bHandled*/)
{
	CloseDialog(wID);
	return 0;
}

void CMainDlg::CloseDialog(int nVal)
{
	DestroyWindow();
	::PostQuitMessage(nVal);
}

LRESULT CMainDlg::OnTitleChange(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM lParam, BOOL& /*bHandled*/)
{

	m_Title = (LPCTSTR)lParam;

	return 0;
}

LRESULT CMainDlg::OnLoadComplete(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	static BOOL isUpload = FALSE;
	if (isUpload) {
		return 0;
	}
	ShowWindow(SW_SHOW);
	isUpload = TRUE;
	return CKernel::GetInstance()->UploadData(TEXT("show"));
}

LRESULT CMainDlg::OnNewWindow(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return CKernel::GetInstance()->UploadData(TEXT("click"));
}

LRESULT CMainDlg::OnNcHitTest(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	return HTCAPTION;
}

LRESULT CMainDlg::OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
{
	CPaintDC hdc(m_hWnd);

	CRect rect;
	GetClientRect(rect);
	//Graphics graphics(dc);

	//graphics.DrawImage(m_pDownImage, 0, 0);
	//DrawIconEx(dc, 372, 0, hSmall, 16, 16, 0, NULL, DI_NORMAL);

	DrawIconEx(hdc, 4, 4, m_hIconSmall, 16, 16, 0, NULL, DI_NORMAL);
	// 更新Title
	rect.top += 6;
	rect.left += 23;
	HFONT hOldFont = hdc.SelectFont(AtlGetDefaultGuiFont());

	hdc.SetBkMode(TRANSPARENT);
	hdc.DrawText(m_Title, -1, rect, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
	hdc.SelectFont(hOldFont);

	return 0;
}