#pragma once
#include "HttpCurl.h"
#include "common/xml/xmlconfig.h"
extern string urlbase;
class CNotifyInterface
{
public:
	CNotifyInterface(void);
	~CNotifyInterface(void);

private:
	static bool SendMsg(string type,string desc,string recvlist,string content="");

public:
	static bool LoadConfig(CXmlConfig& xmlConfig);
	static bool	SendSMSMsg(string desc,string recvlist);
	static bool SendMailMsg(string desc,string recvlist,string content);
	static bool SendOCMsg(string desc,string recvlist);

};
