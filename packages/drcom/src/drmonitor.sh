#!/bin/sh

isfound=$(ps | grep "drcom" | grep -v "grep");

connected=0

for url in www.baidu.com www.so.com ; do
    result=$(curl -o /dev/null -s -m 10 -w %{http_code} $url)
        for flag in 200 301 302 404 ; do
            if [ $flag = $result ];then
                connected=$(expr $connected + 1)
                echo "connected to $url"
            fi
        done
done

if [ $connected -eq 0 -o -z "$isfound" -o `date +%H%M` = '810' -o `date +%H%M` = '0810' ]; then
    echo "$(date): start drcom...">>/tmp/log/drcom-monitor.log;
    killall drcom >/dev/null 2>&1;
    [ -f '/var/run/drcom.pid' ] && {
        rm -f /var/run/drcom.pid >/dev/null;
    }
    uci set drcom.config.enabled=1;
    uci commit drcom;
    /etc/init.d/drcom restart >/dev/null 2>&1;
fi
