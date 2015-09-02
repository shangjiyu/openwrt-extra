/**
@file		advconfparse.h
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		去广告配置解析接口

@note
去广告配置应用中解析用户配置的相关接口
*/
#ifndef _ADV_CONF_PARSE_H_
#define _ADV_CONF_PARSE_H_

#include "advkill.h"

#define ADV_CONF_FIELD_2_LEN					16
#define ADV_CONF_FIELD_3_LEN					128
#define ADV_CONF_FIELD_4_LEN					256
#define ADV_CONF_FIELD_5_LEN					128
#define ADV_CONF_FIELD_6_LEN					256

#define ADV_CONF_DATA_SEPC_CHAR					' '
#define ADV_TAG_SEPARATIR_CHAR					','
#define ADV_CONF_REDIRECT_PLAYER				'R'		///< 重定向模式 302
#define ADV_CONF_DROP_REQUEST					'D'		///< drop模式 404
#define ADV_CONF_MODIFY_URL						'M'		///< 修改源数据包
#define ADV_CONF_BAD_GW							'G'		///< 网关错误 502
#define ADV_CONF_FAKE_PACK						'F'		///< 发送伪造数据包

/**
解析一行配置，解析完成后配置存放在结构体 advconf_hashtable 里

@param linedata 输入配置行
@param table 配置哈希表首地址
@param tablenum 配置哈希表大小
@return 成功返回ADV_KILL_OK，失败返回ADV_KILL_FAIL
*/
int parse_advconf_line_data(char *linedata, struct advconf_hashtable *table, int tablenum);

#endif
