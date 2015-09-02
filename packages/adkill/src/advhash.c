/**
@file		advhash.c
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		配置哈希表相关操作接口

@note
哈希表相关操作，包括哈希算法，初始化，节点插入/删除，查找，释放等。
*/

#include "advhash.h"
#include "strcmd.h"

#ifdef ADVKILL_CHECK_MEM
extern unsigned long long int g_calloc_times;
extern unsigned long long int g_calloc_size;
extern unsigned long long int g_free_times;
extern unsigned long long int g_free_size;
#endif

/**
计算字符串的hash值，返回索引位置

@param str 输入字符串
@return 返回字符串在哈希表中的索引
*/
unsigned int host_bkdr_hash(unsigned char *str)
{
	unsigned int seed = HOST_HASH_SEED;    
	unsigned int hash = 0;     

	while (*str)    
	{        
		hash = hash * seed + (*str++);    
	}     
	return (hash & HOST_HASH_SIZE_MASK);
}

/**
释放URL配置占用的内存空间

@param map URL配置结构首地址
@param mapnum URL配置个数
*/
void advconf_hostmap_free(struct advconf_hostmap *map, int mapnum)
{
	int i = 0;
	if (!map || mapnum <=0 )
		return;
	for (i=0; i<mapnum; i++)
	{
		if (map[i].surl != NULL)
		{
			ADVKILL_FREE(map[i].surl, (map[i].surllen+1));
			map[i].surl = NULL;
		}
		if (map[i].durl != NULL)
		{
			ADVKILL_FREE(map[i].durl, (map[i].durllen+1));
			map[i].durl = NULL;
		}
	}
	ADVKILL_FREE(map, (mapnum * sizeof(struct advconf_hostmap)));
}

/**
释放配置哈希节点的内存空间

@param node 要释放的哈希节点
*/
void advconf_hashnode_free(struct advconf_hashnode *node)
{
	if(!node)
		return;
	if (node->s_host != NULL)
	{
		ADVKILL_FREE(node->s_host, (node->s_host_len+1));
		node->s_host = NULL;
	}
	if (node->d_host != NULL)
	{
		ADVKILL_FREE(node->d_host, (node->d_host_len+1));
		node->d_host = NULL;
	}
	if (node->map != NULL)
	{
		advconf_hostmap_free(node->map, node->mapnum);
	}
	ADVKILL_FREE(node, sizeof(struct advconf_hashnode));
}

/**
初始化配置哈希表

@param table 哈希表首地址需要保存的地址
@param tablenum 哈希表大小
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int advconf_hashtable_init(struct advconf_hashtable **table, int tablenum)
{
	struct advconf_hashtable *tmp = NULL;
	int i = 0;
	if(tablenum <= 0)
		return ADV_KILL_FAIL;
	tmp = ADVKILL_CALLOC(tablenum, sizeof(struct advconf_hashtable));
	if(tmp == NULL)
		return ADV_KILL_FAIL;
	*table = tmp;

	for (i=0; i<tablenum; i++)
		INIT_HLIST_HEAD(&tmp[i].head);
	return ADV_KILL_OK;
}

/**
添加配置哈希节点到哈希表

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param node 要添加的配置结构
*/
void advconf_hashnode_add(struct advconf_hashtable *table, int tablenum, struct advconf_hashnode *node)
{
	unsigned int index = 0;
	char tmphost[ADV_MAX_SHOST_LEN] = {0};
	unsigned char *tmp = NULL;

	if(!table || !node)
		return;
	tmp = strchr(node->s_host, HOST_EXCEPT_CHAR);
	if (tmp != NULL)
	{
		strncpy(tmphost, node->s_host, tmp - (node->s_host));
		index = host_bkdr_hash(tmphost);
	}
	else
		index = host_bkdr_hash(node->s_host);

	hlist_add_head(&node->node, &table[index].head);
}

/**
根据URL从哈希表中查找配置节点

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param url 源URL字符串
@return 成功返回 配置项结构体地址，失败返回 NULL。
*/
struct advconf_hashnode * advconf_hashnode_find_by_url(struct advconf_hashtable *table, int tablenum, char *url)
{
	struct hlist_node *pos = NULL;
	struct hlist_node *n = NULL;
	struct advconf_hashnode *info = NULL;
	int index = 0;
	int i = 0;

	EnterFunction();
	if(!table || !url)
		return NULL;
	for (index=0; index<tablenum; index++)
	{
		if (hlist_empty(&table[index].head))
			continue;
		hlist_for_each_safe(pos, n, &table[index].head)
		{
			if (pos != NULL)
			{
				info = (struct advconf_hashnode *)pos;
				if(info->mapnum <= 0)
					continue;
				for (i=0; i<info->mapnum; i++)
				{
					if (strstr(url, info->map[i].surl) != NULL)
					{
						return info;
					}
				}
			}
		}
	}

	return NULL;
}

