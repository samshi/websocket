#ifndef __OKCOINWEBSOCKETAPI_H__
#define __OKCOINWEBSOCKETAPI_H__

#include <string>

using namespace std;


#include "websocket.h"

#define URL_CN						"wss://real.okcoin.cn:10440/websocket/okcoinapi"
#define URL_COM						"wss://real.okcoin.com:10440/websocket/okcoinapi"

#define MAX_RETRY_COUNT		3000

class OKCoinWebSocketApi	//国内站
{
protected:
	OKCoinWebSocketApi();
	void SetKey(string api_key,string secret_key);
	void SetUri(string uri);
	string m_api_key;			//用户申请的apiKey
	string m_secret_key;		//请求参数签名的私钥
	string m_uri;

	void Emit(const char *channel,string &parameter);
	void Emit(const char *channel);
	void Emit(string &channel);
	void Remove(string channel);


private:

	websocketpp_callbak_open		m_callbak_open;
	websocketpp_callbak_close		m_callbak_close;
	websocketpp_callbak_message		m_callbak_message;



public:

	virtual ~OKCoinWebSocketApi();
	void Run();
	void Close();
	static unsigned __stdcall OKCoinWebSocketApi::RunThread( LPVOID arg );
	WebSocket *pWebsocket;
	HANDLE hThread;

	void SetCallBackOpen(websocketpp_callbak_open callbak_open);
	void SetCallBackClose(websocketpp_callbak_close callbak_close);
	void SetCallBackMessage(websocketpp_callbak_message callbak_message);

	/////////////////////////////////////////////////////////////////////////////
	
};


class OKCoinWebSocketApiCn:public OKCoinWebSocketApi	//国内站
{

public:
	OKCoinWebSocketApiCn(string api_key,string secret_key)
	{
		SetKey(api_key,secret_key);
		SetUri(URL_CN);		//国内站
	};
	~OKCoinWebSocketApiCn(){};
	
	
	//行情 API
	//注册获取OKCoin最新市场行情数据请求
	void ok_btccny_ticker();					//比特币行情数据
	void ok_btccny_depth();						//比特币20条市场深度
	void ok_btccny_depth60();					//比特币60条市场深度
	void ok_btccny_trades_v1();					//比特币实时成交记录
	void ok_btccny_kline_X(string x);			//比特币K线数据
	void ok_ltccny_ticker();					//莱特币行情数据
	void ok_ltccny_depth();						//莱特币20条市场深度
	void ok_ltccny_depth60();					//莱特币60条市场深度
	void ok_ltccny_trades_v1();					//莱特币实时成交记录
	void ok_ltccny_kline_X(string x);			//莱特币K线数据


	//交易 API
	//用于OKCoin快速进行交易

	void ok_cny_realtrades();															//实时交易数据
	void ok_spotcny_trade(string &symbol,string &type,string &price,string &amount);	//下单交易
	void ok_spotcny_cancel_order(string &symbol,string &order_id);						//取消订单
	void ok_spotcny_userinfo();															//账户信息
	void ok_spotcny_order_info(string &symbol,string &order_id);						//订单查询

	/////////////////////////////////////////////////

	
	//行情 API
	//注销OKCoin最新市场行情数据请求
	void remove_ok_btccny_ticker();						//比特币行情数据
	void remove_ok_btccny_depth();						//比特币20条市场深度
	void remove_ok_btccny_depth60();					//比特币60条市场深度
	void remove_ok_btccny_trades_v1();					//比特币实时成交记录
	void remove_ok_btccny_kline_X(string x);			//比特币K线数据
	void remove_ok_ltccny_ticker();						//莱特币行情数据
	void remove_ok_ltccny_depth();						//莱特币20条市场深度
	void remove_ok_ltccny_depth60();					//莱特币60条市场深度
	void remove_ok_ltccny_trades_v1();					//莱特币实时成交记录
	void remove_ok_ltccny_kline_X(string x);			//莱特币K线数据

};


class OKCoinWebSocketApiCom:public OKCoinWebSocketApi	//国际站
{
public:
	OKCoinWebSocketApiCom(string api_key,string secret_key)
	{
		SetKey(api_key, secret_key);
		SetUri(URL_COM);				//国际站
	};
	~OKCoinWebSocketApiCom(){};
	
	
	//现货行情 API
	//注册请求获取OKCoin最新市场现货行情数据

