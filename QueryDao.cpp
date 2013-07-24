#include "QueryDao.h"
#include "common/log/LogMgr.h"

list<stGoodsInfo> CQueryDao::QueryGoodsList(const ::Trade::Goods& info,int page,int limit,int& sum)
{
	ostringstream ss;
	ss << info.nGameId;
	std::string game_id = ss.str();
	ss.str("");
	std::string where = "WHERE game_id="+game_id;

	if (info.strPtAccount != "")
	{
		where += " AND sdpt='"+info.strPtAccount+"'";
	}

	if (info.strCharacterName != "")
	{
		where += " AND character_name = '"+info.strCharacterName+"'";
	}

	if (info.nCharacterId != 0)
	{		
		ss << info.nCharacterId;
		std::string character_id = ss.str();
		ss.str("");
		where += " AND character_id = '"+character_id+"'";
	}

	if (info.nStartTime != 0)
	{
		ss << info.nStartTime;
		std::string start_time = ss.str();
		ss.str("");
		where += " AND create_time > "+start_time;
	}

	if (info.nEndTime != 0)
	{
		ss << info.nEndTime;
		std::string end_time = ss.str();
		ss.str("");
		where += " AND create_time < "+end_time;
	}

	if (info.nAreaId != 0)
	{
		ss << info.nAreaId;
		std::string area_id = ss.str();
		ss.str("");
		where += " AND area_id = "+area_id;
	}

	if (info.nGroupId != 0)
	{
		ss << info.nGroupId;
		std::string group_id = ss.str();
		ss.str("");
		where += " AND group_id = "+group_id;
	}

	if (info.strBookId != "")
	{
		where += " AND book_id = '"+info.strBookId+"'";
	}
/*
	if (info.nState != 0)
	{
		ss << info.nState;
		std::string state = ss.str();
		ss.str("");
		where += " AND state = "+state;
	}
*/
	char sql[2000];
	char get_sum_sql[2000];
	std::string strError;

	int cur_page = (page - 1) * limit;
	sprintf(get_sum_sql,"SELECT count(*) as mount FROM xcsProductCenter.`product` %s ",where.c_str());
	sprintf(sql,"SELECT * FROM xcsProductCenter.`product` %s LIMIT %d,%d",where.c_str(),cur_page,limit);

	LOGDEBUG("query order get_sum_sql:%s",get_sum_sql);
	LOGDEBUG("query order sql:%s",sql);

	MYSQL_RES *res = CDbConnectPool::Instance()->SelectRecord(get_sum_sql);
	if (res !=0 )
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		if (row)
		{
			sum = CDbConnectPool::Instance()->GetIntField(res,row,"mount");
		}
	}

	res = CDbConnectPool::Instance()->SelectRecord(sql);

	list<stGoodsInfo> goods_list;
	if (res != 0)
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		while (row)
		{
			stGoodsInfo good_info;
			good_info.game_id = CDbConnectPool::Instance()->GetIntField(res,row,"game_id");

			goods_list.push_back(good_info);
			row = CDbConnectPool::Instance()->MoveNext(res);
		}
		CDbConnectPool::Instance()->FreeRecord(res);
	}

	return goods_list;
}

bool CQueryDao::HandleException(const ::Trade::ExceptionInfo info)
{
	string dbname;
	if(info.nGameId == 777)
	{
		dbname= "xcsTradeCenter";
	}
	else if (info.nGameId  ==1)
	{
		dbname = "cqsjTradeCenter";
	}
	else
	{
		return false;
	}

	char sql[1000];
	sprintf(sql,"update %s.exceptions set result = '%s', state = 1 where id = %d",dbname.c_str(),info.result.c_str(), info.id);
	string error;
	if(CDbConnectPool::Instance()->ExcuteSql(sql,error) != 0)
	{
		LOGWARNING("HandleException error: %s",error.c_str());
		return false;
	}
	
	return true;
}

