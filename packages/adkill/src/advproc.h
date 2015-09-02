/**
@file		advproc.h
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		proc文件系统相关操作接口

@note
创建proc文件，删除proc文件，读写proc文件操作
*/

#ifndef _ADV_PROC_H_
/**
@file		advproc.h
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		去广告应用的proc文件

@note
创建proc文件，删除proc文件
*/
#define _ADV_PROC_H_

#include <linux/kernel.h>
#if (LINUX_VERSION_CODE >= KERNEL_VERSION (3, 10, 0))
#include <linux/fs.h>
#include <linux/seq_file.h>
#endif
#include <linux/proc_fs.h>
#include <linux/string.h>
#include <linux/vmalloc.h>
#include <asm/uaccess.h>

#define MAX_ADVKILL_CONF_LEN					1024			///< proc文件系统中每条配置最大长度
#define ADV_KILL_PROC_DIRECTORY					"advkill"		///< proc文件目录名称
#define ADV_KILL_PROC_FILE						"advconf"		///< proc文件名称

/**
创建需要用到的proc文件

@return 成功返回 RR_OK，失败返回 RR_FAIL。
*/
int create_proc_file(void);

/**
删除proc文件
*/
void destroy_proc_file(void);

#endif
