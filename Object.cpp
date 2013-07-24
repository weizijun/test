/********************************************************************
	created:	2010/09/07
	created:	7:9:2010   13:59
	filename: 	d:\Project\NMNetLib\NMNetLib\src\Object.cpp
	file path:	d:\Project\NMNetLib\NMNetLib\src
	file base:	Object
	file ext:	cpp
	author:		Eric.wang
	
	purpose:	�������
*********************************************************************/
#include "Object.h"

CObj::CObj()
{	
}

CObj::~CObj()
{
}

CObj::CObj(string strName)
{
	m_strName=strName;
}

string& CObj::GetName()
{
	return m_strName;
}

void	CObj::SetName(const char* lpName)
{
	m_strName=lpName;
}

void	CObj::SetName(const string strName)
{
	m_strName=strName;
}

string& CObj::GetDesc()
{
	return m_strDesc;
}

void	CObj::SetDesc(const char* lpDesc)
{
	m_strDesc=lpDesc;
}

