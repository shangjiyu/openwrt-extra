#
# Copyright (C) 2008-2014 The LuCI Team <luci@lists.subsignal.org>
#
# This is free software, licensed under the Apache License, Version 2.0 .
#

include $(TOPDIR)/rules.mk

LUCI_TITLE:=LuCI Support for Shadowsocks
LUCI_DEPENDS:=+shadowsocks-libev +kmod-ipt-nat +kmod-mod-nat-extra +kmod-ipt-ipset +kmod-mod-tproxy +ip +ipset +iptables-mod-tproxy +resolveip

include ../../luci.mk

# call BuildPackage - OpenWrt buildroot signature
