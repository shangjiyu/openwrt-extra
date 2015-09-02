/**
@file		advproc.c
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		proc文件系统相关操作接口

@note
创建proc文件，删除proc文件，读写proc文件操作
*/

#include "advkill.h"
#include "advproc.h"
#include "advconfparse.h"
#include "advhash.h"

extern struct advconf_hashtable *g_advconf_hashtable;

#ifdef ADVKILL_CHECK_MEM
extern unsigned long long int g_calloc_times;
extern unsigned long long int g_calloc_size;
extern unsigned long long int g_free_times;
extern unsigned long long int g_free_size;
#endif

static struct proc_dir_entry *proc_dir = NULL;	///< proc文件中文件夹
static struct proc_dir_entry *proc_advkill_conf = NULL; ///< proc文件中文件
static int advkill_conf_index;  
static int advkill_conf_next;
static char *advkillconfdata = NULL; ///< 内核中去广告配置缓冲区地址

/**
从用户空间将数据读取到内核空间，解析后保存在哈希表中

@param filp 文件结构体指针，本接口对此没有操作，为内核函数参数格式
@param buff 用户空间数据地址
@param len 用户空间数据长度
@param data 本接口对此没有操作，为内核函数参数格式
@return 返回读取的长度
*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION (3, 10, 0))
static ssize_t advkill_conf_write( struct file *filp, const char __user *buff, unsigned long len, void *data);
#else
static ssize_t advkill_conf_write( struct file *filp, const char __user *buff, size_t len, loff_t *data);
#endif
/**
从内核空间将去广告配置内存拷贝到用户空间

@param page 用户空间接收数据的地址
@param start 本接口对此没有操作，为内核函数参数格式
@param off 本接口对此没有操作，为内核函数参数格式
@param count 本接口对此没有操作，为内核函数参数格式
@param eof 本接口对此没有操作，为内核函数参数格式
@param data 本接口对此没有操作，为内核函数参数格式
@return 返回写入的长度
*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION (3, 10, 0))
static int advkill_conf_read( char *page, char **start, off_t off, int count, int *eof, void *data);
#endif

/**
从用户空间将数据读取到内核空间，解析后保存在哈希表中

@param filp 文件结构体指针，本接口对此没有操作，为内核函数参数格式
@param buff 用户空间数据地址
@param len 用户空间数据长度
@param data 本接口对此没有操作，为内核函数参数格式
@return 返回读取的长度
*/

#if (LINUX_VERSION_CODE < KERNEL_VERSION (3, 10, 0))
static ssize_t advkill_conf_write( struct file *filp, const char __user *buff, unsigned long len, void *data)
#else
static ssize_t advkill_conf_write( struct file *filp, const char __user *buff, size_t len, loff_t *data)
#endif
{
	int space_available = (MAX_ADVKILL_CONF_LEN-advkill_conf_index)+1;

	if (len > space_available) 
	{
		memset(advkillconfdata, 0, MAX_ADVKILL_CONF_LEN);
		advkill_conf_index = 0;
		advkill_conf_next = 0;
	}
	if(advkill_conf_index>0)advkillconfdata[advkill_conf_index-1] = '\n';
	if (copy_from_user(&advkillconfdata[advkill_conf_index], buff, len )) 
	{
		return -EFAULT;
	}

#ifdef ADVKILL_PRINT_DEBUG_INFO
	ADV_PRINT_INFO(&advkillconfdata[advkill_conf_index]);
#endif

	//parse advconf
	parse_advconf_line_data(&advkillconfdata[advkill_conf_index], g_advconf_hashtable, HOST_HASH_SIZE);

	advkill_conf_index += len;
	advkillconfdata[advkill_conf_index-1] = '\0';

	return len;
}

