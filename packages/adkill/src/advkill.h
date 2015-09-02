/**
@file		advkill.h
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		去广告应用中用到的主要数据结构

@note		
去广告应用中主要数据结构，记录了需要修改数据包的网站，修改规则等信息。
*/

#ifndef _ADV_KILL_H_
#define _ADV_KILL_H_

#include <linux/slab.h>
#include <linux/types.h>
#include <linux/mm.h>
#include <linux/string.h>
#include <linux/list.h>
#include <linux/version.h>

#define _BO_TONG_								1

//#define	ADVKILL_PRINT_DEBUG_INFO				1
//#define ADVKILL_CHECK_MEM						1

#define ADV_KILL_OK								0
#define ADV_KILL_FAIL							-1

#define ADV_MAX_LOCATION_LEN					256
#define ADV_MAX_SHOST_LEN						128
#define ADV_MAX_REFERER_LEN						128
#define ADV_MAX_SURL_LEN						1024

#ifndef ADVKILL_CHECK_MEM
#define ADVKILL_CALLOC(n, size)				kcalloc(n, size, GFP_KERNEL)
#define ADVKILL_FREE(addr, size)			kfree(addr)
#else
#define ADVKILL_CALLOC(n, size)				kcalloc(n, size, GFP_KERNEL);(g_calloc_times++);(g_calloc_size+=((n)*(size)))
#define ADVKILL_FREE(addr, size)			kfree(addr);(g_free_times++);(g_free_size+=(size))
#endif

#define ADV_PRINT_INFO(info)				printk(KERN_ALERT "%s\n", info)
#define ADV_PRINT_ERROR(error)				printk(KERN_ERR "%s\n", error)
#define EnterFunction()						//printk(KERN_INFO "Enter function[%s],line[%d]\n", __FUNCTION__, __LINE__); PrintCurrentTime()
#define LeaveFunction()						//PrintCurrentTime(); printk(KERN_ALERT "Leave function[%s],line[%d]\n", __FUNCTION__, __LINE__)
#define LeaveFunction_fail()				//PrintCurrentTime(); printk(KERN_ALERT "----Failed,Leave function[%s],line[%d]\n", __FUNCTION__, __LINE__)

#define ADVKILL_MUTEX_LOCK(mutex)			mutex_lock(mutex)
#define ADVKILL_MUTEX_UNLOCK(mutex)			mutex_unlock(mutex)

/**
记录去广告配置规则

主要记录要匹配的源URL和要修改成的URL
*/
struct advconf_hostmap
{
	char *surl;		///< 匹配到的源URL
	char *durl;		///< 要修改成的URL
	int surllen;	///< 源RUL长度
	int durllen;	///< 要修改成的URL长度
};

/**
去广告配置模式

adv_redirect_player: 重定向播放器，一般返回302
adv_drop_request: 丢弃请求包，一般返回404
adv_modify_url: 修改源URL内容
adv_bad_gw: 返回错误的网关 502
*/
typedef enum {adv_redirect_player=0, adv_drop_request=1, adv_modify_url=2, adv_bad_gw=3, adv_fake_pack=4} ADV_CMD;

/**
去广告应用中的配置信息结构体

主要记录操作类型，源Host，修改成的目的Host，以及URL规则，
其中URl规则可以是多套。
*/
struct advconf_hashnode
{
	struct hlist_node node;	///< 哈希链表节点
	ADV_CMD type;	///< 配置操作类型
	int index;	///< 配置索引
	unsigned char *s_host;	///< 要匹配的源Host
	unsigned char *d_host;	///< 要修改成的目的Host
	unsigned short int s_host_len;	///< 要匹配的源Host占用内存大小
	unsigned short int d_host_len;	///< 要修改成的目的Host占用内存大小
	struct advconf_hostmap *map;	///< 配置规则中URL配置结构首地址
	int mapnum;	///< 配置规则中URl配置的个数(同一个Host几套URl规则)
};

/**
去广告配置中哈希链表头

*/
struct advconf_hashtable
{
	struct hlist_head head;  ///< 哈希链表头，代表哈希表中的其中一个哈希链表
};

#endif
