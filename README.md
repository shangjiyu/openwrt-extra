OpenWrt-Extra
=============

Some extra packages for OpenWrt

Add "src-git extra git://github.com/shangjiyu/openwrt-extra.git" to feeds.conf.default.

```bash
./scripts/feeds update -a
./scripts/feeds install -a
```

the list of packages:
* dnscrypt-proxy
* dns2socks
* luci-app-aira2
* shadowsocks-libev
* webui-aria2
* yaaw

added
* ShanXun (sxplugin)
* Dr.COM (ZJUT only)
* Node.js (untested)
* ~~mentohust~~
* ChinaDNS
* ShadowVPN
* REDSOCKS2
