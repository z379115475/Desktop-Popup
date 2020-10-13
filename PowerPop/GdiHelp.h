#pragma once

class CGdiHelp
{
public:
	static Gdiplus::Image* ImageFromIDResource(UINT resID, LPCTSTR resType)
	{
		//HINSTANCE hInst = AfxGetResourceHandle();
		HINSTANCE hInst = NULL;
		HRSRC hRsrc = ::FindResource (hInst, MAKEINTRESOURCE(resID), resType);
		if (!hRsrc)
			return NULL;
		// load resource into memory
		DWORD len = SizeofResource(hInst, hRsrc);
		BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);
		if (!lpRsrc)
			return NULL;
		// Allocate global memory on which to create stream
		HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);
		BYTE* pmem = (BYTE*)GlobalLock(m_hMem);
		memcpy(pmem,lpRsrc,len);
		IStream* pstm;
		CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);
		// load from stream
		Gdiplus::Image* ima = Gdiplus::Image::FromStream(pstm, TRUE);
		// free/release stuff
		GlobalUnlock(m_hMem);
		pstm->Release();
		FreeResource(lpRsrc);

		return ima;
	}
};