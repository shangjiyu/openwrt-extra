/*
  drcom zjut dirty hacked
*/

#include "drcom.h"

long power(int base, int exp)
{
    long result = 1;
    while(exp) { result *= base; exp--; }
    return result;
}

/* _readconf
	A routine for parsing the config file.
*/

int _readconf(struct drcom_conf *conf, struct drcom_info *info, struct drcom_host *host, \
        struct user_info_pkt *user_info)
{
	FILE *dotconf;
	char buf[__OPTLEN], *s;
	struct _opt_checklist opts;
	int lineno = 0, r = 0;

	memset(&opts, 0, sizeof(opts));

	init_conf(conf);

	dotconf = fopen(DRCOM_CONF, "r");
	if (!dotconf)
		return -1;

	while (1)
	{
		s = fgets(buf, __OPTLEN, dotconf);
		if (s == NULL)
			break;

		r = __parseopt(conf, buf, &opts);

		++lineno;

		if (r < 0 || r > 1) {
			fprintf(stderr, "Error processing config file at line %d.\n", lineno);
			break;
		}
	}

	/* Even if there was an error, we should close the file first */
	fclose(dotconf);

	if (r < 0 || r > 1)
		return -1;

	r = __fillopts(conf, info, user_info, host, &opts);
	if (r) {fprintf(stderr, "fillopts failed\n");goto out;}
	/*r = add_except(conf, conf->dnsp, 0xffffffff);
	if (r) goto out;
	r = add_except(conf, conf->dnss, 0xffffffff);*/

out:
	if (r)
	{
		fprintf(stderr, "Error digesting configuration!\n");
		return -1;
	}

	return 0;
}

int __stripcomments(char *buf)
{
	char *c;

	c = strchr(buf, '#');
	if (c != NULL)
		*c = '\0';

	return (c - buf + 1);
}

int __tidyup(char *buf)
{
	int i, len, tidylen, inquote;

	len = strlen(buf);
	tidylen = 0;
	inquote = 0;
	for (i = 0; i < len; ++i)
	{
		if (__istidy(buf[i]) || (inquote && (buf[i] != '"')))
		{
			buf[tidylen] = buf[i];
			++tidylen;
		}
		else if (buf[i] == '"')
		{
			inquote = (inquote) ? 0 : 1;
		}
	}
	buf[tidylen] = '\0';

	return tidylen;
}

int __optname(char *buf, char *optname)
{
	char *t;
	int len;

	memset(optname, 0, 256);

	t = strchr(buf, '=');
	if (t != NULL)
	{
		len = t - buf;
		strncpy(optname, buf, len);
	}
	else
	{
		len = -1;
		optname[0] = '\0';
	}

	return len;
}

int __optval(char *buf, char *optval)
{
	char *t;
	int len;

	memset(optval, 0, 256);

	t = strchr(buf, '=');
	if (t != NULL)
	{
		len = strlen(buf) - (t - buf + 1);
		strncpy(optval, t + 1, len);
	}
	else
	{
		len = -1;
		optval[0] = '\0';
	}

	return len;
}

void init_conf(struct drcom_conf *conf)
{
	memset(conf, 0, sizeof(struct drcom_conf));
}

