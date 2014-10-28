/*
 *  simple zjut drcom client
 *  dirty hack version
 *
 */

/*
 * doc
 * 数据包类型表示
 * 0x01 challenge request
 * 0x02 challenge response
 * 0x03 login request
 * 0x04 login success response
 * 0xff keep_alive request
 * 0x07	keep_alive response
 * 0x06	logout request
 * 0x04	logout response
 * 0x09	change_pass request
 * 0x04	change_pass success response
 *
 */

#include "drcom.h"

int main(int argc, char **argv)
{
	int sock, ret, r;
	struct sockaddr_in serv_addr;
	unsigned char send_data[SEND_DATA_SIZE];
	char recv_data[RECV_DATA_SIZE];
	//char recv_data_challenge[RECV_DATA_SIZE];
	//char recv_data_login[RECV_DATA_SIZE];
	//char recv_data_keepalive[RECV_DATA_SIZE];
	struct drcom_conf *conf;
	struct drcom_info *info;
	struct drcom_host *host;
	struct user_info_pkt *user_info;

	/*FIXME: check malloc failure */
    conf = (struct drcom_conf *) malloc(sizeof(struct drcom_conf));
    info = (struct drcom_info *) malloc(sizeof(struct drcom_info));
    //session = (struct drcom_session_info *) malloc(sizeof(struct drcom_session_info));
    host = (struct drcom_host *) malloc(sizeof(struct drcom_host));
    //auth = (struct drcom_auth *) malloc(sizeof(struct drcom_auth));
    //keepalive = (struct drcom_host_msg *) malloc(sizeof(struct drcom_host_msg));
    //response = (struct drcom_host_msg *) malloc(sizeof(struct drcom_host_msg));
    user_info = (struct user_info_pkt *) malloc(sizeof(struct user_info_pkt));

	r = _readconf(conf, info, host, user_info);
	if (r){
	    fprintf(stderr, "[drcom]: read conf file failed.\n");
	    return r;
	}

	sock = socket(AF_INET, SOCK_DGRAM, 0);
	if (sock < 0) {
		fprintf(stderr, "[drcom]: create sock failed.\n");
		exit(EXIT_FAILURE);
	}
	serv_addr.sin_family = AF_INET;
	serv_addr.sin_addr.s_addr = inet_addr(SERVER_ADDR);
	serv_addr.sin_port = htons(SERVER_PORT);

	// challenge data length 20
	//fprintf(stdout, "[login memset]: rev_data len: %d.\n", sizeof(recv_data)/sizeof(recv_data[0]));
	//memset(recv_data, 0x00, RECV_DATA_SIZE);
	challenge(sock, serv_addr, send_data, 20, recv_data, RECV_DATA_SIZE);

	//keep information for alive_data
	unsigned char keep_alive_msg[16];
	unsigned char keep_alive_authInfo[16];

	// login data length 338, salt length 4
	set_login_data(user_info, send_data, 330, (unsigned char *)(recv_data + 4), 4, (unsigned char *)keep_alive_msg);
	fprintf(stdout, "[login memset]: recv_data_challenge len: %ld.\n", sizeof(recv_data));
	memset(recv_data, 0x00, RECV_DATA_SIZE);
	fprintf(stdout, "[login memset]: recv_data_login len: %ld.\n", sizeof(recv_data));
	login(sock, serv_addr, send_data, 330, recv_data, RECV_DATA_SIZE, keep_alive_authInfo, 16);

	// keep alive alive data length 42 or 40
	unsigned char tail[4];
	int tail_len = 4;
	memset(tail, 0x00, tail_len);

	time_t tmp_t;
	srand((int)time(&tmp_t));
	int random = rand() % 0xFFFF;

	int alive_data_len = 0;
	int alive_count = 0;
	int alive_fail_count = 0;
	do {
		if (alive_fail_count > ALIVE_TRY) {
			close(sock);
			fprintf(stderr, "[drcom]: couldn't connect to network, check please.\n");
			exit(EXIT_FAILURE);
		}
		//alive_data_len = alive_count > 0 ? 40 : 42;
		alive_data_len = 42;
		set_alive_data(send_data, alive_data_len, keep_alive_msg, sizeof(keep_alive_msg), keep_alive_authInfo, sizeof(keep_alive_authInfo));
		ret = sendto(sock, send_data, alive_data_len, 0, (struct sockaddr *)&serv_addr, sizeof(serv_addr));
		if (ret != alive_data_len) {
			alive_fail_count++;
			fprintf(stderr, "[drcom]: send keep-alive data failed.\n");
			continue;
		} else {
			alive_fail_count = 0;
		}
		memset(recv_data, 0x00, RECV_DATA_SIZE);
		ret = recvfrom(sock, recv_data, RECV_DATA_SIZE, 0, NULL, NULL);
		if (ret < 0 || *recv_data != 0x07) {
			alive_fail_count++;
			fprintf(stderr, "[drcom]: recieve keep-alive response data from server failed.\n");
			continue;
		} else {
			alive_fail_count = 0;
		}
		if (alive_count > 1) memcpy(tail, recv_data+16, tail_len);
		sleep(20);
		fprintf(stdout, "[drcom]: keep alive.\n");
		alive_count = (alive_count + 1) % 3;
	} while (1);

	close(sock);
	return 0;
}
