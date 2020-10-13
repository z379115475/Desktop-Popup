#pragma once
#include "IPCModule.h"

class CHideProcModule
{
public:
	static BOOL HideProcess(UINT dwProcID, LPCTSTR lpToke);
};

typedef struct _tPid_Toke
{
	INT pid;
	TString toke;
} PID_TOKE;

class CHideProcServer
{
public:
	static CHideProcServer* GetInstance() 
	{
		static CHideProcServer obj;

		return &obj;
	}
	~CHideProcServer() {}

public:
	BOOL LoadHideModule(TString &strPath);
	BOOL ProcessMsg(IPCMSG* pMsg);
	BOOL HideFolder(LPCTSTR lpFolderName);
	BOOL HideProcess(PID_TOKE &pt);
	
private:
	CHideProcServer()  {}
	HMODULE m_hModule;
	vector<PID_TOKE> vPidTbl;
};