list<stException> CQueryDao::GetExceptionList(const ::Trade::ExceptionInfo info,int page, int limit, int & sum)
{
	ostringstream ss;
	ss << info.nGameId;
	std::string game_id = ss.str();
	ss.str("");
	std::string where = "WHERE b_game_id="+game_id;
	string dbname;
	list<stException> exceptionList;

	if(game_id == "777")
	{
		dbname= "xcsTradeCenter";
	}
	else if(game_id =="1")
	{
		dbname = "cqsjTradeCenter";
	}
	else
	{
		return exceptionList;
	}

	if (info.nAreaId != 0)
	{
		ss << info.nAreaId;
		std::string area_id = ss.str();
		ss.str("");
		where += " AND area_id = "+area_id;
	}

	if (info.nGroupId != 0)
	{
		ss << info.nGroupId;
		std::string group_id = ss.str();
		ss.str("");
		where += " AND group_id = "+group_id;
	}

	if (info.type != 0)
	{
		ss << info.type;
		std::string type = ss.str();
		ss.str("");
		where += " AND type = "+type;
	}

	if (info.startTime != "")
	{
		where += " AND create_time > '"+info.startTime  +"'";
	}

	if (info.endTime != "")
	{
		where += " AND create_time < '"+info.endTime+"'";
	}

	if (info.strOrderId != "")
	{
		where += " AND order_id = "+info.strOrderId;
	}

	if (info.strBookId != "")
	{
		where += " AND book_id = '"+info.strBookId+"'";
	}

	if(info.handled == "yes")
	{
		where += " AND state = 1";	
	}
	else
	{
		where += " AND state = 0";	
	}

	char sql[2000];
	char get_sum_sql[2000];
	std::string strError;

	int cur_page = (page - 1) * limit;
	sprintf(get_sum_sql,"SELECT count(*) as mount FROM %s.`exceptions` %s ",dbname.c_str(),where.c_str());
	sprintf(sql,"SELECT * FROM %s.`exceptions` %s LIMIT %d,%d",dbname.c_str(),where.c_str(),cur_page,limit);

	LOGDEBUG("query get_sum_sql:%s",get_sum_sql);
	LOGDEBUG("query sql:%s",sql);

	MYSQL_RES *res = CDbConnectPool::Instance()->SelectRecord(get_sum_sql);
	if (res !=0 )
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		if (row)
		{
			sum = CDbConnectPool::Instance()->GetIntField(res,row,"mount");
		}
	}

	res = CDbConnectPool::Instance()->SelectRecord(sql);


	if (res != 0)
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		while (row)
		{
			stException exception;
			exception.id			 = CDbConnectPool::Instance()->GetIntField(res,row,"id");
			exception.type			 = CDbConnectPool::Instance()->GetIntField(res,row,"type");
			exception.state			 = CDbConnectPool::Instance()->GetIntField(res,row,"state");
			exception.order_id		 = CDbConnectPool::Instance()->GetStringField(res,row,"order_id");
			exception.book_id		 = CDbConnectPool::Instance()->GetStringField(res,row,"book_id");
			exception.product_name	 = CDbConnectPool::Instance()->GetStringField(res,row,"product_name");
			exception.price			 = CDbConnectPool::Instance()->GetStringField(res,row,"price");
			exception.num			 = CDbConnectPool::Instance()->GetIntField(res,row,"num");
			exception.game_id		 = CDbConnectPool::Instance()->GetIntField(res,row,"game_id");
			exception.area_id		 = CDbConnectPool::Instance()->GetIntField(res,row,"area_id");
			exception.group_id		 = CDbConnectPool::Instance()->GetIntField(res,row,"group_id");
			exception.b_sdpt		 = CDbConnectPool::Instance()->GetStringField(res,row,"b_sdpt"); 
			exception.b_character_id	 = CDbConnectPool::Instance()->GetStringField(res,row,"b_character_id"); 
			exception.b_character_name	 = CDbConnectPool::Instance()->GetStringField(res,row,"b_character_name");
			exception.s_sdpt			 = CDbConnectPool::Instance()->GetStringField(res,row,"s_sdpt"); 
			exception.s_character_id	 = CDbConnectPool::Instance()->GetStringField(res,row,"s_character_id"); 
			exception.s_character_name	 = CDbConnectPool::Instance()->GetStringField(res,row,"s_character_name");
			exception.reason_desc		 = CDbConnectPool::Instance()->GetStringField(res,row,"reason_desc");
			exception.create_time		 = CDbConnectPool::Instance()->GetStringField(res,row,"create_time");
			exception.result			 = CDbConnectPool::Instance()->GetStringField(res,row,"result"); 
			exceptionList.push_back(exception);
			row = CDbConnectPool::Instance()->MoveNext(res);
		}
		CDbConnectPool::Instance()->FreeRecord(res);
	}

	return exceptionList;	
}