	void ok_btcusd_ticker();							//比特币行情数据
	void ok_btcusd_depth();								//比特币20条市场深度
	void ok_btcusd_depth60();							//比特币60条市场深度
	void ok_btcusd_trades_v1();							//比特币实时成交记录
	void ok_btcusd_kline_X(string x);					//比特币K线数据
	void ok_ltcusd_ticker();							//莱特币行情数据
	void ok_ltcusd_depth();								//莱特币20条市场深度
	void ok_ltcusd_depth60();							//莱特币60条市场深度
	void ok_ltcusd_trades_v1();							//莱特币实时成交记录
	void ok_ltcusd_kline_X(string x);					//莱特币K线数据

	//现货交易 API								
	//用于OKCoin快速进行现货交易				
	void ok_usd_realtrades();															//实时交易数据
	void ok_spotusd_trade(string &symbol,string &type,string &price,string &amount);	//下单交易
	void ok_spotusd_cancel_order(string &symbol,string &order_id);						//取消订单
	void ok_spotusd_userinfo();															//账户信息
	void ok_spotusd_order_info(string &symbol,string &order_id);						//订单查询

	//期货行情 API								
	//注册请求获取OKCoin期货行情数据					
	//
	void ok_btcusd_future_ticker_this_week();			//比特币期货当周合约行情
	void ok_btcusd_future_ticker_next_week();			//比特币期货次周合约行情
	void ok_btcusd_future_ticker_quarter();				//比特币期货季度合约行情
	void ok_btcusd_kline_this_week_X(string x);					//比特币期货当周合约K线数据
	void ok_btcusd_kline_next_week_X(string x);					//比特币期货次周合约K线数据
	void ok_btcusd_kline_quarter_X(string x);					//比特币期货季度合约K线数据
	void ok_ltcusd_future_ticker_this_week();			//莱特币期货当周合约行情
	void ok_ltcusd_future_ticker_next_week();			//莱特币期货次周合约行情
	void ok_ltcusd_future_ticker_quarter();				//莱特币期货季度合约行情
	void ok_ltcusd_kline_this_week_X(string x);					//莱特币期货当周合约K线数据
	void ok_ltcusd_kline_next_week_X(string x);					//莱特币期货次周合约K线数据
	void ok_ltcusd_kline_quarter_X(string x);					//莱特币期货季度合约K线数据
	void ok_btcusd_future_depth_this_week();			//比特币期货当周市场深度
	void ok_btcusd_future_depth_next_week();			//比特币期货次周市场深度
	void ok_btcusd_future_depth_quarter();				//比特币期货季度市场深度
	void ok_ltcusd_future_depth_this_week();			//莱特币期货当周市场深度
	void ok_ltcusd_future_depth_next_week();			//莱特币期货次周市场深度
	void ok_ltcusd_future_depth_quarter();				//莱特币期货季度市场深度
	void ok_btcusd_future_depth_this_week_60();			//比特币期货当周市场深度（60条）
	void ok_btcusd_future_depth_next_week_60();			//比特币期货次周市场深度（60条）
	void ok_btcusd_future_depth_quarter_60();			//比特币期货季度市场深度（60条）
	void ok_ltcusd_future_depth_this_week_60();			//莱特币期货当周市场深度（60条）
	void ok_ltcusd_future_depth_next_week_60();			//莱特币期货次周市场深度（60条）
	void ok_ltcusd_future_depth_quarter_60();			//莱特币期货季度市场深度（60条）
	void ok_btcusd_future_trade_v1_this_week();			//比特币期货当周交易信息
	void ok_btcusd_future_trade_v1_next_week();			//比特币期货次周交易信息
	void ok_btcusd_future_trade_v1_quarter();			//比特币期货季度交易信息
	void ok_ltcusd_future_trade_v1_this_week();			//莱特币期货当周交易信息
	void ok_ltcusd_future_trade_v1_next_week();			//莱特币期货次周交易信息
	void ok_ltcusd_future_trade_v1_quarter();			//莱特币期货季度交易信息
	void ok_btcusd_future_index();						//比特币期货指数
	void ok_ltcusd_future_index();						//莱特币期货指数


	//期货交易 API								
	//获取OKCoin期货交易数据					
	void ok_futuresusd_trade(string &symbol,string &contract_type,string &price,string &amount,string &type,string &match_price,string &lever_rate); 						//期货下单交易
	void ok_futuresusd_cancel_order(string &symbol,string &order_id,string &contract_type);					//取消期货订单
	void ok_usd_future_realtrades();					//实时交易数据
	void ok_futureusd_userinfo();						//账户信息
	void ok_futureusd_order_info(string &symbol,string &order_id,string &contract_type,string &status,string &current_page,string &page_length);						//订单查询


