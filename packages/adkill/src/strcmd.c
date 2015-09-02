/**
@file		strcmd.c
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		字符操作接口

@note		
主要解析对HTTP协议相关字符串进行操作
*/

#include "strcmd.h"

#ifdef ADVKILL_CHECK_MEM
extern unsigned long long int g_calloc_times;
extern unsigned long long int g_calloc_size;
extern unsigned long long int g_free_times;
extern unsigned long long int g_free_size;
#endif

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
char *get_http_field(char *httpstr, int httpstrlen, char *fieldflag, int fieldflaglen, char *field, int maxfieldlen, int *realfieldlen)
{
	char *beginpos = NULL;
	char *endpos = NULL;
	char *tmppos = NULL;
	int len = 0;

	if (!httpstr || !fieldflag || !field || !realfieldlen)
		return NULL;
	endpos = (httpstr + httpstrlen);
	beginpos = strstr(httpstr, fieldflag);
	if (beginpos == NULL)
		return NULL;
	beginpos += fieldflaglen;
	tmppos = beginpos;
	while(*tmppos != '\r' && tmppos < endpos)
	{
		len++;
		tmppos++;
	}
	if (len <= 0)
		return NULL;
	if (len > (maxfieldlen -1))
	{
		*realfieldlen = (maxfieldlen -1);
		memcpy(field, beginpos, maxfieldlen -1);
		return field;
	}

	*realfieldlen = len;
	memcpy(field, beginpos, len);
	return field;
}

/**
获取HTTP协议中URL内容

@param httpstr HTTP协议字符串
@param httpstrlen HTTP协议字符串长度
@param field 要保存字段内容的地址
@param maxfieldlen 要保存字段内容的最大长度
@param geturllen 获取到的URL的实际长度
@return 获取到的URL内容的地址
*/
char *get_http_get_url(char *httpstr, int httpstrlen, char *field, int maxfieldlen, int *geturllen)
{
	char *beginpos = NULL;
	char *endpos = NULL;
	int len = 0;

	if (!httpstr || !geturllen)
		return NULL;

	beginpos = httpstr;
	beginpos += HTTP_METHOD_FLAG_LEN;
	endpos = strstr(beginpos, HTTP_URL_END_FLAG);

	if (endpos == NULL)
		return NULL;
	len = (endpos - beginpos);
	if(len <= 0 || len > (maxfieldlen - 1))
		return NULL;

	*geturllen = len;
	memcpy(field, beginpos, len);
	return field;
}

/**
根据Host和URL生成location内容

@param location 要保存的location内容的地址
@param maxlen 要保存location内容的最大长度
@param host location内容用到的Host
@param url location内容用到的URL
*/
void http_location_generate(char *location, int maxlen, char *host, char *url)
{
	if(!location || !host || !url)
		return;
	snprintf(location, maxlen, "%s%s%s", HTTP_LOCATION_PREFIX, host, url);
}

/**
检测Host内容是否全为IP地址而非域名

@param host 要检测的Host内容
@return 是IP地址返回 ADV_KILL_OK，否则返回 ADV_KILL_FAIL。
*/
int is_digit_host(char *host)
{
	char *tmphost = NULL;
	
	if(!host)
		return ADV_KILL_FAIL;
	tmphost = host;
	while(*tmphost != '\0')
	{
		if (*tmphost < '.' || *tmphost > '9')
		{
			return ADV_KILL_FAIL;
		}
		tmphost++;
	}
	return ADV_KILL_OK;
}

/**
检测域名中是否至少包含指定个数的数字

@param domain 要检测的域名内容
@param digitnum 要包含的数字最少个数
@return 若是IP地址返回 ADV_KILL_OK，否则返回 ADV_KILL_FAIL。
*/
int domain_contain_digits(char *domain, int digitnum)
{
	char *tmpdomain = NULL;
	int tmpnum = 0;
	int maxlen = 0;
	int i = 0;

	if (!domain || digitnum <= 0)
		return ADV_KILL_FAIL;
	tmpdomain = domain;
	maxlen = strlen(tmpdomain);

	for (i=0; i<maxlen; i++)
	{
		if (*tmpdomain >= '0' && *tmpdomain <= '9')
		{
			tmpnum++;
		}
		else
		{
			tmpnum = 0;
		}
		if (tmpnum >= digitnum)
			return ADV_KILL_OK;
		tmpdomain++;
	}
	return ADV_KILL_FAIL;
}

/**
检测域名是否符合组匹配模式

@param domain 要检测的域名内容
@param fieldindex 域名第几个子域开始匹配
@return 成功返回 匹配模式地址，失败返回 NULL。
*/
char * domain_regroup_match_mode(char *domain, int fieldindex)
{
	int index = 0;
	int maxlen = 0;
	int fieldnum = 0;

	if (!domain || fieldindex <= 0)
	{
		return NULL;
	}
	maxlen = strlen(domain);
	for (index=0; index<maxlen; index++)
	{
		if (*domain == DOMAIN_SEP_CHAR)
			fieldnum++;
		if (fieldnum == fieldindex)
		{
			domain--;
			*domain = DOMAIN_MATCH_CHAR;
			return domain;
		}
		domain++;
	}
	return NULL;
}

/**
检测指定配置节点中是否包含URL例外模式

@param surl 原始URL
@param node 要检测的配置节点地址
@return 若是返回 ADV_KILL_OK，否则返回 ADV_KILL_FAIL。
*/
int is_contain_except_url(char *surl, struct advconf_hashnode *node)
{
	int i = 0;
	char *tmppos = NULL;

	if(!node || !surl || node->mapnum <= 0)
		return ADV_KILL_FAIL;
	for (i=0; i<node->mapnum; i++)
	{
		tmppos = strchr(node->map[i].durl, URL_EXCEPT_CHAR);
		if (tmppos != NULL)
		{
			tmppos++;
			if (strstr(surl, tmppos) != NULL)
				return ADV_KILL_OK;
		}
	}
	return ADV_KILL_FAIL;
}
