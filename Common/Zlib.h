#include "Utility.h"
#include "unzip.h" 

class CZlib { 
public: 
	CZlib(void); 
	~CZlib(void); 

public:  
	BOOL Zip_UnPackFiles(TString& mZipFileFullPath, TString& mUnPackPath); 

private: 
	VOID GetRelativePath(TString& pFullPath, TString& pSubString); 
	BOOL CreatedMultipleDirectory(LPCTSTR direct); 

private:
	HZIP hz;  
	ZRESULT zr; 
	ZIPENTRY ze;
	TString m_FolderPath;
	TString m_FolderName;
};