list<stOrderInfo> CQueryDao::QueryOrderList(const ::Trade::Order& info, int page, int limit,int& sum)
{
	ostringstream ss;
	ss << info.nBuyerGameId;
	std::string game_id = ss.str();
	ss.str("");
	std::string where = "WHERE b_game_id="+game_id;
	if(game_id == "0")
		where = "where b_game_id>=0";
	string dbname;

	if(info.strScope == "account")
	{
		dbname = "globalTradeCenter";
		//dbname = "TradeAccount";
	}
	else
	{
		if(game_id == "777" || game_id == "88")
		{
			dbname= "xcsTradeCenter";
		}
		else if(game_id == "1")
		{
			dbname= "cqsjTradeCenter";
		}
		else if(game_id == "78")
		{
			dbname= "aionTradeCenter";
		}
		else if(game_id == "89")
		{
			dbname= "lzgTradeCenter";
		}
	}

	if (info.strBuyerAccount != "")
	{
		where += " AND b_sdpt='"+info.strBuyerAccount+"'";
	}

	if (info.strBuyerCharacterName != "")
	{
		where += " AND b_character_name like '%"+info.strBuyerCharacterName+"%'";
	}

	if (info.buyerCharacterId != "")
	{		
		where += " AND b_character_id = '"+info.buyerCharacterId +"'";
	}

	if (info.strSellerAccount != "")
	{
		where += " AND s_sdpt = '"+info.strSellerAccount+"'";
	}

	if (info.strSellerCharacterName != "")
	{
		where += " AND s_character_name like '%"+info.strSellerCharacterName+"%'";
	}

	if (info.sellerCharacterId != "")
	{

		where += " AND s_character_id = "+info.sellerCharacterId;
	}

	if (info.startTime != "")
	{
		where += " AND create_time > '"+info.startTime  +"'";
	}

	if (info.endTime != "")
	{
		where += " AND create_time < '"+info.endTime+"'";
	}

	if (info.nBuyerAreaId != 0)
	{
		ss << info.nBuyerAreaId;
		std::string area_id = ss.str();
		ss.str("");
		where += " AND b_area_id = "+area_id;
	}

	if (info.nBuyerGroupId != 0)
	{
		ss << info.nBuyerGroupId;
		std::string group_id = ss.str();
		ss.str("");
		where += " AND b_group_id = "+group_id;
	}

	if (info.nSellerAreaId != 0)
	{
		ss << info.nSellerAreaId;
		std::string area_id = ss.str();
		ss.str("");
		where += " AND s_area_id = "+area_id;
	}

	if (info.nSellerGroupId != 0)
	{
		ss << info.nSellerGroupId;
		std::string group_id = ss.str();
		ss.str("");
		where += " AND s_group_id = "+group_id;
	}

	if (info.strOrderId != "")
	{
		where += " AND order_id = '"+info.strOrderId + "'";
	}

	if (info.strBookId != "")
	{
		where += " AND book_id = '"+info.strBookId+"'";
	}

	if (info.stateList != "")
	{
		where += " AND state in ("+info.stateList + ")";
	}

	if(info.strOrderTypeList != "")
	{
		where += " AND order_type in ("+info.strOrderTypeList + ")";
	}

	ss << info.nLocked;
	std::string lock = ss.str();
	ss.str("");
	where += " AND locked = "+lock;	

	char sql[2000];
	char get_sum_sql[2000];
	std::string strError;

	int cur_page = (page - 1) * limit;
	sprintf(get_sum_sql,"SELECT count(*) as mount FROM %s.`tOrder` %s ",dbname.c_str(),where.c_str());
	sprintf(sql,"SELECT * FROM %s.`tOrder` %s order by create_time desc LIMIT %d,%d",dbname.c_str(),where.c_str(),cur_page,limit);

	LOGDEBUG("query order get_sum_sql:%s",get_sum_sql);
	LOGDEBUG("query order sql:%s",sql);

	MYSQL_RES *res = CDbConnectPool::Instance()->SelectRecord(get_sum_sql);
	if (res !=0 )
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		if (row)
		{
			sum = CDbConnectPool::Instance()->GetIntField(res,row,"mount");
		}
	}

	res = CDbConnectPool::Instance()->SelectRecord(sql);

	list<stOrderInfo> order_list;
	if (res != 0)
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		while (row)
		{
			stOrderInfo order_info;
			order_info.order_id = CDbConnectPool::Instance()->GetStringField(res,row,"order_id");
			order_info.order_type =CDbConnectPool::Instance()->GetIntField(res,row,"order_type");
			order_info.book_id = CDbConnectPool::Instance()->GetStringField(res,row,"book_id");
			order_info.b_sdpt =  CDbConnectPool::Instance()->GetStringField(res,row,"b_sdpt");
			order_info.s_sdpt = CDbConnectPool::Instance()->GetStringField(res,row,"s_sdpt");
			order_info.b_character_id =  CDbConnectPool::Instance()->GetIntField(res,row,"b_character_id");
			order_info.s_character_id =  CDbConnectPool::Instance()->GetIntField(res,row,"s_character_id");
			order_info.b_character_name = CDbConnectPool::Instance()->GetStringField(res,row,"b_character_name");
			order_info.s_character_name = CDbConnectPool::Instance()->GetStringField(res,row,"s_character_name");
			order_info.goods_name = CDbConnectPool::Instance()->GetStringField(res,row,"goods_name");
			order_info.goods_type = CDbConnectPool::Instance()->GetIntField(res,row,"goods_type");
			order_info.num		  = CDbConnectPool::Instance()->GetIntField(res,row,"num");
			order_info.batches	  = CDbConnectPool::Instance()->GetIntField(res,row,"batches");
			order_info.price	  = CDbConnectPool::Instance()->GetStringField(res,row,"price");
			order_info.p_real_amount = CDbConnectPool::Instance()->GetStringField(res,row,"p_real_amount");
			order_info.b_fee = CDbConnectPool::Instance()->GetStringField(res,row,"b_fee");
			order_info.s_fee =  CDbConnectPool::Instance()->GetStringField(res,row,"s_fee");
			order_info.state = CDbConnectPool::Instance()->GetIntField(res,row,"state");
			order_info.b_game_id	= CDbConnectPool::Instance()->GetIntField(res,row,"b_game_id");
			order_info.b_area_id = CDbConnectPool::Instance()->GetIntField(res,row,"b_area_id");
			order_info.b_group_id = CDbConnectPool::Instance()->GetIntField(res,row,"b_group_id");
			order_info.s_game_id	= CDbConnectPool::Instance()->GetIntField(res,row,"s_game_id");
			order_info.s_area_id = CDbConnectPool::Instance()->GetIntField(res,row,"s_area_id");
			order_info.s_group_id = CDbConnectPool::Instance()->GetIntField(res,row,"s_group_id");
			order_info.pay_time = CDbConnectPool::Instance()->GetStringField(res,row,"pay_time");
			order_info.create_time = CDbConnectPool::Instance()->GetStringField(res,row,"create_time");
			order_info.update_time = CDbConnectPool::Instance()->GetStringField(res,row,"update_time");
			order_info.locked	   = CDbConnectPool::Instance()->GetIntField(res,row,"locked");
			order_info.backup	   = CDbConnectPool::Instance()->GetStringField(res,row,"backup");
			order_list.push_back(order_info);
			row = CDbConnectPool::Instance()->MoveNext(res);
		}
		CDbConnectPool::Instance()->FreeRecord(res);
	}

	return order_list;
}

