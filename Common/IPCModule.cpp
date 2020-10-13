#include "stdafx.h"
#include "Unzip.h"
#include "IPCModule.h"

BOOL CIPCModule::Init()
{
	m_hPipeServer = CreateNamedPipe(
		PIPE_NAME,
		PIPE_ACCESS_INBOUND,
		PIPE_TYPE_MESSAGE | PIPE_WAIT,
		PIPE_UNLIMITED_INSTANCES,
		PIPE_BUFFER_SIZE,
		PIPE_BUFFER_SIZE,
		1000,				// 超时时间10分钟
		NULL
	);
	if (m_hPipeServer == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	return TRUE;
}

BOOL CIPCModule::SendMsg(MSGTYPE type, LPCTSTR data, INT length)
{
	HANDLE hPipeClient = CreateFile(PIPE_NAME, GENERIC_WRITE, FILE_SHARE_READ, NULL, OPEN_EXISTING, 0, NULL);
	if (hPipeClient == INVALID_HANDLE_VALUE) {
		return FALSE;
	}

	LPSTR msgbuf = new CHAR[sizeof(IPCMSG)+length];

	IPCMSG *pMsg = (IPCMSG *)msgbuf;
	pMsg->xttag = XTTAG;
	pMsg->type = type;
	pMsg->len = length;

	CopyMemory(pMsg->data, CT2A(data), length + 1);
	DWORD dwWrite;
	BOOL bRet = WriteFile(hPipeClient, msgbuf, sizeof(IPCMSG)+length, &dwWrite, NULL);
	delete[] msgbuf;
	CloseHandle(hPipeClient);

	return bRet;
}

BOOL CIPCModule::GetMsg(IPCMSG **msg)
{
	BOOL bRet = FALSE;
	LPSTR pBuf = new CHAR[PIPE_BUFFER_SIZE];
	ZeroMemory(pBuf, PIPE_BUFFER_SIZE);
	if (ConnectNamedPipe(m_hPipeServer, NULL)) {
		DWORD dwRead;
		bRet = ReadFile(m_hPipeServer, pBuf, PIPE_BUFFER_SIZE, &dwRead, NULL);
		*msg = (IPCMSG *)pBuf;
	}

	DisconnectNamedPipe(m_hPipeServer);

	return bRet;
}