int __parseopt(struct drcom_conf *conf, char *buf, struct _opt_checklist *opts)
{
/* define them here because they are __parseopt()-specific */
#define __optprefix(y, n) \
	(strncmp(optname, y, n) == 0)
#define __isopt(y, n) \
	(strncmp(optname, y, n) == 0 && optname_len == n)

    fprintf(stdout, "[challenge]: parse option!\n");
	int len, optname_len, optval_len, r;
	unsigned int _mac[6];
	long sum=0;
	char optname[256], optval[256];
	struct in_addr ip = {0};

	len = strlen(buf);
	if (len > 256)
	{
		len = 256;
		buf[len] = '\0';
	}

	len = __stripcomments(buf);
	if (len == 0)
		goto skip;
	len = __tidyup(buf);
	if (len == 0)
		goto skip;

	optname_len = __optname(buf, optname);
	if (optname_len < 0)
		goto err;
	optval_len = __optval(buf, optval);
	if (optval_len < 0)
		goto err;

	if (optname_len == 0)
	{
	    goto err;
	}
	else if (__isopt("username", 8))
	{
		if (opts->username != 0) { opts->username = 4; goto ok; }
		if (optval_len == 0) { opts->username = 3; goto ok; }
		memset(conf->username, 0, 36);
		strncpy(conf->username, optval, 36);
		opts->username = 1; goto ok;
	}
	else if (__isopt("password", 8))
	{
		if (opts->password != 0) { opts->password = 4; goto ok; }
		if (optval_len == 0) { opts->password = 3; goto ok; }
		memset(conf->password, 0, 16);
		strncpy(conf->password, optval, 16);
		opts->password = 1; goto ok;
	}
	/*else if (__isopt("except", 6)) {
		if (optval_len == 0) goto ok;
		else {
			int r = get_except(conf, optval);
			if (r==0)
				goto ok;
			else{
				fprintf(stderr, "except list error\n");
				goto err;
			}
		}
	}*/
	/*else if (__isopt("device", 6))
	{
		if (optval_len == 0) { goto ok; }
		else
		{
			int s;
			struct ifreq ifr;
			struct sockaddr_in *sin = (struct sockaddr_in *)&ifr.ifr_addr;

			s = socket (AF_INET, SOCK_DGRAM, 0);
			if (s == -1) {
				fprintf(stderr, "Cannot Create DGRAM socket to get device address\n");
				goto err;
			}
			strncpy(ifr.ifr_name, optval, IFNAMSIZ);
			r = ioctl(s, SIOCGIFHWADDR, &ifr);
			if (r != 0) {
				fprintf(stderr, "Cannot get device mac address\n");
				close(s);
				goto err;
			}
			memcpy(conf->mac0, ifr.ifr_hwaddr.sa_data, 6);
			opts->mac0 = 1;

			strncpy(ifr.ifr_name, optval, IFNAMSIZ);
			r = ioctl(s, SIOCGIFADDR, &ifr);
			if (r != 0) {
				fprintf(stderr, "Cannot get device mac address\n");
				close(s);
				goto err;
			}
			conf->nic[0] = sin->sin_addr.s_addr; opts->nic[0] = 1;

			close(s);

			strncpy(conf->device, optval, IFNAMSIZ);
			conf->device[IFNAMSIZ-1] = '\0';
			opts->dev = 1;

			goto ok;
		}
	}*/
	else if (__isopt("mac", 3))
	{
		if (opts->mac != 0) { opts->mac = 4; goto ok; }
		if (optval_len == 0) { opts->mac = 3; goto ok; }
		else
		{
			r = sscanf(optval, "%02x:%02x:%02x:%02x:%02x:%02x",
				 &_mac[0], &_mac[1], &_mac[2], &_mac[3], &_mac[4], &_mac[5]);
			if (r < 6) { opts->mac = 2; goto err; }
			/*conf->mac[0] = _mac[0]; conf->mac[1] = _mac[1]; conf->mac[2] = _mac[2];
			conf->mac[3] = _mac[3]; conf->mac[4] = _mac[4]; conf->mac[5] = _mac[5];*/
			int i;
			for(i=0;i<6;i++){
			    sum += (_mac[6 - i - 1])*power(256, i);
			  }
			conf->mac = sum;
			opts->mac = 1; goto ok;
		}
	}
	else if (__optprefix("nic", 3)) goto ok;
/*
	else if (__optprefix("nic", 3))
	{
		if (optname_len > 4) goto err;
		l = optname[3] - '0';
		if (l >= 0 && l <= 3)
		{
			if (opts->nic[l] != 0) { opts->nic[l] = 4; goto ok; }
			if (optval_len == 0) { opts->nic[l] = 3; goto ok; }
			r = inet_pton(AF_INET, optval, &ip);
			if (r == 0) { opts->nic[l] = 2; goto err; }
			conf->nic[l] = ip.s_addr;
			opts->nic[l] = 1;
		}
		else goto err;
	}
*/
	else if (__optprefix("dns", 3))
	{
		if (optname_len > 4) goto err;
		switch (optname[3])
		{
			case 'p':
				if (opts->dnsp != 0) { opts->dnsp = 4; goto ok; }
				if (optval_len == 0) { opts->dnsp = 3; goto ok; }
				r = inet_pton(AF_INET, optval, &ip);
				if (r == 0) { opts->dnsp = 2; goto err; }
				conf->dnsp = ip.s_addr;
				opts->dnsp =	1; goto ok;
				break;
			case 's':
				if (opts->dnss != 0) { opts->dnss = 4; goto ok; }
				if (optval_len == 0) { opts->dnss = 3; goto ok; }
				r = inet_pton(AF_INET, optval, &ip);
				if (r == 0) { opts->dnss = 2; goto err; }
				conf->dnss = ip.s_addr;
				opts->dnss =	1; goto ok;
				break;
			default: goto err; break;
		}
	}
	else if (__isopt("dhcp", 4))
	{
		if (opts->dhcp != 0) { opts->dhcp = 4; goto ok; }
		if (optval_len == 0) { opts->dhcp = 3; goto ok; }
		r = inet_pton(AF_INET, optval, &ip);
		if (r == 0) { opts->dhcp = 2; goto err; }
		conf->dhcp = ip.s_addr;
		opts->dhcp = 1; goto ok;
	}
	else if (__isopt("hostip", 6))
	{
		if (opts->hostip != 0) { opts->hostip = 4; goto ok; }
		if (optval_len == 0) { opts->hostip = 3; goto ok; }
		/*r = inet_pton(AF_INET, optval, &ip);
		if (r == 0)
		{ opts->hostip = (get_interface_ipaddr(optval, &conf->hostip) == 0) ? 1 : 2; goto ok; }
		conf->hostip = ip.s_addr;*/
		opts->hostip = 1;
	}
	else if (__isopt("servip", 6))
	{
		if (opts->servip != 0) { opts->servip = 4; goto ok; }
		if (optval_len == 0) { opts->servip = 3; goto ok; }
		r = inet_pton(AF_INET, optval, &ip);
		if (r == 0) { opts->servip = 2; goto err; }
		conf->servip = ip.s_addr;
		opts->servip = 1;
	}
	else if (__isopt("hostport", 8))
	{
		if (opts->hostport != 0) { opts->hostport = 4; goto ok; }
		if (optval_len == 0) { opts->hostport = 3; goto ok; }
		conf->hostport = atoi(optval);
		if (conf->hostport != 0) {
			opts->hostport = 1;
			goto ok;
		}
	}
	else if (__isopt("servport", 8))
	{
		if (opts->servport != 0) { opts->servport = 4; goto ok; }
		if (optval_len == 0) { opts->servport = 3; goto ok; }
		conf->servport = atoi(optval);
		if (conf->servport != 0) {
			opts->servport = 1;
			goto ok;
		}
	}
	else if (__isopt("hostname", 8))
	{
		if (opts->hostname != 0) { opts->hostname = 4; goto ok; }
		if (optval_len == 0) { opts->hostname = 3; goto ok; }
		memset(conf->hostname, 0, 32);
		strncpy(conf->hostname, optval, 32);
		opts->hostname = 1; goto ok;
	}
	else if (__isopt("winver", 6))
	{
		if (opts->winver != 0) { opts->winver = 4; goto ok; }
		if (optval_len == 0) { opts->winver = 3; goto ok; }
		r = sscanf(optval, "%u.%u", &conf->winver_major, &conf->winver_minor);
		if (r < 2) { opts->winver = 2; goto err; }
		opts->winver = 1; goto ok;
	}
	else if (__isopt("winbuild", 8))
	{
		if (opts->winbuild != 0) { opts->winbuild = 4; goto ok; }
		if (optval_len == 0) { opts->winbuild = 3; goto ok; }
		r = sscanf(optval, "%u", &conf->winver_build);
		if (r < 1) { opts->winbuild = 2; goto err; }
		opts->winbuild = 1; goto ok;
	}
	else if (__isopt("servicepack", 11))
	{
		if (opts->servicepack != 0) { opts->servicepack = 4; goto ok; }
		if (optval_len == 0) { opts->servicepack = 3; goto ok; }
		memset(conf->servicepack, 0, 32);
		strncpy(conf->servicepack, optval, 32);
		opts->servicepack = 1; goto ok;
	}
	else if(__isopt("autologout", 10))
	{
		if(optval_len != 0 && optval[0] == '1' ){
			conf->autologout = 1; goto ok;
		}else{
			conf->autologout =0; goto ok;
		}
	}else{
        fprintf(stderr, "filloption : mac\n");
        goto err;
    }

ok:
	return 0;

skip:
	return 1;

err:
	return 2;
}

