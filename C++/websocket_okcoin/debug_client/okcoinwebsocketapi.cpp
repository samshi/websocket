#include "string.h"
#include "okcoinwebsocketapi.h"
#include <sstream>
#include "parameter.h"

OKCoinWebSocketApi::OKCoinWebSocketApi():
pWebsocket(0),
m_callbak_open(0),
m_callbak_close(0),
m_callbak_message(0)
{
	hThread = 0;
};


OKCoinWebSocketApi::~OKCoinWebSocketApi()
{
	Close();
};

void OKCoinWebSocketApi::SetKey(string api_key,string secret_key)
{	
	m_api_key		=	api_key;
	m_secret_key	=	secret_key;	
}

void OKCoinWebSocketApi::SetUri(string uri)
{
	m_uri = uri;
}
void OKCoinWebSocketApi::Close()
{
	if(pWebsocket != 0)
	{
		pWebsocket->doclose();
		WaitForSingleObject(hThread,INFINITE);
		if (NULL != hThread)
		{
			CloseHandle(hThread);
		}
	}
}

void OKCoinWebSocketApi::Emit(const char *channel,string &parameter)
{
	if(pWebsocket != 0)
	{
		pWebsocket->emit(channel,parameter);
	}
};
void OKCoinWebSocketApi::Emit(const char *channel)
{
	if(pWebsocket != 0)
	{
		pWebsocket->emit(channel);
	}
};
void OKCoinWebSocketApi::Emit(string &channel)
{
	if(pWebsocket != 0)
	{
		pWebsocket->emit(channel);
	}
};

void OKCoinWebSocketApi::Remove(string channel)
{
	if(pWebsocket != 0)
	{
		pWebsocket->remove(channel);
	}
};


void OKCoinWebSocketApi::Run()
{
	unsigned int threadId = 0;
	hThread = (HANDLE)_beginthreadex(NULL,0,OKCoinWebSocketApi::RunThread,this,0,&threadId);
}

unsigned __stdcall OKCoinWebSocketApi::RunThread( LPVOID arg )
{
	if(arg != 0)
	{
		OKCoinWebSocketApi *api = (OKCoinWebSocketApi *)arg;
		
		for(int i = 0;i < MAX_RETRY_COUNT;i++)
		{
			if(api->pWebsocket != 0)
			{
				api->pWebsocket->doclose();
			}
			
			if(api->pWebsocket == 0)
			{
				api->pWebsocket = new WebSocket();
			}

			if(api->pWebsocket != 0)
			{
				api->pWebsocket->callbak_open = api->m_callbak_open;
				api->pWebsocket->callbak_close = api->m_callbak_close;
				api->pWebsocket->callbak_message = api->m_callbak_message;
				api->pWebsocket->run(api->m_uri);
				bool bManualClose = api->pWebsocket->m_manual_close;
				delete api->pWebsocket;
				api->pWebsocket = 0;
				if(bManualClose == false)//是否为主动关闭连接，如果不是用户主动关闭，当接到断开联接回调时则自动执行重新连接机制。
				{
					Sleep(2000);
				}
				else
				{
					return 0;
				}
			}
			else
			{
				return 0;
			}
		}
		
	}
	//::SetEvent(Global::g_hExit);

	return 0;
}


void OKCoinWebSocketApi::SetCallBackOpen(websocketpp_callbak_open callbak_open)
{
	m_callbak_open = callbak_open;
}

void OKCoinWebSocketApi::SetCallBackClose(websocketpp_callbak_close callbak_close)
{
	m_callbak_close = callbak_close;
}

void OKCoinWebSocketApi::SetCallBackMessage(websocketpp_callbak_message callbak_message)
{
	m_callbak_message = callbak_message;
}

//////////////////////////////////////////////////////////////////////////////////////////////


//现货行情 API
//注册请求获取OKCoin最新市场现货行情数据