list<stCharacterInfo> CQueryDao::QueryCharacterList(const ::Trade::Character info,int page, int limit,int& sum)
{
	ostringstream ss;
	list<stCharacterInfo> charcater_list;

	ss << info.nGameId;
	std::string game_id = ss.str();
	ss.str("");
	std::string where = "WHERE game_id="+game_id;

	if (info.strPtAccount != "")
	{
		ss << info.nGameId;
		std::string game_id = ss.str();
		ss.str("");
		where += " AND sdpt='"+info.strPtAccount+"'";
	}

	if (info.nAreaId != 0)
	{
		ss << info.nAreaId;
		std::string area_id = ss.str();
		ss.str("");
		where += " AND area_id="+area_id;
	}

	if (info.nGroupId != 0)
	{
		ss << info.nGroupId;
		std::string group_id = ss.str();
		ss.str("");
		where += " AND group_id="+group_id;
	}

	if (info.strCharacterName != "")
	{
		where += " AND character_name='"+info.strCharacterName+"'";
	}

	if (info.strCharacterId != "")
	{

		where += " AND character_id="+info.strCharacterId;
	}

	char sql[2000];
	char get_sum_sql[2000];
	std::string strError;

	int cur_page = (page - 1) * limit;
	sprintf(get_sum_sql,"SELECT count(*) as mount FROM UserCenter.`charactersActive` %s",where.c_str());
	sprintf(sql,"SELECT * FROM UserCenter.`charactersActive` %s LIMIT %d,%d",where.c_str(),cur_page,limit);

	LOGDEBUG("query order get_sum_sql:%s",get_sum_sql);
	LOGDEBUG("query order sql:%s",sql);

	MYSQL_RES *res = CDbConnectPool::Instance()->SelectRecord(get_sum_sql);
	if (res !=0 )
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		if (row)
		{
			sum = CDbConnectPool::Instance()->GetIntField(res,row,"mount");
		}
	}
	res = CDbConnectPool::Instance()->SelectRecord(sql);

	list<stOrderInfo> order_list;
	if (res != 0)
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		while (row)
		{
			stCharacterInfo character_info;
			character_info.game_id		= CDbConnectPool::Instance()->GetIntField(res,row,"game_id");
			character_info.area_id		= CDbConnectPool::Instance()->GetIntField(res,row,"area_id");
			character_info.group_id		= CDbConnectPool::Instance()->GetIntField(res,row,"group_id");
			character_info.character_id = CDbConnectPool::Instance()->GetStringField(res,row,"character_id");
			character_info.character_name = CDbConnectPool::Instance()->GetStringField(res,row,"character_name");
			character_info.sdid			= CDbConnectPool::Instance()->GetIntField(res,row,"sdid");
			character_info.sdpt			= CDbConnectPool::Instance()->GetStringField(res,row,"sdpt");
			character_info.create_time  = CDbConnectPool::Instance()->GetStringField(res,row,"create_time");
			character_info.last_login_time = CDbConnectPool::Instance()->GetStringField(res,row,"login_time");
			character_info.last_login_ip   = CDbConnectPool::Instance()->GetStringField(res,row,"login_ip");
			charcater_list.push_back(character_info);
			row = CDbConnectPool::Instance()->MoveNext(res);
		}
	}

	return charcater_list;
}