/**
根据Host从哈希表中查找配置节点

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param host Host名称(字符串)
@param referer HTTP协议中的referer字符串，若配置项中有例外选项，则host不在referer中的配置项才匹配
@return 成功返回 配置项结构体地址，失败返回 NULL。
*/
struct advconf_hashnode * advconf_hashnode_find_by_host(struct advconf_hashtable *table, int tablenum, char *host, char *referer)
{
	struct hlist_node *pos = NULL;
	struct hlist_node *n = NULL;
	struct advconf_hashnode *info = NULL;
	int index = 0;
	unsigned char *tmp = NULL;

	EnterFunction();
	if(!table || !host)
		return NULL;
	
	index = host_bkdr_hash(host);
	if (hlist_empty(&table[index].head))
		return NULL;
	
	hlist_for_each_safe(pos, n, &table[index].head)
	{
		if (pos != NULL)
		{
			info = (struct advconf_hashnode *)pos;
			if (referer == NULL)
			{
				if (strcmp(info->s_host, host) == 0)
				{
					LeaveFunction();
					return info;
				}
			}

			tmp = strchr(info->s_host, HOST_EXCEPT_CHAR);
			if(tmp != NULL)
			{
				if (strncmp(info->s_host, host, tmp- (info->s_host)) == 0)
				{
					if (referer != NULL)
					{
						tmp++;
						if (strstr(referer, tmp) == NULL)
						{
							LeaveFunction();
							return info;
						}
					}
				}
			}
			else
			{
				if (strcmp(info->s_host, host) == 0)
				{
					LeaveFunction();
					return info;
				}
			}
			
		}
	}

	return NULL;
}

/**
根据Host从哈希表中删除配置节点

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param host Host名称(字符串)
@return 成功返回 ADV_KILL_FAIL，失败返回 ADV_KILL_OK。
*/
int advconf_hashnode_del_by_host(struct advconf_hashtable *table, int tablenum, char *host)
{
	struct hlist_node *pos = NULL;
	struct hlist_node *n = NULL;
	struct advconf_hashnode *info = NULL;
	int index = 0;

	EnterFunction();
	if(!table || !host)
		return ADV_KILL_FAIL;
	index = host_bkdr_hash(host);

	if (hlist_empty(&table[index].head))
		return ADV_KILL_FAIL;
	hlist_for_each_safe(pos, n, &table[index].head)
	{
		if (pos != NULL)
		{
			info = (struct advconf_hashnode *)pos;
			if (strcmp(info->s_host, host) == 0)
			{
				hlist_del(pos);
				advconf_hashnode_free(info);
				LeaveFunction();
				return ADV_KILL_OK;
			}
		}
	}

	return ADV_KILL_FAIL;
}

/**
删除一条哈希链表中的所有配置节点并释放内存

@param table 配置哈希链表头
*/
void web_conf_data_hashtable_del_all(struct advconf_hashtable *table)
{
	struct hlist_node *pos = NULL;
	struct hlist_node *n = NULL;
	struct advconf_hashnode *info = NULL;

	EnterFunction();
	if(!table)
		return;

	if (hlist_empty(&table->head))
		return;
	hlist_for_each_safe(pos, n, &table->head)
	{
		if (pos != NULL)
		{
			info = (struct advconf_hashnode *)pos;
			hlist_del(pos);
			advconf_hashnode_free(info);
		}
	}
}

/**
根据URL从哈希配置节点中查找URL配置

@param node 配置节点地址
@param surl 源URL字符串
@return 成功返回 URL配置地址，失败返回 NULL。
*/
struct advconf_hostmap *advconf_hostmap_find_by_url(struct advconf_hashnode *node, char *surl)
{
	int i = 0;
	if(!node || !surl || node->mapnum <= 0)
		return NULL;
	for (i=0; i<node->mapnum; i++)
	{
		if (strstr(surl, node->map[i].surl) != NULL)
		{
			return &node->map[i];
		}
	}
	return NULL;
}

/**
释放配置哈希表

@param table 哈希表首地址
@param tablenum 哈希表大小
*/
void advconf_hashtable_release(struct advconf_hashtable *table, int tablenum)
{
	int i = 0;
	EnterFunction();
	if(!table || tablenum<=0 )
		return;
	for (i=0; i<tablenum; i++)
	{
		web_conf_data_hashtable_del_all(&table[i]);
	}
	ADVKILL_FREE(table, (sizeof(struct advconf_hashtable) * tablenum));
	LeaveFunction();
}

/**
清空哈希表中所有配置

@param table 哈希表首地址
@param tablenum 哈希表大小
*/
void advconf_hashtable_empty(struct advconf_hashtable *table, int tablenum)
{
	int i = 0;
	EnterFunction();
	if(!table || tablenum<=0 )
		return;
	for (i=0; i<tablenum; i++)
	{
		web_conf_data_hashtable_del_all(&table[i]);
	}
	LeaveFunction();
}

