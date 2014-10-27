/*
  drcom zjut dirty hacked
*/

#ifndef DRCOMD_H_
#define DRCOMD_H_

#include <stdint.h>
#include <unistd.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <linux/if.h>

#include <time.h>
#include "md5.h"

#define SERVER_ADDR "192.168.6.1"
#define SERVER_PORT 61440

#define RECV_DATA_SIZE 1000
#define SEND_DATA_SIZE 1000
#define CHALLENGE_TRY 10
#define LOGIN_TRY 5
#define ALIVE_TRY 5

/* Use a simple handle */
struct msg_item
{
    struct msg_item *next;
    ssize_t msg_len;
    unsigned char *msg;
};

/* Used by drcomcd to initialize drcom.o */

struct drcom_conf
{
  char username[36];
  char password[16];
  char device[IFNAMSIZ];
  long mac;
  u_int8_t mac0[6];
  u_int32_t nic[4];
  u_int32_t dnsp;
  u_int32_t dnss;
  u_int32_t dhcp;
  u_int32_t hostip;
  u_int32_t servip;
  u_int16_t hostport;
  u_int16_t servport;
  char hostname[32];
  u_int32_t winver_major;
  u_int32_t winver_minor;
  u_int32_t winver_build;
  char servicepack[32];
  int  autologout;
  int except_count;
  struct e_address *except;
};

struct drcom_host
{
    char hostname[32];
    u_int32_t dnsp;
    u_int32_t dhcp;
    u_int32_t dnss;
    u_int32_t zero0[2];
    u_int32_t unknown0;
    u_int32_t winver_major;
    u_int32_t winver_minor;
    u_int32_t winver_build;
    u_int32_t unknown1;
    char servicepack[32];
} __attribute__ ((__packed__));

struct drcom_info
{
  char username[36];
  char password[16];
  char device[IFNAMSIZ];
  long mac;
  u_int32_t nic[4];
  u_int32_t hostip;
  u_int32_t servip;
  u_int16_t hostport;
  u_int16_t servport;
};

struct user_info_pkt {
    char *username;
    char *password;
    char *hostname;
    char *os_name;
    //long ip_addr;
    long mac_addr;
    int username_len;
    int password_len;
    int hostname_len;
    int os_name_len;
};

struct drcom_auth
{
    char drcom[4];
    u_int32_t servip;
    u_int16_t servport;
    u_int32_t hostip;
    u_int16_t hostport;
} __attribute__ ((__packed__));

struct drcom_session_info
{
  uint8_t auth[sizeof(struct drcom_auth)];
  uint32_t hostip;
  uint32_t servip;
  uint16_t hostport;
  uint16_t servport;
  uint32_t dnsp;
  uint32_t dnss;
};

/*
    Some kind of checklist:
    + 0 means option was never encountered in the config file
    + 1 means option found, value specified and valid
    + 2 means option found, value specified and invalid
    + 3 means option found, but no value specified
    + 4 means option found more than once
*/
struct _opt_checklist
{
    u_int8_t username;
    u_int8_t password;
    u_int8_t dev;
    u_int8_t mac;
    u_int8_t mac0;
    u_int8_t nic[4];
    u_int8_t dnsp;
    u_int8_t dnss;
    u_int8_t dhcp;
    u_int8_t hostip;
    u_int8_t servip;
    u_int8_t hostport;
    u_int8_t servport;
    u_int8_t hostname;
    u_int8_t winver;
    u_int8_t winbuild;
    u_int8_t servicepack;
};

/* Log file */
#define DRCOMCD_LOG_FILE "/var/log/drcomcd"

#define DRCOM_CONF "/etc/drcom.conf"

#define READ_END        0
#define WRITE_END       1

#define STATUS_IDLE             0
#define STATUS_LOGGED_IN        1
#define STATUS_BUSY             2

extern int status;

#define __OPTLEN 4096

#define __istidy(x) \
    ((x >= 'a' && x <= 'z') \
    || (x >= 'A' && x <= 'Z') \
    || (x >= '0' && x <= '9') \
    || (x == '_' || x == '=' || x == '.' || x == ',' || x == ':' || x == '/'))


int __stripcomments(char *);
int __tidyup(char *);
int __optname(char *, char *);
int __optval(char *, char *);
int __parseopt(struct drcom_conf *, char *, struct _opt_checklist *opts);
int __fillopts(struct drcom_conf *, struct drcom_info *, struct user_info_pkt *, struct drcom_host *, struct _opt_checklist *);

void init_conf(struct drcom_conf *);
int _readconf(struct drcom_conf *, struct drcom_info *, struct drcom_host *, struct user_info_pkt *);
void inflate_user_info(struct user_info_pkt *, struct drcom_conf *);

void set_challenge_data(unsigned char *, int , int );
void challenge(int , struct sockaddr_in , unsigned char *, int , char *, int );

void set_login_data(struct user_info_pkt *, unsigned char *, int , unsigned char *, \
        int , unsigned char *);
//extern void do_command_login(int, struct drcom_handle *);
void login(int , struct sockaddr_in , unsigned char *, int , char *, int , unsigned char *, int );

void set_alive_data(unsigned char *, int , unsigned char *,int , unsigned char *, int );

void keep_alive();

//extern void do_command_logout(int, struct drcom_handle *);
void set_logout_data();
void logout();

//extern void do_command_passwd(int, struct drcom_handle *);

//extern int drcom_logout(int, struct drcom_handle *, int);

//extern struct drcom_session_info *drcom_get_session_info(struct drcom_handle *);

#define NIPQUAD(addr) \
        ((unsigned char *)&addr)[0], \
        ((unsigned char *)&addr)[1], \
        ((unsigned char *)&addr)[2], \
        ((unsigned char *)&addr)[3]

#endif
