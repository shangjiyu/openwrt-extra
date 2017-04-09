openwrt-vlmcsd
-----
#### A OpenWRT package for vlmcsd.

You can use [luci-app-vlmcsd](https://github.com/cokebar/luci-app-vlmcsd "") to control it. luci-app-vlmscd support KMS auto-activation.

Travis CI: [![Build Status](https://travis-ci.org/cokebar/openwrt-vlmcsd.svg?branch=master)](https://travis-ci.org/cokebar/openwrt-vlmcsd)

Using without luci-app-vlmcsd
-----
If you don't use luci-app-vlmcsd and you want vlmcsd support KMS auto activation, you should modify the settings of dnsmasq manually:

1. Add the following line at the end of `/etc/dnsmasq.conf`:

   `srv-host=_vlmcs._tcp.lan,hostname.lan,1688,0,100`
   
   (replace "hostname.lan" with your actual host name, eg: openwrt.lan, or just replace it with your IP of LAN）

2. Restart dnsmasq:

   `/etc/init.d/dnsmasq restart`

   You can check if the dnsmasq setting works with the following cammand in Windows:
   
   `nslookup -type=srv _vlmcs._tcp.lan`
   
   The response should be your router's IP.

3. `/etc/init.d/vlmcsd enable && /etc/init.d/vlmcsd start && /etc/init.d/dnsmasq restart`

Pre-compiled Download
-----
Your can find pre-compiled ipk:
- in this branch: https://github.com/cokebar/openwrt-vlmcsd/tree/gh-pages
- on release page: https://github.com/cokebar/openwrt-vlmcsd/releases