int __fillopts(struct drcom_conf *conf, struct drcom_info *info, struct user_info_pkt *user_info, \
        struct drcom_host *host, struct _opt_checklist *opts)
{
	/* Check the options one by one */

	if (opts->username == 1)
		memcpy(info->username, conf->username, 36);
	else
		return -1;

	if (opts->password == 1)
		memcpy(info->password, conf->password, 16);
	else
		return -1;

	if (opts->dev == 1)
		memcpy(info->device, conf->device, IFNAMSIZ);
	/*else {
		fprintf(stderr, "device not specified\n");
		return -1;
	}*/

	if (opts->mac == 1)
		//memcpy(info->mac, conf->mac, 6);
	    info->mac = conf->mac;
	/*else if (opts->mac0 == 1) {
		memcpy(info->mac, conf->mac0, 6);
		memcpy(conf->mac, conf->mac0, 6);
	}*/
	else if (opts->mac == 0 || opts->mac == 3)
		//memset(info->mac, 0, 6);
	    info->mac = 0;
	else{
	    fprintf(stderr, "filloption : mac\n");
	    return -1;
	}

	/*if (opts->nic[0] == 1)
		info->nic[0] = conf->nic[0];
	else{
        fprintf(stderr, "filloption : nic[0]\n");
        return -1;
    }

	if (opts->nic[1] == 1)
		info->nic[1] = conf->nic[1];
	else if (opts->nic[1] == 0 || opts->nic[1] == 3)
		info->nic[1] = 0;
	else{
        fprintf(stderr, "filloption : nic[1]\n");
        return -1;
    }

	if (opts->nic[2] == 1)
		info->nic[2] = conf->nic[2];
	else if (opts->nic[2] == 0 || opts->nic[2] == 3)
		info->nic[2] = 0;
	else{
	        fprintf(stderr, "filloption : mac[2]\n");
	        return -1;
	    }

	if (opts->nic[3] == 1)
		info->nic[3] = conf->nic[3];
	else if (opts->nic[3] == 0 || opts->nic[3] == 3)
		info->nic[3] = 0;
	else{
	        fprintf(stderr, "filloption : mac[3]\n");
	        return -1;
	    }*/