	//////////////////////////////////////////////////////


	//现货行情 API
	//注销获取OKCoin最新市场现货行情数据请求

	void remove_ok_btcusd_ticker();							//比特币行情数据
	void remove_ok_btcusd_depth();								//比特币20条市场深度
	void remove_ok_btcusd_depth60();							//比特币60条市场深度
	void remove_ok_btcusd_trades_v1();							//比特币实时成交记录
	void remove_ok_btcusd_kline_X(string x);					//比特币K线数据
	void remove_ok_ltcusd_ticker();							//莱特币行情数据
	void remove_ok_ltcusd_depth();								//莱特币20条市场深度
	void remove_ok_ltcusd_depth60();							//莱特币60条市场深度
	void remove_ok_ltcusd_trades_v1();							//莱特币实时成交记录
	void remove_ok_ltcusd_kline_X(string x);					//莱特币K线数据




	
	//期货行情 API								
	//注销获取OKCoin期货行情数据请求
	void remove_ok_btcusd_future_ticker_this_week();			//比特币期货当周合约行情
	void remove_ok_btcusd_future_ticker_next_week();			//比特币期货次周合约行情
	void remove_ok_btcusd_future_ticker_quarter();				//比特币期货季度合约行情
	void remove_ok_btcusd_kline_this_week_X(string x);					//比特币期货当周合约K线数据
	void remove_ok_btcusd_kline_next_week_X(string x);					//比特币期货次周合约K线数据
	void remove_ok_btcusd_kline_quarter_X(string x);					//比特币期货季度合约K线数据
	void remove_ok_ltcusd_future_ticker_this_week();			//莱特币期货当周合约行情
	void remove_ok_ltcusd_future_ticker_next_week();			//莱特币期货次周合约行情
	void remove_ok_ltcusd_future_ticker_quarter();				//莱特币期货季度合约行情
	void remove_ok_ltcusd_kline_this_week_X(string x);					//莱特币期货当周合约K线数据
	void remove_ok_ltcusd_kline_next_week_X(string x);					//莱特币期货次周合约K线数据
	void remove_ok_ltcusd_kline_quarter_X(string x);					//莱特币期货季度合约K线数据
	void remove_ok_btcusd_future_depth_this_week();			//比特币期货当周市场深度
	void remove_ok_btcusd_future_depth_next_week();			//比特币期货次周市场深度
	void remove_ok_btcusd_future_depth_quarter();				//比特币期货季度市场深度
	void remove_ok_ltcusd_future_depth_this_week();			//莱特币期货当周市场深度
	void remove_ok_ltcusd_future_depth_next_week();			//莱特币期货次周市场深度
	void remove_ok_ltcusd_future_depth_quarter();				//莱特币期货季度市场深度
	void remove_ok_btcusd_future_depth_this_week_60();			//比特币期货当周市场深度（60条）
	void remove_ok_btcusd_future_depth_next_week_60();			//比特币期货次周市场深度（60条）
	void remove_ok_btcusd_future_depth_quarter_60();			//比特币期货季度市场深度（60条）
	void remove_ok_ltcusd_future_depth_this_week_60();			//莱特币期货当周市场深度（60条）
	void remove_ok_ltcusd_future_depth_next_week_60();			//莱特币期货次周市场深度（60条）
	void remove_ok_ltcusd_future_depth_quarter_60();			//莱特币期货季度市场深度（60条）
	void remove_ok_btcusd_future_trade_v1_this_week();			//比特币期货当周交易信息
	void remove_ok_btcusd_future_trade_v1_next_week();			//比特币期货次周交易信息
	void remove_ok_btcusd_future_trade_v1_quarter();			//比特币期货季度交易信息
	void remove_ok_ltcusd_future_trade_v1_this_week();			//莱特币期货当周交易信息
	void remove_ok_ltcusd_future_trade_v1_next_week();			//莱特币期货次周交易信息
	void remove_ok_ltcusd_future_trade_v1_quarter();			//莱特币期货季度交易信息
	void remove_ok_btcusd_future_index();						//比特币期货指数
	void remove_ok_ltcusd_future_index();						//莱特币期货指数

};

#endif /* __OKCOINWEBSOCKETAPI_H__ */