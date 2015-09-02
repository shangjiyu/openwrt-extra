/**
@file		advhash.h
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		配置哈希表相关操作接口

@note
哈希表相关操作，包括哈希算法，初始化，节点插入/删除，查找，释放等。
*/
#ifndef _ADV_HASH_H_
#define _ADV_HASH_H_
#include "advkill.h"

#define MAX_HASH_SIZE				0xFFFFFFFF

#define HOST_HASH_SEED				13
#define HOST_HASH_SIZE_BIT			5
#define HOST_BIT_MOVE				(32-HOST_HASH_SIZE_BIT)
#define HOST_HASH_SIZE_MASK			(MAX_HASH_SIZE >> HOST_BIT_MOVE)
#define HOST_HASH_SIZE				(HOST_HASH_SIZE_MASK+1)		///< 配置哈希表最大个数

/**
计算字符串的hash值，返回索引位置

@param str 输入字符串
@return 返回字符串在哈希表中的索引
*/
unsigned int host_bkdr_hash(unsigned char *str);

/**
释放URL配置占用的内存空间

@param map URL配置结构首地址
@param mapnum URL配置个数
*/
void advconf_hostmap_free(struct advconf_hostmap *map, int mapnum);

/**
释放配置哈希节点的内存空间

@param node 要释放的哈希节点
*/
void advconf_hashnode_free(struct advconf_hashnode *node);

/**
添加配置哈希节点到哈希表

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param node 要添加的配置结构
*/
void advconf_hashnode_add(struct advconf_hashtable *table, int tablenum, struct advconf_hashnode *node);

/**
根据Host从哈希表中删除配置节点

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param host Host名称(字符串)
@return 成功返回 ADV_KILL_FAIL，失败返回 ADV_KILL_OK。
*/
int advconf_hashnode_del_by_host(struct advconf_hashtable *table, int tablenum, char *host);

/**
根据Host从哈希表中查找配置节点

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param host Host名称(字符串)
@param referer HTTP协议中的referer字符串，若配置项中有例外选项，则host不在referer中的配置项才匹配
@return 成功返回 配置项结构体地址，失败返回 NULL。
*/
struct advconf_hashnode * advconf_hashnode_find_by_host(struct advconf_hashtable *table, int tablenum, char *host, char *referer);

/**
根据URL从哈希表中查找配置节点

@param table 配置哈希表首地址
@param tablenum 哈希表大小
@param url 源URL字符串
@return 成功返回 配置项结构体地址，失败返回 NULL。
*/
struct advconf_hashnode * advconf_hashnode_find_by_url(struct advconf_hashtable *table, int tablenum, char *url);

/**
删除一条哈希链表中的所有配置节点并释放内存

@param table 配置哈希链表头
*/
void web_conf_data_hashtable_del_all(struct advconf_hashtable *table);

/**
根据URL从哈希配置节点中查找URL配置

@param node 配置节点地址
@param surl 源URL字符串
@return 成功返回 URL配置地址，失败返回 NULL。
*/
struct advconf_hostmap *advconf_hostmap_find_by_url(struct advconf_hashnode *node, char *surl);

/**
初始化配置哈希表

@param table 哈希表首地址需要保存的地址
@param tablenum 哈希表大小
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int advconf_hashtable_init(struct advconf_hashtable **table, int tablenum);

/**
释放配置哈希表

@param table 哈希表首地址
@param tablenum 哈希表大小
*/
void advconf_hashtable_release(struct advconf_hashtable *table, int tablenum);

/**
清空哈希表中所有配置

@param table 哈希表首地址
@param tablenum 哈希表大小
*/
void advconf_hashtable_empty(struct advconf_hashtable *table, int tablenum);

/**
打印配置项内所有信息(用于调试)

@param node 配置节点地址
*/
void advconf_hashnode_print(struct advconf_hashnode *node);

/**
根据Host从哈希表中查找配置节点并打印其信息

@param table 哈希表首地址
@param tablenum 哈希表大小
@param host 要打印的Host字符串
*/
void advconf_hashnode_print_by_host(struct advconf_hashtable *table, int tablenum, char *host);

/**
打印一条哈希链表中所有配置信息

@param table 配置哈希链表地址
*/
void advconf_hashtable_print(struct advconf_hashtable *table);

/**
打印配置哈希表中所有配置信息

@param table 哈希表首地址
@param tablenum 哈希表大小
*/
void advconf_hashtable_print_all(struct advconf_hashtable *table, int tablenum);

#ifdef ADVKILL_PRINT_DEBUG_INFO
struct advconf_hashnode *advconf_hashnode_generate(int flag, int mapnum);
#endif
#endif
