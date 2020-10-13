#pragma once

/*该类只实现xml文件的解析，不能写xml文件，他的设计是非常独特的，利用字符串是以\0为结尾这个特性，
  所有的节点信息都是由指向原始xml文件的buf指针构成，解析的过程就是将特殊位置的字符修改成\0,例如下面的xml
  解析后的结果如下：
  原始xml
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
/* 节点属性 */
struct _tProperty
{
	LPCSTR key;
	LPCSTR value;
};

/* xml节点类 */
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

/* xml解析类 */
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