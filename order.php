<?php

require_once 'control.php';	

class Order extends Control {

	public function __construct() {
		parent::__construct ();
		$this->_checkLogin ();
		$this->load->helper ( 'url' );
	}
	
	public function detail($bookid, $page_type = 10, $page = "detail") {
		
		$this->load->model ( "UserInfo" );
		$userData = $this->UserInfo->getAllData ();
		$matrixId = $userData ["matrixid"];
		
		$this->load->model ( "ItemInfo" );
		$item_res = $this->ItemInfo->getItemInfo ( $bookid, $matrixId );		
		if (empty($item_res) || $item_res["return_code"] != 0)
		{
			$this->_error_show("查看物品详情失败");
		}	
		$itemData = $item_res["data"];
		
		$itemData ["str_goods_type"] = $this->ItemInfo->get_product_type ( $itemData ["product_type"] );
		$tips = $this->ItemInfo->deal_tips ( $itemData ["product_tips"] );
		$itemData["real_price"] = $itemData["price"];
		$itemData ["tips"] = $this->ItemInfo->setTips ( $itemData );
		
		if ($this->config->item("publication") == $page_type) {
			$this->load->model ( "trade/AdminAgent" );
			$this->load->model ( "AdminInfo" );
			
			$fee_type = $this->config->item ( "buyer_type" );
			$fee_result = $this->AdminAgent->GetFeeRate ( $fee_type );
			
			if (empty ( $fee_result ) || $fee_result ["return_code"] != "0") {
				$this->_error_show ( "获得手续费率失败" );
			}
			
			$fee_rate = $fee_result ["data"] ["item"]["fee_rate"];	
			$fee_min = $fee_result["data"] ["item"]["min"];
			$fee_max = $fee_result["data"] ["item"]["max"];
			
			$itemData ["fee"] = $fee_rate * $itemData ["price"];			
			$itemData ["fee"] = $this->AdminInfo->get_fee($itemData ["fee"],$fee_max,$fee_min);

 			$itemData["price"] += $itemData["fee"];
 			
 			
		}

		$params = array( 
			"page_type" => $page_type 
		);
		$head = $this->UserData->headData ( $params );
	
		$data = array( 
			"item_data" => $itemData, 
			"user_data" => $userData,
			"tips"		=> $tips,
		);

		
		//设置前端页面
		$view_path = $this->_select_view();		
		$this->load->view ( $view_path."frame/head", $head );		
		$this->load->view ( $view_path."order/" . $page, $data );
		$this->load->view ( $view_path."frame/foot" );
	}

	//得到验证码
	public function getCaptcha() {
		$this->load->library ( 'securimage' );
		$this->securimage->image_width = 100;
		$this->securimage->image_height = 37;
		$this->securimage->num_lines = 0;
		$this->securimage->perturbation = 1;
		$this->securimage->code_length = 4;
		$this->securimage->show ();
	}

	//验证用户输入的验证码
	public function docheckcaptcha() {
		$this->load->library ( 'securimage' );
		$captcha = $this->input->get ( "captcha" );
		//d($captcha);
		if ($this->securimage->check ( $captcha ) == false) {
			echo "error";
		} else {
			echo "yes";
		}
		exit ();
	}

	/*	
	private function _getCaptcha() {

		$this->load->helper('captcha');
		$this->load->helper('url');
		$vals = array(
				//'word'	 => ''.rand(),
				'img_path'	 => './captcha/',
				'img_url'	 =>  base_url().'/captcha/',
		//	'font_path'	 => './path/to/fonts/texb.ttf',
				'img_width'	 => '60',
				'img_height' => 27,
				'expiration' => 7200
		);
		
		$cap = create_captcha($vals);
		
		return  $cap['image'];
	}		
*/
	
	public function goldDetail() {
		$book_id = $this->input->get ( "book_id" );
		if (empty ( $book_id )) {
			$this->_error_show ( "购买失败" );
		}
		$type = $this->config->item ( "gold" );
		$this->detail ( $book_id, $type );
	}

