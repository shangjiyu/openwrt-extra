#!/bin/sh

########################################
#
# Tommy DnsPod DDNS Client v0.2.0
#
# Author: Tommy Lau <tommy@gen-new.com>
#
# Created: 2015-02-23 08:52:00 UTC
# Updated: 2016-07-15 15:48:00 UTC
#
########################################

# Use 'json', other option is 'xml'
format='json'

# Use English for default, 'cn' for Chinese
language='en'

# API URL
api_url='https://dnsapi.cn/'

# Get current IP
get_ip() {
    local inter="http://members.3322.org/dyndns/getip"
    wget --quiet --no-check-certificate --output-document=- $inter
    #curl --silent $inter
}

# Send the API request to DnsPod API
# @param1: The command to execute, for example, Info.Version and etc.
# @param2: The parameters to send to the API, for example, domain='domain.tld'
api_post() {
    # Client agent
    local agent="Tommy DnsPod Client/0.2.0 (tommy@gen-new.com)"

    # Stop if no API command is given
    local inter="$api_url${1:?'Info.Version'}"

    # Default post content for every request
    local param="login_token=$token&format=$format&lang=$language&${2}"

    wget --quiet --no-check-certificate --output-document=- --post-data "$param" --user-agent="$agent" $inter
    #curl --silent --request POST --data "$param" --user-agent "$agent" $inter
}

# Lookup current ip
# @param1: The domain to nslookup
dns_lookup() {
    local server="223.5.5.5"
    local os=`uname -n`
    if [ "$os" = "ubuntu" ]; then
        nslookup ${1} $server | awk '/^Address: / { print $2 }'
    elif [ "$os" = "OpenWrt" -o "$os" = "LEDE" ]; then
        nslookup ${1} $server | tr -d '\n[:blank:]' | sed 's/.\+1 \([0-9\.]\+\).*/\1/'
    fi
}

# Update the DNS record
# @param1: The domain name to update, for example, 'domain.tld'
# @param2: The subdomain, for example, 'www'
dns_update() {
    local current_ip=$(get_ip)
    local dns_ip=$(dns_lookup "${2}.${1}")

    echo "${current_ip} : ${dns_ip}"

    if [ "$current_ip" = "$dns_ip" ]; then
	echo "No need to update DDNS."
        return 0
    fi

    # Get domain id
    local domain_id=$(api_post "Domain.Info" "domain=${1}")
    domain_id=$(echo $domain_id | sed 's/.\+{"id":"\([0-9]\+\)".\+/\1/')

    # Get record id of the subdomain
    local record_id=$(api_post "Record.List" "domain_id=${domain_id}&sub_domain=${2}")
    record_id=$(echo $record_id | sed 's/.\+\[{"id":"\([0-9]\+\)".\+/\1/')

    # Update the record
    local result=$(api_post "Record.Ddns" "domain_id=${domain_id}&record_id=${record_id}&record_line=默认&sub_domain=${2}")
    result_code=$(echo $result | sed 's/.\+{"code":"\([0-9]\+\)".\+/\1/')
    result_message=$(echo $result | sed 's/.\+,"message":"\([^"]\+\)".\+/\1/')

    # Output
    echo "Code: $result_code, Message: $result_message"
}

# User token
token="${1:?'Please input your token'}"

# Domain
domain=${2:?'Please input domain'}

# Sub domain
subdomain=${3:?'Please input sub domain'}

# Update the DDNS
dns_update "$domain" "$subdomain"

