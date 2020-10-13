#pragma once
#include <string>
#include <vector>
#include <map>
#include <WinSock2.h>
#include <process.h>
// C 运行时头文件
#include <stdlib.h>
#include <malloc.h>
#include <memory.h>
#include <tchar.h>
#include <Shellapi.h>
#include <Shlwapi.h>

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS      // 某些 CString 构造函数将是显式的

#include <atlbase.h>
#include <atlstr.h>

// TODO:  在此处引用程序需要的其他头文件
#include "ResTool.h"

#pragma comment(lib,"ws2_32.lib")
#pragma comment(lib,"Shell32.lib")
#pragma comment(lib,"Shlwapi.lib")

using namespace std;

#define TString ATL::CString

#define _S_OK(a) if (!a) return FALSE
#define _S_CONTINUE(a) if (!a) continue

#define MD5SIG				TEXT("awangba.com")

#define PIPE_NAME			TEXT("\\\\.\\pipe\\wangbax")
#define PIPE_BUFFER_SIZE	1024

#define TONGJI_URL          TEXT("tongji.52wblm.com")       // 数据上报
#define DOMAIN_URL          TEXT("domain.52wblm.com")       // 配置文件
#define MAINCTRL_XML		TEXT("MainCtrl.xml")			// 业务配置文件

#define P2P_FILENAME		TEXT("filename")
#define P2P_PLATFORM		TEXT("platform")
#define PLATFORM_ALL		TEXT("all")
#define PLATFORM_X86		TEXT("x86")
#define PLATFORM_X64		TEXT("x64")
#define P2P_MD5				TEXT("md5")
#define P2P_PATH			TEXT("path")
#define WB_PROJXML_NAME		TEXT("FileList")

#define P2P_CLIENT_PACKLEN	32768		// 数据包长度
#define P2P_SERVER_PACKLEN	65536		// 数据包长度
#define P2P_PACKBUFFER_LEN	65536		// 数据包长度
#define MAX_CLIENT_COUNT    5			// 最大客户端连接数

#define BUFFER_SIZE_1K		1024		//1K缓冲区大小
#define BUFFER_SIZE_4K		4096		//4K缓冲区大小

typedef struct _t_IPINFO {
	TString ip;
	USHORT port;
}IPINFO;

typedef enum {
	FINIT_STATUS = 0,		// 初期状态
	FUNDOWNLOAD,			// 业务文件包未下载
	FDOWNLOAD_OK,			// 业务文件包下载成功
	FDOWNLOAD_NG,			// 业务文件包下载失败
	FMD5CHECK_OK,			// MD5 Check成功
	FMD5CHECK_NG,			// MD5 Check失败
	FUNZIP_OK,				// 业务文件包解压成功
	FUNZIP_NG,				// 业务文件包解压失败
	FRUN_OK,				// 业务文件包运行成功
	FRUN_NG,				// 业务文件包运行失败
	FDELFILE				// 排除这个文件
} FSTATUS;

// 文件信息
typedef struct _t_FILEINFO{
	TString fileid;			// 文件id
	TString filename;		// 文件名
	TString md5;			// MD5
	TString downaddr;		// 下载URL地址
	TString bootcmd;		// 引导命令
	TString platform;		// 平台
	TString localpath;		// 存放路径
	FSTATUS	status;			// 文件状态
} FILEINFO;

BOOL CreateHomeFolder(TString &strOutPath);
BOOL GetKeyValue(TString szFileName, TString &szSection, TString szKeyName, TString &strValue);
BOOL SetKeyValue(TString szFileName, TString &szSection, TString szKeyName, TString &strValue);
BOOL GetUsername(TString& strUserName);
BOOL ToLower(TString &strSrc);
BOOL RemoveFolder(LPCTSTR lpszPath);
BOOL Is64BitOpSystem();
BOOL IsXPAboveOS();
BOOL SetPrivilege(HANDLE hProcess, LPCTSTR lpszPrivilege, BOOL bEnablePrivilege);
BOOL GetCurrentPath(TString& strCurPath);
BOOL GetCurrentPath(LPTSTR szCurPath, DWORD dwSize);
BOOL AnalyzeCmdLine(LPTSTR lpCmdLine, map<TString, TString> &paramList);
BOOL SplitString(LPTSTR lpszSrc, LPTSTR lpszTag, vector<TString> &strArray);
BOOL SplitString(TString &strSrc, LPTSTR lpszTag, vector<TString> &strArray);
BOOL ReNameDirectory(TString &pFrom, TString &pTo);
BOOL StringToKeyValue(TString& strSrc, map<TString, TString> &mapOutKeyVal);
BOOL GetTxtByXmlTag(TString content, TString left, TString right, TString& txt);
BOOL GetTempPathString(TString &strTempPath);
BOOL GetSystemPathString(TString &strSystemPath);
BOOL GetTxtByXmlTag(TString content, TString left, TString right, TString& txt);
BOOL GetSubString(TString &strOring, LPCTSTR lpLeftTag, LPCTSTR lpRightTag, TString &strSub);
BOOL GetKeyFormString(TString& lpContent, LPCTSTR lpKey, LPCTSTR lpTag, TString &strValue);
CHAR* GetLocalIP();
INT GetRadomNum(DWORD dwMax);
BOOL DeleteUrlCache();
BOOL Encrypt(LPCTSTR lpOrgStr, LPTSTR lpOutStr, INT len);
BOOL Decryption(LPCTSTR lpOrgStr, LPTSTR lpOutStr, INT len);
TString DosDevicePath2LogicalPath(LPTSTR lpszDosPath, INT len);