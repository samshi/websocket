#!/usr/bin/python
# -*- coding: utf-8 -*-
# encoding: utf-8

import socket
import json
import md5
import logging
import hashlib
from websocket import WebSocketException,WebSocket

            
class WebSocketApp(object):
    """
    Higher level of APIs are provided. 
    The interface is like JavaScript WebSocket object.
    """
    def __init__(self, url,
                 on_open = None, on_message = None, on_error = None, 
                 on_close = None):
        """
        url: websocket url.
        on_open: callable object which is called at opening websocket.
          this function has one argument. The arugment is this class object.
        on_message: callbale object which is called when recieved data.
         on_message has 2 arguments. 
         The 1st arugment is this class object.
         The passing 2nd arugment is utf-8 string which we get from the server.
       on_error: callable object which is called when we get error.
         on_error has 2 arguments.
         The 1st arugment is this class object.
         The passing 2nd arugment is exception object.
       on_close: callable object which is called when closed the connection.
         this function has one argument. The arugment is this class object.
        """
        self.url = url
        self.on_open = on_open
        self.on_message = on_message
        self.on_error = on_error
        self.on_close = on_close
        self.sock = None

    def send(self, data):
        """
        send message. data must be utf-8 string or unicode.
        """
        self.sock.send(data)

    def close(self):
        """
        close websocket connection.
        """
        self.sock.close()

    def run_forever(self):
        """
        run event loop for WebSocket framework.
        This loop is infinite loop and is alive during websocket is available.
        """
        if self.sock:
            raise WebSocketException("socket is already opened")
        try:
            self.sock = WebSocket()
            self.sock.connect(self.url)
            self._run_with_no_err(self.on_open)
            while True:
                data = self.sock.recv()
                if data is None:
                    break
                self._run_with_no_err(self.on_message, data)
        except Exception, e:
            self._run_with_no_err(self.on_error, e)
        finally:
            self.sock.close()
            self._run_with_no_err(self.on_close)
            self.sock = None

    def _run_with_no_err(self, callback, *args):
        if callback:
            try:
                callback(self, *args)
            except Exception, e:
                if logger.isEnabledFor(logging.DEBUG):
                    logger.error(e)


#business
def buildMySign(params,secretKey):
    sign = ''
    for key in sorted(params.keys()):
        sign += key + '=' + str(params[key]) +'&'
    return  hashlib.md5(sign+'secret_key='+secretKey).hexdigest().upper()

#现货下单
def spotTrade(channel,api_key,secretkey,symbol,tradeType,price,amount):
    params={
        'api_key':api_key,
        'symbol':symbol,
        'type':tradeType,
        'price':price,
        'amount':amount
    }
    sign = buildMySign(params,secretkey)
    return "{'event':'addChannel','channel':'"+channel+"','parameters':{'api_key':'"+api_key+"',\
    'sign':'"+sign+"','symbol':'"+symbol+"','type':'"+tradeType+"','price':'"+price+"','amount':'"+amount+"'}}"

#现货取消订单
def spotCancelOrder(channel,api_key,secretkey,symbol,orderId):
    params = {
        'api_key':api_key,
        'symbol':symbol,
        'order_id':orderId
    }
    sign = buildMySign(params,secretkey)
    return "{'event':'addChannel','channel':'"+channel+"','parameters':{'api_key':'"+api_key+"','sign':'"+sign+"','symbol':'"+symbol+"','order_id':'"+orderId+"'}}"

#注册现货实时交易数据
def realtrades(channel,api_key,secretkey):
    params={'api_key':api_key}
    sign=buildMySign(params,secretkey)
    return "{'event':'addChannel','channel':'"+channel+"','parameters':{'api_key':'"+api_key+"','sign':'"+sign+"'}}"

#期货下单交易
def futureTrade(api_key,secretkey,symbol,contractType,price,amount,tradeType,matchPrice,leverRate):
    params = {
        'api_key':api_key,
        'symbol':symbol,
        'contract_type':contractType,
        'price':price,
        'amount':amount,
        'type':tradeType,
        'match_price':matchPrice,
        'lever_rate':leverRate
    }
    sign = buildMySign(params,secretkey)
    return "{'event':'addChannel','channel':'ok_futuresusd_trade','parameters':{'api_key':'"+api_key+"',\
    'sign':'"+sign+"','symbol':'"+symbol+"','contract_type':'"+contractType+"','price':'"+price+"',\
    'amount':'"+amount+"','type':'"+tradeType+"','match_price':'"+matchPrice+"','lever_rate':'"+leverRate+"'}}"

#期货取消订单
def futureCancelOrder(api_key,secretkey,symbol,orderId,contractType):
    params = {
        'api_key':api_key,
        'symbol':symbol,
        'order_id':orderId,
        'contract_type':contractType
    }
    sign = buildMySign(params,secretkey)
    return "{'event':'addChannel','channel':'ok_futuresusd_cancel_order','parameters':{'api_key':'"+api_key+"',\
    'sign':'"+sign+"','symbol':'"+symbol+"','contract_type':'"+contractType+"','order_id':'"+orderId+"'}}"

#期货实时交易数据
def futureRealTrades(api_key,secretkey):
    params = {'api_key':api_key}
    sign = buildMySign(params,secretkey)
    return "{'event':'addChannel','channel':'ok_usd_future_realtrades','parameters':{'api_key':'"+api_key+"','sign':'"+sign+"'}}"

def on_open(self):
    #国际站比特币现货行情
    self.send("{'event':'addChannel','channel':'ok_btcusd_ticker'}")

    #国际站比特币期货当周合约行情
    #self.send("{'event':'addChannel','channel':'ok_btcusd_future_ticker_this_week'}")

    #国际站莱特币期货次周市场深度
    #self.send("{'event':'addChannel','channel':'ok_ltcusd_future_depth_next_week'}")

    #现货下单
    #spotTradeMsg = spotTrade('ok_spotusd_trade','101','683FF5EC26641B231F1CD75109DA76DE','ltc_usd','buy','0.05','0.1')
    #self.send(spotTradeMsg)

    #现货注册实时交易
    #realtradesMsg = realtrades('ok_usd_realtrades','101','683FF5EC26641B231F1CD75109DA76DE')
    #self.send(realtradesMsg)

    #现货取消订单
    #spotCancelOrderMsg = spotCancelOrder('ok_spotusd_cancel_order','XXXX','XXXXXXX','btc_usd','125433027')
    #self.send(spotCancelOrderMsg)

    #期货下单
    #futureTradeMsg = futureTrade('XXXX','XXXXXXX','btc_usd','this_week','2','2','1','0','20')
    #self.send(futureTradeMsg)

    #期货取消订单
    #futureCancelOrderMsg = futureCancelOrder('XXXX','XXXXXXX','btc_usd','65464','this_week')
    #self.send(futureCancelOrderMsg)

    #期货注册实时交易
    #futureRealTradesMsg = futureRealTrades('XXXX','XXXXXXX')
    #self.send(futureRealTradesMsg)
    
def on_message(self,evt):
    data = json.loads(evt)
    print data

def on_error(self,evt):
    print 'error :'+evt

def on_close(self,evt):
    print 'DISCONNECT'

if __name__ == "__main__":
    url = "wss://real.okcoin.com:10440/websocket/okcoinapi"      #国内站请换成wss://real.okcoin.cn:10440/websocket/okcoinapi               
    ws = WebSocketApp(url,on_open,on_message,on_error,on_close)        
    #start
    ws.run_forever()