	if (opts->dnsp == 1)
		host->dnsp = conf->dnsp;
	else{
	        fprintf(stderr, "filloption : dnsp\n");
	        return -1;
	    }

	if (opts->dnss == 1)
		host->dnss = conf->dnss;
	else{
	        fprintf(stderr, "filloption : dnss\n");
	        return -1;
	    }

	if (opts->dhcp == 1)
		host->dhcp = conf->dhcp;
	else if (opts->dhcp == 0 || opts->dhcp == 3)
		//host->dhcp = 0xffffffff;	/* Like Windows XP */
		host->dhcp = 0x00000000;	/* Like Windows XP */
	else{
	        fprintf(stderr, "filloption : dhcp\n");
	        return -1;
	    }

	/* No need to check for validity of conf->nic[0] here,
		 since it's already checked */
	if (opts->hostip == 1)
		info->hostip = conf->hostip;
	else if (opts->hostip == 0 || opts->hostip == 3)
		info->hostip = conf->nic[0];
	else{
	        fprintf(stderr, "filloption : hostip\n");
	        return -1;
	    }

	if (opts->servip == 1)
		info->servip = conf->servip;
	else if (opts->servip == 0 || opts->servip == 3)
		info->servip = 0x01010101; /* 1.1.1.1 */
	else
		return -1;