/**
从内核空间将去广告配置内存拷贝到用户空间

@param page 用户空间接收数据的地址
@param start 本接口对此没有操作，为内核函数参数格式
@param off 本接口对此没有操作，为内核函数参数格式
@param count 本接口对此没有操作，为内核函数参数格式
@param eof 本接口对此没有操作，为内核函数参数格式
@param data 本接口对此没有操作，为内核函数参数格式
@return 返回写入的长度
*/
#if (LINUX_VERSION_CODE < KERNEL_VERSION (3, 10, 0))
static int advkill_conf_read( char *page, char **start, off_t off, int count, int *eof, void *data)
{
	int len;

	if (off > 0) 
	{
		*eof = 1;
		return 0;
	}
	/* Wrap-around */
	if (advkill_conf_next >= advkill_conf_index) 
		advkill_conf_next = 0;
	len = sprintf(page, "%s\n", &advkillconfdata[advkill_conf_next]);
	advkill_conf_next += len;

	return len;
}
#else
static int advkill_conf_seq_read(struct seq_file *seq, void *v)
{
	return seq_printf(seq, "%s\n", advkillconfdata);
}
static int advkill_conf_seq_open(struct inode *inode, struct file *file)
{
	return single_open(file, advkill_conf_seq_read, NULL);
}
#endif

#if (LINUX_VERSION_CODE >= KERNEL_VERSION (3, 10, 0))
static const struct file_operations advkill_fops = {
	.owner = THIS_MODULE,
	.open		= advkill_conf_seq_open,
	.read		= seq_read,
	.llseek		= seq_lseek,
	.write 		= advkill_conf_write,
	.release	= single_release,
};
#endif
/**
创建需要用到的proc文件

@return 成功返回 RR_OK，失败返回 RR_FAIL。
*/
int create_proc_file(void)
{
	int ret = ADV_KILL_OK;
	//web config
	advkillconfdata = (char *)ADVKILL_CALLOC(1, MAX_ADVKILL_CONF_LEN);
	if (advkillconfdata == NULL)
		return ADV_KILL_FAIL;
	
	proc_dir = proc_mkdir(ADV_KILL_PROC_DIRECTORY, NULL);
	if (proc_dir == NULL) 
	{
		ret = -ENOMEM;
		printk(KERN_ERR "Couldn't create proc dir[/proc/%s]\n", ADV_KILL_PROC_DIRECTORY);
		goto exit_fail;
	}
	else
	{
#if (LINUX_VERSION_CODE < KERNEL_VERSION (3, 10, 0))
		proc_advkill_conf = create_proc_entry(ADV_KILL_PROC_FILE, 0644, proc_dir);
		if (proc_advkill_conf == NULL)
		{
			printk(KERN_ERR "Couldn't create proc entry[/proc/%s/%s]\n", ADV_KILL_PROC_DIRECTORY, ADV_KILL_PROC_FILE);
			ret = -ENOMEM;
			goto exit_fail;
		}
		proc_advkill_conf->read_proc = advkill_conf_read;
		proc_advkill_conf->write_proc = advkill_conf_write;
#else
		proc_advkill_conf = proc_create(ADV_KILL_PROC_FILE, 0644, proc_dir, &advkill_fops);
		if (proc_advkill_conf == NULL)
		{
			printk(KERN_ERR "Couldn't create proc entry[/proc/%s/%s]\n", ADV_KILL_PROC_DIRECTORY, ADV_KILL_PROC_FILE);
			ret = -ENOMEM;
			goto exit_fail;
		}
#endif
	}

	return ret;

exit_fail:
	if (advkillconfdata != NULL)
	{
		ADVKILL_FREE(advkillconfdata, MAX_ADVKILL_CONF_LEN);
		advkillconfdata = NULL;
	}
	if (proc_dir != NULL)
	{
		remove_proc_entry(ADV_KILL_PROC_FILE, proc_dir);
		proc_dir = NULL;
	}
	return ret;	
}

/**
删除proc文件
*/
void destroy_proc_file(void)
{
	if (proc_advkill_conf != NULL)
	{
		remove_proc_entry(ADV_KILL_PROC_FILE, proc_dir);
		proc_advkill_conf = NULL;
	}
	if (proc_dir != NULL)
	{
		remove_proc_entry(ADV_KILL_PROC_DIRECTORY, NULL);
		proc_dir = NULL;
	}
	if (advkillconfdata != NULL)
	{
		ADVKILL_FREE(advkillconfdata, MAX_ADVKILL_CONF_LEN);
		advkillconfdata = NULL;
	}
}