void OKCoinWebSocketApiCn::ok_btccny_ticker()							//比特币行情数据
{
	Emit("ok_btccny_ticker");
}
void OKCoinWebSocketApiCn::ok_btccny_depth()								//比特币20条市场深度
{
	Emit("ok_btccny_depth");
}
void OKCoinWebSocketApiCn::ok_btccny_depth60()							//比特币60条市场深度
{
	Emit("ok_btccny_depth60");
}
void OKCoinWebSocketApiCn::ok_btccny_trades_v1()							//比特币实时成交记录
{
	Emit("ok_btccny_trades_v1");
}
void OKCoinWebSocketApiCn::ok_btccny_kline_X(string x)			//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week 				//比特币K线数据
{
	string X = "ok_btccny_kline_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCn::ok_ltccny_ticker()							//莱特币行情数据
{
	Emit("ok_ltccny_ticker");
}
void OKCoinWebSocketApiCn::ok_ltccny_depth()								//莱特币20条市场深度
{
	Emit("ok_ltccny_depth");
}
void OKCoinWebSocketApiCn::ok_ltccny_depth60()							//莱特币60条市场深度
{
	Emit("ok_ltccny_depth60");
}
void OKCoinWebSocketApiCn::ok_ltccny_trades_v1()							//莱特币实时成交记录
{
	Emit("ok_ltccny_trades_v1");
}
void OKCoinWebSocketApiCn::ok_ltccny_kline_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week							//莱特币K线数据
{
	string X = "ok_ltccny_kline_";
	X += x;
	Emit(X);
}


//现货交易 API								
//用于OKCoin快速进行现货交易				
void OKCoinWebSocketApiCn::ok_cny_realtrades()							//实时交易数据
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_cny_realtrades",prmtstr);
	return ;
}

void OKCoinWebSocketApiCn::ok_spotcny_trade(string &symbol,string &type,string &price,string &amount)							//下单交易
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("type",type);
	prmt.AddParam("price",price);
	prmt.AddParam("amount",amount);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotcny_trade",prmtstr);
	return ;

}
void OKCoinWebSocketApiCn::ok_spotcny_cancel_order(string &symbol,string &order_id)						//取消订单
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("order_id",order_id);
	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotcny_cancel_order",prmtstr);
}
void OKCoinWebSocketApiCn::ok_spotcny_userinfo()							//账户信息
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotcny_userinfo",prmtstr);
}
void OKCoinWebSocketApiCn::ok_spotcny_order_info(string &symbol,string &order_id)						//订单查询
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("order_id",order_id);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotcny_order_info",prmtstr);
}




//现货行情 API
//注册请求获取OKCoin最新市场现货行情数据

void OKCoinWebSocketApiCn::remove_ok_btccny_ticker()							//比特币行情数据
{
	Remove("ok_btccny_ticker");
}
void OKCoinWebSocketApiCn::remove_ok_btccny_depth()								//比特币20条市场深度
{
	Remove("ok_btccny_depth");
}
void OKCoinWebSocketApiCn::remove_ok_btccny_depth60()							//比特币60条市场深度
{
	Remove("ok_btccny_depth60");
}
void OKCoinWebSocketApiCn::remove_ok_btccny_trades_v1()							//比特币实时成交记录
{
	Remove("ok_btccny_trades_v1");
}
void OKCoinWebSocketApiCn::remove_ok_btccny_kline_X(string x)			//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week 				//比特币K线数据
{
	string X = "ok_btccny_kline_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCn::remove_ok_ltccny_ticker()							//莱特币行情数据
{
	Remove("ok_ltccny_ticker");
}
void OKCoinWebSocketApiCn::remove_ok_ltccny_depth()								//莱特币20条市场深度
{
	Remove("ok_ltccny_depth");
}
void OKCoinWebSocketApiCn::remove_ok_ltccny_depth60()							//莱特币60条市场深度
{
	Remove("ok_ltccny_depth60");
}
void OKCoinWebSocketApiCn::remove_ok_ltccny_trades_v1()							//莱特币实时成交记录
{
	Remove("ok_ltccny_trades_v1");
}
void OKCoinWebSocketApiCn::remove_ok_ltccny_kline_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week							//莱特币K线数据
{
	string X = "ok_ltccny_kline_";
	X += x;
	Remove(X);
}


//////////////////////////////////////////////////////////////////////////////////






//现货行情 API
//注册请求获取OKCoin最新市场现货行情数据

void OKCoinWebSocketApiCom::ok_btcusd_ticker()							//比特币行情数据
{
	Emit("ok_btcusd_ticker");
}
void OKCoinWebSocketApiCom::ok_btcusd_depth()								//比特币20条市场深度
{
	Emit("ok_btcusd_depth");
}
void OKCoinWebSocketApiCom::ok_btcusd_depth60()							//比特币60条市场深度
{
	Emit("ok_btcusd_depth60");
}
void OKCoinWebSocketApiCom::ok_btcusd_trades_v1()							//比特币实时成交记录
{
	Emit("ok_btcusd_trades_v1");
}
void OKCoinWebSocketApiCom::ok_btcusd_kline_X(string x)			//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week 				//比特币K线数据
{
	string X = "ok_btcusd_kline_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCom::ok_ltcusd_ticker()							//莱特币行情数据
{
	Emit("ok_ltcusd_ticker");
}
void OKCoinWebSocketApiCom::ok_ltcusd_depth()								//莱特币20条市场深度
{
	Emit("ok_ltcusd_depth");
}
void OKCoinWebSocketApiCom::ok_ltcusd_depth60()							//莱特币60条市场深度
{
	Emit("ok_ltcusd_depth60");
}
void OKCoinWebSocketApiCom::ok_ltcusd_trades_v1()							//莱特币实时成交记录
{
	Emit("ok_ltcusd_trades_v1");
}
void OKCoinWebSocketApiCom::ok_ltcusd_kline_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week							//莱特币K线数据
{
	string X = "ok_ltcusd_kline_";
	X += x;
	Emit(X);
}



//现货交易 API								
//用于OKCoin快速进行现货交易				
void OKCoinWebSocketApiCom::ok_usd_realtrades()							//实时交易数据
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_usd_realtrades",prmtstr);
	return ;
}
void OKCoinWebSocketApiCom::ok_spotusd_trade(string &symbol,string &type,string &price,string &amount)							//下单交易
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("type",type);
	prmt.AddParam("price",price);
	prmt.AddParam("amount",amount);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotusd_trade",prmtstr);
	return ;

}
void OKCoinWebSocketApiCom::ok_spotusd_cancel_order(string &symbol,string &order_id)						//取消订单
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("order_id",order_id);
	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotusd_cancel_order",prmtstr);
}
void OKCoinWebSocketApiCom::ok_spotusd_userinfo()							//账户信息
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotusd_userinfo",prmtstr);
}
void OKCoinWebSocketApiCom::ok_spotusd_order_info(string &symbol,string &order_id)						//订单查询
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("order_id",order_id);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_spotusd_order_info",prmtstr);
}


