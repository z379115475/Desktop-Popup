#include "stdafx.h"
#include "Utility.h"
#include "CxLog.h"
#include "HardInfo.h"
#include "md5.h"
#include "CloudHelper.h"
#include "Kernel.h"

BOOL CKernel::ParseCmdline(TString strCmdLine)
{
	LOG1(TEXT("√¸¡Ó––:%s"), strCmdLine);
	_S_OK(GetKeyFormString(strCmdLine, TEXT("uname"), TEXT(" "), m_UName));
	_S_OK(GetKeyFormString(strCmdLine, TEXT("uid"), TEXT(" "), m_UID));
	_S_OK(GetKeyFormString(strCmdLine, TEXT("ip"), TEXT(" "), m_strIP));

	return TRUE;
}

BOOL CKernel::GetConfigUrl(TString &strOutUrl)
{
	TString url = TEXT("/ycgg/kjt.ini");
	TString host = DOMAIN_URL;

	CCloudHelper cloud;

	TString data;
	_S_OK(cloud.GetReq(host, url, data));

	return GetSubString(data, TEXT("url="), TEXT("\r\n"), strOutUrl);
}

BOOL CKernel::UploadData(TString type)
{
	CHardInfo info;

	TString strUrl = TEXT("/stat/show");
	TString strBody;
	strBody.Format(TEXT("id=%s&mac=%s&type=%s&user=%s&pw=%s"),
		m_UID,
		info.GetMac(),
		type,
		m_UName,
		CMd5::GetSignature(MD5SIG)
		);


	TString strRetData;

	CCloudHelper cloud;
	return cloud.PostReq(strUrl, strBody, strRetData);
}