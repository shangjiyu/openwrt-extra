/**
@file		strcmd.h
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		字符操作接口

@note		
主要解析对HTTP协议相关字符串进行操作
*/
#ifndef _STR_CMD_H_
#define _STR_CMD_H_
#include "advkill.h"

#define HTTP_METHOD_FLAG							"GET "
#define HTTP_METHOD_FLAG_LEN						4
#define HTTP_URL_END_FLAG							" HTTP/"
#define HTTP_URL_END_LEN							9
#define HTTP_HOST_FLAG								"Host: "
#define HTTP_HOST_FLAG_LEN							6
#define HTTP_REFERER_FLAG							"Referer: "
#define HTTP_REFERER_FLAG_LEN						9
#define HTTP_MIN_LEN								(HTTP_METHOD_FLAG_LEN + HTTP_URL_END_LEN + 2 + HTTP_HOST_FLAG_LEN + 2)

#define HTTP_LOCATION_PREFIX						"http://"

#define XUNLEI_KANKAN_DOMAIN_DIGIT_NUM				8
#define	XUNLEI_KANKAN_DOMAIN_DIGIT_INDEX			1
#define	DOMAIN_SEP_CHAR								'.'
#define DOMAIN_MATCH_CHAR							'*'		///< 域名模糊匹配标识
#define URL_EXCEPT_CHAR								'!'		///< URL例外标识
#define HOST_EXCEPT_CHAR							'!'		///< Host例外标识

/**
根据字段标识获取HTTP协议中字段内容

@param httpstr HTTP协议字符串
@param httpstrlen HTTP协议字符串长度
@param fieldflag 要获取字段的标识
@param fieldflaglen 要获取字段的标识长度
@param field 要保存字段内容的地址
@param maxfieldlen 要保存字段内容的最大长度
@param realfieldlen 获取到的字段内容的实际长度
@return 获取到的字段内容的地址
*/
char *get_http_field(char *httpstr, int httpstrlen, char *fieldflag, int fieldflaglen, char *field, int maxfieldlen, int *realfieldlen);

/**
获取HTTP协议中URL内容

@param httpstr HTTP协议字符串
@param httpstrlen HTTP协议字符串长度
@param field 要保存字段内容的地址
@param maxfieldlen 要保存字段内容的最大长度
@param geturllen 获取到的URL的实际长度
@return 获取到的URL内容的地址
*/
char *get_http_get_url(char *httpstr, int httpstrlen, char *field, int maxfieldlen, int *geturllen);

/**
根据Host和URL生成location内容

@param location 要保存的location内容的地址
@param maxlen 要保存location内容的最大长度
@param host location内容用到的Host
@param url location内容用到的URL
*/
void http_location_generate(char *location, int maxlen, char *host, char *url);

/**
检测Host内容是否全为IP地址而非域名

@param host 要检测的Host内容
@return 是IP地址返回 ADV_KILL_OK，否则返回 ADV_KILL_FAIL。
*/
int is_digit_host(char *host);

/**
检测域名中是否至少包含指定个数的数字

@param domain 要检测的域名内容
@param digitnum 要包含的数字最少个数
@return 若是IP地址返回 ADV_KILL_OK，否则返回 ADV_KILL_FAIL。
*/
int domain_contain_digits(char *domain, int digitnum);

/**
检测域名是否符合组匹配模式

@param domain 要检测的域名内容
@param fieldindex 域名第几个子域开始匹配
@return 成功返回 匹配模式地址，失败返回 NULL。
*/
char * domain_regroup_match_mode(char *domain, int fieldindex);

/**
检测指定配置节点中是否包含URL例外模式

@param surl 原始URL
@param node 要检测的配置节点地址
@return 若是返回 ADV_KILL_OK，否则返回 ADV_KILL_FAIL。
*/
int is_contain_except_url(char *surl, struct advconf_hashnode *node);

#endif
