package com.okcoin.websocket.test;

import com.okcoin.websocket.WebSocketService;
/**
 * WebSocket API使用事例
 * @author okcoin
 *
 */
public class Example {
	
	
	public static void main(String[] args){
		
		//apiKey 为用户申请的apiKey
		String apiKey = "XXXXX";
		
		//secretKey为用户申请的secretKey
		String secretKey = "XXXXX";
		
		//国内站WebSocket地址  注意如果访问国际站 请将 real.okcoin.cn 改为 real.okcoin.com
		String url = "wss://real.okcoin.cn:10440/websocket/okcoinapi"; 
	
		//订阅消息处理类,用于处理WebSocket服务返回的消息
		WebSocketService service = new BuissnesWebSocketServiceImpl();
		
		//WebSocket客户端
		WebSoketClient client = new WebSoketClient(url,service);
		
		//启动客户端
		client.start();
		
		//添加订阅
		client.addChannel("ok_btccny_ticker");
		
		//删除定订阅
		//client.removeChannel("ok_btccny_ticker");
		
		// 期货下单交易
		// client.futureTrade(apiKey, secretKey, "btc_cny", "this_week", 2.3, 2, 1, 0, 10);
		
	        // 实时交易数据   apiKey
		//client.futureRealtrades(apiKey, secretKey);
		
		//取消期货交易
		//client.cancleFutureOrder(apiKey, secretKey, "btc_cny", 123456L, "this_week");
		
		//现货下单交易
		//client.spotTrade(apiKey, secretKey, "btc_cny", 3.2, 2.3, "buy");
		
		//现货交易数据
		//client.realTrades(apiKey, secretKey);
		
		//现货取消订单
		//client.cancleOrder(apiKey, secretKey, "btc_cny", 123L);
			
		// 获取用户信息
		//client.getUserInfo(apiKey,secretKey);
		  		 
	}
}
