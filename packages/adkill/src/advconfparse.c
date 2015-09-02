/**
@file		advconfparse.c
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		去广告配置解析接口

@note
去广告配置应用中解析用户配置的相关接口
*/

#include <linux/mm.h>

#include "advconfparse.h"
#include "advhash.h"

extern struct mutex g_advconf_mutex;

#ifdef ADVKILL_CHECK_MEM
extern unsigned long long int g_calloc_times;
extern unsigned long long int g_calloc_size;
extern unsigned long long int g_free_times;
extern unsigned long long int g_free_size;
#endif

/**
配置行内容结构体，临时保存配置行中的每一个字段

*/
struct adv_conf_data
{
	char field1;	///< 操作类型
	char field2[ADV_CONF_FIELD_2_LEN];	///< 第二个字段	
	char field3[ADV_CONF_FIELD_3_LEN];	///< 第三个字段
	char field4[ADV_CONF_FIELD_4_LEN];	///< 第四个字段
	char field5[ADV_CONF_FIELD_5_LEN];	///< 第五个字段
	char field6[ADV_CONF_FIELD_6_LEN];	///< 第六个字段
};

/**
将输入配置行转换到结构体 adv_conf_data 中

@param linedata 输入配置行
@param data 配置保存在变量data里
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL
*/
static int get_advconf_line_data(char *linedata, struct adv_conf_data *data)
{
	char *tmpdata = NULL;
	int index = 0;
	int sepnum = 0;

	EnterFunction();
	if(!linedata || !data)
		return ADV_KILL_FAIL;
	tmpdata = linedata;

	while (*tmpdata != '\n' && *tmpdata != '\0')
	{
		if (( *tmpdata == ADV_CONF_DATA_SEPC_CHAR )&&( sepnum<5 ))
		{
			sepnum++;
			index = 0;
			tmpdata++;
			continue;
		}
		switch(sepnum)
		{
		case 0:
			data->field1 = (*tmpdata);
			break;
		case 1:
			data->field2[index] = (*tmpdata);
			break;
		case 2:
			data->field3[index] = (*tmpdata);
			break;
		case 3:
			data->field4[index] = (*tmpdata);
			break;
		case 4:
			data->field5[index] = (*tmpdata);
			break;
		case 5:
			data->field6[index] = (*tmpdata);
			break;
		default:
			break;
		}
		tmpdata++;
		index++;
	}
	LeaveFunction();
	return ADV_KILL_OK;
}

/**
解析配置操作标识

@param oper 操作字段(一个字符)
@return 返回相应的操作类型(枚举值)
*/
static int get_oper_flag(char oper)
{
	switch(oper)
	{
	case ADV_CONF_REDIRECT_PLAYER:
		return adv_redirect_player;
	case ADV_CONF_DROP_REQUEST:
		return adv_drop_request;
	case ADV_CONF_MODIFY_URL:
		return adv_modify_url;
	case ADV_CONF_BAD_GW:
		return adv_bad_gw;
	case ADV_CONF_FAKE_PACK:
		return adv_fake_pack;
	default:
		return adv_redirect_player;
	}
}

static int parse_str2int(char *str)
{
	int number = 0;
	sscanf(str, "%d", &number);
	return number;
}

static int get_tag_num_from_tagdata(char *tagdata, int len)
{
	char *tmpdata = tagdata;
	int i = 0;
	int num = 0;

	if(strlen(tmpdata) < 1)
		return 0;

	while(i<len)
	{
		if(*tmpdata == ADV_TAG_SEPARATIR_CHAR)
		{
			num++;
		}
		i++;
		tmpdata++;
	}

	return (num+1);
}

/**
解析配置行中URL配置规则

@param data 要解析的配置结构体
@param node 要保存到的配置哈希节点
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL
*/
static int parse_advconf_url(struct adv_conf_data *data, struct advconf_hashnode *node)
{
	int i = 0;
	char *tmpsurlbegin = NULL;
	char *tmpsurlend = NULL;
	char *tmpdurlbegin = NULL;
	char *tmpdurlend = NULL;

	if (!data || !node)
		return ADV_KILL_FAIL;
	node->mapnum = get_tag_num_from_tagdata(data->field4, strlen(data->field4));
	node->map = (struct advconf_hostmap *)ADVKILL_CALLOC(node->mapnum, sizeof(struct advconf_hostmap));
	if (node->map == NULL)
		return ADV_KILL_FAIL;

	tmpsurlbegin = data->field4;
	tmpdurlbegin = data->field6;

	for (i=0; i<node->mapnum; i++)
	{
		//surl
		tmpsurlend = strchr(tmpsurlbegin, ADV_TAG_SEPARATIR_CHAR);
		if (tmpsurlend == NULL)
			node->map[i].surllen = strlen(tmpsurlbegin);
		else
			node->map[i].surllen = (tmpsurlend - tmpsurlbegin);
		if (node->map[i].surllen > 0)
		{
			node->map[i].surl = (char *)ADVKILL_CALLOC(1, node->map[i].surllen+1);
			if (node->map[i].surl == NULL)
			{
				return ADV_KILL_FAIL;
			}
			memcpy(node->map[i].surl, tmpsurlbegin, node->map[i].surllen);
		}
		tmpsurlbegin += (node->map[i].surllen + 1);
		//durl
		tmpdurlend = strchr(tmpdurlbegin, ADV_TAG_SEPARATIR_CHAR);
		if (tmpdurlend == NULL)
			node->map[i].durllen = strlen(tmpdurlbegin);
		else
			node->map[i].durllen = (tmpdurlend - tmpdurlbegin);
		if (node->map[i].durllen > 0)
		{
			node->map[i].durl = (char *)ADVKILL_CALLOC(1, node->map[i].durllen+1);
			if (node->map[i].durl == NULL)
			{
				return ADV_KILL_FAIL;
			}
			memcpy(node->map[i].durl, tmpdurlbegin, node->map[i].durllen);
		}
		tmpdurlbegin += (node->map[i].durllen + 1);
	}
	return ADV_KILL_OK;
}