/**
打印配置项内所有信息(用于调试)

@param node 配置节点地址
*/
void advconf_hashnode_print(struct advconf_hashnode *node)
{
	int i = 0;
	printk(KERN_ALERT "type[%d],index[%d],shost[%s] len[%d],dhost[%s] len[%d],mapnum[%d]\n", node->type, node->index, node->s_host, node->s_host_len, node->d_host, node->d_host_len, node->mapnum);
	for (i=0; i<node->mapnum; i++)
	{
		printk(KERN_ALERT "\t[%d],surl[%s],durl[%s]\n", i, node->map[i].surl, node->map[i].durl);
	}
	return;
}

/**
打印一条哈希链表中所有配置信息

@param table 配置哈希链表地址
*/
void advconf_hashtable_print(struct advconf_hashtable *table)
{
	struct hlist_node *pos = NULL;
	struct hlist_node *n = NULL;
	struct advconf_hashnode *info = NULL;

	EnterFunction();
	if(!table)
	{
		printk(KERN_ALERT "hashtable is null\n");
		return;
	}
	if (hlist_empty(&table->head))
	{
		//printk(KERN_ALERT "hashtable is empty\n");
		return;
	}
	hlist_for_each_safe(pos, n, &table->head)
	{
		if (pos != NULL)
		{
			info = (struct advconf_hashnode *)pos;
			advconf_hashnode_print(info);
		}
	}
}

/**
根据Host从哈希表中查找配置节点并打印其信息

@param table 哈希表首地址
@param tablenum 哈希表大小
@param host 要打印的Host字符串
*/
void advconf_hashnode_print_by_host(struct advconf_hashtable *table, int tablenum, char *host)
{
	struct hlist_node *pos = NULL;
	struct hlist_node *n = NULL;
	struct advconf_hashnode *info = NULL;
	int index = 0;

	EnterFunction();
	if(!table || !host)
		return;
	index = host_bkdr_hash(host);

	if (hlist_empty(&table[index].head))
		return;
	hlist_for_each_safe(pos, n, &table[index].head)
	{
		if (pos != NULL)
		{
			info = (struct advconf_hashnode *)pos;
			if (strcmp(info->s_host, host) == 0)
			{
				advconf_hashnode_print(info);
				break;
			}
		}
	}

	return;
}

/**
打印配置哈希表中所有配置信息

@param table 哈希表首地址
@param tablenum 哈希表大小
*/
void advconf_hashtable_print_all(struct advconf_hashtable *table, int tablenum)
{
	int i = 0;
	EnterFunction();
	if(!table || tablenum<=0 )
		return;
	for (i=0; i<tablenum; i++)
	{
		advconf_hashtable_print(&table[i]);
	}
	LeaveFunction();
}

#ifdef ADVKILL_PRINT_DEBUG_INFO

static char *shost = "static.youku.com";
static char *dhost = "192.168.1.118";
static char *surl = "loader.swf";
static char *durl = "/loader.swf";
struct advconf_hashnode *advconf_hashnode_generate(int flag, int mapnum)
{
	struct advconf_hashnode *tmp = NULL;
	int i = 0;

	tmp = ADVKILL_CALLOC(1, sizeof(struct advconf_hashnode));
	if(tmp == NULL)
		return NULL;

	tmp->type = adv_redirect_player;
	tmp->index = flag;
	//shost
	tmp->s_host_len = (strlen(shost)+1);
	tmp->s_host = ADVKILL_CALLOC(1, tmp->s_host_len);
	if(tmp->s_host == NULL)
		goto exit_fail;
	snprintf(tmp->s_host, tmp->s_host_len, "%s", shost);
	//dhost
	tmp->d_host_len = (strlen(dhost)+1);
	tmp->d_host = ADVKILL_CALLOC(1, tmp->d_host_len);
	if(tmp->d_host == NULL)
		goto exit_fail;
	snprintf(tmp->d_host, tmp->d_host_len, "%s", dhost);
	//host map
	tmp->mapnum = mapnum;
	if (mapnum > 0)
	{
		tmp->map = (struct advconf_hostmap *)ADVKILL_CALLOC(mapnum, sizeof(struct advconf_hostmap));
		if (tmp->map == NULL)
		{
			goto exit_fail;
		}
		for (i=0; i<mapnum; i++)
		{
			tmp->map[i].surllen = (strlen(surl) + 1);
			tmp->map[i].durllen = (strlen(durl) + 1);
			tmp->map[i].surl = (char *)ADVKILL_CALLOC(1, tmp->map[i].surllen);
			if(tmp->map[i].surl == NULL)
				goto exit_fail;
			snprintf(tmp->map[i].surl, tmp->map[i].surllen, "%s", surl);
			tmp->map[i].durl = (char *)ADVKILL_CALLOC(1, tmp->map[i].durllen);
			if(tmp->map[i].durl == NULL)
				goto exit_fail;
			snprintf(tmp->map[i].durl, tmp->map[i].durllen, "%s", durl);
		}
	}
	
	return tmp;
exit_fail:
	advconf_hashnode_free(tmp);
	return NULL;
}
#endif
