/**
@file		pkgoper.c
@date		2014/07/31
@author		WangChunyan
@version	1.0.0
@brief		去广告应用中数据包操作接口

@note		
数据包操作接口，包括组装数据包，发送数据包等。
*/

#include <linux/if_packet.h>
#include <linux/if_ether.h>
#include <linux/inet.h>
#include <linux/netdevice.h>
#include <linux/skbuff.h>
#include <linux/ip.h>
#include <linux/tcp.h>
#include <net/tcp.h>
#include <linux/inetdevice.h>
#include "pkgoper.h"
#include "advkill.h"

#ifdef ADVKILL_CHECK_MEM
extern unsigned long long int g_calloc_times;
extern unsigned long long int g_calloc_size;
extern unsigned long long int g_free_times;
extern unsigned long long int g_free_size;
#endif

struct client_nicname lan_name[] = 
{
	{	0,		ETH_CLIENT_LAN0			},
	{	1,		ETH_CLIENT_LAN1			},
	{	-1,		NULL					}
};

struct client_nicname wlan_name[] = 
{
	{	0,		ETH_CLIENT_WLAN0		},
	{	1,		ETH_CLIENT_WLAN1		},
	{	-1,		NULL					}
};

/**
重新计算IP校验和

@param skb 要校验的skb地址
*/
static void refresh_ip_checksum(struct sk_buff *skb)
{
	ip_send_check((struct iphdr *)skb->data);
}

/**
重新计算TCP校验和

@param skb 要校验的skb地址
*/
static void refresh_tcp_checksum(struct sk_buff *skb)
{
	struct iphdr *iph;
	struct tcphdr *tcph;
	int tcplen;

	iph = (struct iphdr *)skb->data;
	tcph = (struct tcphdr *)(skb->data + (iph->ihl << 2));
	tcplen = ntohs(iph->tot_len) - (iph->ihl << 2);
	tcph->check = 0;
	tcph->check = csum_tcpudp_magic(iph->saddr, iph->daddr,  tcplen, IPPROTO_TCP, csum_partial((char *)tcph, tcplen, 0));
}

/**
重新对IP头和TCP头进行校验

@param skb 原始的sk_buff结构地址
*/
void refresh_skb_checksum(struct sk_buff *skb)
{
	refresh_ip_checksum(skb);
	refresh_tcp_checksum(skb);
}

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
struct sk_buff *pkg_skbuff_generate(struct sk_buff *skb, struct client_nicname *names, int num, char *tcpdata, int tcpdatalen)
{
	struct sk_buff *new_skb = NULL;
	struct net_device *dev = NULL;
	struct iphdr *iph = NULL,*new_iph = NULL;
	struct tcphdr *tcph = NULL,*new_tcph = NULL;
	struct ethhdr *ethdr = NULL;
	char *newpdata = NULL;
	unsigned char * mac_header_addr = NULL;
	int i = 0;

	if(!skb || !names)
	{
		goto out;    
	}
	iph = ip_hdr(skb);
	if(iph == NULL)
	{
		goto out;    
	}
	tcph = (struct tcphdr *)((char *)iph + iph->ihl*4);
	if(tcph == NULL)
	{
		goto out;    
	}   
	ethdr = eth_hdr(skb);
	if(ethdr == NULL)
	{
		goto out;    
	}

	for (i=0; names[i].index != -1; i++)
	{
#if (LINUX_VERSION_CODE < KERNEL_VERSION (2, 6, 24))//不确定版本号是否应该更早
		dev = dev_get_by_name(names[i].name);
#else
		dev = dev_get_by_name(&init_net, names[i].name);
#endif
		if (dev != NULL)
			break;
	}
	
	if (dev == NULL)
	{
		goto out;    
	}
	
	new_skb = alloc_skb(tcpdatalen + iph->ihl*4 + tcph->doff*4 + 14, GFP_ATOMIC);
	if(new_skb == NULL) 
	{
		goto out;    
	}    
#if (LINUX_VERSION_CODE < KERNEL_VERSION (3, 11, 0))
	new_skb->mac_header = new_skb->data;
	skb_reserve(new_skb,14);
	new_skb->transport_header = new_skb->data;
	new_skb->network_header = new_skb->data;

