#!/bin/sh

isfound=`pidof drcom`
timeStr=`date +%H%M`
isCurl=$(which curl)
isWget=$(which wget)
connected=0
#logger -s -t Dr.COM "$isfound $timeStr $isCurl $isWget"

if [ -n "$isCurl" ];then
        for url in www.baidu.com www.haosou.com ; do
                result=$(curl -o /dev/null -s -m 10 -w %{http_code} $url)
                for flag in 200 301 404 ; do
                        if [ $flag = $result ];then
                                connected=$(expr $connected + 1)
                                echo "connected to $url"
                        fi
                done
        done
elif [ -n "$isWget" ];then
        if [ "`wget -q -O - www.baidu.com | grep "Dr.COM"`" ];then
                connected=0
        else
                connected=1
        fi
else
        connected=0
        logger -t Dr.COM -s "Neither CURL nor WGET was found"
fi

if [ $connected -eq 0 -o -z "$isfound" -o $timeStr = '810' -o $timeStr = '0810' ];then
        logger -t Dr.COM -s "restart due to (connected: $connected, pid: $isfound, timeStr: $timeStr)..."
        echo "`date`: start drcom (connected: $connected, pid: $isfound, timeStr: $timeStr)..." >>/tmp/log/drcom-monitor.log;
        killall wget >/dev/null 2>&1;
        killall grep >/dev/null 2>&1;
        killall drcom >/dev/null 2>&1;
        [ -f '/var/run/drcom.pid' ] && {
            rm -f /var/run/drcom.pid >/dev/null;
        }
        uci set drcom.config.enabled=1;
        uci commit drcom;
        /etc/init.d/drcom restart >/dev/null 2>&1;
elif [ $connected -gt 0 ]
then
        logger -s -t Dr.COM "Dr.COM is RUNING!"
fi
