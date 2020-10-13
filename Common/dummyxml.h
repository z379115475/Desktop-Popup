#pragma once

/*����ֻʵ��xml�ļ��Ľ���������дxml�ļ�����������Ƿǳ����صģ������ַ�������\0Ϊ��β������ԣ�
  ���еĽڵ���Ϣ������ָ��ԭʼxml�ļ���bufָ�빹�ɣ������Ĺ��̾��ǽ�����λ�õ��ַ��޸ĳ�\0,���������xml
  ������Ľ�����£�
  ԭʼxml
  <?xml version="1.0" encoding="UTF-8"?>
  <root>
		<user id="23" name="lhkyzh" type="com"/>
		<config>
			<run>yes</run>
			<p2p cudp="19034" sudp="19033" stcp="19042"/>
			<rename/>
			<exp/>
		</config>
		<projlist>
			<item id="id1" name="eda" uid="1234">1234</item>
			<item id="id2" name="mainpage" uid="mpid"/>
		</projlist>
  </root>
*/
/* �ڵ����� */
struct _tProperty
{
	LPCSTR key;
	LPCSTR value;
};

/* xml�ڵ��� */
class CXmlNode
{
private:
	
	LPSTR  m_Pos;
	LPCSTR m_name;
	LPCSTR m_text;
	vector<_tProperty> m_PropertyList;
	vector<CXmlNode*> m_SubList;

public:
	CXmlNode() { m_name = NULL; m_text = NULL; m_Pos = NULL;  }
	CXmlNode(LPSTR lpPos) { m_Pos = lpPos; }
	~CXmlNode()
	{
		for (size_t i = 0; i < m_SubList.size(); i++) {
			delete m_SubList[i];
		}
	}

	BOOL Parse();
	CXmlNode* GetSubNode(INT index);
	CXmlNode* GetSubNode(LPCTSTR lpNodeName);
	size_t GetSubNodeCount();
	BOOL GetProperty(LPCTSTR lpPropertyName, ATL::CString &strValue);
	BOOL GetText(ATL::CString& text);

private:
	inline BOOL Parse(LPSTR &lpEnd);
	inline BOOL ClearBlank();
	inline BOOL ParseHeadTag(LPSTR &lpEnd);
	inline BOOL ParseContent(LPSTR &lpEnd);
};

/* xml������ */
class CDummyXml
{
private:
	LPSTR m_lpFileContents;
	vector<CXmlNode*> m_NodeList;
	
public:
	CDummyXml()
	{ 
		m_rootNode = NULL;
		m_lpFileContents = NULL;
	}

	~CDummyXml()
	{
		if (m_lpFileContents) {
			delete m_lpFileContents;
		}
		for (size_t i = 0; i < m_NodeList.size(); i++) {
			delete m_NodeList[i];
		}
	}

	BOOL Parse();
	BOOL LoadFile(LPCTSTR lpFileName);
	CXmlNode* GetNode(DWORD dwCount, ...);
	CXmlNode* GetProjNode(LPCTSTR lpProjName);

private:
	CXmlNode* m_rootNode;
};