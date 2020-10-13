#pragma once
#include "Utility.h"

typedef struct _t_UserParam {
	TString strUserName;
	TString strUserId;
	TString strHomePath;
	TString strUserType;
} USERPARAM;

#define XTTAG	0xCC00
enum MSGTYPE 
{
	NEWUSER,			// 传递用户信息
	HIDEPROC			// 进程隐藏
};

typedef struct _tMsg
{
	INT xttag;
	INT type;
	INT len;
	CHAR data[1];
} IPCMSG;

class CIPCModule
{
public:
	static CIPCModule* GetInstance()
	{
		static CIPCModule obj;

		return &obj;
	}
	~CIPCModule() {}

	BOOL Init();
	BOOL SendMsg(MSGTYPE type, LPCTSTR data, INT length);
	BOOL GetMsg(IPCMSG **msg);

private:
	CIPCModule() {}

private:
	HANDLE m_hPipeClient;
	HANDLE m_hPipeServer;
};