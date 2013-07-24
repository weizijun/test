#pragma once
#include <string>
using namespace std;
#pragma warning(disable: 4251)

class CObj
{
public:
	CObj();
	CObj(string strName);
	~CObj();

public:
	string&		GetName();
	void		SetName(const char* lpName);
	void		SetName(const string strName);
	string&		GetDesc();
	void		SetDesc(const char* lpDesc);

protected:
	string		m_strName;
	string		m_strDesc;
};