/**
解析单条配置，并保存到哈希节点内

@param data 配置临时结构体
@param node 解析后保存的结构体地址
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
static int parse_advconf_node_from_adv_conf_data(struct adv_conf_data *data, struct advconf_hashnode **node)
{
	struct advconf_hashnode *tmpnode = NULL;

	EnterFunction();
	if(!data)
		return ADV_KILL_FAIL;
	tmpnode = (struct advconf_hashnode *)ADVKILL_CALLOC(1, sizeof(struct advconf_hashnode));
	if(!tmpnode)
		return ADV_KILL_FAIL;
	//oper flag
	tmpnode->type = get_oper_flag(data->field1);
	//index
	tmpnode->index = parse_str2int(data->field2);
	//shost
	tmpnode->s_host_len = strlen(data->field3);
	tmpnode->s_host = (char *)ADVKILL_CALLOC(1, tmpnode->s_host_len+1);
	if (tmpnode->s_host == NULL)
	{
		goto exit_fail;
	}
	strcpy(tmpnode->s_host, data->field3);
	//dhost
	tmpnode->d_host_len = strlen(data->field5);
	tmpnode->d_host = (char *)ADVKILL_CALLOC(1, tmpnode->d_host_len+1);
	if (tmpnode->d_host == NULL)
	{
		goto exit_fail;
	}
	strcpy(tmpnode->d_host, data->field5);
	//surl durl
	if(tmpnode->type != adv_fake_pack)
	{
		if (parse_advconf_url(data, tmpnode) != ADV_KILL_OK)
		{
			goto exit_fail;
		}
	} else {
		tmpnode->mapnum = 1;
		tmpnode->map = (struct advconf_hostmap *)ADVKILL_CALLOC(1, sizeof(struct advconf_hostmap));
		tmpnode->map[0].surllen = strlen(data->field4);
		tmpnode->map[0].surl = (char *)ADVKILL_CALLOC(1, tmpnode->map[0].surllen+1);
		if (tmpnode->map[0].surl == NULL)
		{
			ADVKILL_FREE(tmpnode->map, sizeof(struct advconf_hostmap));
			return ADV_KILL_FAIL;
		}
		memcpy(tmpnode->map[0].surl, data->field4, tmpnode->map[0].surllen);
		tmpnode->map[0].durllen = strlen(data->field6);
		tmpnode->map[0].durl = (char *)ADVKILL_CALLOC(1, tmpnode->map[0].durllen+1);
		if (tmpnode->map[0].durl == NULL)
		{
			ADVKILL_FREE(tmpnode->map[0].surl, tmpnode->map[0].surllen+1);
			ADVKILL_FREE(tmpnode->map, sizeof(struct advconf_hostmap));
			return ADV_KILL_FAIL;
		}
		memcpy(tmpnode->map[0].durl, data->field6, tmpnode->map[0].durllen);
	}
	*node = tmpnode;
	return ADV_KILL_OK;

exit_fail:
	return ADV_KILL_FAIL;
}

#ifdef ADVKILL_PRINT_DEBUG_INFO
static void print_adv_conf_data(struct adv_conf_data *data)
{
	if(!data)
		return;
	printk(KERN_ALERT "field1[%c],field2[%s],field3[%s],field4[%s],field5[%s],field6[%s]\n\n",
			data->field1, data->field2, data->field3, data->field4, data->field5, data->field6);
}
#endif

/**
解析一行配置，解析完成后配置存放在结构体 advconf_hashtable 里

@param linedata 输入配置行
@param table 配置哈希表首地址
@param tablenum 配置哈希表大小
@return 成功返回ADV_KILL_OK，失败返回ADV_KILL_FAIL
*/
int parse_advconf_line_data(char *linedata, struct advconf_hashtable *table, int tablenum)
{
	struct advconf_hashnode *advconfnode = NULL;
	struct adv_conf_data *data = NULL;

	if(!linedata || !table)
		return ADV_KILL_FAIL;
	data = (struct adv_conf_data *)ADVKILL_CALLOC(1, sizeof(struct adv_conf_data));
	if (data == NULL)
	{
		return ADV_KILL_FAIL;
	}
	get_advconf_line_data(linedata, data);

#ifdef ADVKILL_PRINT_DEBUG_INFO
	print_adv_conf_data(data);
#endif

	if (parse_advconf_node_from_adv_conf_data(data, &advconfnode) != ADV_KILL_OK)
	{
		ADVKILL_FREE(data, sizeof(struct adv_conf_data));
		data = NULL;
		advconf_hashnode_free(advconfnode);
		return ADV_KILL_FAIL;
	}

#ifdef ADVKILL_PRINT_DEBUG_INFO
	advconf_hashnode_print(advconfnode);
#endif

	ADVKILL_MUTEX_LOCK(&g_advconf_mutex);
	advconf_hashnode_add(table, tablenum, advconfnode);
	ADVKILL_MUTEX_UNLOCK(&g_advconf_mutex);

	ADVKILL_FREE(data, sizeof(struct adv_conf_data));
	data = NULL;
	return ADV_KILL_OK;
}

