#include <unistd.h>
#include <sys/ioctl.h>
#include <sys/socket.h>
#include <linux/if_packet.h>
#include <linux/if.h>
#include <arpa/inet.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <signal.h>

#define ETH_P_PAE 0x888e
#define EAPOL_EAPPACKET 0
#define EAPOL_START 1
#define EAPOL_LOGOFF 2
#define EAP_REQUEST 1
#define EAP_RESPONSE 2
#define EAP_SUCCESS 3
#define EAP_FAILURE 4
#define EAP_TYPE_IDENTITY 1
#define EAP_TYPE_MD5 4
#define EAP_TYPE_ALLOCATED 7
#define PKT_SIZE 1500

typedef unsigned char uchar;
typedef unsigned short ushort;
typedef unsigned int uint;

static const uchar s_broadcast_addr[6] = {0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
static const char *s_interface = "eth0";
static uchar s_local_addr[6];
static int s_fd;
const char *g_username;
const char *g_password;

static ushort pack_eap(uchar *buf, ushort len, uchar code, uchar id, uchar type)
{
	if (code == EAP_REQUEST || code == EAP_RESPONSE)
	{
		memmove(buf + 5, buf, len);
		len += 1;
		buf[4] = type;
	}
	len += 4;
	buf[0] = code;
	buf[1] = id;
	*(ushort*)&buf[2] = htons(len);
	return len;
}

static ushort pack_eapol(uchar *buf, ushort len, uchar ver, uchar type)
{
	memmove(buf + 4, buf, len);
	buf[0] = ver;
	buf[1] = type;
	*(ushort*)&buf[2] = htons(len);
	len += 4;
	return len;
}

static ushort pack_eth(uchar *buf, ushort len, const uchar dst[6], const uchar src[6], ushort type)
{
	memmove(buf + 14, buf, len);
	len += 14;
	memcpy(buf, dst, 6);
	memcpy(buf + 6, src, 6);
	*(ushort*)&buf[12] = htons(type);
	return len;
}

static void authentication()
{
	s_fd = socket(PF_PACKET, SOCK_RAW, htons(ETH_P_PAE));
	if (s_fd < 0)
	{
		fprintf(stderr, "Create socket failed.\n");
		exit(-1);
	}
	struct ifreq req;
	memset(&req, 0, sizeof(struct ifreq));
	strcpy(req.ifr_name, s_interface);
	if (ioctl(s_fd, SIOCGIFINDEX, &req) < 0)
	{
		fprintf(stderr, "Get interface index failed.\n");
		exit(-1);
	}
	struct sockaddr_ll addr;
	memset(&addr, 0, sizeof(struct sockaddr_ll));
	addr.sll_family = AF_PACKET;
	addr.sll_ifindex = req.ifr_ifindex;
	if (bind(s_fd, (struct sockaddr*)&addr, sizeof(struct sockaddr_ll)) < 0)
	{
		fprintf(stderr, "Bind interface failed.\n");
	}
	uint size = sizeof(struct sockaddr_ll);
	if (getsockname(s_fd, (struct sockaddr*)&addr, &size) < 0)
	{
		fprintf(stderr, "Get MAC address failed.\n");
	}
	memcpy(s_local_addr, addr.sll_addr, addr.sll_halen);
	//EAPOL Start
	fprintf(stderr, "EAPOL Start\n");
	uchar buf[PKT_SIZE];
	ushort len = pack_eapol(buf, 0, 1, EAPOL_START);
	len = pack_eth(buf, len, s_broadcast_addr, s_local_addr, ETH_P_PAE);
	send(s_fd, buf, len, 0);
	while (1)
	{
		len = recv(s_fd, buf, sizeof(buf), 0);
		uchar eth_dst_addr[6], eth_src_addr[6];
		memcpy(eth_dst_addr, &buf[0], 6);
		memcpy(eth_src_addr, &buf[6], 6);
		ushort eth_proto = ntohs(*(ushort*)&buf[12]);
		if (eth_proto == ETH_P_PAE && !memcmp(eth_dst_addr, s_local_addr, 6))
		{
			len -= 14;
			memmove(buf, buf + 14, len);
			uchar eapol_ver = buf[0];
			uchar eapol_type = buf[1];
			ushort eapol_len = ntohs(*(ushort*)&buf[2]);
			if (eapol_type == EAPOL_EAPPACKET)
			{
				len -= 4;
				memmove(buf, buf + 4, len);
				uchar eap_code = buf[0];
				uchar eap_id = buf[1];
				ushort eap_len = ntohs(*(ushort*)&buf[2]);
				if (eap_code == EAP_REQUEST)
				{
					uchar eap_type = buf[4];
					len -= 5;
					memmove(buf, buf + 5, len);
					if (eap_type == EAP_TYPE_IDENTITY)
					{
						fprintf(stderr, "EAP Request Identity\n");
						ushort eap_handle_identity(uchar* buf, ushort len);
						len = eap_handle_identity(buf, len);
					}
					else if (eap_type == EAP_TYPE_MD5)
					{
						fprintf(stderr, "EAP Request MD5\n");
						ushort eap_handle_md5(uchar* buf, ushort len);
						len = eap_handle_md5(buf, len);
					}
					else
					{
						fprintf(stderr, "Unknown EAP request type %d.\n", eap_type);
						exit(-1);
					}
					//Response
					len = pack_eap(buf, len, EAP_RESPONSE, eap_id, eap_type);
					len = pack_eapol(buf, len, eapol_ver, eapol_type);
					len = pack_eth(buf, len, eth_src_addr, s_local_addr, ETH_P_PAE);
					send(s_fd, buf, len, 0);
				}
				else if (eap_code == EAP_SUCCESS)
				{
					fprintf(stderr, "EAP Success\n");
				}
				else if (eap_code == EAP_FAILURE)
				{
					fprintf(stderr, "EAP Failure\n");
					exit(0);
				}
			}
		}
	}
}
	
void signal_exit(int signo)
{
	uchar buf[PKT_SIZE];
	fprintf(stderr, "EAPOL Logoff\n");
	ushort len = pack_eapol(buf, 0, 1, EAPOL_LOGOFF);
	len = pack_eth(buf, len, s_broadcast_addr, s_local_addr, ETH_P_PAE);
	send(s_fd, buf, len, 0);
	exit(0);
}

void main(int argc, char *argv[])
{
	if (getuid() != 0)
	{
		fprintf(stderr, "Require root privilege.\n");
		exit(-1);
	}
	if (argc < 3 || argc > 4)
	{
		fprintf(stderr, "Usage: h3clite username password [interface]\n");
		exit(-1);
	}
	g_username = argv[1];
	g_password = argv[2];
	if (argc == 4)
	{
		s_interface = argv[3];
	}
	signal(SIGINT, &signal_exit);
	signal(SIGTERM, &signal_exit);
	authentication();
}

//For SHNU 2013
ushort eap_handle_identity(uchar* buf, ushort len)
{
	strcpy(buf, "\x06\x07PTNYR0ZTYCZ/GUs4dAEqL1xl/r8=\x20\x20");
	strcat(buf, g_username);
	return strlen(buf);
}

ushort eap_handle_md5(uchar* buf, ushort len)
{
	for (int i = 0; i < 16; ++i)
	{
		if (g_password[i] == '\0') break;
		buf[1 + i] ^= g_password[i];
	}
	strcat(buf, g_username);
	return strlen(buf);
}