	//get_route_mac(iph->saddr, iph->daddr);
	memcpy(&new_skb->mac_header[0], ethdr->h_source, 6);
	memcpy(&new_skb->mac_header[6], ethdr->h_dest, 6);
	new_skb->mac_header[12] = 0x08;
	new_skb->mac_header[13] = 0x00;
#else
	skb_reset_mac_header(new_skb);
	skb_reserve(new_skb,14);
	skb_reset_transport_header(new_skb);
	skb_reset_network_header(new_skb);

	mac_header_addr=skb_mac_header(new_skb);
	if(mac_header_addr==NULL)
	{
		printk("Can't get header address!\n");
		goto out;
	}
	//get_route_mac(iph->saddr, iph->daddr);
	memcpy(mac_header_addr, ethdr->h_source, 6);
	memcpy(mac_header_addr+6, ethdr->h_dest, 6);
	mac_header_addr[12] = 0x08;
	mac_header_addr[13] = 0x00;
#endif
	skb_put(new_skb, iph->ihl*4 + tcph->doff*4);
	new_skb->mac_len = 14;
	new_skb->dev = dev;
	new_skb->pkt_type = PACKET_OTHERHOST;
	new_skb->protocol = __constant_htons(ETH_P_IP);
	new_skb->ip_summed = CHECKSUM_NONE;
	new_skb->priority = 0;
	/*
	 *IP set
	 */
	new_iph = (struct iphdr *)new_skb->data;
	memset((char *)new_iph, 0, iph->ihl*4);
	new_iph->version = iph->version;
	new_iph->ihl = iph->ihl;
	new_iph->tos = iph->tos;
	new_iph->id = iph->id;
	new_iph->ttl = iph->ttl;
	new_iph->frag_off = iph->frag_off;
	new_iph->protocol = IPPROTO_TCP;
	//new_iph->saddr = iph->saddr;
	new_iph->saddr = iph->daddr;
	new_iph->daddr = iph->saddr;
	new_iph->tot_len = htons(tcpdatalen + iph->ihl*4 + tcph->doff*4);
	new_iph->check = 0;
	/*
	 *TCP set
	 */
	new_tcph = (struct tcphdr *)(new_skb->data + iph->ihl*4);
	memset((char *)new_tcph, 0, tcph->doff*4);

	new_tcph->source = tcph->dest;
	new_tcph->dest = tcph->source;
	new_tcph->seq =  tcph->ack_seq;
	new_tcph->ack_seq = htonl(ntohl(tcph->seq) + (ntohs(iph->tot_len) - iph->ihl*4 - tcph->doff*4));
	new_tcph->doff = tcph->doff;
	new_tcph->fin = tcph->fin;
	new_tcph->ack = tcph->ack;
	new_tcph->psh = tcph->psh;
	new_tcph->window = tcph->window;
	new_tcph->check = 0;

	if (tcpdatalen > 0)
	{
		newpdata = skb_put(new_skb, tcpdatalen);
		if (newpdata != NULL)
		{
			if (tcpdata != NULL)
				memcpy(newpdata, tcpdata, tcpdatalen);
		}
	}
	refresh_skb_checksum(new_skb);
	return new_skb;
out:
	if (NULL != skb)
	{
		dev_put (dev); 
		kfree_skb (skb);
	}
	return NULL;
}