//查询指定数量的排行榜信息
vector<stGoodsInfo> CQueryDao::QueryRankGoodsType(int foodtype)
{
	char sql[2000];
	char get_sum_sql[2000];
	std::string strError;

	sprintf(sql,"SELECT ssg.game_id,ssg.area_id,ssg.group_id,ssg.goods_type,ssg.goods_id,ssg.quantity as soldQty ,IFNULL(sog.quantity,0) AS onsellQty \
		FROM xcsProductCenter.statSoldGoods ssg LEFT JOIN xcsProductCenter.statOnsellGoods sog \
		ON ssg.game_id = sog.game_id AND ssg.area_id = sog.area_id AND ssg.group_id = sog.group_id \
		AND ssg.goods_type = sog.goods_type AND ssg.goods_id = sog.goods_id\
		WHERE ssg.goods_type = %d\
		ORDER BY soldQty DESC LIMIT 27;",foodtype);

	LOGDEBUG("query order sql:%s",sql);

	vector<stGoodsInfo> goods_list;
	MYSQL_RES *res = CDbConnectPool::Instance()->SelectRecord(sql);
	if (res != 0)
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		while (row)
		{
			stGoodsInfo good_info;
			good_info.game_id = CDbConnectPool::Instance()->GetIntField(res,row,"game_id");
			good_info.area_id = CDbConnectPool::Instance()->GetIntField(res,row,"area_id");
			good_info.group_id = CDbConnectPool::Instance()->GetIntField(res,row,"group_id");
			good_info.goods_id = CDbConnectPool::Instance()->GetIntField(res,row,"goods_id");
			good_info.goods_type =CDbConnectPool::Instance()->GetIntField(res,row,"goods_type");
			goods_list.push_back(good_info);
			row = CDbConnectPool::Instance()->MoveNext(res);
		}
		CDbConnectPool::Instance()->FreeRecord(res);
	}

	return goods_list;
}

//查询排行榜中的商品列表
vector<stGoodsInfo> CQueryDao::QueryRankGoodsList(int gameid,int areaid,int groupid,int goodstype,int goodsid)
{
	char sql[2000];
	sprintf(sql,"SELECT goods_type,goods_id,product_name, amount,quantity,product_tips FROM xcsProductCenter.product \
		WHERE game_id = %d AND area_id = %d AND group_id = %d AND state IN (1,2) AND goods_type = %d and goods_id = %d \
		ORDER BY price ASC LIMIT 5;",gameid,areaid,groupid,goodstype, goodsid);

	LOGDEBUG("query order sql:%s",sql);

	vector<stGoodsInfo> goods_list;
	MYSQL_RES *res = CDbConnectPool::Instance()->SelectRecord(sql);
	if (res != 0)
	{
		MYSQL_ROW row = CDbConnectPool::Instance()->MoveFirst(res);
		while (row)
		{
			stGoodsInfo good_info;
			good_info.game_id = gameid;
			good_info.area_id = areaid;
			good_info.group_id = groupid;
			good_info.goods_id = goodsid;
			good_info.goods_type = goodstype;
			good_info.goods_name = CDbConnectPool::Instance()->GetStringField(res,row,"product_name");
			good_info.quantity = CDbConnectPool::Instance()->GetIntField(res,row,"quantity");
			good_info.price = CDbConnectPool::Instance()->GetIntField(res,row,"price");
			goods_list.push_back(good_info);
			row = CDbConnectPool::Instance()->MoveNext(res);
		}
		CDbConnectPool::Instance()->FreeRecord(res);
	}

	return goods_list;
}
