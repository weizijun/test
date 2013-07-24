#ifndef ORDERDAO_H
#define ORDERDAO_H
#include "database/DbConnectPool.h"
#include "trade_admin.h"
#include <list>

struct stGoodsInfo
{
	string	book_id;
	int		game_id;
	int		area_id;
	int		group_id;
	int		character_id;
	string	character_name;
	int		sdid;
	string	sdpt;
	string	product_name;
	string	product_tips;
	int		goods_type;
	int		goods_id;
	string	goods_name;
	float	price;
	int		quantity;
	int		state;
	string	create_time;
	string	update_time;

	stGoodsInfo()
	{
		game_id = 0;
		area_id = 0 ;
		group_id = 0;
		character_id = 0;
		sdid =0 ;
		goods_type =0;
		goods_id =0;
		price = 0.0;
		quantity =0;
		state =0;
	}	
};

struct stOrderInfo
{
	string	order_id;
	string	book_id;
	int     order_type;
	int		b_game_id;
	int		b_area_id;
	int		b_group_id;
	int		s_game_id;
	int		s_area_id;
	int		s_group_id;
	int		b_sdid;
	string	b_sdpt;
	int		b_character_id;
	string	b_character_name;
	int		s_sdid;
	string	s_sdpt;
	int		s_character_id;
	string	s_character_name;
	int		goods_id;
	int		goods_type;
	string	goods_name;
	string	pic_url;
	string	price;
	int		num;
	int		batches;
	string	productName;
	string  p_amount;
	string	p_real_amount;
	string  s_fee;
	string  b_fee;
	int		fee_type;
	int		state;
	int		locked;
	int		type;
	string	pay_id;
	string	goods_transfer_id;
	string	pay_time;
	string	create_time;
	string  update_time;
	string  backup;
};

struct stCharacterInfo
{
	int		game_id;
	int		area_id;
	int		group_id;
	string	character_id;
	string	character_name;
	int		sdid;
	string  sdpt;
	string  create_time;
	string	last_login_time;
	string  last_login_ip;
};

struct stException
{
	int			id;
	int			type;
	int			state;
	string		order_id;
	string		book_id;
	string      product_name;
	string		price;
	int			num;
	int			game_id;
	int			area_id;
	int			group_id;
	int			b_sdid;
	string		b_sdpt;
	string		b_character_id;
	string		b_character_name;
	int			s_sdid;
	string		s_sdpt;
	string		s_character_id;
	string		s_character_name;
	int			reason_code;
	string      reason_desc;
	string      result;
	string      create_time;

};

class CQueryDao
{
public:
	static list<stGoodsInfo> QueryGoodsList(const ::Trade::Goods& info,int page,int limit,int& sum);

	static list<stOrderInfo> QueryOrderList(const ::Trade::Order& info, int page, int limit,int& sum);
	static stOrderInfo OrderOrderInfo(const string& order_id);

	static list<stCharacterInfo>  QueryCharacterList(const ::Trade::Character info,int page, int limit,int& sum);

	//查询指定数量的排行榜商品类别信息
	static vector<stGoodsInfo> QueryRankGoodsType(int foodtype);
	//查询排行榜商品信息
	static vector<stGoodsInfo> QueryRankGoodsList(int gameid,int areaid,int groupid,int goodstype,int goodsid);

	//获取异常列表
	static list<stException>GetExceptionList(const ::Trade::ExceptionInfo info,int page, int limit, int & sum);
	static bool HandleException(const ::Trade::ExceptionInfo info);
};

#endif
