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

	m_Title = TEXT("Ӱ��");
	DWORD dwStyle = GetStyle();		//��ȡ����ʽ  
	DWORD dwNewStyle = WS_OVERLAPPED | WS_VISIBLE | WS_SYSMENU | WS_MINIMIZEBOX | WS_MAXIMIZEBOX | WS_CLIPCHILDREN | WS_CLIPSIBLINGS;
	dwNewStyle &= dwStyle;			//��λ�뽫����ʽȥ��  
	SetWindowLong(GWL_STYLE, dwNewStyle);		//���ó��µ���ʽ  
	DWORD dwExStyle = GetExStyle();				//��ȡ����չ��ʽ  
	DWORD dwNewExStyle = WS_EX_LEFT | WS_EX_LTRREADING | WS_EX_RIGHTSCROLLBAR;
	dwNewExStyle &= dwExStyle;					//��λ�뽫����չ��ʽȥ��  
	SetWindowLong(GWL_EXSTYLE, dwNewExStyle);	//�����µ���չ��ʽ  
	SetWindowPos(NULL, 0, 0, 0, 0, SWP_NOZORDER | SWP_NOMOVE | SWP_NOSIZE | SWP_FRAMECHANGED);//����windows���ҵ���ʽ�ı��ˣ�����λ�úʹ�С����ԭ�����䣡
	//ModifyStyleEx(WS_EX_APPWINDOW, WS_EX_TOOLWINDOW);		// ������������

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
	// ����Title
	rect.top += 6;
	rect.left += 23;
	HFONT hOldFont = hdc.SelectFont(AtlGetDefaultGuiFont());

	hdc.SetBkMode(TRANSPARENT);
	hdc.DrawText(m_Title, -1, rect, DT_LEFT | DT_VCENTER | DT_END_ELLIPSIS);
	hdc.SelectFont(hOldFont);

	return 0;
}