/**
发送tcp数据包

@param skb 原始的sk_buff结构地址
@param tcpdata tcp数据地址
@param tcpdatalen tcp数据长度
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int pkg_skbuff_dev_xmit(struct sk_buff *skb, char *tcpdata, int tcpdatalen)
{
	struct sk_buff *new_skb_lan = NULL;
	int ret = 0;

	new_skb_lan = pkg_skbuff_generate(skb, lan_name, sizeof(lan_name)/sizeof(struct client_nicname), tcpdata, tcpdatalen);
	new_skb_lan->data -= 14;
	new_skb_lan->len  += 14;
	ret = dev_queue_xmit(new_skb_lan);
	if(ret != 0)
	{
#ifdef ADVKILL_PRINT_DEBUG_INFO
		printk(KERN_ALERT "dev queue xmit failed %d\n",ret);
#endif
		return ADV_KILL_FAIL;
	}
	return ADV_KILL_OK;
}

/**
根据Host生成location字符串

@param httplen 生成的location长度
@param host Host字符串内容
@return 成功返回location地址，失败返回NULL。
*/
char *http_location_str_generate(int *httplen, char *host)
{
	char *new_data = NULL;
	int http_len = 0, new_offset = 0;
	char *response_head = HTTP_RESPONSE_HEAD_NOT_FIND;
	char *location = HTTP_RESPONSE_LOCATION;
	char *ending = HTTP_RESPONSE_END;
	int hostlen = 0;

	if(!httplen || !host)
		return NULL;

	hostlen = strlen(host);
	http_len = (HTTP_RESPONSE_HEAD_NOT_FIND_LEN + HTTP_RESPONSE_LOCATION_LEN + hostlen + 4);
	*httplen = http_len;
	new_data = (char *)ADVKILL_CALLOC(1, http_len+1);
	if(new_data == NULL)
	{
		goto err;
	}
	//http 302
	memcpy(new_data, response_head, HTTP_RESPONSE_HEAD_NOT_FIND_LEN);
	new_offset += HTTP_RESPONSE_HEAD_NOT_FIND_LEN;
	//location
	memcpy(new_data + new_offset, location, HTTP_RESPONSE_LOCATION_LEN);
	new_offset += HTTP_RESPONSE_LOCATION_LEN;
	//host
	memcpy(new_data + new_offset, host, hostlen);
	new_offset += hostlen;
	//\r\n
	memcpy(new_data + new_offset, ending, HTTP_RESPONSE_END_LEN);
	new_offset += HTTP_RESPONSE_END_LEN;
	memcpy(new_data + new_offset, ending, HTTP_RESPONSE_END_LEN);

	return new_data;

err:
	return NULL;
}

/**
根据location发送302消息

@param skb 原始的sk_buff结构地址
@param location 需要发送的location内容
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int send_client_location(struct sk_buff *skb, char *location)
{
	char *tcpdata = NULL;
	int tcpdatalen = 0;

	if(!location || !skb)
		return ADV_KILL_FAIL;
	tcpdata = http_location_str_generate(&tcpdatalen, location);
	if (tcpdata == NULL)
	{
		return ADV_KILL_FAIL;
	}
	pkg_skbuff_dev_xmit(skb, tcpdata, tcpdatalen);
	
	ADVKILL_FREE(tcpdata, tcpdatalen+1);
	return ADV_KILL_OK;
}

/**
发送404消息

@param skb 原始的sk_buff结构地址
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int send_client_notfound(struct sk_buff *skb)
{
	if(!skb)
		return ADV_KILL_FAIL;
	
	return pkg_skbuff_dev_xmit(skb, HTTP_NOT_FOUND_STR, HTTP_NOT_FOUND_STR_LEN);
}

/**
发送502消息

@param skb 原始的sk_buff结构地址
@return 成功返回 ADV_KILL_OK，失败返回 ADV_KILL_FAIL。
*/
int send_client_bad_gateway(struct sk_buff *skb)
{
	if(!skb)
		return ADV_KILL_FAIL;

	return pkg_skbuff_dev_xmit(skb, HTTP_BAD_GATEWAY, HTTP_BAD_GATEWAY_STR_LEN);
}

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
int send_client_fake_message(struct sk_buff *skb, const char * CType, const char * Cont)
{
	char *new_data = NULL;
	int retval = 0, datlen = 0;

	if(!skb)
		return ADV_KILL_FAIL;

	datlen = strlen(CType)+strlen(Cont)+120;
	new_data = (char *)ADVKILL_CALLOC(1, datlen);
	if(new_data==NULL)
		return ADV_KILL_FAIL;
	sprintf(new_data,"HTTP/1.1 200 OK\r\nServer: QWS\r\nContent-Type: %s\r\nContent-Length: %d\r\nConnection: close\r\nAccept-Ranges: bytes\r\n\r\n%s",CType,strlen(Cont),Cont);
	retval=pkg_skbuff_dev_xmit(skb, new_data, strlen(new_data));
	ADVKILL_FREE(new_data, datlen);
	return retval;
}