	if (opts->hostport == 1)
		info->hostport = conf->hostport;
	else if (opts->hostport == 0 || opts->hostport == 3)
		info->hostport = 0xf000; /* 61440 */
	else
		return -1;

	if (opts->servport == 1)
		info->servport = conf->servport;
	else if (opts->servport == 0 || opts->servport == 3)
		info->servport = 0xf000; /* 61440 */
	else
		return -1;

	if (opts->hostname == 1)
		memcpy(host->hostname, conf->hostname, 32);
	else if (opts->hostname == 0 || opts->hostname == 3)
	{
		memset(host->hostname, 0, 32);
		strncpy(host->hostname, "localhost", 32);	/* Does Windows XP use this? */
	}
	else
		return -1;

	if (opts->winver == 1)
	{
		host->winver_major = conf->winver_major;
		host->winver_minor = conf->winver_minor;
	}
	else if (opts->winver == 0 || opts->winver == 3)
	{
		host->winver_major = 5;	/* Windows XP's version */
		host->winver_minor = 1;	/* is NT 5.1 */
	}
	else
		return -1;

	if (opts->winbuild == 1)
		host->winver_build = conf->winver_build;
	else if (opts->winbuild == 0 || opts->winbuild == 3)
		host->winver_build = 2600;	/* Windows XP is of build 2600 */
	else
		return -1;

	if (opts->servicepack == 1)
		memcpy(host->servicepack, conf->servicepack, 32);
	else if (opts->servicepack == 0 || opts->servicepack == 3)
	{
		memset(host->servicepack, 0, 32);
		strncpy(host->servicepack, "Service Pack 2", 32); /* The latest XP sp */
	}
	else
		return -1;

	inflate_user_info(user_info, conf);

	/* Finally, the easy ones */
	host->zero0[0] = 0;
	host->zero0[1] = 0;
	host->unknown0 = 148;
	host->unknown1 = 2;

	return 0;
}

void inflate_user_info(struct user_info_pkt *user_info, struct drcom_conf *conf)
{
    user_info->username = conf->username;
    fprintf(stdout, "[inflate user_info]: username: %s!\n", (user_info->username));
    user_info->username_len = strlen(user_info->username);
    user_info->password = conf->password;
    fprintf(stdout, "[inflate user_info]: password: %s!\n", (user_info->password));
    user_info->password_len = strlen(user_info->password);
    user_info->hostname = conf->hostname;
    fprintf(stdout, "[inflate user_info]: hostname: %s!\n", (user_info->hostname));
    user_info->hostname_len = strlen(user_info->hostname);
    user_info->os_name = conf->hostname;
    fprintf(stdout, "[inflate user_info]: os_name: %s!\n", (user_info->os_name));
    user_info->os_name_len = strlen(user_info->os_name);
    user_info->mac_addr = conf->mac;
    fprintf(stdout, "[inflate user_info]: mac in long: %ld!\n", (user_info->mac_addr));
}

void set_challenge_data(unsigned char *clg_data, int clg_data_len, int clg_try_count)
{
    /* set challenge */
    time_t tmp_t;
    double sec_t = (double)time(&tmp_t);
    srand((int)time(&tmp_t));
    int random = rand() % 0xF0 + 0xF + (int)sec_t;
    int data_index = 0;
    memset(clg_data, 0x00, clg_data_len);
    /* 0x01 challenge request */
    clg_data[data_index++] = 0x01;
    /* clg_try_count first 0x02, then increment */
    clg_data[data_index++] = 0x02 + (unsigned char)clg_try_count;
    /* two byte of challenge_data */
    clg_data[data_index++] = (unsigned char)(random % 0xFFFF);
    clg_data[data_index++] = (unsigned char)((random % 0xFFFF) >> 8);
    /* end with 0x09 */
    clg_data[data_index++] = 0x09;
}

