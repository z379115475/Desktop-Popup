// PowerPop.cpp : main source file for PowerPop.exe
//

#include "stdafx.h"

#include "resource.h"
#include "Utility.h"
#include "Kernel.h"
#include "BrowserView.h"
#include "Kbutton.h"
#include "aboutdlg.h"
#include "MainDlg.h"

#pragma comment(lib, "gdiplus.lib")

CAppModule _Module;

struct GdiPlusInit
{
	ULONG_PTR m_gdiplusToken;
	GdiPlusInit()
	{
		Gdiplus::GdiplusStartupInput StartupInput;
		GdiplusStartup(&m_gdiplusToken, &StartupInput, NULL);
	}

	~GdiPlusInit()
	{
		Gdiplus::GdiplusShutdown(m_gdiplusToken);
	}
};

int Run(LPTSTR lpstrCmdLine = NULL, int nCmdShow = SW_SHOWDEFAULT)
{
	_S_OK(CKernel::GetInstance()->ParseCmdline(lpstrCmdLine));
	CMessageLoop theLoop;
	_Module.AddMessageLoop(&theLoop);

	CMainDlg dlgMain;

	if(dlgMain.Create(NULL) == NULL)
	{
		ATLTRACE(_T("Main dialog creation failed!\n"));
		return 0;
	}

	dlgMain.ShowWindow(SW_HIDE);

	int nRet = theLoop.Run();

	_Module.RemoveMessageLoop();
	return nRet;
}

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int nCmdShow)
{
	HRESULT hRes = ::CoInitialize(NULL);
// If you are running on NT 4.0 or higher you can use the following call instead to 
// make the EXE free threaded. This means that calls come in on a random RPC thread.
//	HRESULT hRes = ::CoInitializeEx(NULL, COINIT_MULTITHREADED);
	ATLASSERT(SUCCEEDED(hRes));

	// this resolves ATL window thunking problem when Microsoft Layer for Unicode (MSLU) is used
	::DefWindowProc(NULL, 0, 0, 0L);

	AtlInitCommonControls(ICC_BAR_CLASSES);	// add flags to support other controls

	GdiPlusInit Init;			// GdiPlus init
	hRes = _Module.Init(NULL, hInstance);
	ATLASSERT(SUCCEEDED(hRes));

	int nRet = Run(lpstrCmdLine, nCmdShow);

	_Module.Term();
	::CoUninitialize();

	return nRet;
}
