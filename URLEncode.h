/*******************************************
* URLCode.h
******************************************/
#pragma once
#include <string>
using namespace std;

class CURLCode 
{
public:
	CURLCode(){};
	virtual ~CURLCode(){};
	static string Encode(const char* lpszData)
	{

				pInTmp++;
		}

		return strResult;
	}

	static string Decode(string inTmp)
	{
		string strResult = "";
		int inlen = inTmp.length();
		for(int i=0;i<inlen;i++)
		{
			if(inTmp.at(i)=='%')
			{
				i++;
				char c = fromHex(inTmp.at(i++));
				c = c << 4;
				c += fromHex(inTmp.at(i));
				strResult += c;
			}
			else if(inTmp.at(i)=='+')
				strResult += ' ';
			else
				strResult += inTmp.at(i);
		}
		return strResult;
	}
	
	ddddddddddddddddddd