void challenge(int sock, struct sockaddr_in serv_addr, unsigned char *clg_data, int clg_data_len, \
        char *recv_data, int recv_len)
{
    int ret;
    int challenge_try = 0;
    do {
        if (challenge_try > CHALLENGE_TRY) {
            fprintf(stderr, "[challenge]: try challenge, but failed, please check your network connection.\n");
            close(sock);
            exit(EXIT_FAILURE);
        }
        set_challenge_data(clg_data, clg_data_len, challenge_try);
        challenge_try++;
        ret = sendto(sock, clg_data, clg_data_len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (ret != clg_data_len) {
            fprintf(stderr, "[challenge]: send challenge data failed.\n");
            continue;
        }
        ret = recvfrom(sock, recv_data, recv_len, 0, NULL, NULL);
        if (ret < 0) {
            fprintf(stderr, "[challenge]: recieve data from server failed.\n");
            continue;
        }
        if (*recv_data != 0x02) {
            if (*recv_data == 0x07) {
                fprintf(stderr, "[challenge]: wrong challenge data.\n");
                close(sock);
                exit(EXIT_FAILURE);
            }
            fprintf(stderr, "[challenge]: challenge failed!, try again.\n");
        }
    } while ((*recv_data != 0x02));
    fprintf(stdout, "[challenge]: challenge success!\n");
}

void set_login_data(struct user_info_pkt *user_info, unsigned char *login_data, int login_data_len,\
        unsigned char *salt, int salt_len, unsigned char *keep_alive_msg)
{
    /* login data */
    fprintf(stdout, "[login_data]: start build login data!\n");
    int i, j;
    unsigned char md5_str[16];
    unsigned char md5_str_tmp[100];
    int md5_str_len;

    int data_index = 0;

    memset(login_data, 0x00, login_data_len);

    /* magic 3 byte, username_len 1 byte */
    login_data[data_index++] = 0x03;
    login_data[data_index++] = 0x01;
    login_data[data_index++] = 0x00;
    login_data[data_index++] = (unsigned char)(user_info->username_len + 20);

    /* md5 0x03 0x01 salt password */
    md5_str_len = 2 + salt_len + user_info->password_len;
    memset(md5_str_tmp, 0x00, md5_str_len);
    md5_str_tmp[0] = 0x03;
    md5_str_tmp[1] = 0x01;
    memcpy(md5_str_tmp + 2, salt, salt_len);
    memcpy(md5_str_tmp + 2 + salt_len, user_info->password, user_info->password_len);
    MD5(md5_str_tmp, md5_str_len, md5_str);
    memcpy(login_data + data_index, md5_str, 16);
    memcpy(keep_alive_msg, md5_str, 16);
    data_index += 16;

    /* user name 36 */
    memcpy(login_data + data_index, user_info->username, user_info->username_len);
    data_index += user_info->username_len > 36 ? user_info->username_len : 36;

    /* 0x00 0x00 */
    //data_index += 2;
    login_data[data_index++] = 0x20;
    login_data[data_index++] = 0x01;

    /* (data[4:10].encode('hex'),16)^mac */
    long sum = 0;
    for (i = 0; i < 6; i++) {
        sum = (int)md5_str[i] + sum * 256;
    }
    sum ^= user_info->mac_addr;
    for (i = 6; i > 0; i--) {
        login_data[data_index + i - 1] = (unsigned char)(sum % 256);
        sum /= 256;
    }
    data_index += 6;

    /* md5 0x01 pwd salt 0x00 0x00 0x00 0x00 */
    md5_str_len = 1 + user_info->password_len + salt_len + 4;
    memset(md5_str_tmp, 0x00, md5_str_len);
    md5_str_tmp[0] = 0x01;
    memcpy(md5_str_tmp + 1, user_info->password, user_info->password_len);
    memcpy(md5_str_tmp + 1 + user_info->password_len, salt, salt_len);
    MD5(md5_str_tmp, md5_str_len, md5_str);
    memcpy(login_data + data_index, md5_str, 16);
    data_index += 16;

    /* 0x01 0x31 0x8c 0x21 0x28 0x00*12 */
    login_data[data_index++] = 0x01;
    /* ip address */
    memcpy(login_data + data_index, salt+16, 4);
    data_index += 16;

    /* md5 login_data[0-data_index] 0x14 0x00 0x07 0x0b 8 bytes */
    md5_str_len = data_index + 4;
    memset(md5_str_tmp, 0x00, md5_str_len);
    memcpy(md5_str_tmp, login_data, data_index);
    md5_str_tmp[data_index+0] = 0x14;
    md5_str_tmp[data_index+1] = 0x00;
    md5_str_tmp[data_index+2] = 0x07;
    md5_str_tmp[data_index+3] = 0x0b;
    MD5(md5_str_tmp, md5_str_len, md5_str);
    memcpy(login_data + data_index, md5_str, 8);
    data_index += 8;

    /* 0x01 0x00*4 */
    login_data[data_index++] = 0x01;
    data_index += 4;

    /* hostname */
    i = user_info->hostname_len > 71 ? 71 : user_info->hostname_len;
    memcpy(login_data + data_index, user_info->hostname, i);
    data_index += 71;

    /* 0x01 ip dog*/
    login_data[data_index++] = 0x01;

    /* osname */
    i = user_info->os_name_len > 128 ? 128 : user_info->os_name_len;
    memcpy(login_data + data_index, user_info->os_name, i);
    data_index += 204;

    /* 0x09 0x00 0x02 0c /len(pass) */
    login_data[data_index++] = 0x09;
    login_data[data_index++] = 0x00;
    login_data[data_index++] = 0x02;
    login_data[data_index++] = 0x0c;/*(unsigned char)(user_info->password_len)*/

    memcpy(login_data + data_index, md5_str+10, 4);


    /* 0x00 0x00 mac */
    login_data[data_index++] = 0x00;
    login_data[data_index++] = 0x00;
    long mac = user_info->mac_addr;
    for (i = 0; i < 6; i++) {
        login_data[data_index + i - 1] = (unsigned char)(mac % 256);
        mac /= 256;
    }
    data_index += 6;
}

void login(int sock, struct sockaddr_in serv_addr, unsigned char *login_data, int login_data_len, \
        char *recv_data, int recv_len, unsigned char *keep_alive_authInfo, \
        int keep_alive_authInfo_len)
{
    /* login */
    int ret = 0;
    int login_try = 0;
    do {
        if (login_try > LOGIN_TRY) {
            fprintf(stderr, "[login]: try login, but failed, something wrong.\n");
            close(sock);
            exit(EXIT_FAILURE);
        }
        login_try++;
        ret = sendto(sock, login_data, login_data_len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
        if (ret != login_data_len) {
            fprintf(stderr, "[login]: send login data failed.\n");
            continue;
        }
        ret = recvfrom(sock, recv_data, recv_len, 0, NULL, NULL);
        if (ret < 0) {
            fprintf(stderr, "[login]: recieve data from server failed.\n");
            continue;
        }
        if (*recv_data != 0x04) {
            if (*recv_data == 0x05) {
                fprintf(stderr, "[login]: wrong password or username!\n\n");
                close(sock);
                exit(EXIT_FAILURE);
            }
            fprintf(stderr, "[login]: login failed!, try again\n");
        }
    } while ((*recv_data != 0x04));
    memcpy(keep_alive_authInfo, recv_data+23, keep_alive_authInfo_len);
    fprintf(stdout, "[login]: login success!\n");
}

void set_alive_data(unsigned char *alive_data, int alive_data_len, unsigned char *keep_alive_msg, \
        int keep_alive_msg_len, unsigned char *keep_alive_authInfo, int keep_alive_authInfo_len)
{
    // 0: 84 | 1: 82 | 2: 82
    int i = 0;
    memset(alive_data, 0x00, alive_data_len);
    alive_data[i++] = 0xff;
    memcpy(alive_data + i, keep_alive_msg, 16);
    i += 19;
    memcpy(alive_data + i, keep_alive_authInfo, keep_alive_authInfo_len);
    i += 19;
    time_t _time_info = (time(NULL) % 86400);
    u_int8_t time1 = ((u_int8_t)(_time_info & 0xff));
    u_int8_t time2 = ((u_int8_t)(_time_info >> 8 & 0xff));
    alive_data[i++] = time1;
    alive_data[i++] = time2;
    i += 4;
}