	public function moneyDetail() {
		$book_id = $this->input->get ( "book_id" );
		if (empty ( $book_id )) {
			$this->_error_show ( "购买失败" );
		}
		$type = $this->config->item ( "money" );
		$this->detail ( $book_id, $type );
	}

	public function itemDetail() {
		$book_id = $this->input->get ( "book_id" );
		if (empty ( $book_id )) {
			$this->_error_show ( "购买失败" );
		}
		
		$type = $this->config->item ( "item" );
		$this->detail ( $book_id, $type, "item_detail_page" );
	}

	public function publicationDetail() {
		$book_id = $this->input->get ( "book_id" );
		$type = $this->input->get("type");
		$item = $this->config->item ( "item" );
		
		if ($type == $item) {
			$page = "item_detail_page";
		}
		else {
			$page = "detail";
		}
		
		if (empty ( $book_id )) {
			$this->_error_show ( "购买失败" );
		}	
		
		$page_type = $this->config->item ( "publication" );
		$this->detail ( $book_id, $page_type,$page );
	}
	
	public function storedetail() {
		$product_id = $this->input->get ( "product_id" );
		if (empty ( $product_id ) || ! is_numeric ( $product_id )) {
			$this->_error_show ( "获取商品失败" );
		}
	
		$num = $this->input->get ( "num" );
		if (empty ( $num ) || ! is_numeric ( $num ) || $num <= 1) {
			$num = 1;
		}
		$num = ( int ) $num;
	
	
		$max_num = $this->config->item("max_store_num");
		if ($num > $max_num) {
			$num = $max_num;
		}
	
		$this->load->model ( "trade/AdminAgent" );
		$result = $this->AdminAgent->StoreGoodsDetail ( $product_id );
	
		if (empty ( $result ) || $result ["return_code"] != 0) {
			$this->_error_show ( "获取商品失败" );
		}
	
		$item_data = $result ["data"] ["data"];
	
		$this->load->model ( "trade/TradeAgent" );
		$conpon_result = $this->TradeAgent->UserCouponList ( false, 1 );
		if (empty ( $conpon_result ) || $conpon_result ["return_code"] != 0) {
			$this->_error_show ( "获取优惠券失败" );
		}

		log_message("error","storedetail mark");
	
		$tips_arr = explode ( "\\\\", $item_data ["product_tips"] );
		$tips = "";
		foreach ( ( array ) $tips_arr as $key => $val ) {
			$val = str_replace ( "　　", "", $val );
			$tips .= '<span class="text1">' . $val . '</span>';
		}
	
		$item_data ["tips"] = $tips;
	
		//获得物品库存
		if ($item_data["sell_limit"] == "-1") {
			$sell_num = "充足";
		} else {
			$sell_num = ": ";
			$sell_num .= $item_data["sell_limit"] - $item_data["sell_num"];
			$sell_num .= "件 ";
		}
		
		// 选择默认的优惠卷
		//$store_flag = $this->UserInfo->getStoreFlag();
		//有优惠券且在能使用的区服且是能使用优惠券的物品
		if (count ( $conpon_result ["data"] ["data"] ) >= 1 && in_array ( $product_id, $this->config->item ( "coupon_black_list" ) ) == false) {
			$default_type = $conpon_result ["data"] ["data"] [0] ["c_name"];
			$default_content = $conpon_result ["data"] ["data"] [0] ["c_name_cn"];
			$defalut_price_arr = $this->_compute_real_price ( $item_data ["real_price"] * $num, $conpon_result ["data"] ["data"] [0] );
			$defalut_price = $defalut_price_arr ["price"];
			$defalut_compute = $defalut_price_arr ["compute"];
			$default_explain = $conpon_result ["data"] ["data"] [0] ["explain"];
		} else {
			$default_type = "none";
			$default_content = "不使用";
			$defalut_price = $num*$item_data ["real_price"];
			$defalut_compute = "";
			$default_explain = "不使用优惠卷";
		}
	
		$data = array (
				"item_data" => $item_data,
				"conpon_data" => $conpon_result ["data"] ["data"],
				"sell_num" => $sell_num,
				"default" => array (
						"type" => $default_type,
						"content" => $default_content,
						"compute" => $defalut_compute,
						"price" => $defalut_price,
						"explain" => $default_explain
				),
				"num" => $num
		);
	
		$params = array (
				"page_type" => $this->config->item ( "store" )
		);
		$head = $this->UserData->headData ( $params );
	
		// 道具商城开关
		$matrix_id = $this->UserInfo->getMatrixId();
		$store_view = $this->_select_view($matrix_id);
			
		$this->load->view ( $store_view."frame/head", $head );
		$this->load->view ( $store_view."order/storedetail_page_back", $data );
		
// 		if (in_array ( $product_id, $this->config->item ( "coupon_black_list" ) ) == true) {
// 			$this->load->view ( $store_view."order/storedetail_page_back", $data );
// 		} else {
// 			$this->load->view ( $store_view."order/storedetail_page", $data );
// 		}
	
		$this->load->view ( $store_view."frame/foot" );
	}
	
