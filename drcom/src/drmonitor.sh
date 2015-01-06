#!/bin/sh

isfound=$(ps | grep "drcom" | grep -v "grep");

if [ -z "$isfound" -o `date +%H%M` = '810' -o `date +%H%M` = '0810' ]; then
        echo "$(date): start drcom...">>/tmp/log/drcom-monitor.log;
        killall drcom;
        /usr/bin/drcom>/dev/null &
fi
