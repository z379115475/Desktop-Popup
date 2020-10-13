#include "stdafx.h"
#include "Utility.h"
#include <stdarg.h> 
#include "dummyxml.h"

CXmlNode* CXmlNode::GetSubNode(INT index)
{
	if (index < m_SubList.size()) {
		return m_SubList[index];
	}

	return NULL;
}

CXmlNode* CXmlNode::GetSubNode(LPCTSTR lpNodeName)
{
	CT2A name(lpNodeName);

	for (size_t i = 0; i < m_SubList.size(); i++) {
		if (strcmp(name, m_SubList[i]->m_name) == 0) {
			return m_SubList[i];
		}
	}

	return NULL;
}

size_t CXmlNode::GetSubNodeCount()
{
	return m_SubList.size();
}

BOOL CXmlNode::GetProperty(LPCTSTR lpPropertyName, ATL::CString &strValue)
{
	CT2A key(lpPropertyName);
	for (size_t i = 0; i < m_PropertyList.size(); i++) {
		if (strcmp(key, m_PropertyList[i].key) == 0) {
			strValue = CA2T(m_PropertyList[i].value);
			return TRUE;
		}
	}

	return FALSE;
}

BOOL CXmlNode::ClearBlank()
{
	while (*m_Pos == ' ' || *m_Pos == '\r' || *m_Pos == '\n' || *m_Pos == '\t') {
		*(m_Pos++) = '\0';
	}

	return TRUE;
}

BOOL CXmlNode::ParseHeadTag(LPSTR &lpEnd)
{
	BOOL bRet = FALSE;
	m_Pos++;
	ClearBlank();		// 清除空白字符

	m_name = m_Pos;		// 保存名字
	while (*m_Pos != ' ' && *m_Pos != '/' && *m_Pos != '>') m_Pos++;

	while (*m_Pos) {
		if (*m_Pos == ' ') {			// <item id="业务1"/>
			ClearBlank();
			continue;
		}
		else if (*m_Pos == '>') {		// <run>yes</run>
			bRet = TRUE;
			break;
		}
		else if (*m_Pos == '/') {		// <exp/>				 空标记
			*m_Pos++ = '\0';
			break;
		}
		else if (*m_Pos == '<') {
			break;
		}
		else if ((*m_Pos >= 'a' && *m_Pos <= 'z') ||
			(*m_Pos >= 'A' && *m_Pos <= 'Z') ||
			(*m_Pos >= '0' && *m_Pos <= '9')) {
			_tProperty prop;
			prop.key = m_Pos;
			while (*++m_Pos != '=');
			*m_Pos = '\0';
			while (*++m_Pos != '"');
			prop.value = m_Pos+1;
			while (*++m_Pos != '"');
			*m_Pos = '\0';
			m_PropertyList.push_back(prop);
		}

		m_Pos++;
	}

	*(m_Pos++) = '\0';
	lpEnd = m_Pos;
	return bRet;
}

BOOL CXmlNode::ParseContent(LPSTR &lpEnd)
{
	BOOL bRet = FALSE;
NEXT:
	ClearBlank();

	if (*m_Pos == '<') {
		if (*(m_Pos+1) == '/') {		// 节点结束
			while (*m_Pos++ != '>');
			lpEnd = m_Pos;
			bRet = TRUE;
		}
		else {				// 包含子节点的处理
			CXmlNode *sub = new CXmlNode(m_Pos);
			m_SubList.push_back(sub);
			sub->Parse(m_Pos);
			goto NEXT;
		}
	}
	else {
		m_text = m_Pos;
		while (*++m_Pos != '<');
		*m_Pos = '\0';
		while (*m_Pos++ != '>');
		lpEnd = m_Pos;
		bRet = TRUE;
	}

	return bRet;
}

BOOL CXmlNode::Parse(LPSTR &lpEnd)
{
	BOOL bRet = FALSE;

	if (!ParseHeadTag(lpEnd)) {
		return TRUE;
	}

	return ParseContent(lpEnd);
}

BOOL CXmlNode::Parse()
{
	return Parse(m_Pos);
}

BOOL CXmlNode::GetText(ATL::CString& text)
{
	text = m_text;
	return (m_text == NULL) ? FALSE:TRUE;
}

BOOL CDummyXml::LoadFile(LPCTSTR lpFileName)
{
	if (m_lpFileContents) {
		return FALSE;
	}
	HANDLE hFile = CreateFile(lpFileName, GENERIC_READ, FILE_SHARE_READ, NULL, OPEN_EXISTING, NULL, NULL);
	if (hFile == INVALID_HANDLE_VALUE) {
		return FALSE;
	}
	CHandleRelease ResHandle(hFile);

	DWORD dwFileSize = GetFileSize(hFile, 0);    // 获取文件的大小
	if (dwFileSize == 0xFFFFFFFF){
		return FALSE;
	}

	m_lpFileContents = new CHAR[dwFileSize];

	BOOL bRet = FALSE;
	DWORD dwTotal = 0;
	do {
		DWORD lpReadNumberOfBytes;
		bRet = ReadFile(hFile, m_lpFileContents, dwFileSize, &lpReadNumberOfBytes, NULL);
		if (!bRet) {
			break;
		}
		dwTotal += lpReadNumberOfBytes;
		if (dwTotal == dwFileSize) {
			break;
		}
	} while (bRet);

	return bRet;
}

CXmlNode* CDummyXml::GetProjNode(LPCTSTR lpProjName)
{
	if (m_rootNode == NULL) {
		return FALSE;
	}

	CXmlNode* pProjNode = m_rootNode->GetSubNode(TEXT("projlist"));
	if (pProjNode == NULL) {
		return FALSE;
	}

	for (size_t i = 0; i < pProjNode->GetSubNodeCount(); i++) {
		CXmlNode *pSubNode = pProjNode->GetSubNode(i);
		if (pSubNode && pSubNode->GetSubNode(TEXT("name"))) {
			TString text;
			if (pSubNode->GetSubNode(TEXT("name"))->GetText(text) && text == lpProjName) {
				return pSubNode;
			}
		}
	}

	return NULL;
}

BOOL CDummyXml::Parse()
{
	m_rootNode = GetNode(1, TEXT("root"));
	if (m_rootNode == NULL) {
		return FALSE;
	}

	return m_rootNode->Parse();
}

CXmlNode* CDummyXml::GetNode(DWORD dwCount, ...)
{
	CXmlNode * pNode = NULL;
	LPCTSTR lpTag = NULL;
	LPSTR lpCurPos = m_lpFileContents;
	va_list args;
	va_start(args, dwCount);
	while (dwCount--) {
		lpTag = va_arg(args, LPCTSTR);
		ATL::CString strTag = _T("<");
		strTag += lpTag;
		lpCurPos = strstr(lpCurPos, CT2CA(strTag));
		if (lpCurPos == NULL) {
			break;
		}
	}
	va_end(args);

	if (lpCurPos) {
		pNode = new CXmlNode(lpCurPos);
		m_NodeList.push_back(pNode);
	}
	
	return pNode;
}