	//获取使用优惠券后的价格和计算公式
	public function realprice() {
		$price = $this->input->get ( "price" );
	
		$type = $this->input->get ( "type" );
	
		if ("none" == $type) {
			echo json_encode ( array (
					"price" => $price,
					"compute" => ""
			) );
			exit ();
		}
	
		$this->load->model ( "trade/TradeAgent" );
		$conpon_result = $this->TradeAgent->UserCouponList ( false, 1 );
		if (empty ( $conpon_result ) || $conpon_result ["return_code"] != 0) {
			echo json_encode ( array (
					"price" => "",
					"compute" => ""
			) );
			exit ();
		}
	
		$conpon_arr = array ();
		foreach ( ( array ) $conpon_result ["data"] ["data"] as $val ) {
			if ($type == $val ["c_name"]) {
				$conpon_arr = $val;
				break;
			}
		}
	
		if (count ( $conpon_arr ) > 0) {
			echo json_encode ( $this->_compute_real_price ( $price, $conpon_arr ) );
			exit ();
		}
	
		echo json_encode ( array (
				"price" => "",
				"compute" => ""
		) );
		exit ();
	}
	
	//计算使用优惠券后的价格和计算公式
	private function _compute_real_price($price, $conpon_arr) {
		if (strncmp($conpon_arr["c_name"], "A", 1) == 0) {
			if ($price >= $conpon_arr ["max"]) {
				$discount = $conpon_arr ["max"] * $conpon_arr ["discount"] / $conpon_arr ["condition"];
				$real_price = $price - $discount;
				$compute = $price . "-" . sprintf ( "%.2f", $discount ) . " =";
			} else {
				$rate = ( int ) ($price / $conpon_arr ["condition"]);
				$real_price = $price - $rate * $conpon_arr ["discount"];
				$compute = $price . "-" . sprintf ( "%.2f", $rate * $conpon_arr ["discount"] ) . " =";
			}
				
			return array (
					"price" => sprintf ( "%.2f", $real_price ),
					"compute" => $compute
			);
		} else if(strncmp($conpon_arr["c_name"], "B", 1) == 0) {
			$percent = $conpon_arr ["discount"] / $conpon_arr ["condition"];
			if ($price >= $conpon_arr["max"]) {
				$real_price = $price - $conpon_arr["max"] * $percent;
				$compute = $price."-".$conpon_arr["max"]."x".$percent. " =";
			} else {
				$real_price = $price * $percent;
				$compute = $price."x".$percent. " =";
			}
	
			return array (
					"price" => sprintf ( "%.2f", $real_price ),
					"compute" => $compute,
			);
		} else {
			return array (
					"price" => sprintf ( "%.2f", $price ),
					"compute" => "",
			);
		}
	}	
}
