#!/bin/sh
curl -s -k -X POST https://dnsapi.cn/Record.Modify -d "login_token=*****,************&format=json&domain_id=1234535&record_id=201975441&sub_domain=wifi&value=$1&record_type=A&record_line=%e9%bb%98%e8%ae%a4"
return $?
