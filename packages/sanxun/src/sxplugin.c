#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
#include <pppd/pppd.h>
#include <pppd/md5.h>

typedef unsigned char byte;

char pppd_version[] = VERSION;

static char saveuser[MAXNAMELEN] = {0};
static char savepwd[MAXSECRETLEN] = {0};

static void getPIN(byte *userName, byte *PIN) {
    int i,j;//循环变量
    long timedivbyfive;//时间除以五
    time_t timenow;//当前时间，从time()获得
    byte RADIUS[16];//凑位字符
    byte timeByte[4];//时间 div 5
    byte* beforeMD5;//时间 div 5+用户名+凑位
    MD5_CTX md5;//MD5结构体
    byte afterMD5[16];//MD5输出
    byte MD501H[2]; //MD5前两位
    byte MD501[3];
    byte timeHash[4]; //时间div5经过第一次转后后的值
    byte temp[32]; //第一次转换时所用的临时数组
    byte PIN27[6]; //PIN的2到7位，由系统时间转换

    info("sxplugin : using singlenet01");
    strcpy(RADIUS, "singlenet01");
    timenow = time(NULL);
    info("-------------------------------------");
    info("timenow(Hex)=%x\n",timenow);
    timedivbyfive = timenow / 5;

    for(i = 0; i < 4; i++) {
        timeByte[i] = (byte)(timedivbyfive >> (8 * (3 - i)) & 0xFF);
    }

    info("Begin : beforeMD5");
    /*
    beforeMD5={time encryption}+{user name}+{RADIUS}+'\0';
    default length is 31
    */
    beforeMD5 = malloc(strlen(timeByte)+strlen(userName)+strlen(RADIUS)+1);
    memcpy(beforeMD5,timeByte,4);
    info("1.<%s>",beforeMD5);

    memcpy(beforeMD5 + 4, userName, strcspn(userName,"@"));
    info("2.<%s>",beforeMD5);
    
    strcat(beforeMD5,RADIUS);//string_copy
    info("3.<%s>",beforeMD5);
    info("4.length=<%d>",strlen(beforeMD5));
    info("End : beforeMD5");

    info("Begin : afterMD5");
    MD5_Init(&md5);
    MD5_Update (&md5, beforeMD5, strlen(beforeMD5));
    free(beforeMD5);
    MD5_Final (afterMD5, &md5);
    MD501H[0] = afterMD5[0] >> 4 & 0xF;
    MD501H[1] = afterMD5[0] & 0xF;
    info("1.MD5use_1=<%2x>", MD501H[0]);
    info("2.MD5use_2=<%2x>", MD501H[1]);
    info("End : afterMD5");
    sprintf(MD501,"%x%x",MD501H[0],MD501H[1]);

    for(i = 0; i < 32; i++) {
        temp[i] = timeByte[(31 - i) / 8] & 1;
        timeByte[(31 - i) / 8] = timeByte[(31 - i) / 8] >> 1;
    }

    for (i = 0; i < 4; i++) {
        timeHash[i] = temp[i] * 128 + temp[4 + i] * 64 + temp[8 + i]
            * 32 + temp[12 + i] * 16 + temp[16 + i] * 8 + temp[20 + i]
            * 4 + temp[24 + i] * 2 + temp[28 + i];
    }

    temp[1] = (timeHash[0] & 3) << 4;
    temp[0] = (timeHash[0] >> 2) & 0x3F;
    temp[2] = (timeHash[1] & 0xF) << 2;
    temp[1] = (timeHash[1] >> 4 & 0xF) + temp[1];
    temp[3] = timeHash[2] & 0x3F;
    temp[2] = ((timeHash[2] >> 6) & 0x3) + temp[2];
    temp[5] = (timeHash[3] & 3) << 4;
    temp[4] = (timeHash[3] >> 2) & 0x3F;

    for (i = 0; i < 6; i++) {
        PIN27[i] = temp[i] + 0x020;
        if(PIN27[i]>=0x40) {
            PIN27[i]++;
        }
    }

    PIN[0] = '\r';
    PIN[1] = '\n';
    memcpy(PIN+2, PIN27, 6);
    PIN[8] = MD501[0];
    PIN[9] = MD501[1];
    strcpy(PIN+10, userName);
    info("-------------------------------------");
}

static int pap_modifyusername(char *user, char* passwd)
{
    byte PIN[MAXSECRETLEN] = {0};
    getPIN(saveuser, PIN);
    strcpy(user, PIN);
    info("sxplugin : user is <%s>",user);
}

static int check(){
    return 1;
}

void plugin_init(void)
{
    info("sxplugin init");
    strcpy(saveuser,user);
    strcpy(savepwd,passwd);
    pap_modifyusername(user, saveuser);
    info("sxplugin : passwd loaded");
    pap_check_hook=check;
    chap_check_hook=check;
}