//期货行情 API								
//注册请求获取OKCoin期货行情数据					
//
void OKCoinWebSocketApiCom::ok_btcusd_future_ticker_this_week()			//比特币期货当周合约行情
{
	Emit("ok_btcusd_future_ticker_this_week");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_ticker_next_week()			//比特币期货次周合约行情
{
	Emit("ok_btcusd_future_ticker_next_week");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_ticker_quarter()				//比特币期货季度合约行情
{
	Emit("ok_btcusd_future_ticker_quarter");
}
void OKCoinWebSocketApiCom::ok_btcusd_kline_this_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//比特币期货当周合约K线数据
{
	string X = "ok_btcusd_kline_this_week_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCom::ok_btcusd_kline_next_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//比特币期货次周合约K线数据
{
	string X = "ok_btcusd_kline_next_week_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCom::ok_btcusd_kline_quarter_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//比特币期货季度合约K线数据
{
	string X = "ok_btcusd_kline_quarter_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_ticker_this_week()			//莱特币期货当周合约行情
{
	Emit("ok_ltcusd_future_ticker_this_week");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_ticker_next_week()			//莱特币期货次周合约行情
{
	Emit("ok_ltcusd_future_ticker_next_week");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_ticker_quarter()				//莱特币期货季度合约行情
{
	Emit("ok_ltcusd_future_ticker_quarter");
}
void OKCoinWebSocketApiCom::ok_ltcusd_kline_this_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//莱特币期货当周合约K线数据
{
	string X = "ok_ltcusd_kline_this_week_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCom::ok_ltcusd_kline_next_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//莱特币期货次周合约K线数据
{
	string X = "ok_ltcusd_kline_next_week_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCom::ok_ltcusd_kline_quarter_X(string x)	//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week				//莱特币期货季度合约K线数据
{
	string X = "ok_ltcusd_kline_quarter_";
	X += x;
	Emit(X);
}
void OKCoinWebSocketApiCom::ok_btcusd_future_depth_this_week()				//比特币期货当周市场深度
{
	Emit("ok_btcusd_future_depth_this_week");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_depth_next_week()				//比特币期货次周市场深度
{
	Emit("ok_btcusd_future_depth_next_week");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_depth_quarter()				//比特币期货季度市场深度
{
	Emit("ok_btcusd_future_depth_quarter");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_depth_this_week()				//莱特币期货当周市场深度
{
	Emit("ok_ltcusd_future_depth_this_week");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_depth_next_week()				//莱特币期货次周市场深度
{
	Emit("ok_ltcusd_future_depth_next_week");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_depth_quarter()				//莱特币期货季度市场深度
{
	Emit("ok_ltcusd_future_depth_quarter");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_depth_this_week_60()			//比特币期货当周市场深度（60条）
{
	Emit("ok_btcusd_future_depth_this_week_60");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_depth_next_week_60()			//比特币期货次周市场深度（60条）
{
	Emit("ok_btcusd_future_depth_next_week_60");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_depth_quarter_60()				//比特币期货季度市场深度（60条）
{
	Emit("ok_btcusd_future_depth_quarter_60");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_depth_this_week_60()			//莱特币期货当周市场深度（60条）
{
	Emit("ok_ltcusd_future_depth_this_week_60");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_depth_next_week_60()			//莱特币期货次周市场深度（60条）
{
	Emit("ok_ltcusd_future_depth_next_week_60");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_depth_quarter_60()				//莱特币期货季度市场深度（60条）
{
	Emit("ok_ltcusd_future_depth_quarter_60");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_trade_v1_this_week()			//比特币期货当周交易信息
{
	Emit("ok_btcusd_future_trade_v1_this_week");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_trade_v1_next_week()			//比特币期货次周交易信息
{
	Emit("ok_btcusd_future_trade_v1_next_week");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_trade_v1_quarter()				//比特币期货季度交易信息
{
	Emit("ok_btcusd_future_trade_v1_quarter");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_trade_v1_this_week()			//莱特币期货当周交易信息
{
	Emit("ok_ltcusd_future_trade_v1_this_week");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_trade_v1_next_week()			//莱特币期货次周交易信息
{
	Emit("ok_ltcusd_future_trade_v1_next_week");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_trade_v1_quarter()				//莱特币期货季度交易信息
{
	Emit("ok_ltcusd_future_trade_v1_quarter");
}
void OKCoinWebSocketApiCom::ok_btcusd_future_index()						//比特币期货指数
{
	Emit("ok_btcusd_future_index");
}
void OKCoinWebSocketApiCom::ok_ltcusd_future_index()						//莱特币期货指数
{
	Emit("ok_ltcusd_future_index");
}


//期货交易 API								
//获取OKCoin期货交易数据					
void OKCoinWebSocketApiCom::ok_futuresusd_trade(string &symbol,string &contract_type,string &price,string &amount,string &type,string &match_price,string &lever_rate) 						//期货下单交易
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("contract_type",contract_type);
	prmt.AddParam("price",price);
	prmt.AddParam("amount",amount);
	prmt.AddParam("type",type);
	prmt.AddParam("match_price",match_price);
	prmt.AddParam("lever_rate",lever_rate);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_futuresusd_trade",prmtstr);
}
void OKCoinWebSocketApiCom::ok_futuresusd_cancel_order(string &symbol,string &order_id,string &contract_type)					//取消期货订单
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("order_id",order_id);
	prmt.AddParam("contract_type",contract_type);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_futuresusd_cancel_order",prmtstr);

}
void OKCoinWebSocketApiCom::ok_usd_future_realtrades()					//实时交易数据
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_usd_future_realtrades",prmtstr);
}


void OKCoinWebSocketApiCom::ok_futureusd_userinfo()						//账户信息
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_futureusd_userinfo",prmtstr);

}
void OKCoinWebSocketApiCom::ok_futureusd_order_info(string &symbol,string &order_id,string &contract_type,string &status,string &current_page,string &page_length)						//订单查询
{
	Parameter prmt;
	prmt.AddParam("api_key",m_api_key);
	prmt.AddParam("symbol",symbol);
	prmt.AddParam("order_id",order_id);
	prmt.AddParam("contract_type",contract_type);
	prmt.AddParam("status",status);
	prmt.AddParam("current_page",current_page);
	prmt.AddParam("page_length",page_length);

	string sign = prmt.GetSign(m_secret_key);
	prmt.AddParam("sign",sign);
	string prmtstr = prmt.ToJsonString();
	Emit("ok_futureusd_order_info",prmtstr);
}



//现货行情 API
//注销获取OKCoin最新市场现货行情数据请求

void OKCoinWebSocketApiCom::remove_ok_btcusd_ticker()							//比特币行情数据
{
	Remove("ok_btcusd_ticker");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_depth()								//比特币20条市场深度
{
	Remove("ok_btcusd_depth");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_depth60()							//比特币60条市场深度
{
	Remove("ok_btcusd_depth60");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_trades_v1()							//比特币实时成交记录
{
	Remove("ok_btcusd_trades_v1");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_kline_X(string x)			//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week 				//比特币K线数据
{
	string X = "ok_btcusd_kline_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_ticker()							//莱特币行情数据
{
	Remove("ok_ltcusd_ticker");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_depth()								//莱特币20条市场深度
{
	Remove("ok_ltcusd_depth");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_depth60()							//莱特币60条市场深度
{
	Remove("ok_ltcusd_depth60");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_trades_v1()							//莱特币实时成交记录
{
	Remove("ok_ltcusd_trades_v1");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_kline_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week							//莱特币K线数据
{
	string X = "ok_ltcusd_kline_";
	X += x;
	Remove(X);
}




//期货行情 API								
//注销获取OKCoin期货行情数据请求					
//
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_ticker_this_week()			//比特币期货当周合约行情
{
	Remove("ok_btcusd_future_ticker_this_week");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_ticker_next_week()			//比特币期货次周合约行情
{
	Remove("ok_btcusd_future_ticker_next_week");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_ticker_quarter()				//比特币期货季度合约行情
{
	Remove("ok_btcusd_future_ticker_quarter");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_kline_this_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//比特币期货当周合约K线数据
{
	string X = "ok_btcusd_kline_this_week_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_kline_next_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//比特币期货次周合约K线数据
{
	string X = "ok_btcusd_kline_next_week_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_kline_quarter_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//比特币期货季度合约K线数据
{
	string X = "ok_btcusd_kline_quarter_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_ticker_this_week()			//莱特币期货当周合约行情
{
	Remove("ok_ltcusd_future_ticker_this_week");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_ticker_next_week()			//莱特币期货次周合约行情
{
	Remove("ok_ltcusd_future_ticker_next_week");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_ticker_quarter()				//莱特币期货季度合约行情
{
	Remove("ok_ltcusd_future_ticker_quarter");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_kline_this_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//莱特币期货当周合约K线数据
{
	string X = "ok_ltcusd_kline_this_week_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_kline_next_week_X(string x)//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week					//莱特币期货次周合约K线数据
{
	string X = "ok_ltcusd_kline_next_week_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_kline_quarter_X(string x)	//①x值为：1min,3min,5min,15min,30min,1hour,2hour,4hour,6hour,12hour,day,3day,week				//莱特币期货季度合约K线数据
{
	string X = "ok_ltcusd_kline_quarter_";
	X += x;
	Remove(X);
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_depth_this_week()				//比特币期货当周市场深度
{
	Remove("ok_btcusd_future_depth_this_week");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_depth_next_week()				//比特币期货次周市场深度
{
	Remove("ok_btcusd_future_depth_next_week");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_depth_quarter()				//比特币期货季度市场深度
{
	Remove("ok_btcusd_future_depth_quarter");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_depth_this_week()				//莱特币期货当周市场深度
{
	Remove("ok_ltcusd_future_depth_this_week");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_depth_next_week()				//莱特币期货次周市场深度
{
	Remove("ok_ltcusd_future_depth_next_week");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_depth_quarter()				//莱特币期货季度市场深度
{
	Remove("ok_ltcusd_future_depth_quarter");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_depth_this_week_60()			//比特币期货当周市场深度（60条）
{
	Remove("ok_btcusd_future_depth_this_week_60");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_depth_next_week_60()			//比特币期货次周市场深度（60条）
{
	Remove("ok_btcusd_future_depth_next_week_60");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_depth_quarter_60()				//比特币期货季度市场深度（60条）
{
	Remove("ok_btcusd_future_depth_quarter_60");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_depth_this_week_60()			//莱特币期货当周市场深度（60条）
{
	Remove("ok_ltcusd_future_depth_this_week_60");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_depth_next_week_60()			//莱特币期货次周市场深度（60条）
{
	Remove("ok_ltcusd_future_depth_next_week_60");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_depth_quarter_60()				//莱特币期货季度市场深度（60条）
{
	Remove("ok_ltcusd_future_depth_quarter_60");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_trade_v1_this_week()			//比特币期货当周交易信息
{
	Remove("ok_btcusd_future_trade_v1_this_week");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_trade_v1_next_week()			//比特币期货次周交易信息
{
	Remove("ok_btcusd_future_trade_v1_next_week");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_trade_v1_quarter()				//比特币期货季度交易信息
{
	Remove("ok_btcusd_future_trade_v1_quarter");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_trade_v1_this_week()			//莱特币期货当周交易信息
{
	Remove("ok_ltcusd_future_trade_v1_this_week");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_trade_v1_next_week()			//莱特币期货次周交易信息
{
	Remove("ok_ltcusd_future_trade_v1_next_week");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_trade_v1_quarter()				//莱特币期货季度交易信息
{
	Remove("ok_ltcusd_future_trade_v1_quarter");
}
void OKCoinWebSocketApiCom::remove_ok_btcusd_future_index()						//比特币期货指数
{
	Remove("ok_btcusd_future_index");
}
void OKCoinWebSocketApiCom::remove_ok_ltcusd_future_index()						//莱特币期货指数
{
	Remove("ok_ltcusd_future_index");
}
