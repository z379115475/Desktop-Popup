#pragma once

class CKernel
{
	TString m_UName;
	TString m_UID;
	TString m_strIP;

public:
	~CKernel() {}

	static CKernel* GetInstance()
	{
		static CKernel obj;
		return &obj;
	}
	
	BOOL ParseCmdline(TString strCmdLine);
	BOOL GetConfigUrl(TString &strOutUrl);
	BOOL UploadData(TString type);

private:
	CKernel() {}
};