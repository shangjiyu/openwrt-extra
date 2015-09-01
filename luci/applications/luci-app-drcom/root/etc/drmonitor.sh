#!/bin/sh

isfound=$(ps | grep "drcom" | grep -v "grep");
iswget=$(which wget)
if [ -z "$iswget" ];then
        echo 'no wget'
        connected=1
else
	if [ -z `wget -q -O - www.baidu.com | grep "Dr.COM"` ];then
		connected=1
	else
		connected=0
	fi
fi
if [ $connected -eq 0 -o -z "$isfound" -o `date +%H%M` = '810' -o `date +%H%M` = '0810' ]; then
    echo "$(date): start drcom(connected: $connected)...">>/tmp/log/drcom-monitor.log;
    killall drcom >/dev/null 2>&1;
    [ -f '/var/run/drcom.pid' ] && {
        rm -f /var/run/drcom.pid >/dev/null;
    }
    uci set drcom.config.enabled=1;
    uci commit drcom;
    /etc/init.d/drcom restart >/dev/null 2>&1;
fi
