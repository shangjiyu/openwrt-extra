/**
@file		pkgoper.h
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		去广告应用中数据包操作接口

@note		
数据包操作接口，包括组装数据包，发送数据包等。
*/

#ifndef _PKG_OPER_H_
#define _PKG_OPER_H_

#define HTTP_RESPONSE_HEAD_NOT_FIND							"HTTP/1.1 307 Temporary Redirect\r\n"
#define HTTP_RESPONSE_HEAD_NOT_FIND_LEN							33
#define HTTP_RESPONSE_LOCATION								"Location: "
#define HTTP_RESPONSE_LOCATION_LEN							10
#define HTTP_RESPONSE_END								"\r\n"
#define HTTP_RESPONSE_END_LEN								2
//#define HTTP_RESPONSE_CONNECTION							"Connection: closed"

/* HTTP 404 代码 */
#define HTTP_NOT_FOUND_STR									"HTTP/1.1 404 Not Found\r\n" \
															"Server: QWS\r\n" \
															"Content-Type: text/html\r\n" \
															"Content-Length: 130\r\n" \
															"Connection: keep-alive\r\n\r\n" \
															"<html><head><title>404 Not Found</title></head><body><center><h1>404 Not Found</h1></center><hr><center>QWS</center></body></html>"
#define HTTP_NOT_FOUND_STR_LEN								239

/* HTTP 502 代码 */
#define HTTP_BAD_GATEWAY									"HTTP/1.1 502 Bad Gateway\r\n" \
															"Server: QWS\r\n" \
															"Content-Type: text/html\r\n" \
															"Content-Length: 134\r\n" \
															"Connection: keep-alive\r\n\r\n" \
															"<html><head><title>502 Bad Gateway</title></head><body><center><h1>502 Bad Gateway</h1></center><hr><center>QWS</center></body></html>"
#define HTTP_BAD_GATEWAY_STR_LEN							245

/* HTTP 302 代码*/
#define HTTP_FIND_LOCAL_STR									"HTTP/1.1 302 Found\r\nLocation: http://127.0.0.1/\r\n\r\n"
#define HTTP_FIND_LOCAL_STR_LEN								51

/* lan interface */
#define ETH_CLIENT_LAN0									"br-lan"
#define ETH_CLIENT_LAN1									"br-lan"

/* wlan interface */
#define ETH_CLIENT_WLAN0								"vlan0"
#define ETH_CLIENT_WLAN1								"vlan1"

/**
网卡信息结构体

主要在往网卡发送数据包时使用，包括有线和无线网卡。
*/
struct client_nicname
{
	int index;	///< 网卡索引
	char *name;	///< 网卡名称
};

/**
根据tcp数据生成数据包

根据tcp数据，生成数据包，并填充 mac/ip/tcp 头部信息
@param skb 原始的sk_buff结构地址
@param names 网卡名称结构首地址
@param num 网卡个数
@param tcpdata tcp数据地址
@param tcpdatalen tcp数据长度
@return 成功返回数据包地址，失败返回NULL。
*/
struct sk_buff *pkg_skbuff_generate(struct sk_buff *skb, struct client_nicname *names, int num, char *tcpdata, int tcpdatalen);

/**
发送tcp数据包

@param skb 原始的sk_buff结构地址
@param tcpdata tcp数据地址
@param tcpdatalen tcp数据长度
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int pkg_skbuff_dev_xmit(struct sk_buff *skb, char *tcpdata, int tcpdatalen);

/**
根据Host生成location字符串

@param httplen 生成的location长度
@param host Host字符串内容
@return 成功返回location地址，失败返回NULL。
*/
char *http_location_str_generate(int *httplen, char *host);

/**
根据location发送302消息

@param skb 原始的sk_buff结构地址
@param location 需要发送的location内容
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int send_client_location(struct sk_buff *skb, char *location);

/**
发送404消息

@param skb 原始的sk_buff结构地址
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int send_client_notfound(struct sk_buff *skb);

/**
发送502消息

@param skb 原始的sk_buff结构地址
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int send_client_bad_gateway(struct sk_buff *skb);

/**
重新对IP头和TCP头进行校验

@param skb 原始的sk_buff结构地址
*/
void refresh_skb_checksum(struct sk_buff *skb);

/*
 发送伪造的HTTP200响应
 @param skb 原始的sk_buff结构地址 CType Content-Type Cont 网页内容
 @return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
HTTP/1.1 200 OK\r\n
Server: QWS\r\n
Content-Type: text/xml\r\n
Content-Length: 125\r\n
Connection: close\r\n
Accept-Ranges: bytes\r\n\r\n
*/
int send_client_fake_message(struct sk_buff *skb, const char * CType, const char * Cont);
#endif

