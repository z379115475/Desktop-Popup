#pragma once
#include <GdiPlus.h>
#include "GdiHelp.h"

using namespace Gdiplus;
enum {

	btn_state_normal = 0,
	btn_state_over	 = 1,
	btn_state_down	 = 2
};

class KButton : public CWindowImpl<KButton, CButton>
{
	Image*  m_pNormalImage;
	Image*  m_pOverImage;
	Image*  m_pDownImage;
	CRect   m_rtButton;
	int		m_nState;
	BOOL	m_bOver;

public:
	KButton()
	{
		m_nState = btn_state_normal;
		m_bOver = FALSE;
	}

	~KButton()
	{

	}

protected:

	BEGIN_MSG_MAP(KButton)
		MESSAGE_HANDLER(WM_INITDIALOG,	OnInitDialog)
		MESSAGE_HANDLER(WM_DESTROY,		OnDestroy)
		MESSAGE_HANDLER(WM_PAINT,		OnPaint)
		MESSAGE_HANDLER(WM_LBUTTONDOWN, OnLButtonDown)
		MESSAGE_HANDLER(WM_LBUTTONUP,	OnLButtonUp)
		MESSAGE_HANDLER(WM_MOUSELEAVE,	OnMouseLeave)
		MESSAGE_HANDLER(WM_MOUSEMOVE,	OnMouseMove)
		MESSAGE_HANDLER(WM_LBUTTONDBLCLK, OnLButtonDbclick)
	END_MSG_MAP()

	LRESULT OnLButtonDbclick(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return TRUE;
	}

	LRESULT OnLButtonUp(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		::PostMessage(GetParent(), WM_COMMAND, IDC_CLOSE_BTN, NULL);
		m_nState = btn_state_normal;
		InvalidateRect(NULL);

		return TRUE;
	}

	LRESULT OnMouseLeave(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_bOver = FALSE;
		m_nState = btn_state_normal;

		InvalidateRect(NULL);

		return TRUE;
	}

	LRESULT OnMouseMove(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		if ( m_bOver == FALSE ) {
			m_nState = btn_state_over;
			m_bOver = TRUE;
			InvalidateRect(NULL);

			TRACKMOUSEEVENT   t_MouseEvent; 
			t_MouseEvent.cbSize       =     sizeof(TRACKMOUSEEVENT); 
			t_MouseEvent.dwFlags      =     TME_LEAVE|TME_HOVER; 
			t_MouseEvent.hwndTrack    =     m_hWnd; 
			t_MouseEvent.dwHoverTime  =     30; 
			::_TrackMouseEvent(&t_MouseEvent);
		}
		
		return TRUE;
	}

	LRESULT OnLButtonDown(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_nState = btn_state_down;
		InvalidateRect(NULL);
		return TRUE;
	}

	LRESULT OnPaint(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		CPaintDC dc(m_hWnd);

		CRect rect;
		GetClientRect( rect );
		Graphics graphics(dc);

		switch( m_nState )
		{
		case btn_state_down:
			graphics.DrawImage(m_pDownImage, 0, 0);
			break;
		case btn_state_over:
			graphics.DrawImage(m_pOverImage, 0, 0);
			break;
		default:
			graphics.DrawImage(m_pNormalImage, 0, 0);
			break;
		}
		
		return TRUE;
	}

	BOOL SubclassWindow(HWND hWnd, CRect &rect)
	{
		__super::SubclassWindow(hWnd);

		// 使用SetWindowPos代替MoveWindow，实现更简单

		SetWindowPos(NULL, rect.right - m_pNormalImage->GetWidth(), rect.top, m_pNormalImage->GetWidth(), m_pNormalImage->GetHeight(), SWP_NOZORDER);
		return TRUE;
	}

	LRESULT OnInitDialog(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		m_pNormalImage = CGdiHelp::ImageFromIDResource(IDB_PNG_NORMAL, TEXT("PNG"));
		m_pOverImage = CGdiHelp::ImageFromIDResource(IDB_PNG_OVER, TEXT("PNG"));
		m_pDownImage = CGdiHelp::ImageFromIDResource(IDB_PNG_DOWN, TEXT("PNG"));

		return TRUE;
	}

	LRESULT OnDestroy(UINT /*uMsg*/, WPARAM /*wParam*/, LPARAM /*lParam*/, BOOL& /*bHandled*/)
	{
		return 0;
	}
	
};