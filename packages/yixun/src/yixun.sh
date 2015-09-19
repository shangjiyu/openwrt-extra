#!/bin/sh
source /etc/yixun.cfg
echo $USER|grep -E "@ca" || USER=$USER"@ca"
USER=`/usr/bin/yixun.bin $USER`
USER=`echo $USER|sed -f /usr/share/yixun/yixun.sed`
SITE="http://admin:admin@192.168.1.1/userRpm/PPPoECfgRpm.htm?wan=0&wantype=1&acc=%0D%0A"${USER}"&psw="${PASS}"&confirm=208841&specialDial=100&SecType=0&sta_ip=0.0.0.0&sta_mask=0.0.0.0&linktype=1&Save=%B1%A3+%B4%E6"
wget -qO- ${SITE} 2>&1 >/dev/null